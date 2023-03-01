#include "procedural.h"

RenderToy::ProceduralTexture::CheckerBoard::CheckerBoard(int scale_)
    : scale(scale_)
{
}

const float RenderToy::ProceduralTexture::CheckerBoard::Sample(const Vector2f &p) const
{
    if (((int(p.x() * int(scale)) & 1) ^ (int(p.y() * int(scale))) & 1) == 1)
    {
        return 1.0f;
    }
    return 1.0f;
}

RenderToy::ProceduralTexture::Wave::Wave(float period_)
    : period(period_)
{
}

const float RenderToy::ProceduralTexture::Wave::Sample(const Vector2f &p) const
{
    return (std::sin(2.0f * kPi<float> * period * p.x()) + 1.0f) / 2.0f;
}

RenderToy::ProceduralTexture::PerlinNoise::PerlinNoise(siv::PerlinNoise::seed_type seed_)
    : seed(seed_)
{
}

const siv::PerlinNoise::seed_type &RenderToy::ProceduralTexture::PerlinNoise::GetSeed() const
{
    return seed;
}

void RenderToy::ProceduralTexture::PerlinNoise::SetSeed(const siv::PerlinNoise::seed_type seed_)
{
    seed = seed_;
    perlin_noise_generator.reseed(seed);
}

const float RenderToy::ProceduralTexture::PerlinNoise::Sample(const Vector2f &p) const
{
    return perlin_noise_generator.octave3D_01(p.x() * scale, p.y() * scale, time, octaves);
}
