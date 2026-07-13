#pragma once
#include "flecs.h"
#include "core/scene/Transform.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/LightManager.h"
#include "core/assets/Camera.h"
#include "core/AudioPipeline/AudioEngine.h"
#include "core/scene/RenderObject.h"

struct TransformComponent {
    Transform transform;
};

//rendering components
struct MeshComponent {
    MeshHandle handle;
};

struct MaterialComponent {
    MaterialHandle handle;
};

struct StaticTag {};
struct QuasiStaticTag {};
struct DynamicSlowTag {};
struct DynamicFastTag {};


//audio components
struct AudioSourceComponent {
    AudioSourceHandle handle; //only a reference to che internal system in audio manager
};

//light components
struct LightComponent {
    LightHandle handle;
    LightDesc desc;  // cached to detect dirty
};

//physics components
struct VelocityComponent {
    Vector3 linear;
    Vector3 angular;
};

struct ColliderComponent {
};

//camera component (should be a singleton)
struct CameraComponent {
    Camera camera;
};

//sigleton services
struct ResourceManagerSingleton  { ResourceManager*  ptr; };
struct LightManagerSingleton     { LightManager*     ptr; };
struct AudioEngineSingleton      { AudioEngine*      ptr; };

//ecs system functions 
//not wrapped in a class because of the already existing metods from flecs
void registerComponents(flecs::world& world);

struct RenderBuckets
{
    std::vector<RenderObject> staticObjects;
    std::vector<RenderObject> quasiStaticObjects;
    std::vector<RenderObject> dynamicSlowObjects;
    std::vector<RenderObject> dynamicFastObjects;

    void clear()
    {
        staticObjects.clear();
        quasiStaticObjects.clear();
        dynamicSlowObjects.clear();
        dynamicFastObjects.clear();
    }

    bool staticDirty = true;
    bool quasiStaticDirty = true;
    bool staticJustRebuilt = false;
    bool quasiStaticJustRebuilt = false;
};

inline void registerRenderExtractSystems(flecs::world& world)
{
    world.system<>("ClearRenderBuckets")
        .kind(flecs::OnLoad)
        .run([](flecs::iter& it)
        {
            it.world().get_mut<RenderBuckets>().dynamicSlowObjects.clear();
            it.world().get_mut<RenderBuckets>().dynamicFastObjects.clear();
        });

    world.system<TransformComponent, MeshComponent, MaterialComponent, StaticTag>("ExtractStaticObjects")
    .kind(flecs::OnUpdate)
    .run([](flecs::iter& it)
    {
        RenderBuckets& buckets = it.world().get_mut<RenderBuckets>();
        
        buckets.staticJustRebuilt = true;
        if (!buckets.staticDirty) return;

        while (it.next())
        {
            auto t   = it.field<TransformComponent>(0);
            auto m   = it.field<MeshComponent>(1);
            auto mat = it.field<MaterialComponent>(2);

            for (auto i : it)
            {
                buckets.staticObjects.push_back({ m[i].handle, mat[i].handle, t[i].transform });
            }
        }

        buckets.staticDirty = false;
    });

    world.system<TransformComponent, MeshComponent, MaterialComponent, QuasiStaticTag>("ExtractQuasiStaticObjects")
    .kind(flecs::OnUpdate)
    .run([](flecs::iter& it)
    {
        RenderBuckets& buckets = it.world().get_mut<RenderBuckets>();
        buckets.quasiStaticJustRebuilt = true;
        if (!buckets.quasiStaticDirty) return;

        while (it.next())
        {
            auto t   = it.field<TransformComponent>(0);
            auto m   = it.field<MeshComponent>(1);
            auto mat = it.field<MaterialComponent>(2);

            for (auto i : it)
            {
                buckets.quasiStaticObjects.push_back({ m[i].handle, mat[i].handle, t[i].transform });
            }
        }

        buckets.quasiStaticDirty = false;
    });

    world.system<TransformComponent, MeshComponent, MaterialComponent>("ExtractDynamicSlowObjects")
        .with<DynamicSlowTag>()
        .kind(flecs::PostUpdate)
        .each([](flecs::entity e, TransformComponent& t, MeshComponent& m, MaterialComponent& mat)
        {
            RenderBuckets& buckets = e.world().get_mut<RenderBuckets>();
            buckets.dynamicSlowObjects.push_back({ m.handle, mat.handle, t.transform });
        });

    world.system<TransformComponent, MeshComponent, MaterialComponent>("ExtractDynamicFastObjects")
        .with<DynamicFastTag>()
        .kind(flecs::PostUpdate)
        .each([](flecs::entity e, TransformComponent& t, MeshComponent& m, MaterialComponent& mat)
        {
            RenderBuckets& buckets = e.world().get_mut<RenderBuckets>();
            buckets.dynamicFastObjects.push_back({ m.handle, mat.handle, t.transform });
        });
}
