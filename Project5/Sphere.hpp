#pragma once

#include "Object.hpp"
#include "Vector.hpp"

class Sphere : public Object
{
public:
    Sphere(const Vector3f& c, const float& r)
        : center(c)
        , radius(r)
        , radius2(r * r)
    {}

    // 用课件里学到 Ray Intersection With Sphere方法计算
    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t&, Vector2f&) const override
    {

        // analytic solution             公式（o + td -c）^2 - R^2 = 0
        Vector3f L = orig - center;
        float a = dotProduct(dir, dir);         //  a = d 点乘d
        float b = 2 * dotProduct(dir, L);       //  b = 2(o - c)点乘d
        float c = dotProduct(L, L) - radius2;   //  c = （o - c）(o - c) - R^2 
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1))
            return false;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)   // 如果两个解都是小于0的，则都无效
            return false;
        tnear = t0;

        return true;
    }

    void getSurfaceProperties(const Vector3f& P, const Vector3f&, const uint32_t&, const Vector2f&,
                              Vector3f& N, Vector2f&) const override
    {
        N = normalize(P - center);
    }

    Vector3f center;
    float radius, radius2;
};
