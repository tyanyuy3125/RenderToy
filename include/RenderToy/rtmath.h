#ifndef RTMATH_H
#define RTMATH_H

#include <array>
#include <istream>
#include <stack>
#include <cmath>
#include <algorithm>
#include <initializer_list>
#include <type_traits>

namespace RenderToy
{
#pragma region ForwardDeclaration
    /// @brief Pi.
    /// @tparam T 
    template <typename T = float>
    inline constexpr T kPi = T(3.14159265358979323846L);

    /// @brief Minimum available float in RenderToy.
    inline constexpr float kFloatEpsilon = 1e-7f;
    /// @brief Maximum available float in RenderToy.
    inline constexpr float kFloatInfinity = 1e7f;

    struct SizeN;
    struct PointN;

    /// @brief Represents an orientation in a 2D space.
    enum class Orientation
    {
        X = 0,
        Y = 1,
        All = 2
    };

    template <typename _Tp, std::size_t _Dm>
    struct Vector;
    template <typename _Tp, std::size_t _Dm>
    struct VectorConstants;
    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool> = true>
    struct Matrix;
    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool> = true>
    struct MatrixConstants;

    template <typename _Tp>
    using Vector2 = Vector<_Tp, 2>;
    template <typename _Tp>
    using Vector3 = Vector<_Tp, 3>;
    template <typename _Tp>
    using Vector4 = Vector<_Tp, 4>;

    using Vector2d = Vector2<double>;
    using Vector3d = Vector3<double>;
    using Vector4d = Vector4<double>;

    using Vector2f = Vector2<float>;
    using Vector3f = Vector3<float>;
    using Vector4f = Vector4<float>;

    using Matrix2x2f = Matrix<float, 2>;
    using Matrix3x3f = Matrix<float, 3>;
    using Matrix4x4f = Matrix<float, 4>;

    using Matrix2x2d = Matrix<double, 2>;
    using Matrix3x3d = Matrix<double, 3>;
    using Matrix4x4d = Matrix<double, 4>;
#pragma endregion ForwardDeclaration

#pragma region Vector

    /// @brief Generalized Vector definition.
    /// @tparam _Tp Numerical type.
    /// @tparam _Dm Dimension.
    template <typename _Tp, std::size_t _Dm>
    struct Vector : public VectorConstants<_Tp, _Dm>
    {
        /// @brief Construct default Vector with all components zero.
        /// @tparam _Tp Numerical type.
        /// @tparam _Dm Dimension.
        constexpr Vector() = default;

        /// @brief Construct a vector with each component specified value.
        /// @param ...values Value specification.
        /// @warning You should be careful with the TYPE, or a warning may be generated.
        template <typename... Args, std::enable_if_t<(sizeof...(Args) == _Dm), bool> = true>
        constexpr Vector(Args &&...values)
            : arr{static_cast<_Tp>(std::forward<Args>(values))...} {}

        /// @brief Construct a vector with all components the same value.
        /// @param value
        Vector(const _Tp &value)
            : arr()
        {
            std::generate(arr.begin(), arr.end(), [&]()
                          { return value; });
        }

        /// @brief
        /// @return If Vector is zero, return true.
        inline const bool
        IsZero() const
        {
            for (_Tp a : arr)
            {
                if (std::abs(a) > kFloatEpsilon)
                {
                    return false;
                }
            }
            return true;
        }

        inline static const Vector
        Abs(const Vector &vec)
        {
            Vector ret;
            for (int i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = std::abs(vec.arr[i]);
            }
            return ret;
        }

        /// @brief
        /// @return Returns length of the vector.
        inline const _Tp
        Length2() const
        {
            _Tp ret = _Tp(0);
            for (_Tp a : arr)
                ret += a * a;
            return ret;
        }

        /// @brief
        /// @return Returns length of the vector.
        inline const _Tp
        Length() const
        {
            return std::sqrt(Length2());
        }

        /// @brief Normalizes the vector itself.
        inline void
        Normalize()
        {
            _Tp length = Length();
            for (_Tp &a : arr)
                a /= length;
        }

        /// @brief
        /// @return Returns a normalized copy of the vector.
        [[nodiscard]] inline const Vector
        Normalized() const
        {
            Vector<_Tp, _Dm> ret(*this);
            ret.Normalize();
            return ret;
        }

        /// @brief Safe normalize.
        /// @return Returns O if IsZero() returns true.
        [[nodiscard]] inline const Vector
        Normalized_s() const
        {
            if (IsZero())
            {
                return {};
            }
            Vector<_Tp, _Dm> ret(*this);
            ret.Normalize();
            return ret;
        }

        constexpr inline const _Tp &
        operator[](const size_t i) const
        {
            return arr[i];
        }

        constexpr inline _Tp &
        operator[](const size_t i)
        {
            return arr[i];
        }

        inline const Vector
        operator+(const Vector &a) const
        {
            Vector<_Tp, _Dm> ret;
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = arr[i] + a.arr[i];
            }
            return ret;
        }
        inline const Vector
        operator-(const Vector &a) const
        {
            Vector<_Tp, _Dm> ret;
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = arr[i] - a.arr[i];
            }
            return ret;
        }
        inline const Vector
        operator-(void) const
        {
            Vector<_Tp, _Dm> ret;
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = -arr[i];
            }
            return ret;
        }
        inline const Vector
        operator*(const _Tp a) const
        {
            Vector<_Tp, _Dm> ret;
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = arr[i] * a;
            }
            return ret;
        }
        inline const Vector
        operator*(const Vector &a) const
        {
            Vector<_Tp, _Dm> ret(*this);
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] *= a.arr[i];
            }
            return ret;
        }
        inline const Vector
        operator/(const _Tp a) const
        {
            Vector<_Tp, _Dm> ret;
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = arr[i] / a;
            }
            return ret;
        }
        inline const Vector
        operator/(const Vector &a) const
        {
            Vector<_Tp, _Dm> ret(*this);
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] /= a.arr[i];
            }
            return ret;
        }

        inline const Vector &
        operator+=(const Vector &a)
        {
            for (size_t i = 0; i < _Dm; ++i)
            {
                arr[i] += a.arr[i];
            }
            return (*this);
        }
        inline const Vector &
        operator-=(const Vector &a)
        {
            for (size_t i = 0; i < _Dm; ++i)
            {
                arr[i] -= a.arr[i];
            }
            return (*this);
        }
        inline const Vector &
        operator*=(const _Tp a)
        {
            for (_Tp &elem : arr)
            {
                elem *= a;
            }
            return (*this);
        }
        inline const Vector &
        operator/=(const _Tp a)
        {
            for (_Tp &elem : arr)
            {
                elem /= a;
            }
            return (*this);
        }

        inline const bool
        operator==(const Vector &a) const
        {
            // return arr == a.arr;
            for (int i = 0; i < _Dm; ++i)
            {
                if (std::abs(arr[i] - a.arr[i]) > kFloatEpsilon)
                {
                    return false;
                }
            }
            return true;
        }

        inline const bool
        operator!=(const Vector &a) const
        {
            return !((*this) == a);
        }

        /// @brief Dot product.
        /// @param a
        /// @param b
        /// @return
        [[nodiscard]] inline const static _Tp
        Dot(const Vector &a, const Vector &b)
        {
            _Tp ret(0);
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret += a.arr[i] * b.arr[i];
            }
            return ret;
        }

        /// @brief Respective power.
        /// @param a
        /// @param b
        /// @return
        [[nodiscard]] inline const static Vector
        Pow(const Vector &a, const Vector &b)
        {
            Vector ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = std::pow(a.arr[i], b.arr[i]);
            }
            return ret;
        }

        /// @brief Respective natural logarithmic.
        /// @param a
        /// @return
        [[nodiscard]] const static Vector
        Log(const Vector &a)
        {
            Vector ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = std::log(a.arr[i]);
            }
            return ret;
        }

        /// @brief Dot product.
        /// @param a
        /// @return
        inline const _Tp
        Dot(const Vector &a) const
        {
            _Tp ret(0);
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret += arr[i] * a.arr[i];
            }
            return ret;
        }

        /// @brief Cross product.
        /// @param a
        /// @param b
        /// @return
        template <int..., size_t I = _Dm, std::enable_if_t<I == 2, bool> = true>
        [[nodiscard]] constexpr inline const static Vector
        Cross(const Vector &a, const Vector &b)
        {
            return a.arr[0] * b.arr[1] - a.arr[1] * b.arr[0];
        }

        /// @brief Cross product.
        /// @param a
        /// @return
        template <int..., size_t I = _Dm, std::enable_if_t<I == 2, bool> = true>
        constexpr inline const Vector
        Cross(const Vector &a) const
        {
            return arr[0] * a.arr[1] - arr[1] * a.arr[0];
        }

        /// @brief Cross product.
        /// @param a
        /// @param b
        /// @return
        template <int..., size_t I = _Dm, std::enable_if_t<I == 3, bool> = true>
        [[nodiscard]] constexpr inline const static Vector
        Cross(const Vector &a, const Vector &b)
        {
            return Vector(a.arr[1] * b.arr[2] - a.arr[2] * b.arr[1],
                          a.arr[2] * b.arr[0] - a.arr[0] * b.arr[2],
                          a.arr[0] * b.arr[1] - a.arr[1] * b.arr[0]);
        }

        /// @brief Cross product.
        /// @param a
        /// @return
        template <int..., size_t I = _Dm, std::enable_if_t<I == 3, bool> = true>
        constexpr inline const Vector
        Cross(const Vector &a) const
        {
            return Vector(arr[1] * a.arr[2] - arr[2] * a.arr[1],
                          arr[2] * a.arr[0] - arr[0] * a.arr[2],
                          arr[0] * a.arr[1] - arr[1] * a.arr[0]);
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 0), bool> = true>
        constexpr inline _Tp &
        x()
        {
            return arr[0];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 0), bool> = true>
        constexpr inline const _Tp &
        x() const
        {
            return arr[0];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 1), bool> = true>
        constexpr inline _Tp &
        y()
        {
            return arr[1];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 1), bool> = true>
        constexpr inline const _Tp &
        y() const
        {
            return arr[1];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 2), bool> = true>
        constexpr inline _Tp &
        z()
        {
            return arr[2];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 2), bool> = true>
        constexpr inline const _Tp &
        z() const
        {
            return arr[2];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 3), bool> = true>
        constexpr inline _Tp &
        w()
        {
            return arr[3];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<(I > 3), bool> = true>
        constexpr inline const _Tp &
        w() const
        {
            return arr[3];
        }

        friend inline const Vector operator*(const float lambda, const Vector &a)
        {
            Vector ret(a);
            for (auto &val : ret.arr)
            {
                val *= lambda;
            }
            return ret;
        }

        friend std::istream &operator>>(std::istream &is, Vector &a)
        {
            for (int i = 0; i < _Dm; ++i)
            {
                is >> a.arr[i];
            }
            return is;
        }

        [[nodiscard]] inline const Vector TransformEach(const std::function<_Tp(_Tp)> &func) const
        {
            Vector ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret[i] = func(arr[i]);
            }
            return ret;
        }

        inline const Vector Floor() const
        {
            return TransformEach([](_Tp _) -> _Tp
                                 { return _Tp(std::floor(_)); });
        }

        inline const Vector Fract() const
        {
            return TransformEach([](_Tp _) -> _Tp
                                 { return _ - _Tp(std::floor(_)); });
        }

        inline const Vector Sin() const
        {
            return TransformEach([](_Tp _) -> _Tp
                                 { return _Tp(std::sin(_)); });
        }

        inline const Vector Cos() const
        {
            return TransformEach([](_Tp _) -> _Tp
                                 { return _Tp(std::cos(_)); });
        }

        friend Matrix<_Tp, _Dm>;

    private:
        std::array<_Tp, _Dm> arr = {};
        // _Tp arr[d] = {};
    };

    /// @brief Storages constants of k-dimensional vector space by specialization.
    /// @tparam _Tp Component type.
    /// @tparam _Dm Dimension.
    template <typename _Tp, size_t _Dm>
    struct VectorConstants
    {
    };

    template <typename _Tp>
    struct VectorConstants<_Tp, 2>
    {
        const static Vector<_Tp, 2> O;
        const static Vector<_Tp, 2> X;
        const static Vector<_Tp, 2> Y;
    };

    template <typename _Tp>
    constexpr Vector<_Tp, 2> VectorConstants<_Tp, 2>::O = Vector<_Tp, 2>(_Tp(0), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 2> VectorConstants<_Tp, 2>::X = Vector<_Tp, 2>(_Tp(1), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 2> VectorConstants<_Tp, 2>::Y = Vector<_Tp, 2>(_Tp(0), _Tp(1));

    template <typename _Tp>
    struct VectorConstants<_Tp, 3>
    {
        const static Vector<_Tp, 3> O;
        const static Vector<_Tp, 3> X;
        const static Vector<_Tp, 3> Y;
        const static Vector<_Tp, 3> Z;
        const static Vector<_Tp, 3> White;
    };

    template <typename _Tp>
    constexpr Vector<_Tp, 3> VectorConstants<_Tp, 3>::O = Vector<_Tp, 3>(_Tp(0), _Tp(0), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 3> VectorConstants<_Tp, 3>::X = Vector<_Tp, 3>(_Tp(1), _Tp(0), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 3> VectorConstants<_Tp, 3>::Y = Vector<_Tp, 3>(_Tp(0), _Tp(1), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 3> VectorConstants<_Tp, 3>::Z = Vector<_Tp, 3>(_Tp(0), _Tp(0), _Tp(1));
    template <typename _Tp>
    constexpr Vector<_Tp, 3> VectorConstants<_Tp, 3>::White = Vector<_Tp, 3>(_Tp(1), _Tp(1), _Tp(1));

    template <typename _Tp>
    struct VectorConstants<_Tp, 4>
    {
        const static Vector<_Tp, 4> O;
        const static Vector<_Tp, 4> X;
        const static Vector<_Tp, 4> Y;
        const static Vector<_Tp, 4> Z;
        const static Vector<_Tp, 4> W;
    };

    template <typename _Tp>
    constexpr Vector<_Tp, 4> VectorConstants<_Tp, 4>::O = Vector<_Tp, 4>(_Tp(0), _Tp(0), _Tp(0), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 4> VectorConstants<_Tp, 4>::X = Vector<_Tp, 4>(_Tp(1), _Tp(0), _Tp(0), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 4> VectorConstants<_Tp, 4>::Y = Vector<_Tp, 4>(_Tp(0), _Tp(1), _Tp(0), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 4> VectorConstants<_Tp, 4>::Z = Vector<_Tp, 4>(_Tp(0), _Tp(0), _Tp(1), _Tp(0));
    template <typename _Tp>
    constexpr Vector<_Tp, 4> VectorConstants<_Tp, 4>::W = Vector<_Tp, 4>(_Tp(0), _Tp(0), _Tp(0), _Tp(1));

#pragma endregion Vector

#pragma region Matrix
    /// @brief Generalized Matrix definition.
    /// @tparam _Tp Value type.
    /// @tparam _Dm Dimension.
    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool>>
    struct Matrix : public MatrixConstants<_Tp, _Dm>
    {
        typedef Vector<_Tp, _Dm> VectorType;

        constexpr Matrix() = default;

        // C++ doesn't recognize explicit conversion from initializer_list<_Tp> to array<_Tp, _Dm> for templated classes.
        // Make it happy:

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 2), bool> = true>
        constexpr Matrix(const Vector2<_Tp> &r0, const Vector2<_Tp> &r1)
            : row{r0, r1} {}
        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 2), bool> = true>
        constexpr Matrix(const std::array<Vector2<_Tp>, 2> &tuple)
            : row(tuple) {}

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 3), bool> = true>
        constexpr Matrix(const Vector3<_Tp> &r0, const Vector3<_Tp> &r1, const Vector3<_Tp> &r2)
            : row{r0, r1, r2} {}
        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 3), bool> = true>
        constexpr Matrix(const std::array<Vector3<_Tp>, 3> &triple)
            : row(triple) {}

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 4), bool> = true>
        constexpr Matrix(const Vector4<_Tp> &r0, const Vector4<_Tp> &r1, const Vector4<_Tp> &r2, const Vector4<_Tp> &r3)
            : row{r0, r1, r2, r3} {}
        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 4), bool> = true>
        constexpr Matrix(const std::array<Vector4<_Tp>, 4> &quadruple)
            : row(quadruple) {}

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I > 4), bool> = true>
        constexpr Matrix(const std::array<const std::array<_Tp, _Dm>, _Dm> &row_)
            : row{}
        {
            for (size_t i = 0; i < _Dm; ++i)
            {
                row[i].arr = row_[i];
            }
        }

        constexpr inline VectorType &
        operator[](const size_t i)
        {
            return row[i];
        }

        constexpr inline const VectorType &
        operator[](const size_t i) const
        {
            return row[i];
        }

        /// @brief Get column.
        inline const VectorType &
        Col(const size_t c)
        {
            VectorType ret;
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret[i] = row[i][c];
            }
            return ret;
        }

        inline const bool
        operator==(const Matrix &a) const
        {
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                if (row[i] != a.row[i])
                {
                    return false;
                }
            }
            return true;
        }

        inline const bool
        operator!=(const Matrix &a) const
        {
            return !((*this) == a);
        }

        inline const Matrix
        operator+(const Matrix &a) const
        {
            Matrix ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.row[i] = row[i] + a.row[i];
            }
            return ret;
        }

        inline const Matrix
        operator-(const Matrix &a) const
        {
            Matrix ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.row[i] = row[i] - a.row[i];
            }
            return ret;
        }

        inline const Matrix
        operator-(void) const
        {
            Matrix ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.row[i] = -row[i];
            }
            return ret;
        }

        inline const VectorType
        operator*(const VectorType &a) const
        {
            VectorType ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = row[i].Dot(a);
            }
            return ret;
        }

        inline const Matrix
        operator*(const Matrix &a) const
        {
            Matrix ret;
            for (int i = 0; i < _Dm; ++i)
            {
                for (int j = 0; j < _Dm; ++j)
                {
                    for (int k = 0; k < _Dm; ++k)
                    {
                        ret.row[i][j] += row[i][k] * a.row[k][j];
                    }
                }
            }
            return ret;
        }

        inline const Matrix &
        operator+=(const Matrix &a)
        {
            for (int i = 0; i < _Dm; ++i)
            {
                row[i] += a.row[i];
            }
            return (*this);
        }

        inline const Matrix &
        operator-=(const Matrix &a)
        {
            for (int i = 0; i < _Dm; ++i)
            {
                row[i] -= a.row[i];
            }
            return (*this);
        }

        /// @brief Transpose itself.
        inline void
        Transpose()
        {
            for (int i = 0; i < _Dm; ++i)
            {
                for (int j = 0; j < _Dm; ++j)
                {
                    row[i][j] = row[j][i];
                }
            }
        }

        [[nodiscard]] inline const Matrix
        Transposed() const
        {
            Matrix ret;
            for (int i = 0; i < _Dm; ++i)
            {
                for (int j = 0; j < _Dm; ++j)
                {
                    ret.row[i][j] = row[j][i];
                }
            }
            return ret;
        }

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 2), bool> = true>
        constexpr inline const static float
        Determinant(const Matrix &mat)
        {
            return mat.row[0][0] * mat.row[1][1] - mat.row[0][1] * mat.row[1][0];
        }

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 3), bool> = true>
        constexpr inline const static float
        Determinant(const Matrix &mat)
        {
            return mat.row[0][0] * mat.row[1][1] * mat.row[2][2] + mat.row[0][1] * mat.row[1][2] * mat.row[2][0] + mat.row[0][2] * mat.row[1][0] * mat.row[2][1] - mat.row[0][2] * mat.row[1][1] * mat.row[2][0] - mat.row[0][0] * mat.row[1][2] * mat.row[2][1] - mat.row[2][2] * mat.row[0][1] * mat.row[1][0];
        }

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 2), bool> = true>
        constexpr inline const _Tp
        ComplementMinor(const size_t i, const size_t j) const
        {
            return row[i ^ 1][j ^ 1];
        }

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I >= 3), bool> = true>
        const Matrix<_Tp, I - 1>
        ComplementMinor(const size_t i, const size_t j) const
        {
            Matrix<_Tp, _Dm - 1> ret;
            for (int x = 0, ret_x = 0; x < _Dm; ++x)
            {
                if (x == i)
                {
                    continue;
                }
                for (int y = 0, ret_y = 0; y < _Dm; ++y)
                {
                    if (y == j)
                    {
                        continue;
                    }
                    ret[ret_x][ret_y] = row[x][y];
                    ++ret_y;
                }
                ++ret_x;
            }
            return ret;
        }

    private:
        std::array<VectorType, _Dm> row = {};
    };

    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool>>
    struct MatrixConstants
    {
    };

    template <typename _Tp>
    struct MatrixConstants<_Tp, 2>
    {
        const static Matrix<_Tp, 2> I;
        const static Matrix<_Tp, 2> O;
    };

    template <typename _Tp>
    constexpr Matrix<_Tp, 2> MatrixConstants<_Tp, 2>::I = {{_Tp(1), _Tp(0)},
                                                           {_Tp(0), _Tp(1)}};
    template <typename _Tp>
    constexpr Matrix<_Tp, 2> MatrixConstants<_Tp, 2>::O = {{_Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0)}};

    template <typename _Tp>
    struct MatrixConstants<_Tp, 3>
    {
        const static Matrix<_Tp, 3> I;
        const static Matrix<_Tp, 3> O;
    };

    template <typename _Tp>
    constexpr Matrix<_Tp, 3> MatrixConstants<_Tp, 3>::I = {{_Tp(1), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(1), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(1)}};
    template <typename _Tp>
    constexpr Matrix<_Tp, 3> MatrixConstants<_Tp, 3>::O = {{_Tp(0), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(0)}};

    template <typename _Tp>
    struct MatrixConstants<_Tp, 4>
    {
        const static Matrix<_Tp, 4> I;
        const static Matrix<_Tp, 4> O;
    };

    template <typename _Tp>
    constexpr Matrix<_Tp, 4> MatrixConstants<_Tp, 4>::I = {{_Tp(1), _Tp(0), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(1), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(1), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(0), _Tp(1)}};
    template <typename _Tp>
    constexpr Matrix<_Tp, 4> MatrixConstants<_Tp, 4>::O = {{_Tp(0), _Tp(0), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(0), _Tp(0)},
                                                           {_Tp(0), _Tp(0), _Tp(0), _Tp(0)}};

#pragma endregion Matrix

#pragma region GeneralizedVector
    /// @brief GeneralizedVectors are vectors with run-time initialized dimensions.
    /// @tparam _Tp Component type.
    template <typename _Tp>
    struct GeneralizedVector
    {
        const std::size_t column;

        GeneralizedVector(const std::size_t column_, _Tp default_val_ = _Tp(0))
            : column(column_), data(column_, default_val_)
        {
        }

        template <typename... Args>
        explicit GeneralizedVector(Args &&...data_)
            : column(sizeof...(data_)), data{std::forward<Args>(data_)...}
        {
        }

        GeneralizedVector(std::initializer_list<_Tp> data_)
            : column(data_.size()), data{data_}
        {
        }

        inline const _Tp &operator[](const std::size_t c) const
        {
            return data[c];
        }
        inline _Tp &operator[](const std::size_t c)
        {
            return data[c];
        }

    private:
        std::vector<_Tp> data;
    };
#pragma endregion GeneralizedVector

#pragma region GeneralizedMatrix
    /// @brief GeneralizedMatrixs are matrices with run-time initialized dimensions.
    /// @tparam _Tp Component type.
    template <typename _Tp>
    struct GeneralizedMatrix
    {
    public:
        const std::size_t row, column;

        GeneralizedMatrix(std::size_t row_ = 2, std::size_t column_ = 2, _Tp default_val_ = _Tp(0))
            : row(row_), column(column_), data(row_, GeneralizedVector<_Tp>(column_, default_val_)) {}

        GeneralizedMatrix(std::initializer_list<GeneralizedVector<_Tp>> data_)
            : row(data_.size()), column(data_.begin()->column), data{data_} {}

        inline const GeneralizedVector<_Tp> &operator[](const std::size_t r) const
        {
            return data[r];
        }

        inline GeneralizedVector<_Tp> &operator[](const std::size_t r)
        {
            return data[r];
        }

        std::vector<GeneralizedVector<_Tp>> data;
    };
#pragma endregion GeneralizedMatrix

#pragma region Convolution
    /// @brief Interface for convolution kernels.
    /// @tparam _Tp Component type.
    template <typename _Tp>
    struct IConvolutionKernel
    {
        IConvolutionKernel() = default;
        IConvolutionKernel(std::size_t r, std::size_t c)
            : kernel_mat(r, c) {}
        IConvolutionKernel(const GeneralizedMatrix<_Tp> &kernel_mat_)
            : kernel_mat(kernel_mat_) {}
        GeneralizedMatrix<_Tp> kernel_mat;
    };

    /// @brief Unitized gaussian distribution kernel.
    /// @tparam _Tp Component type.
    /// @tparam _O Decide whether to generate the entire distribution or its two-dimensional components.
    template <typename _Tp, Orientation _O = Orientation::All>
    struct UnitizedGaussianKernel : public IConvolutionKernel<_Tp>
    {
        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::All), bool> = true>
        UnitizedGaussianKernel(const std::size_t size, const _Tp sigma)
            : IConvolutionKernel<_Tp>(size, size)
        {
            _Tp orig = _Tp(size >> 1);
            _Tp x2, y2;
            _Tp sum = _Tp(0);
            for (std::size_t i = 0; i < size; ++i)
            {
                y2 = std::pow(_Tp(i) - orig, _Tp(2));
                for (std::size_t j = 0; j < size; ++j)
                {
                    x2 = std::pow(_Tp(j) - orig, _Tp(2));
                    _Tp g = std::exp(-(x2 + y2) / (_Tp(2) * sigma * sigma));
                    sum += g;
                    IConvolutionKernel<_Tp>::kernel_mat[i][j] = g;
                }
            }

            for (std::size_t i = 0; i < size; ++i)
            {
                for (std::size_t j = 0; j < size; ++j)
                {
                    IConvolutionKernel<_Tp>::kernel_mat[i][j] /= sum;
                }
            }
        }

        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::X), bool> = true>
        UnitizedGaussianKernel(const std::size_t size, const _Tp sigma)
            : IConvolutionKernel<_Tp>(1, size)
        {
            _Tp orig = _Tp(size >> 1);
            _Tp x2;
            _Tp sum = _Tp(0);
            for (std::size_t j = 0; j < size; ++j)
            {
                x2 = std::pow(_Tp(j) - orig, _Tp(2));
                _Tp g = std::exp(-(x2) / (_Tp(2) * sigma * sigma));
                sum += g;
                IConvolutionKernel<_Tp>::kernel_mat[0][j] = g;
            }

            for (std::size_t j = 0; j < size; ++j)
            {
                IConvolutionKernel<_Tp>::kernel_mat[0][j] /= sum;
            }
        }

        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::Y), bool> = true>
        UnitizedGaussianKernel(const std::size_t size, const _Tp sigma)
            : IConvolutionKernel<_Tp>(size, 1)
        {
            _Tp orig = _Tp(size >> 1);
            _Tp y2;
            _Tp sum = _Tp(0);
            for (std::size_t i = 0; i < size; ++i)
            {
                y2 = std::pow(_Tp(i) - orig, _Tp(2));
                _Tp g = std::exp(-(y2) / (_Tp(2) * sigma * sigma));
                sum += g;
                IConvolutionKernel<_Tp>::kernel_mat[i][0] = g;
            }

            for (std::size_t i = 0; i < size; ++i)
            {
                IConvolutionKernel<_Tp>::kernel_mat[i][0] /= sum;
            }
        }
    };

    /// @brief Prewitt differential kernel.
    /// @tparam _Tp Component type.
    /// @tparam _O Orientation.
    template <typename _Tp, Orientation _O>
    struct PrewittKernel : public IConvolutionKernel<_Tp>
    {
        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::X), bool> = true>
        PrewittKernel()
            : IConvolutionKernel<_Tp>(GeneralizedMatrix<_Tp>({{_Tp(-1), _Tp(0), _Tp(1)}, {_Tp(-1), _Tp(0), _Tp(1)}, {_Tp(-1), _Tp(0), _Tp(1)}})) {}

        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::Y), bool> = true>
        PrewittKernel()
            : IConvolutionKernel<_Tp>(GeneralizedMatrix<_Tp>({{_Tp(1), _Tp(1), _Tp(1)}, {_Tp(0), _Tp(0), _Tp(0)}, {_Tp(-1), _Tp(-1), _Tp(-1)}})) {}
    };

    /// @brief Sobel differential kernel.
    /// @tparam _Tp Component type.
    /// @tparam _O Orientation.
    template <typename _Tp, Orientation _O>
    struct SobelKernel : public IConvolutionKernel<_Tp>
    {
        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::X), bool> = true>
        SobelKernel()
            : IConvolutionKernel<_Tp>(GeneralizedMatrix<_Tp>({{_Tp(-1), _Tp(0), _Tp(1)}, {_Tp(-2), _Tp(0), _Tp(2)}, {_Tp(-1), _Tp(0), _Tp(1)}})) {}

        template <int..., Orientation I = _O, std::enable_if_t<(I == Orientation::Y), bool> = true>
        SobelKernel()
            : IConvolutionKernel<_Tp>(GeneralizedMatrix<_Tp>({{_Tp(1), _Tp(2), _Tp(1)}, {_Tp(0), _Tp(0), _Tp(0)}, {_Tp(-1), _Tp(-2), _Tp(-1)}})) {}
    };

#pragma endregion Convolution

#pragma region Convert
    /// @brief Contains functions for unit/concept converting.
    namespace Convert
    {
        template <typename _TParam = float, typename _TRet = float>
        constexpr inline const _TRet
        InchToMM(const _TParam inch)
        {
            return _TRet(inch) * _TRet(25.4);
        }

        template <typename _TParam = int, typename _TRet = float>
        constexpr inline const _TRet
        DegreeToRadians(const _TParam deg)
        {
            return _TRet(deg) * kPi<_TRet> / _TRet(180);
        }

        /// @brief Get black body color @ thermodynamic temperature T.
        /// @param t T
        /// @return 
        const Vector3f BlackBody(const float t);
        /// @brief Convert XYZ color to RGB color.
        /// @param x 
        /// @return 
        const Vector3f XYZToSRGB(const Vector3f &x);

        /// @brief Contains all color standards available in RenderToy.
        enum class ColorStandard
        {
            /// @brief ITU-R BT.709 http://www.itu.int/rec/R-REC-BT.709
            kITURBT709 = 0,
            /// @brief ITU-R BT.601 http://www.itu.int/rec/R-REC-BT.601
            kITURBT601 = 1,
            /// @brief ITU-R BT.2020 https://www.itu.int/rec/R-REC-BT.2020/
            kITURBT2020 = 2,
            /// @brief SMPTE 240M https://www5.in.tum.de/lehre/vorlesungen/graphik/info/csc/COL_33.htm
            kSMPTE240M = 3,
            /// @brief Average. Take the average of the three brightnesses of RGB.
            kAverage = 4
        };

        /// @brief Get the luminance of given color.
        /// @tparam _CS 
        /// @param vec 
        /// @return 
        template <ColorStandard _CS = ColorStandard::kITURBT709>
        constexpr inline const float
        Luma(const Vector3f &vec)
        {
            if constexpr (_CS == ColorStandard::kITURBT709)
            {
                // Separating components keeps constexpr.
                return 0.2126f * vec.x() + 0.7152f * vec.y() + 0.0722f * vec.z();
            }
            else if constexpr (_CS == ColorStandard::kITURBT601)
            {
                return 0.299f * vec.x() + 0.587f * vec.y() + 0.114f * vec.z();
            }
            else if constexpr (_CS == ColorStandard::kITURBT2020)
            {
                return 0.2627f * vec.x() + 0.6780f * vec.y() + 0.0593f * vec.z();
            }
            else if constexpr (_CS == ColorStandard::kSMPTE240M)
            {
                return 0.2122f * vec.x() + 0.7013f * vec.y() + 0.0865f * vec.z();
            }
            else if constexpr (_CS == ColorStandard::kAverage)
            {
                return (vec.x() + vec.y() + vec.z()) / 3.0f;
            }
        }

        /// @brief Tone mapping function.
        /// @tparam _CS 
        /// @param vec 
        /// @param limit 
        /// @return 
        template <ColorStandard _CS = ColorStandard::kITURBT709>
        [[nodiscard]] const Vector3f
        Tonemap(const Vector3f &vec, const float limit)
        {
            return vec * 1.0f / (1.0f + Luma<_CS>(vec) / limit);
        }
    };
#pragma endregion Convert

#pragma region Drawing
    /// @brief Represents a point in discrete raster space.
    struct PointN
    {
        int x, y;

        PointN();
        PointN(int x_, int y_);
        PointN(const Vector2f &vec);
        PointN(const std::array<int, 2> &tuple);

        const PointN operator+(const PointN &p) const;
        void SizeClamp(const SizeN &size);
    };

    /// @brief Represents a size of a rectangular area in discrete raster space.
    struct SizeN
    {
        std::size_t width, height;

        SizeN();
        SizeN(std::size_t width_, std::size_t height_);
        SizeN(const Vector2f &vec);
        SizeN(const std::array<std::size_t, 2> &tuple);

        /// @brief Get the area of the rectangular area.
        /// @return 
        const std::size_t Area() const;
        /// @brief Get the aspect ratio of the rectangular area.
        /// @return 
        const float AspectRatio() const;
        /// @brief Get the center coordinate of the rectangular area.
        /// @return 
        const PointN Center() const;
    };

#pragma endregion Drawing

#pragma region AffineTransformation
    /// @brief Affine space geometry helpers.
    namespace AffineTransformation
    {
        const Matrix4x4f Translation(const Vector3f &origin);

        /// @brief Use Blender XYZ-Euler convention, matrix representation of intrinsic transformations: Z*Y*X*v.
        /// @param euler_xyz XYZ-Euler angles, in radius.
        /// @return
        const Matrix4x4f RotationEulerXYZ(const Vector3f &euler_xyz);
    };
#pragma endregion AffineTransformation

#pragma region Misc
    const float SSE_InvSqrt(const float number);

    /// @brief Perform linear interpolation.
    /// @tparam _Tp Type of variables.
    /// @tparam _TpCoeff Type of coefficient.
    /// @param x 
    /// @param y 
    /// @param a 
    /// @return x * (1 - a) + y * a
    template <typename _Tp, typename _TpCoeff>
    const _Tp Lerp(const _Tp x, const _Tp y, const _TpCoeff a)
    {
        return x * (_TpCoeff(1) - a) + y * a;
    }
#pragma endregion Misc

#pragma region Interpolation

    /// @brief Interpolation helpers.
    namespace Interpolation
    {
        /// @brief Polynomial interpolation on 2D space.
        struct Polynomial2D
        {
            std::vector<float> t;
            Polynomial2D(const std::vector<Vector2f> &p);
        };
    }

#pragma endregion Interpolation

#pragma region Random
    /// @brief Random helpers.
    namespace Random
    {
        const int Int(int min, int max);
        const float Float();
    };
#pragma endregion

};
#endif // RTMATH_H