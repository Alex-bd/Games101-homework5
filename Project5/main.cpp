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
    Scene scene(1280, 960);     //������С
    // �����������壬����Բ�ġ����ʺ���ɫ
    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);   //(Բ�ģ��뾶)
    sph1->materialType = DIFFUSE_AND_GLOSSY;    //����   ���Կ�������������ϵķ����Ǵֲڷ��䣨���ھ��淴���������֮�䣩��������ĵ�
    sph1->diffuseColor = Vector3f(0.6, 0.7, 0.8);       // ��ɫ

    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;    // ������
    sph2->materialType = REFLECTION_AND_REFRACTION;  // ���Կ�������������ϵķ����Ǿ��淴�������ĵ���

    scene.Add(std::move(sph1));   // �ڳ����������������
    scene.Add(std::move(sph2));

    // ����һ�������������
    Vector3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};   // ������ĸ�����
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};         // ���������εĶ������
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);    // ����һ����������2��ָ���������������
    mesh->materialType = DIFFUSE_AND_GLOSSY;        // �����������
    //������볡��
    scene.Add(std::move(mesh));
    // �����������Դ �����볡��
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));    

    Renderer r;   // ����һ����Ⱦ��
    r.Render(scene);   // �Ѵ����õĳ���������Ⱦ����Ⱦ

    return 0;
}