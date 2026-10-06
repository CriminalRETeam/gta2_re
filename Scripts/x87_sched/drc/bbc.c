/* bbc: basic-block coverage of C2.DLL (0x10700000..0x107a0000) with hit counts.
   Client option: output prefix; writes <prefix>.<pid>.txt with "addr count" lines. See ../README.md. */
#include "dr_api.h"
#define LO 0x10700000u
#define HI 0x107a0000u
static unsigned int hit[HI - LO];
static char prefix[200] = "bbc";
static void at_bb(app_pc pc) { hit[(unsigned)pc - LO]++; }
static dr_emit_flags_t ev_bb(void *drc, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    app_pc pc = dr_fragment_app_pc(tag);
    if ((unsigned)pc >= LO && (unsigned)pc < HI && !for_trace && !translating)
        dr_insert_clean_call(drc, bb, instrlist_first_app(bb), (void *)at_bb, false, 1, OPND_CREATE_INTPTR(pc));
    return DR_EMIT_DEFAULT;
}
static void ev_exit(void)
{
    char name[256];
    dr_snprintf(name, sizeof name, "%s.%d.txt", prefix, dr_get_process_id());
    file_t f = dr_open_file(name, DR_FILE_WRITE_OVERWRITE);
    char line[48]; unsigned i;
    for (i = 0; i < HI - LO; i++) if (hit[i]) { int l = dr_snprintf(line, sizeof line, "%x %u\n", LO + i, hit[i]); dr_write_file(f, line, l); }
    dr_close_file(f);
}
DR_EXPORT void dr_client_main(client_id_t id, int argc, const char *argv[])
{
    if (argc > 1) dr_snprintf(prefix, sizeof prefix, "%s", argv[1]);
    dr_register_bb_event(ev_bb);
    dr_register_exit_event(ev_exit);
}
