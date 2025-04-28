#include "colors.hpp"

#include "vector.hpp"
#include <cmath>
#include <rays.hpp>

bool hit_sphere(const Point3& center, double radius, const Ray& r)
{
    Vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

color::Color3 ray_color(const Ray& r)
{
    if (hit_sphere(Point3(0, 0, +1), 0.5, r))
        return color::Color3(1, 0, 0);

    Vec3 unit_direction = unit_vec(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color::Color3(1.0, 1.0, 1.0) + a * color::Color3(0.5, 0.7, 1.0);
}
