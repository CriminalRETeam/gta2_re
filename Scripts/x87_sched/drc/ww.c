/* ww: write watch. Logs every write to the watched dwords with the writing pc, the old value and a shadow
   call stack of C2.DLL functions: "W <addr> at <pc> old=<value> stk: <innermost> ... <outermost>".
   Client options: output prefix (<prefix>.<pid>.txt), then up to 64 hex addresses. See ../README.md. */
#include "dr_api.h"
#define LO 0x10700000u
#define HI 0x107a0000u
static file_t f;
static int watch_outside = 1;   /* also watch writes from code outside C2 (memset, CRT) */
static unsigned W[64]; static int nW;
static unsigned stk[4096]; static int sp;
static void on_call(app_pc ins, app_pc tgt){ if ((unsigned)tgt < LO || (unsigned)tgt >= HI) return; if (sp < 4096) stk[sp] = (unsigned)tgt; sp++; }
static void on_ret(app_pc ins, app_pc tgt){ if (sp > 0) sp--; }
static void on_w(app_pc pc)
{
    void *drc = dr_get_current_drcontext();
    dr_mcontext_t mc = { sizeof(mc), DR_MC_INTEGER | DR_MC_CONTROL };
    instr_t in; int i, j;
    dr_get_mcontext(drc, &mc);
    instr_init(drc, &in);
    decode(drc, pc, &in);
    for (i = 0; i < instr_num_dsts(&in); i++) {
        opnd_t op = instr_get_dst(&in, i);
        if (!opnd_is_memory_reference(op)) continue;
        unsigned a = (unsigned)opnd_compute_address(op, &mc);
        unsigned sz = opnd_size_in_bytes(opnd_get_size(op));
        for (j = 0; j < nW; j++) if (a < W[j] + 4 && W[j] < a + sz) {
            char line[2048]; int l = dr_snprintf(line, 200, "W %x at %x old=%x stk:", W[j], (unsigned)pc, *(unsigned*)W[j]);
            int k; for (k = (sp > 4096 ? 4096 : sp) - 1; k >= 0 && l < 1900; k--) l += dr_snprintf(line + l, 20, " %x", stk[k]);
            line[l++] = '\n'; dr_write_file(f, line, l);
        }
    }
    instr_free(drc, &in);
}
static dr_emit_flags_t ev_bb(void *drc, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    instr_t *in;
    app_pc pc = dr_fragment_app_pc(tag);
    int inc2 = ((unsigned)pc >= LO && (unsigned)pc < HI);
    for (in = instrlist_first_app(bb); in; in = instr_get_next_app(in)) {
        if (!inc2) { if (watch_outside && instr_writes_memory(in)) dr_insert_clean_call(drc, bb, in, (void*)on_w, false, 1, OPND_CREATE_INTPTR(instr_get_app_pc(in))); continue; }
        if (instr_is_call_direct(in)) dr_insert_call_instrumentation(drc, bb, in, (void*)on_call);
        else if (instr_is_call_indirect(in)) dr_insert_mbr_instrumentation(drc, bb, in, (void*)on_call, SPILL_SLOT_1);
        else if (instr_is_return(in)) dr_insert_mbr_instrumentation(drc, bb, in, (void*)on_ret, SPILL_SLOT_1);
        if (instr_writes_memory(in) && !instr_is_call(in) && instr_get_opcode(in) != OP_push && instr_get_opcode(in) != OP_push_imm)
            dr_insert_clean_call(drc, bb, in, (void*)on_w, false, 1, OPND_CREATE_INTPTR(instr_get_app_pc(in)));
    }
    return DR_EMIT_DEFAULT;
}
static void ev_exit(void){ dr_close_file(f); }
DR_EXPORT void dr_client_main(client_id_t id, int argc, const char *argv[])
{
    char name[256]; int i;

    nW = 0;
    for (i = 2; i < argc && nW < 64; i++) { unsigned v = 0; const char *s = argv[i]; while (*s) { char c = *s++; v = v * 16 + (c <= '9' ? c - '0' : (c | 32) - 'a' + 10); } W[nW++] = v; }
    dr_snprintf(name, sizeof name, "%s.%d.txt", argc > 1 ? argv[1] : "ww", dr_get_process_id());
    f = dr_open_file(name, DR_FILE_WRITE_OVERWRITE);
    dr_register_bb_event(ev_bb);
    dr_register_exit_event(ev_exit);
}
