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

color::Color3 Ray::ray_color()
{
    auto center = Point3(-0.3, 0.5, -2);
    auto t = hit_sphere(center, 0.5, *this);
    Point3 hit_point = this->at(t);

    Vec3 normal = unit_vec(hit_point - center);

    if (t > 0.0)
    {
        Vec3 view_dir = unit_vec(this->direction());

        double cos_theta = std::max(0.0, dot(normal, view_dir));

        double k = 2.0; // Steeper curve
        double angle_exposure = 1.0 / (1.0 + std::exp(-k * cos_theta));

        double alpha = 0.05;
        double distance_exposure = 1.0 / (1.0 + alpha * t);

        double exposure = angle_exposure * distance_exposure;

        return exposure * color::Color3(1, 1, 1);
    }

    Vec3 unit_direction = unit_vec(this->direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color::Color3(1.0, 1.0, 1.0) + a * color::Color3(0.5, 0.7, 1.0);
}
