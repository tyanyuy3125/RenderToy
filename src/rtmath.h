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
        Vector(const _Tp& value)
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
        inline const Vector
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
            Vector<_Tp, _Dm> ret(*this);
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] += a.arr[i];
            }
            return ret;
        }
        inline const Vector
        operator-(const Vector &a) const
        {
            Vector<_Tp, _Dm> ret(*this);
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] -= a.arr[i];
            }
            return ret;
        }
        inline const Vector
        operator-(void) const
        {
            Vector<_Tp, _Dm> ret(*this);
            for (size_t i = 0; i < _Dm; ++i)
            {
                ret.arr[i] = -ret.arr[i];
            }
            return ret;
        }
        inline const Vector
        operator*(const _Tp a) const
        {
            Vector<_Tp, _Dm> ret(*this);
            for (auto &elem : ret.arr)
            {
                elem *= a;
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
            Vector<_Tp, _Dm> ret(*this);
            for (_Tp &elem : ret.arr)
            {
                elem /= a;
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
            return !(arr == a.arr);
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

    using Vector2d = Vector<double, 2>;
    using Vector3d = Vector<double, 3>;
    using Vector4d = Vector<double, 4>;

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;

#pragma endregion

#pragma region Basic Matrix Math
    class Matrix3x3f
    {
    private:
        Vector3f row[3];

    public:
        const static Matrix3x3f I;
        const static Matrix3x3f O;

        Matrix3x3f();
        Matrix3x3f(const Matrix3x3f &a);
        Matrix3x3f(Vector3f r0, Vector3f r1, Vector3f r2);
        Matrix3x3f(std::array<Vector3f, 3> triple);

        Vector3f &operator[](const size_t i);
        const Vector3f &operator[](const size_t i) const;

        const bool operator==(const Matrix3x3f &a) const;

        const Matrix3x3f operator+(const Matrix3x3f &a) const;
        const Matrix3x3f operator-(const Matrix3x3f &a) const;
        const Matrix3x3f operator-(void) const;
        const Vector3f operator*(const Vector3f &a) const;
        const Matrix3x3f operator*(const Matrix3x3f &a) const;

        const Matrix3x3f &operator+=(const Matrix3x3f &a);
        const Matrix3x3f &operator-=(const Matrix3x3f &a);

        void Transpose();
        const Matrix3x3f Transposed() const;

        const static float Determinant(const Matrix3x3f &mat);
    };

    class Matrix4x4f
    {
    private:
        Vector4f row[4];

    public:
        const static Matrix4x4f I;
        const static Matrix4x4f O;

        Matrix4x4f();
        Matrix4x4f(const Matrix4x4f &a);
        Matrix4x4f(Vector4f r0, Vector4f r1, Vector4f r2, Vector4f r3);
        Matrix4x4f(std::array<Vector4f, 4> quadruple);

        Vector4f &operator[](const size_t i);
        const Vector4f &operator[](const size_t i) const;

        const bool operator==(const Matrix4x4f &a) const;

        const Matrix4x4f operator+(const Matrix4x4f &a) const;
        const Matrix4x4f operator-(const Matrix4x4f &a) const;
        const Matrix4x4f operator-(void) const;
        const Vector4f operator*(const Vector4f &a) const;
        const Matrix4x4f operator*(const Matrix4x4f &a) const;

        const Matrix4x4f &operator+=(const Matrix4x4f &a);
        const Matrix4x4f &operator-=(const Matrix4x4f &a);

        void Transpose();
        const Matrix4x4f Transposed() const;
        const Matrix3x3f ComplementMinor(const int i, const int j) const;
    };

    class Matrix2x2f
    {
    private:
        Vector2f row[2];

    public:
        const static Matrix2x2f I;
        const static Matrix2x2f O;

        Matrix2x2f();
        Matrix2x2f(const Matrix2x2f &a);
        Matrix2x2f(Vector2f r0, Vector2f r1);
        Matrix2x2f(std::array<Vector2f, 2> tuple);

        Vector2f &operator[](const size_t i);
        const Vector2f &operator[](const size_t i) const;

        const bool operator==(const Matrix2x2f &a) const;

        const Matrix2x2f operator+(const Matrix2x2f &a) const;
        const Matrix2x2f operator-(const Matrix2x2f &a) const;
        const Matrix2x2f operator-(void) const;
        const Vector2f operator*(const Vector2f &a) const;
        const Matrix2x2f operator*(const Matrix2x2f &a) const;

        const Matrix2x2f &operator+=(const Matrix2x2f &a);
        const Matrix2x2f &operator-=(const Matrix2x2f &a);

        void Transpose();
        const Matrix2x2f Transposed() const;

        const static float Determinant(const Matrix2x2f &mat);
    };
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