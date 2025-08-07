#include "Particle_8.hpp"
#include "Globals.hpp"
#include "error.hpp"

DEFINE_GLOBAL(Particle_947C*, gParticle_947C_6FD5E4, 0x6FD5E4);

// STUB: 105 0x53e320
void Particle_8::sub_53E320()
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x53e880
void Particle_8::sub_53E880(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x53e970
void Particle_8::sub_53E970(Fix16* a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x53f060
void Particle_8::sub_53F060(s32 a1, s32 a2, s32 a3, s16 a4, char_type a5)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x5406b0
void Particle_8::sub_5406B0(s32 a2, char_type a3)
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x5439d0
Particle_8::Particle_8()
{
    if (!gParticle_947C_6FD5E4)
    {
        gParticle_947C_6FD5E4 = new Particle_947C();
        if (!gParticle_947C_6FD5E4)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\particle.cpp", 4167);
        }
    }
    field_0 = 0;
    field_4 = 0;
}

// FUNCTION: 105 0x543a60
Particle_8::~Particle_8()
{
    if (gParticle_947C_6FD5E4)
    {
        GTA2_DELETE_AND_NULL(gParticle_947C_6FD5E4);
    }
    field_0 = 0;
    field_4 = 0;
}