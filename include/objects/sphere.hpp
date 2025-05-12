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

    bool hit(const Ray& r, double ray_t_min, double ray_t_maxm, HitRecord& record) const override;

  private:
    Point3 center;
    double radius;
};

#endif // sphere_H_