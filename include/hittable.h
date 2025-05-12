#ifndef hittable_H_
#define hittable_H_

#include <vector>
#include <memory>
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
    virtual bool hit(const Ray& ray, float ray_t_min, float ray_t_max, HitRecord& record) const = 0;
};

class HitList : public HittableObject
{
public:
    std::vector<std::shared_ptr<HittableObject>> objects;

    explicit HitList(const std::vector<std::shared_ptr<HittableObject>>& objects)
    {
        this->objects = objects;
    }
    explicit HitList(const std::shared_ptr<HittableObject>& object)
    {
        objects.push_back(object);
    }

    void clear()
    {
        objects.clear();
    }
    void add(const std::shared_ptr<HittableObject>& object);

    bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const override
    {
        HitRecord temporary_record;
        auto hit_anything = false;
        auto closest = t_max;

        for (const auto& object : objects)
        {
            if (object->hit(ray, t_min, closest, temporary_record))
            {
                hit_anything = true;
                closest = temporary_record.t;
                rec = temporary_record;
            }
        }

        return hit_anything;
    }
};

#endif // hittable_H_