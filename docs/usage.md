# GLEngine — Usage Guide

This document describes how to use the engine systems in practice.

---

# Loading Resources

All assets are managed by the ResourceManager.
Load a resource and store the returned handle — handles are lightweight IDs
used to reference assets without storing raw pointers.
```cpp
ResourceManager resources;

MeshHandle     mesh = resources.loadMesh("assets/model.obj");
TextureHandle  tex  = resources.loadTexture("img/texture.jpg");
MaterialHandle mat  = resources.loadMaterial(tex);
```

If the same path is loaded twice, the pool returns the existing handle
without duplicating the asset.

If you pass a null TextureHandle to loadMaterial, a default texture is
used automatically:
```cpp
MaterialHandle mat = resources.loadMaterial(TextureHandle{});
```

To check if a handle has been assigned:
```cpp
if (!mesh.isNull()) { ... }
```

To release a resource:
```cpp
resources.deleteMesh(handle);
resources.deleteTexture(handle);
resources.deleteMaterial(handle);
```

---

# Building the Scene

A Scene is a flat list of RenderObjects.
Each RenderObject connects a mesh, a material and a transform.
```cpp
Scene scene;

RenderObject obj;
obj.mesh     = mesh;
obj.material = mat;

// Position
obj.transform.position = { 0.0f, 0.0f, 0.0f };

// Scale
obj.transform.scale = { 1.0f, 1.0f, 1.0f };

// Rotation — Euler angles in degrees (pitch, yaw, roll)
obj.transform.setEuler({ 0.0f, 45.0f, 0.0f });

// Rotation — quaternion directly
obj.transform.setQuaternion(x, y, z, w);

scene.objects.push_back(obj);
```

Multiple RenderObjects can share the same mesh or material handle.
The asset is stored once and reused.

---

# Camera
```cpp
Camera camera;
camera.position  = { -5.0f, 0.0f, 3.0f };
camera.fov       = 45.0f;
camera.nearPlane = 0.1f;
camera.farPlane  = 100.0f;

// Point the camera at a world position
camera.setTarget({ 0.0f, 0.0f, 0.0f });

// Or set pitch and yaw directly (radians)
camera.pitch = 0.0f;
camera.yaw   = 0.0f;
```

---

# Renderer

Initialize the renderer once with a reference to the ResourceManager
and the paths to the vertex and fragment shaders.
```cpp
OpenGLRenderer renderer(resources, "shaders/vertex.txt", "shaders/fragment.txt");
renderer.init();
```

Each frame, update the viewport and render the scene:
```cpp
int w, h;
glfwGetFramebufferSize(window, &w, &h);
renderer.onResize(static_cast<unsigned int>(w), static_cast<unsigned int>(h));
renderer.render(scene, camera);
```

Always use glfwGetFramebufferSize (not glfwGetWindowSize) to get the
correct pixel dimensions on HiDPI displays.

Call shutdown() when done:
```cpp
renderer.shutdown();
```

---

# Typical Frame Loop
```cpp
while (!glfwWindowShouldClose(window))
{
    glfwPollEvents();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    renderer.onResize(static_cast<unsigned int>(w), static_cast<unsigned int>(h));

    renderer.render(scene, camera);
    glfwSwapBuffers(window);
}
```
