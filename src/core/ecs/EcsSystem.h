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
};

inline void registerRenderExtractSystems(flecs::world& world)
{
    world.system<>("ClearRenderBuckets")
        .kind(flecs::OnLoad)
        .run([](flecs::iter& it)
        {
            it.world().get_mut<RenderBuckets>().clear();
        });

    world.system<TransformComponent, MeshComponent, MaterialComponent>("ExtractRenderObjects")
        .kind(flecs::PostUpdate) // dopo che input/fisica hanno già modificato i transform
        .each([](flecs::entity e, TransformComponent& t, MeshComponent& m, MaterialComponent& mat)
        {
            RenderObject obj{ m.handle, mat.handle, t.transform };
            RenderBuckets& buckets = e.world().get_mut<RenderBuckets>();

            if      (e.has<StaticTag>())      buckets.staticObjects.push_back(obj);
            else if (e.has<QuasiStaticTag>()) buckets.quasiStaticObjects.push_back(obj);
            else if (e.has<DynamicSlowTag>()) buckets.dynamicSlowObjects.push_back(obj);
            else if (e.has<DynamicFastTag>()) buckets.dynamicFastObjects.push_back(obj);
        });
}
