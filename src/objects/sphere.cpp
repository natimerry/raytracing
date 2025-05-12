#include <objects/sphere.hpp>

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    const Vec3 oc = center - r.origin();
    const auto a = r.direction().length_squared();
    const auto h = dot(r.direction(), oc);
    const auto c = oc.length_squared() - radius * radius;

    const auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (root <= t_min || t_max <= root)
    {
        root = (h + sqrtd) / a;
        if (root <= t_min || t_max <= root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    // Set the normal based on face

    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

Sphere::Sphere(const Point3& center, double radius)
{
    this->center = center;
    this->radius = radius;
}

Sphere::~Sphere() {}