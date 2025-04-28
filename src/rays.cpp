#include "colors.hpp"
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
    auto center = Point3(-0, 0, -2);
    auto t = hit_sphere(center, 0.5, r);

    if (t > 0.0)
    {
        Point3 hit_point = r.at(t);

        Vec3 normal = unit_vec(hit_point - center);
        Vec3 view_dir = unit_vec(-r.direction());

        double cos_theta = std::max(0.0, dot(normal, view_dir));

        double k = 2.0; // Steeper curve
        double angle_exposure = 1.0 / (1.0 + std::exp(-k * cos_theta));

        double alpha = 0.05;
        double distance_exposure = 1.0 / (1.0 + alpha * t);

        double exposure = angle_exposure * distance_exposure;

        return exposure * color::Color3(1, 1, 1);
    }

    Vec3 unit_direction = unit_vec(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color::Color3(1.0, 1.0, 1.0) + a * color::Color3(0.5, 0.7, 1.0);
}
