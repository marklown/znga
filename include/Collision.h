#ifndef ZNGA_COLLISION_H
#define ZNGA_COLLISION_H

namespace Znga {
namespace Physics {

struct AABB
{
    float xmin = 0;
    float xmax = 0;
    float ymin = 0;
    float ymax = 0;
    float zmin = 0;
    float zmax = 0;
    AABB(float mx, float Mx, float my, float My, float mz, float Mz)
        : xmin(mx), xmax(Mx), ymin(my), ymax(My), zmin(mz), zmax(Mz) {}

    bool Collides(const AABB& other)
    {
        return (xmin <= other.xmax && xmax >= other.xmin) &&
               (ymin <= other.ymax && ymax >= other.ymin) &&
               (zmin <= other.zmax && zmax >= other.zmin);
    }
};

}
}

#endif