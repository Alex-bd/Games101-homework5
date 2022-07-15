#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Renderer.hpp"

// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image width and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
int main()
{
    Scene scene(1280, 960);     //场景大小
    // 创建两个球体，设置圆心、材质和颜色
    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);   //(圆心，半径)
    sph1->materialType = DIFFUSE_AND_GLOSSY;    //材质   可以看作在这个物体上的反射是粗糙反射（介于镜面反射和漫反射之间）与漫反射的叠
    sph1->diffuseColor = Vector3f(0.6, 0.7, 0.8);       // 颜色

    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;    // 折射率
    sph2->materialType = REFLECTION_AND_REFRACTION;  // 可以看作在这个物体上的反射是镜面反射和折射的叠加

    scene.Add(std::move(sph1));   // 在场景中添加两个物体
    scene.Add(std::move(sph2));

    // 创建一个地面矩形网格
    Vector3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};   // 网格的四个顶点
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};         // 两个三角形的顶点序号
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);    // 创建一个三角网格，2是指由两个三角形组成
    mesh->materialType = DIFFUSE_AND_GLOSSY;        // 设置网格材质
    //网格加入场景
    scene.Add(std::move(mesh));
    // 创建两个点光源 并加入场景
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));    

    Renderer r;   // 创建一个渲染器
    r.Render(scene);   // 把创建好的场景加入渲染器渲染

    return 0;
}