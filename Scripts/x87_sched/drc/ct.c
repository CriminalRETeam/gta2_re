/* ct: call/return trace inside C2.DLL. Lines "C <call insn> <target>", "R <ret insn> <return address>",
   "X <call insn> <target outside C2>". Client option: output prefix (<prefix>.<pid>.txt). See ../README.md. */
#include "dr_api.h"
#define LO 0x10700000u
#define HI 0x107a0000u
static file_t f;
static char buf[1<<20]; static int bl;
static void flush(void){ dr_write_file(f, buf, bl); bl = 0; }
static void put(char k, app_pc a, app_pc b){
    if (bl > (1<<20) - 64) flush();
    bl += dr_snprintf(buf + bl, 64, "%c %x %x\n", k, (unsigned)a, (unsigned)b);
}
static void on_call(app_pc ins, app_pc tgt){ put('C', ins, tgt); }
static void on_ret(app_pc ins, app_pc tgt){ put('R', ins, tgt); }
static void on_icall(app_pc ins, app_pc tgt){ if ((unsigned)tgt >= LO && (unsigned)tgt < HI) put('C', ins, tgt); else put('X', ins, tgt); }
static dr_emit_flags_t ev_bb(void *drc, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    instr_t *in;
    app_pc pc = dr_fragment_app_pc(tag);
    if (!((unsigned)pc >= LO && (unsigned)pc < HI)) return DR_EMIT_DEFAULT;
    for (in = instrlist_first_app(bb); in; in = instr_get_next_app(in)) {
        if (instr_is_call_direct(in)) dr_insert_call_instrumentation(drc, bb, in, (void*)on_call);
        else if (instr_is_call_indirect(in)) dr_insert_mbr_instrumentation(drc, bb, in, (void*)on_icall, SPILL_SLOT_1);
        else if (instr_is_return(in)) dr_insert_mbr_instrumentation(drc, bb, in, (void*)on_ret, SPILL_SLOT_1);
    }
    return DR_EMIT_DEFAULT;
}
static void ev_exit(void){ flush(); dr_close_file(f); }
DR_EXPORT void dr_client_main(client_id_t id, int argc, const char *argv[])
{
    char name[256];
    dr_snprintf(name, sizeof name, "%s.%d.txt", argc > 1 ? argv[1] : "ct", dr_get_process_id());
    f = dr_open_file(name, DR_FILE_WRITE_OVERWRITE);
    dr_register_bb_event(ev_bb);
    dr_register_exit_event(ev_exit);
}
