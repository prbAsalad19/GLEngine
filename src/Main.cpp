#include "config.h"
#include "material.h"
#include "algebricOp.h"
#include "objLoader.h"

// ─────────────────────────────────────────────
//  Forward declarations
// ─────────────────────────────────────────────

static unsigned int make_module(const std::string& filepath, unsigned int moduleType);
static unsigned int make_shader(const std::string& vertPath, const std::string& fragPath);
static GLFWwindow*  createWindow(int width, int height, const char* title);

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────

int main()
{
    GLFWwindow* window = createWindow(1280, 720, "engine");
    if (!window)
    {
        std::cout << "OpenGL initialization failed." << std::endl;
        return -1;
    }

    Scene scene; //creating the main scene
    // Load mesh
    CPUMesh cpuMesh;
    if (!MeshLoader::loadOBJ("assets/teapot_with_uv.obj", cpuMesh))
    {
        std::cout << "Failed to load OBJ." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    std::vector<unsigned int> indices;
    std::vector<Vertex> verts = MeshLoader::toVertexArray(cpuMesh, indices);
    Mesh* teapot = new Mesh(verts, indices);

    // Load materials
    Material* albedo  = new Material("img/texture.jpg");
    Material* mask    = new Material("img/vignette_texture.jpg");

    // Compile shader
    unsigned int shader = make_shader("shaders/vertex.txt", "shaders/fragment.txt");
    if (shader == 0)
    {
        std::cout << "Shader compilation failed." << std::endl;
        delete teapot;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Uniform locations
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "material"),    0);
    glUniform1i(glGetUniformLocation(shader, "mask"),        1);
    glUniform1i(glGetUniformLocation(shader, "useMaskAlpha"), 0);

    unsigned int uModel = glGetUniformLocation(shader, "model");
    unsigned int uView  = glGetUniformLocation(shader, "view");
    unsigned int uProj  = glGetUniformLocation(shader, "projection");

    // Camera
    Vector3 cameraPos    = { -5.0f, 0.0f, 3.0f };
    Vector3 cameraTarget = {  0.0f, 0.0f, 0.0f };
    mat4 view = mat4::create_look_at(cameraPos, cameraTarget);
    glUniformMatrix4fv(uView, 1, GL_FALSE, view.entries);

    // Transform
    Vector3 position = { -0.1f, 0.5f, -1.0f };
    Vector3 scale    = {  0.5f, 0.5f,  0.5f };
    mat4 baseRotation = mat4::create_x_rotation(90.0f);
    mat4 scaleMat     = mat4::create_scale(scale);

    // Render state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // ── Render loop ──────────────────────────────
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        int w, h;
        glfwGetWindowSize(window, &w, &h);
        mat4 proj = mat4::create_prospective_projection(
            45.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 10.0f);
        glUniformMatrix4fv(uProj, 1, GL_FALSE, proj.entries);

        mat4 model = mat4::create_matrix_transform(position)
                   * mat4::create_z_rotation(static_cast<float>(glfwGetTime()) * 15.0f)
                   * baseRotation
                   * scaleMat;
        glUniformMatrix4fv(uModel, 1, GL_FALSE, model.entries);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);
        albedo->use(0);
        mask->use(1);
        teapot->draw();

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteProgram(shader);
    delete teapot;
    delete albedo;
    delete mask;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// ─────────────────────────────────────────────
//  Shader helpers
// ─────────────────────────────────────────────

static unsigned int make_module(const std::string& filepath, unsigned int moduleType)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cout << "Failed to open shader: " << filepath << std::endl;
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string src = ss.str();
    const char* csrc = src.c_str();

    unsigned int module = glCreateShader(moduleType);
    glShaderSource(module, 1, &csrc, nullptr);
    glCompileShader(module);

    int success;
    glGetShaderiv(module, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(module, sizeof(log), nullptr, log);
        std::cout << "Shader compile error (" << filepath << "):\n" << log << std::endl;
        glDeleteShader(module);
        return 0;
    }
    return module;
}

static unsigned int make_shader(const std::string& vertPath, const std::string& fragPath)
{
    unsigned int vs = make_module(vertPath, GL_VERTEX_SHADER);
    unsigned int fs = make_module(fragPath, GL_FRAGMENT_SHADER);

    if (!vs || !fs)
    {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return 0;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cout << "Shader link error:\n" << log << std::endl;
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

// ─────────────────────────────────────────────
//  Window / context creation
// ─────────────────────────────────────────────

static GLFWwindow* createWindow(int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create window." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    std::cout << "OpenGL " << glGetString(GL_VERSION)
              << " | " << glGetString(GL_RENDERER) << std::endl;

    return window;
}
