#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include "rtmath.h"
#include "compositor.h"

#include <type_traits>
#include <cmath>

namespace RenderToy::ProceduralMesh
{

}

namespace RenderToy::ProceduralTexture
{
    /// @brief Interface for procedural generated texture.
    /// @tparam _TpRet Return type.
    /// @tparam _TpCoord Sampling coord type.
    template <typename _TpRet, typename _TpCoord>
    struct IPCGT
    {
        IPCGT() = default;
        /// @brief Get texture information at given coordinate.
        /// @param p 
        /// @return 
        virtual inline const _TpRet Sample(const Vector<_TpCoord, 2> &p) const = 0;
        
        /// @brief Generate rasterized image of texture.
        /// @param size 
        /// @return 
        [[nodiscard]] const Image Rasterize(const std::size_t size) const
        {
            Image ret(SizeN(size, size));
            _TpCoord ratio = _TpCoord(1) / _TpCoord(size);
            for (std::size_t i = 0; i < size; ++i)
            {
                for (std::size_t j = 0; j < size; ++j)
                {
                    ret.buffer[i * size + j] = Sample(Vector<_TpCoord, 2>(j * ratio, i * ratio));
                }
            }
            return ret;
        }
    };

    /// @brief Checkerboard shader.
    /// @tparam _TpRet Should be floating point value.
    /// @tparam _TpCoord 
    template <typename _TpRet, typename _TpCoord, std::enable_if_t<std::is_floating_point_v<_TpRet>, bool> = true>
    struct CheckerBoard : public IPCGT<_TpRet, _TpCoord>
    {
        int scale;
        CheckerBoard(int scale_)
            : scale(scale_) {}

        virtual inline const _TpRet
        Sample(const Vector<_TpCoord, 2> &p) const override final
        {
            if ((int(p.x() * int(scale)) & 1) ^ (int(p.y() * int(scale)) & 1) == 1)
            {
                return _TpRet(1);
            }
            return _TpRet(0);
        }
    };

    /// @brief Sine wave shader.
    /// @tparam _TpRet Should be floating point value.
    /// @tparam _TpCoord 
    template <typename _TpRet, typename _TpCoord, std::enable_if_t<std::is_floating_point_v<_TpRet>, bool> = true>
    struct Wave : public IPCGT<_TpRet, _TpCoord>
    {
        _TpCoord period;
        Wave(_TpCoord period_)
            : period(period_) {}

        virtual inline const _TpRet
        Sample(const Vector<_TpCoord, 2> &p) const override final
        {
            return (_TpRet(std::sin(_TpCoord(2) * kPi<_TpCoord> * period * p.x())) + _TpRet(1))/_TpRet(2);
        }
    };

    template <typename _TpRet, typename _TpCoord, std::enable_if_t<std::is_floating_point_v<_TpRet>, bool> = true>
    struct PerlinNoise : public IPCGT<_TpRet, _TpCoord>
    {
    };
}

#endif // PROCEDURAL_H