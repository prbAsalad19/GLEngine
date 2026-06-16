#include "EcsSystem.h"

void registerComponents(flecs::world& world)
{
    // transform
    world.component<TransformComponent>();

    // rendering
    world.component<MeshComponent>();
    world.component<MaterialComponent>();

    // tier tags
    world.component<StaticTag>();
    world.component<QuasiStaticTag>();
    world.component<DynamicSlowTag>();
    world.component<DynamicFastTag>();

    // audio
    world.component<AudioSourceComponent>();

    // light
    world.component<LightComponent>();

    // physics
    world.component<VelocityComponent>();
    world.component<ColliderComponent>();

    // singleton camera
    world.component<CameraComponent>();

    world.component<ResourceManagerSingleton>();
    world.component<LightManagerSingleton>();
    world.component<AudioEngineSingleton>();
}