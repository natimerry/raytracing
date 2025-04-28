#ifndef vector_H_
#define vector_H_

#include <cmath>
#include <ostream>
class Vec3
{
  private:
    double e[3];

  public:
    Vec3()
        : e(0, 0, 0)
    {
    }

    Vec3(double x, double y, double z)
        : e(x, y, z)
    {
    }

    double x() const
    {
        return e[0];
    }

    double y() const
    {
        return e[1];
    }

    double z() const
    {
        return e[2];
    }

    Vec3 operator-() const
    {
        // Just negate the vector to reverse the directions
        return Vec3(this->x(), this->y(), this->z()); // TODO: check in godbolt maybe just use indexing?
    }

    double operator[](int i) const
    {
        return e[i];
    }

    double& operator[](int i)
    {
        return e[i];
    }

    // Arithmetic on vectors
    Vec3& operator+=(const Vec3& v)
    {
        e[0] += v.x();
        e[1] += v.y();
        e[2] += v.z();
        return *this;
    }

    // This is for DOT product
    Vec3& operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double length() const
    {
        return std::sqrt(this->length_squared());
    }
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
    return Vec3(u.x() + v.x(), u.y() + v.y(), v.z() + u.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
    return Vec3(u.x() - v.x(), u.y() - v.y(), v.z() - u.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
    return Vec3(u.x() * v.x(), u.y() * v.y(), v.z() * u.z());
}

inline Vec3 operator*(const Vec3& u, double t)
{
    return Vec3(u.x() * t, u.y() * t, u.z() * t);
}

inline Vec3 operator*(double t, const Vec3& u)
{
    return u * t;
}

inline Vec3 operator/(const Vec3& v, double t)
{
    return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v)
{
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
    return Vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}

inline Vec3 unit_vec(const Vec3& v)
{
    return v / v.length();
}

#endif // vector_H_