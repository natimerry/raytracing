#ifndef hittable_H_
#define hittable_H_

#include "vector.hpp"
#include <rays.hpp>

class HitRecord
{
  public:
    Point3 p;
    Vec3 normal;
    double t{};
    bool front_face{};

    void set_face_normal(const Ray& r, const Vec3& outward_normal);
};

class HittableObject
{
  public:
    virtual ~HittableObject() = default;
    virtual bool hit(const Ray& r, double ray_t_min, double ray_t_maxm, HitRecord& record) const = 0;
};

#endif // hittable_H_