#include "colors.hpp"

#include "utils/logging.hpp"
#include "vector.hpp"
#include <cmath>
#include <rays.hpp>

double hit_sphere(const Point3& center, double radius, const Ray& r)
{
    Vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = -dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (h - std::sqrt(discriminant)) / a;
    }
}

color::Color3 ray_color(const Ray& r)
{
    auto t = hit_sphere(Point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        Point3 hit_point = r.at(t);                          // Compute the hit point
        Vec3 normal = unit_vec(hit_point - Point3(0, 0, 0)); // Normal is from center to point
        logging::trace("{}", normal.print());
        auto exposure = 0.5;
        return exposure * color::Color3(normal.x() + 1, normal.y() + 1, normal.z() + 1);
        // Map normal from (-1,1) -> (0,1)
    }

    Vec3 unit_direction = unit_vec(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color::Color3(1.0, 1.0, 1.0) + a * color::Color3(0.5, 0.7, 1.0);
}
