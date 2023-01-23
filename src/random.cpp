#include "random.h"

namespace OpenPT
{
    const int SEED = 987654321;

    Random::Random()
    {
        z_m[0] = z_m[1] = z_m[2] = z_m[3] = SEED;
    }

    int Random::Int() const
    {
        z_m[0] = ((z_m[0] & 0xFFFFFFFEu) << 18) ^ (((z_m[0] << 6) ^ z_m[0]) >> 13);
        z_m[1] = ((z_m[1] & 0xFFFFFFF8u) << 2) ^ (((z_m[1] << 2) ^ z_m[1]) >> 27);
        z_m[2] = ((z_m[2] & 0xFFFFFFF0u) << 7) ^ (((z_m[2] << 13) ^ z_m[2]) >> 21);
        z_m[3] = ((z_m[3] & 0xFFFFFF80u) << 13) ^ (((z_m[3] << 3) ^ z_m[3]) >> 12);

        return z_m[0] ^ z_m[1] ^ z_m[2] ^ z_m[3];
    }

    float Random::Float() const
    {
        return static_cast<float>(static_cast<int>(Int())) *
                   (1.0 / 4294967296.0) +
               0.5 +
               static_cast<float>(static_cast<int>(Int() & 0x001FFFFFu)) *
                   (1.0 / 9007199254740992.0);
    }
}