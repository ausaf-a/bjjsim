#ifndef GRAPPLEMAP_MATH_HPP
#define GRAPPLEMAP_MATH_HPP

#include <cmath>
#include <iostream>
#include <tuple>
#include <array>
#include <cassert>

namespace GrappleMap {

constexpr double pi() { return 3.141593; }

template<typename T>
struct BasicV3
{
	T x, y, z;

	BasicV3() {}

	BasicV3(T x_, T y_, T z_)
		: x(x_), y(y_), z(z_)
	{}

	template<typename U>
	BasicV3(BasicV3<U> const & v)
		: x(v.x), y(v.y), z(v.z)
	{
		// todo: static_assert(std::is_convertible_v<U, T>); (or enable_if)
	}
};

using V3f = BasicV3<float>;
using V3 = BasicV3<double>;

struct V2 { double x, y; };

inline V3f to_f(V3 v) { return {float(v.x), float(v.y), float(v.z)}; }

inline V3 y0(V2 v) { return {v.x, 0, v.y}; }

template<typename T>
struct BasicV4
{
	T x, y, z, w;

	BasicV4() = default;

	BasicV4(T x_, T y_, T z_, T w_)
		: x(x_), y(y_), z(z_), w(w_)
	{}

	BasicV4(BasicV3<T> v, T w)
		: x(v.x), y(v.y), z(v.z), w(w)
	{}
};

using V4f = BasicV4<float>;
using V4 = BasicV4<double>;

V3f const
	red{1,0,0}, blue{0.1, 0.1, 0.9}, grey{0.2, 0.2, 0.2},
	yellow{1,1,0}, green{0,1,0}, white{1,1,1}, black{0,0,0};

inline double inner_prod(V2 a, V2 b) { return a.x * b.x + a.y * b.y; }

template<typename T>
inline T inner_prod(BasicV3<T> a, BasicV3<T> b)
{ return a.x * b.x + a.y * b.y + a.z * b.z; }

using M = std::array<double, 16>;

inline M yrot(double a) // formalized
{
	return
		{{ cos(a), 0, -sin(a), 0
		, 0, 1, 0, 0
		, sin(a), 0, cos(a), 0
		, 0, 0, 0, 1 }};
}

inline M xrot(double a)
{
	return
		{{ 1, 0, 0, 0
		, 0, cos(a), -sin(a), 0
		, 0, sin(a), cos(a), 0
		, 0, 0, 0, 1 }};
}

inline M translate(V3 v)
{
	return {
		{ 1, 0, 0, 0
		, 0, 1, 0, 0
		, 0, 0, 1, 0
		, v.x, v.y, v.z, 1 }};
}

template<typename T>
inline BasicV3<T> cross(BasicV3<T> a, BasicV3<T> b)
{
	return
		{ a.y * b.z - a.z * b.y
		, a.z * b.x - a.x * b.z
		, a.x * b.y - a.y * b.x };
}

inline V2 xy(V3 v){ return {v.x, v.y}; }
inline V2 xz(V3 v){ return {v.x, v.z}; }
inline V2 xy(V4 v){ return {v.x, v.y}; }

template<typename T>
inline BasicV3<T> xyz(BasicV4<T> v){ return {v.x, v.y, v.z}; }

inline M perspective(double fovy, double aspect, double zNear, double zFar)
{
	auto f = 1/tan(fovy*pi()/360);

	return
		{{ f/aspect, 0, 0, 0
		, 0, f, 0, 0
		, 0, 0, (zFar+zNear)/(zNear-zFar), -1
		, 0, 0, 2*zFar*zNear/(zNear-zFar), 0 }};
}

inline double norm2(V2 v){ return sqrt(inner_prod(v, v)); }

template<typename T>
inline T norm2(BasicV3<T> v){ return sqrt(inner_prod(v, v)); }

inline V2 operator-(V2 a, V2 b) { return {a.x - b.x, a.y - b.y}; }

template<typename T>
inline BasicV3<T> operator-(BasicV3<T> a, BasicV3<T> b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

inline V2 operator-(V2 v) { return {-v.x, -v.y}; }
inline V3 operator-(V3 v) { return {-v.x, -v.y, -v.z}; }
inline V4 operator-(V4 a, V4 b) { return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}; }
inline V2 operator+(V2 a, V2 b) { return {a.x + b.x, a.y + b.y}; }

template<typename T>
inline BasicV3<T> operator+(BasicV3<T> a, BasicV3<T> b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }

template<typename T>
inline BasicV4<T> operator+(BasicV4<T> a, BasicV4<T> b) { return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}; }

inline V2 operator*(V2 v, double s) { return {v.x * s, v.y * s}; }

template<typename T>
inline BasicV3<T> operator*(BasicV3<T> v, T s) { return {v.x * s, v.y * s, v.z * s}; }

inline V2 & operator+=(V2 & a, V2 b) { return a = a + b; }
inline V3 & operator+=(V3 & a, V3 b) { return a = a + b; }
inline V4 & operator+=(V4 & a, V4 b) { return a = a + b; }
inline V2 & operator-=(V2 & a, V2 b) { return a = a - b; }
inline V3 & operator-=(V3 & a, V3 b) { return a = a - b; }
inline V4 & operator-=(V4 & a, V4 b) { return a = a - b; }


inline bool operator<(V2 a, V2 b) { return std::make_tuple(a.x, a.y) < std::make_tuple(b.x, b.y); }

template<typename T>
inline bool operator<(BasicV3<T> a, BasicV3<T> b) { return std::make_tuple(a.x, a.y, a.z) < std::make_tuple(b.x, b.y, b.z); }

inline bool operator==(V2 a, V2 b) { return a.x == b.x && a.y == b.y; }
inline bool operator==(V3 a, V3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }

inline std::ostream & operator<<(std::ostream & o, V2 v)
{ return o << '{' << v.x << ',' << v.y << '}'; }

template<typename T>
inline std::ostream & operator<<(std::ostream & o, BasicV3<T> const v)
{ return o << '{' << v.x << ',' << v.y << ',' << v.z << '}'; }

template<typename T>
inline std::ostream & operator<<(std::ostream & o, BasicV4<T> const v)
{ return o << '{' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << '}'; }

inline V2 operator/(V2 v, double s) { return {v.x / s, v.y / s}; }

template<typename T>
inline BasicV3<T> operator/(BasicV3<T> v, T s) { return {v.x / s, v.y / s, v.z / s}; }

template<typename T>
inline BasicV3<T> normalize(BasicV3<T> v){ return v / norm2(v); }

inline double distance(V3 from, V3 to) { return norm2(to - from); }
inline double distanceSquared(V2 from, V2 to) { return inner_prod(to - from, to - from); }
inline double distanceSquared(V3 from, V3 to) { return inner_prod(to - from, to - from); }

template<typename T>
inline BasicV3<T> between(BasicV3<T> const a, BasicV3<T> const b) { return (a + b) / T(2); }

inline V2 between(V2 const a, V2 const b) { return (a + b) / 2.; }

inline V4 operator*(M const & m, V4 const v) // TODO: formalize
{
	return
		{{ m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12]
		, m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13]
		, m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]}
		, m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15]};
}

inline V3 operator*(M const & m, V3 const v)
{
	return xyz(m * V4(v, 1));
}

inline M operator*(M const & a, M const & b)
{
	return
		{{ a[ 0]*b[ 0] + a[ 4]*b[ 1] + a[ 8]*b[ 2] + a[12]*b[ 3]
		,  a[ 1]*b[ 0] + a[ 5]*b[ 1] + a[ 9]*b[ 2] + a[13]*b[ 3]
		,  a[ 2]*b[ 0] + a[ 6]*b[ 1] + a[10]*b[ 2] + a[14]*b[ 3]
		,  a[ 3]*b[ 0] + a[ 7]*b[ 1] + a[11]*b[ 2] + a[15]*b[ 3]

		,  a[ 0]*b[ 4] + a[ 4]*b[ 5] + a[ 8]*b[ 6] + a[12]*b[ 7]
		,  a[ 1]*b[ 4] + a[ 5]*b[ 5] + a[ 9]*b[ 6] + a[13]*b[ 7]
		,  a[ 2]*b[ 4] + a[ 6]*b[ 5] + a[10]*b[ 6] + a[14]*b[ 7]
		,  a[ 3]*b[ 4] + a[ 7]*b[ 5] + a[11]*b[ 6] + a[15]*b[ 7]

		,  a[ 0]*b[ 8] + a[ 4]*b[ 9] + a[ 8]*b[10] + a[12]*b[11]
		,  a[ 1]*b[ 8] + a[ 5]*b[ 9] + a[ 9]*b[10] + a[13]*b[11]
		,  a[ 2]*b[ 8] + a[ 6]*b[ 9] + a[10]*b[10] + a[14]*b[11]
		,  a[ 3]*b[ 8] + a[ 7]*b[ 9] + a[11]*b[10] + a[15]*b[11]

		,  a[ 0]*b[12] + a[ 4]*b[13] + a[ 8]*b[14] + a[12]*b[15]
		,  a[ 1]*b[12] + a[ 5]*b[13] + a[ 9]*b[14] + a[13]*b[15]
		,  a[ 2]*b[12] + a[ 6]*b[13] + a[10]*b[14] + a[14]*b[15]
		,  a[ 3]*b[12] + a[ 7]*b[13] + a[11]*b[14] + a[15]*b[15] }};
}

struct Reorientation
{
	V3 offset;
	double angle;

	Reorientation(V3 o = {0,0,0}, double a = 0)
		: offset(o), angle(a)
	{}
};

inline std::ostream & operator<<(std::ostream & o, Reorientation const r)
{
	return o << "{offset=" << r.offset << ", angle=" << r.angle << "}";
}

inline auto members(Reorientation const & r) { return std::tie(r.offset, r.angle); }

inline V3 apply(Reorientation const & r, V3 v) // formalized
{
	return (yrot(r.angle) * v) + r.offset;
}

inline Reorientation inverse(Reorientation x) // formalized
{
	return {(yrot(-x.angle) * -x.offset), -x.angle};
}

inline Reorientation compose(Reorientation const a, Reorientation const b) // formalized
{
	return {b.offset + (yrot(b.angle) * a.offset), a.angle + b.angle};
}

inline double angle(V2 const v) { return atan2(v.x, v.y); }

inline double cosAngle(V3 a, V3 b)
{
	return inner_prod(a, b) / norm2(a) / norm2(b);
}

inline double scalarProject(V3 a, V3 b)
{
	return norm2(a) * cosAngle(a, b);
}

inline double closest(V3 rayOrigin, V3 rayDir, V3 point)
	// returns c such that rayOrigin+rayDir*c-point is minimal
{
	auto pointDir = point - rayOrigin;
	return scalarProject(pointDir, rayDir) / norm2(rayDir);
}

}

#endif
