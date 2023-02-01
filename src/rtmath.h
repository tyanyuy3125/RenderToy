#ifndef RTMATH_H
#define RTMATH_H

#define EPS 1e-7
#define INF 1e7

#include <array>
#include <istream>
#include <stack>
#include <cmath>
#include <algorithm>
#include <initializer_list>
#include <complex>
#include <type_traits>

#include "random.h"

namespace RenderToy
{
    struct Size;
    struct Point;

#pragma region Basic Vector Math
    template <typename _Tp, std::size_t _Dm>
    struct VectorConstants;
    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool> = true>
    struct Matrix;

    /// @brief Generalized Vector definition.
    /// @tparam _Tp Numerical type.
    /// @tparam _Dm Dimension.
    template <typename _Tp, std::size_t _Dm>
    struct Vector : VectorConstants<_Tp, _Dm>
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
            : arr{std::forward<Args>(values)...} {}

        /// @brief Construct a vector with all components the same value.
        /// @param value
        Vector(const _Tp &value)
            : arr()
        {
            std::generate(arr.begin(), arr.end(), [&]()
                          { return value; });
        }

        /// @brief
        /// @return Returns length of the vector.
        inline const _Tp
        Length() const
        {
            _Tp ret = _Tp(0);
            for (_Tp a : arr)
                ret += a * a;
            return std::sqrt(ret);
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
                if (std::abs(arr[i] - a.arr[i]) > EPS)
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

        inline const static _Tp Dot(const Vector &a, const Vector &b)
        {
            _Tp ret(0);
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret += a.arr[i] * b.arr[i];
            }
            return ret;
        }

        inline const static Vector
        Pow(const Vector &a, const Vector &b)
        {
            Vector ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = std::pow(a.arr[i], b.arr[i]);
            }
            return ret;
        }

        const static Vector
        Log(const Vector &a)
        {
            Vector ret;
            for (std::size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = std::log(a.arr[i]);
            }
            return ret;
        }

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

        template <int..., size_t I = _Dm, std::enable_if_t<I == 2, bool> = true>
        constexpr inline const static Vector
        Cross(const Vector &a, const Vector &b)
        {
            return a.arr[0] * b.arr[1] - a.arr[1] * b.arr[0];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<I == 2, bool> = true>
        constexpr inline const Vector
        Cross(const Vector &a) const
        {
            return arr[0] * a.arr[1] - arr[1] * a.arr[0];
        }

        template <int..., size_t I = _Dm, std::enable_if_t<I == 3, bool> = true>
        constexpr inline const static Vector
        Cross(const Vector &a, const Vector &b)
        {
            return Vector(a.arr[1] * b.arr[2] - a.arr[2] * b.arr[1],
                          a.arr[2] * b.arr[0] - a.arr[0] * b.arr[2],
                          a.arr[0] * b.arr[1] - a.arr[1] * b.arr[0]);
        }

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

        friend Matrix<_Tp, _Dm>;

    private:
        std::array<_Tp, _Dm> arr = {};
        // _Tp arr[d] = {};
    };

    template <typename _Tp, size_t _Dm>
    struct VectorConstants
    {
    };

    template <typename _Tp>
    struct VectorConstants<_Tp, 2>
    {
        constexpr static Vector<_Tp, 2> O = Vector<_Tp, 2>(_Tp(0), _Tp(0));
        constexpr static Vector<_Tp, 2> X = Vector<_Tp, 2>(_Tp(1), _Tp(0));
        constexpr static Vector<_Tp, 2> Y = Vector<_Tp, 2>(_Tp(0), _Tp(1));
    };

    template <typename _Tp>
    struct VectorConstants<_Tp, 3>
    {
        constexpr static Vector<_Tp, 3> O = Vector<_Tp, 3>(_Tp(0), _Tp(0), _Tp(0));
        constexpr static Vector<_Tp, 3> X = Vector<_Tp, 3>(_Tp(1), _Tp(0), _Tp(0));
        constexpr static Vector<_Tp, 3> Y = Vector<_Tp, 3>(_Tp(0), _Tp(1), _Tp(0));
        constexpr static Vector<_Tp, 3> Z = Vector<_Tp, 3>(_Tp(0), _Tp(0), _Tp(1));
        constexpr static Vector<_Tp, 3> White = Vector<_Tp, 3>(_Tp(1), _Tp(1), _Tp(1));
    };

    template <typename _Tp>
    struct VectorConstants<_Tp, 4>
    {
        constexpr static Vector<_Tp, 4> O = Vector<_Tp, 4>(_Tp(0), _Tp(0), _Tp(0), _Tp(0));
        constexpr static Vector<_Tp, 4> X = Vector<_Tp, 4>(_Tp(1), _Tp(0), _Tp(0), _Tp(0));
        constexpr static Vector<_Tp, 4> Y = Vector<_Tp, 4>(_Tp(0), _Tp(1), _Tp(0), _Tp(0));
        constexpr static Vector<_Tp, 4> Z = Vector<_Tp, 4>(_Tp(0), _Tp(0), _Tp(1), _Tp(0));
        constexpr static Vector<_Tp, 4> W = Vector<_Tp, 4>(_Tp(0), _Tp(0), _Tp(0), _Tp(1));
    };

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

#pragma endregion

#pragma region Basic Matrix Math

    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool> = true>
    struct MatrixConstants;

    template <typename _Tp, size_t _Dm, std::enable_if_t<(_Dm >= 2), bool>>
    struct Matrix : public MatrixConstants<_Tp, _Dm>
    {
        typedef Vector<_Tp, _Dm> VectorType;

        constexpr Matrix() = default;

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 2), bool> = true>
        constexpr Matrix(Vector2<_Tp> r0, Vector2<_Tp> r1)
            : row{r0, r1} {}
        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 2), bool> = true>
        constexpr Matrix(std::array<Vector2<_Tp>, 2> tuple)
            : row(tuple) {}

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 3), bool> = true>
        constexpr Matrix(Vector3<_Tp> r0, Vector3<_Tp> r1, Vector3<_Tp> r2)
            : row{r0, r1, r2} {}
        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 3), bool> = true>
        constexpr Matrix(std::array<Vector3<_Tp>, 3> triple)
            : row(triple) {}

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 4), bool> = true>
        constexpr Matrix(Vector4<_Tp> r0, Vector4<_Tp> r1, Vector4<_Tp> r2, Vector4<_Tp> r3)
            : row{r0, r1, r2, r3} {}
        template <int..., std::size_t I = _Dm, std::enable_if_t<(I == 4), bool> = true>
        constexpr Matrix(std::array<Vector4<_Tp>, 4> quadruple)
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
        constexpr inline const _Tp ComplementMinor(const size_t i, const size_t j) const
        {
            return row[i ^ 1][j ^ 1];
        }

        template <int..., std::size_t I = _Dm, std::enable_if_t<(I >= 3), bool> = true>
        const Matrix<_Tp, I - 1> ComplementMinor(const size_t i, const size_t j) const
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
        constexpr const static Matrix<_Tp, 2> I = {{_Tp(1), _Tp(0)},
                                                   {_Tp(0), _Tp(1)}};
        constexpr const static Matrix<_Tp, 2> O = {{_Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0)}};
    };

    template <typename _Tp>
    struct MatrixConstants<_Tp, 3>
    {
        constexpr const static Matrix<_Tp, 3> I = {{_Tp(1), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(1), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(1)}};
        constexpr const static Matrix<_Tp, 3> O = {{_Tp(0), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(0)}};
    };

    template <typename _Tp>
    struct MatrixConstants<_Tp, 4>
    {
        constexpr const static Matrix<_Tp, 4> I = {{_Tp(1), _Tp(0), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(1), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(1), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(0), _Tp(1)}};
        constexpr const static Matrix<_Tp, 4> O = {{_Tp(0), _Tp(0), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(0), _Tp(0)},
                                                   {_Tp(0), _Tp(0), _Tp(0), _Tp(0)}};
    };

    using Matrix2x2f = Matrix<float, 2>;
    using Matrix3x3f = Matrix<float, 3>;
    using Matrix4x4f = Matrix<float, 4>;

    using Matrix2x2d = Matrix<double, 2>;
    using Matrix3x3d = Matrix<double, 3>;
    using Matrix4x4d = Matrix<double, 4>;

#pragma endregion

#pragma region Unit Conversion
    struct Convert
    {
        Convert() = delete;
        Convert(const Convert &) = delete;
        Convert(const Convert &&) = delete;

        static const float InchToMM(const float inch);
        static const float DegreeToRadians(const float deg);
        static const Vector3f BlackBody(const float t);
        static const Vector3f XYZToSRGB(const Vector3f &x);

        static const float RGBToLuminance(const Vector3f &vec);
        static const Vector3f Tonemap(const Vector3f &vec, const float limit);
    };
#pragma endregion

#pragma region Drawing Math
    struct Point
    {
        int x, y;

        Point();
        Point(int x_, int y_);
        Point(const Vector2f &vec);
        Point(const std::array<int, 2> &tuple);
    };

    struct Size
    {
        int width, height;

        Size();
        Size(int width_, int height_);
        Size(const Vector2f &vec);
        Size(const std::array<int, 2> &tuple);

        const int Area() const;
        const float AspectRatio() const;
    };

#pragma endregion

    class Matrix4x4fStack
    {
    private:
        std::stack<Matrix4x4f> accumulated;

    public:
        Matrix4x4fStack();

        void Push(const Matrix4x4f &mat4);
        void Pop();

        void Transform(Vector3f &Vector3f) const;
    };

    class AffineTransformation
    {
    public:
        AffineTransformation() = delete;
        AffineTransformation(const AffineTransformation &) = delete;
        AffineTransformation(const AffineTransformation &&) = delete;

        const static Matrix4x4f Translation(const Vector3f &origin);

        /// @brief Use Blender XYZ-Euler convention, matrix representation of intrinsic transformations: Z*Y*X*v.
        /// @param euler_xyz XYZ-Euler angles, in radius.
        /// @return
        const static Matrix4x4f RotationEulerXYZ(const Vector3f &euler_xyz);
    };

    const float SSE_InvSqrt(const float number);

    const float GTR1(const float NDotH, const float a);
    const Vector3f SampleGTR1(const float rgh, const float r1, const float r2);
    const float GTR2(const float NDotH, const float a);
    const Vector3f SampleGTR2(const float rgh, const float r1, const float r2);
    const Vector3f SampleGGXVNDF(const Vector3f V, const float rgh, const float r1, const float r2);
    const float GTR2Aniso(const float NDotH, const float HDotX, const float HDotY, const float ax, const float ay);
    const Vector3f SampleGTR2Aniso(const float ax, const float ay, const float r1, const float r2);
    const float SmithG(const float NDotV, const float alphaG);
    const float SmithGAniso(const float NDotV, const float VDotX, const float VDotY, const float ax, const float ay);
    const float SchlickFresnel(const float u);
    const float DielectricFresnel(const float cosThetaI, const float eta);

    const Vector3f CosineSampleHemisphere(const float r1, const float r2);
    const Vector3f UniformSampleHemisphere(const float r1, const float r2);
    const Vector3f UniformSampleSphere(const float r1, const float r2);

    const float PowerHeuristic(const float a, const float b);
    void Onb(const Vector3f N, Vector3f &T, Vector3f &B);

    const Vector3f ToWorld(const Vector3f X, const Vector3f Y, const Vector3f Z, const Vector3f V);
    const Vector3f ToLocal(const Vector3f X, const Vector3f Y, const Vector3f Z, const Vector3f V);
    const Vector3f Refract(const Vector3f &incidentVec, const Vector3f &normal, float eta);
    const Vector3f Reflect(const Vector3f &incidentVec, const Vector3f &normal);

    template <typename _Tp>
    const _Tp Mix(const _Tp x, const _Tp y, const _Tp a)
    {
        return x * (_Tp(1) - a) + y * a;
    }

}
#endif // RTMATH_H