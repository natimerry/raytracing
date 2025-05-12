#ifndef sphere_H_
#define sphere_H_

#include <hittable.h>
#include "../hittable.h"
#include "../vector.hpp"
#include <cmath>
class Sphere : public HittableObject
{
  public:
    Sphere(const Point3& center, double radius);
    ~Sphere();

    bool hit(const Ray&r, float t_min, float t_max, HitRecord& rec) const override;

  private:
    Point3 center;
    double radius;
};

#endif // sphere_H_