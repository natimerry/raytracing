#include "utils/logging.hpp"

#include <objects/sphere.hpp>

Sphere::Sphere(const Point3& center, double radius)
{
    this->center = center;
    this->radius = radius;
}

Sphere::~Sphere() = default;

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    Vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = -dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    const auto discriminant = h * h - a * c;

    if (discriminant < 0) {
        return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (root <= t_min || t_max <= root)
    {
        root = (h + sqrtd) / a;
        if (root <= t_min || t_max <= root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);


    return true;
}