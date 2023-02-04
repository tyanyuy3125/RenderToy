#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include "rtmath.h"

#include <type_traits>

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
        virtual inline const _TpRet Sample(const Vector<_TpCoord, 2> &p) = 0;
    };

    template <typename _TpRet, typename _TpCoord, std::enable_if_t<std::is_floating_point_v<_TpRet>, bool> = true>
    struct CheckerBoard : public IPCGT<_TpRet, _TpCoord>
    {
        CheckerBoard();
        virtual inline const _TpRet
        Sample(const Vector<_TpCoord, 2> &p) override final
        {
        }
    };

}

#endif // PROCEDURAL_H