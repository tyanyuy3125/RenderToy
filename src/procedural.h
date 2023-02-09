#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include "rtmath.h"
#include "compositor.h"
#include "object.h"

#include <type_traits>
#include <cmath>
#include <vector>
#include <PerlinNoise/PerlinNoise.hpp>

namespace RenderToy::ProceduralMesh
{
    /// @brief Interface for procedural generated mesh.
    struct IPCGM
    {
        IPCGM() = default;
        std::vector<Polygon *> polygons;
        [[nodiscard]] const Mesh * Triangularize();
    };
}

namespace RenderToy::ProceduralTexture
{
    /// @brief Interface for procedural generated texture.
    /// @tparam _TpRet Return type.
    /// @tparam _TpCoord Sampling coord type.
    template <typename _TpRet>
    struct IPCGT
    {
        IPCGT() = default;
        /// @brief Get texture information at given coordinate.
        /// @param p
        /// @return
        virtual inline const _TpRet Sample(const Vector2f &p) const = 0;

        /// @brief Generate rasterized image of texture.
        /// @param size
        /// @return
        [[nodiscard]] const Image Rasterize(const std::size_t size) const
        {
            Image ret(SizeN(size, size));
            float ratio = 1.0f / float(size);
            for (std::size_t i = 0; i < size; ++i)
            {
                for (std::size_t j = 0; j < size; ++j)
                {
                    ret.buffer[i * size + j] = Sample(Vector2f(j * ratio, i * ratio));
                }
            }
            return ret;
        }
    };

    /// @brief Checkerboard shader.
    struct CheckerBoard : public IPCGT<float>
    {
        int scale;
        CheckerBoard(int scale_);

        virtual const float Sample(const Vector2f &p) const override final;
    };

    /// @brief Sine wave shader.
    struct Wave : public IPCGT<float>
    {
        float period;
        Wave(float period_);

        virtual const float Sample(const Vector2f &p) const override final;
    };

    /// @brief Perlin noise shader.
    struct PerlinNoise : public IPCGT<float>
    {
        /// @brief Initialize Perlin Noise with given seed.
        /// @param seed_ Seed for RNG.
        PerlinNoise(siv::PerlinNoise::seed_type seed_);

        float scale = 10.0f;
        float time = 0.0f;
        /// @brief Controls level of details.
        int32_t octaves = 4;

        /// @brief Return current seed.
        /// @return Current seed.
        const siv::PerlinNoise::seed_type &GetSeed() const;
        /// @brief Safely set seed for RNG.
        /// @param seed_ 
        void SetSeed(const siv::PerlinNoise::seed_type seed_);

        virtual const float Sample(const Vector2f &p) const override final;

    private:
        siv::BasicPerlinNoise<float> perlin_noise_generator;
        siv::PerlinNoise::seed_type seed;
    };
}

#endif // PROCEDURAL_H