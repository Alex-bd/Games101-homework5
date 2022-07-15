#pragma once

#include "Object.hpp"

#include <cstring>

// moller Trumbore 算法 判断点是不是在三角形内
// 其公式推导：https://zhizhi.pcwanli.com/front/article/11750.html和理解https://zhuanlan.zhihu.com/p/468132444
//三角形三个顶点P0，P1，P2。u为P1的权重，v为P2的权重，
//而1 - u - v是P0的权重，可以理解为沿着边AC移动一段距离，
//然后再沿着边AB移动一段距离，最后求它们的和向量。
//至于移动多大距离，就是由参数u和v控制的，
//所表达的数学意义是三角形及其内部所有点的方程。
bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u, float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.
    // orig：光线起点；  dir  :光线d
    Vector3f E1, E2, S, S1, S2, re;
    E1 = v1 - v0;   // 三角形一条边
    E2 = v2 - v0;   // 三角形一条边
    S = orig - v0;   // 光线起点到三角形一个顶点的向量
    S1 = crossProduct(dir, E2);
    S2 = crossProduct(S, E1);   // 叉乘
    re = Vector3f(dotProduct(S2, E2), dotProduct(S1, S), dotProduct(S2, dir));
    re = re / dotProduct(S1, E1); // 克莱姆法则
    tnear = re.x;
    u = re.y;
    v = re.z;
    if (tnear > 0 && v >= 0 && v <= 1 && u >= 0 && u <= 1)   // 要同时满足
        return true;
    return false;
}

class MeshTriangle : public Object
{
public:
    MeshTriangle(const Vector3f* verts, const uint32_t* vertsIndex, const uint32_t& numTris, const Vector2f* st)
    {
        uint32_t maxIndex = 0;
        for (uint32_t i = 0; i < numTris * 3; ++i)
            if (vertsIndex[i] > maxIndex)
                maxIndex = vertsIndex[i];
        maxIndex += 1;
        vertices = std::unique_ptr<Vector3f[]>(new Vector3f[maxIndex]);
        memcpy(vertices.get(), verts, sizeof(Vector3f) * maxIndex);
        vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
        memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
        numTriangles = numTris;
        stCoordinates = std::unique_ptr<Vector2f[]>(new Vector2f[maxIndex]);
        memcpy(stCoordinates.get(), st, sizeof(Vector2f) * maxIndex);
    }

    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t& index,
                   Vector2f& uv) const override
    {
        //遍历一个网格中的两个三角形，并对它们运行rayTriangleIntersect函数进行求交计算
        bool intersect = false;
        for (uint32_t k = 0; k < numTriangles; ++k)
        {
            const Vector3f& v0 = vertices[vertexIndex[k * 3]];
            const Vector3f& v1 = vertices[vertexIndex[k * 3 + 1]];
            const Vector3f& v2 = vertices[vertexIndex[k * 3 + 2]];
            float t, u, v;
            if (rayTriangleIntersect(v0, v1, v2, orig, dir, t, u, v) && t < tnear)   // 三角形中，通过Moller_Trumbore算法更新参数
            {
                tnear = t;
                uv.x = u;
                uv.y = v;
                index = k;
                intersect |= true;
            }
        }

        return intersect;
    }

    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t& index, const Vector2f& uv, Vector3f& N,
                              Vector2f& st) const override
    {
        const Vector3f& v0 = vertices[vertexIndex[index * 3]];
        const Vector3f& v1 = vertices[vertexIndex[index * 3 + 1]];
        const Vector3f& v2 = vertices[vertexIndex[index * 3 + 2]];
        Vector3f e0 = normalize(v1 - v0);
        Vector3f e1 = normalize(v2 - v1);
        N = normalize(crossProduct(e0, e1));
        const Vector2f& st0 = stCoordinates[vertexIndex[index * 3]];
        const Vector2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
        const Vector2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
        st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
    }

    Vector3f evalDiffuseColor(const Vector2f& st) const override
    {
        float scale = 5;
        float pattern = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
        return lerp(Vector3f(0.815, 0.235, 0.031), Vector3f(0.937, 0.937, 0.231), pattern);
    }

    std::unique_ptr<Vector3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vector2f[]> stCoordinates;
};
