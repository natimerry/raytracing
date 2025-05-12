#include <hittable.h>
void HitRecord::set_face_normal(const Ray& r, const Vec3& outward_normal)
{
    // OUTWARD NORMAL SHOULD HAVE UNIT LENGTH
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}