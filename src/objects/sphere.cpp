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
    logging::trace("a: {}, h: {}, c: {}, discriminant: {}", a, h, c, discriminant);

    if (discriminant < 0) {
        logging::trace("Ray missed the sphere (discriminant < 0)");
        return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    logging::trace("Trying first root: {}", root);
    if (root <= t_min || t_max <= root)
    {
        logging::trace("First root out of range: [{} <= {} <= {}] is false", t_min, root, t_max);
        root = (h + sqrtd) / a;
        logging::trace("Trying second root: {}", root);
        if (root <= t_min || t_max <= root) {
            logging::trace("Both roots out of range. Ray does not hit sphere.");
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    logging::debug("Hit at t: {}, point: ({}, {}, {}), normal: ({}, {}, {})",
                   rec.t, rec.p.x(), rec.p.y(), rec.p.z(),
                   rec.normal.x(), rec.normal.y(), rec.normal.z());

    return true;
}