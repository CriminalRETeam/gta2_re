#include "mss.h"

//#undef DXDEC
//#define DXDEC

DXDEC S32 AILCALL AIL_enumerate_3D_providers(HPROENUM FAR* next, HPROVIDER FAR* dest, C8 FAR* FAR* name)
{
    return 0;
}

DXDEC void FAR* AILCALL AIL_mem_alloc_lock(U32 size)
{
    return 0;
}

DXDEC void AILCALL AIL_mem_free_lock(void FAR* ptr)
{
}

DXDEC S32 AILCALL AIL_startup(void)
{
    return 0;
}

DXDEC S32 AILCALL AIL_get_preference(U32 number)
{
    return 0;
}

DXDEC void AILCALL AIL_shutdown(void)
{
}

DXDEC S32 AILCALL AIL_set_preference(U32 number, S32 value)
{
    return 0;
}

DXDEC void AILCALL AIL_delay(S32 intervals)
{
}

DXDEC S32 AILCALL AIL_waveOutOpen(HDIGDRIVER FAR* drvr, LPHWAVEOUT FAR* lphWaveOut, S32 wDeviceID, LPWAVEFORMAT lpFormat)
{
    return 0;
}

DXDEC void AILCALL AIL_waveOutClose(HDIGDRIVER drvr)
{
}

DXDEC S32 AILCALL AIL_digital_handle_release(HDIGDRIVER drvr)
{
    return 0;
}

DXDEC S32 AILCALL AIL_digital_handle_reacquire(HDIGDRIVER drvr)
{
    return 0;
}

DXDEC HSAMPLE AILCALL AIL_allocate_sample_handle(HDIGDRIVER dig)
{
    return 0;
}

DXDEC void AILCALL AIL_release_sample_handle(HSAMPLE S)
{
}

DXDEC void AILCALL AIL_init_sample(HSAMPLE S)
{
}

DXDEC void AILCALL AIL_set_sample_address(HSAMPLE S, void FAR* start, U32 len)
{
}

DXDEC void AILCALL AIL_set_sample_type(HSAMPLE S, S32 format, U32 flags)
{
}

DXDEC void AILCALL AIL_start_sample(HSAMPLE S)
{
}

DXDEC void AILCALL AIL_end_sample(HSAMPLE S)
{
}

DXDEC void AILCALL AIL_set_sample_playback_rate(HSAMPLE S, S32 playback_rate)
{
}

DXDEC void AILCALL AIL_set_sample_volume(HSAMPLE S, S32 volume)
{
}

DXDEC void AILCALL AIL_set_sample_pan(HSAMPLE S, S32 pan)
{
}

DXDEC void AILCALL AIL_set_sample_loop_count(HSAMPLE S, S32 loop_count)
{
}

DXDEC void AILCALL AIL_set_sample_loop_block(HSAMPLE S, S32 loop_start_offset, S32 loop_end_offset)
{
}

DXDEC U32 AILCALL AIL_sample_status(HSAMPLE S)
{
    return 0;
}

DXDEC void AILCALL AIL_set_digital_master_volume(HDIGDRIVER dig, S32 master_volume)
{
}

DXDEC HSTREAM AILCALL AIL_open_stream(HDIGDRIVER dig, char FAR* filename, S32 stream_mem)
{
    return 0;
}

DXDEC void AILCALL AIL_close_stream(HSTREAM stream)
{
}

DXDEC void AILCALL AIL_start_stream(HSTREAM stream)
{
}

DXDEC void AILCALL AIL_set_stream_volume(HSTREAM stream, S32 volume)
{
}

DXDEC S32 AILCALL AIL_stream_volume(HSTREAM stream)
{
    return 0;
}

DXDEC void AILCALL AIL_set_stream_playback_rate(HSTREAM stream, S32 rate)
{
}

DXDEC S32 AILCALL AIL_stream_playback_rate(HSTREAM stream)
{
    return 0;
}

DXDEC void AILCALL AIL_set_stream_loop_count(HSTREAM stream, S32 count)
{
}

DXDEC S32 AILCALL AIL_stream_status(HSTREAM stream)
{
    return 0;
}

DXDEC void AILCALL AIL_set_stream_ms_position(HSTREAM S, S32 milliseconds)
{
}

DXDEC void AILCALL AIL_stream_ms_position(HSTREAM S, //)
                                          S32 FAR* total_milliseconds,
                                          S32 FAR* current_milliseconds)
{
}

DXDEC M3DRESULT AILCALL AIL_open_3D_provider(HPROVIDER lib)
{
    return 0;
}

DXDEC void AILCALL AIL_close_3D_provider(HPROVIDER lib)
{
}

DXDEC void AILCALL AIL_3D_provider_attribute(HPROVIDER lib, C8 FAR* name, void FAR* val)
{
}

DXDEC void AILCALL AIL_set_3D_provider_preference(HPROVIDER lib, C8 FAR* name, void FAR* val)
{
}

DXDEC H3DSAMPLE AILCALL AIL_allocate_3D_sample_handle(HPROVIDER lib)
{
    return 0;
}

DXDEC void AILCALL AIL_release_3D_sample_handle(H3DSAMPLE S)
{
}

DXDEC void AILCALL AIL_start_3D_sample(H3DSAMPLE S)
{
}

DXDEC void AILCALL AIL_end_3D_sample(H3DSAMPLE S)
{
}

DXDEC S32 AILCALL AIL_set_3D_sample_info(H3DSAMPLE S, AILSOUNDINFO FAR* info)
{
    return 0;
}

DXDEC void AILCALL AIL_set_3D_sample_volume(H3DSAMPLE S, S32 volume)
{
}

DXDEC void AILCALL AIL_set_3D_sample_playback_rate(H3DSAMPLE S, S32 playback_rate)
{
}

DXDEC void AILCALL AIL_set_3D_sample_loop_count(H3DSAMPLE S, U32 loops)
{
}

DXDEC void AILCALL AIL_set_3D_sample_loop_block(H3DSAMPLE S, S32 loop_start_offset, S32 loop_end_offset)
{
}

DXDEC U32 AILCALL AIL_3D_sample_status(H3DSAMPLE S)
{
    return 0;
}

DXDEC void AILCALL
AIL_3D_sample_float_distances(H3DSAMPLE S, F32 FAR* max_front_dist, F32 FAR* min_front_dist, F32 FAR* max_back_dist, F32 FAR* min_back_dist)
{
}

DXDEC void AILCALL AIL_set_3D_position(H3DPOBJECT obj, F32 X, F32 Y, F32 Z)
{
}

DXDEC void AILCALL
AIL_set_3D_sample_float_distances(H3DSAMPLE S, F32 max_front_dist, F32 min_front_dist, F32 max_back_dist, F32 min_back_dist)
{
}