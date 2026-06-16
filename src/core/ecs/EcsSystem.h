#pragma once
#include "flecs.h"
#include "core/scene/Transform.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/LightManager.h"
#include "core/assets/Camera.h"
#include "core/AudioPipeline/AudioEngine.h"

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