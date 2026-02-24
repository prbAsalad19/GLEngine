#include "config.h"
#include "triangle_mesh.h"
#include "material.h"
#include "algebricOp.h"
#include "objLoader.h"

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
unsigned int make_module(const std::string& filepath, unsigned int module_type);
GLFWwindow* openGLEngineInit(int width, int height, const char* title);

int main()
{
	//init and error handling
	GLFWwindow* window = openGLEngineInit(1280, 720, "bruh");
	if (window == nullptr)
	{
		std::cout << "OpenGL initialization failed. Exiting." << std::endl;
		return -1;
	}


	// TriangleMesh* triangle = new TriangleMesh();
	Material* material = new Material("img/texture.jpg");
	Material* mask = new Material("img/vignette_texture.jpg");
	CPUMesh objMesh;
	if (!MeshLoader::GetObjFileData("assets/teapot.obj", objMesh))
	{
		std::cout << "Failed to load OBJ file " << "assets/teapot.obj" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	std::vector<unsigned int> indexMap;
	std::vector<Vertex> formattedVertices;
	formattedVertices = MeshLoader::formatForOpenGL(objMesh, indexMap);
	Mesh* cube = new Mesh(formattedVertices, indexMap);


	//creation of the shader program, with error handling
	unsigned int shader = make_shader("shaders/vertex.txt", "shaders/fragment.txt");
	if (shader == 0)
	{
		std::cout << "Shader program creation failed. Uscita." << std::endl;
		delete cube;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	//set active shader program and set uniform locations
	glUseProgram(shader);

	
	glUniform1i(glGetUniformLocation(shader, "material"), 0);
	glUniform1i(glGetUniformLocation(shader, "mask"), 1);
	glUniform1i(glGetUniformLocation(shader, "useMaskAlpha"), 0);  // 0 = opaco, 1 = alpha da mask

	Vector3 quad_position = { -0.1f, 0.5f, -1.0f };
	Vector3 model_scale = { 0.5f, 0.5f, 0.5f };
	Vector3 camera_pos = { -5.0f, 0.0f, 3.0f };
	Vector3 camera_target = { 0.0f, 0.0f, 0.0f };
	unsigned int model_location = glGetUniformLocation(shader, "model");
	unsigned int view_location = glGetUniformLocation(shader, "view");
	unsigned int proj_location = glGetUniformLocation(shader, "projection");

	mat4 view = mat4::create_look_at(camera_pos, camera_target);
	glUniformMatrix4fv(view_location, 1, GL_FALSE, view.entries);
	int winWidth, winHeight;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	mat4 projection = mat4::create_prospective_projection(45.0f, static_cast<float>(winWidth) / static_cast<float>(winHeight), 0.1f, 10.0f);
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, projection.entries);

	//enable alpha blending and depth test (per cubo 3D)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		int winWidth, winHeight;
		glfwGetWindowSize(window, &winWidth, &winHeight);
		mat4 projection = mat4::create_prospective_projection(
			45.0f,
			static_cast<float>(winWidth) / static_cast<float>(winHeight),
			0.1f,
			10.0f
		);
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, projection.entries);

		mat4 modelR_x = mat4::create_x_rotation(90.0f);
		mat4 modelScale = mat4::create_scale(model_scale);
		mat4 model = mat4::create_matrix_transform(quad_position)
			* mat4::create_z_rotation(glfwGetTime() * 10)
			* modelR_x
			* modelScale;
		glUniformMatrix4fv(model_location, 1, GL_FALSE, model.entries);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);
		material->use(0);
		mask->use(1);
		cube->draw();

		glfwSwapBuffers(window);
	}

	//resource cleanup
	glDeleteProgram(shader);
	delete cube;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
	std::vector<unsigned int> modules;
	unsigned int vert = make_module(vertex_filepath, GL_VERTEX_SHADER);
	unsigned int frag = make_module(fragment_filepath, GL_FRAGMENT_SHADER);

	if (vert == 0 || frag == 0)
	{
		// se una delle due compilazioni è fallita, pulisco i moduli validi e torno 0
		if (vert != 0) glDeleteShader(vert);
		if (frag != 0) glDeleteShader(frag);
		return 0;
	}

	modules.push_back(vert);
	modules.push_back(frag);

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << std::endl;
		glDeleteProgram(shader);
		shader = 0;
	}

	// i moduli possono essere eliminati dopo il link
	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	return shader;
}


//unsigned int make_module(const std::string& filepath, unsigned int module_type)
//{
//	std::ifstream file(filepath, std::ios::binary);
//	if (!file.is_open())
//	{
//		std::cout << "Failed to open shader file: " << filepath << std::endl;
//		return 0;
//	}
//
//	std::stringstream bufferedLines;
//	std::string line;
//	while (std::getline(file, line))
//	{
//		bufferedLines << line << "\n";
//	}
//	file.close();
//
//	std::string shaderSource = bufferedLines.str();
//
//	// Diagnostica: stampa i primi byte in esadecimale per rilevare BOM
//	std::cout << "Shader file: " << filepath << " first bytes:";
//	for (size_t i = 0; i < std::min<size_t>(std::max<size_t>(shaderSource.size(), 3), 8); ++i)
//	{
//		unsigned char c = static_cast<unsigned char>(shaderSource[i]);
//		std::cout << " " << std::hex << std::uppercase << (int)c;
//	}
//	std::cout << std::dec << std::nouppercase << std::endl;
//
//	// Rimuovi UTF-8 BOM se presente (0xEF,0xBB,0xBF)
//	if (shaderSource.size() >= 3)
//	{
//		unsigned char b0 = static_cast<unsigned char>(shaderSource[0]);
//		unsigned char b1 = static_cast<unsigned char>(shaderSource[1]);
//		unsigned char b2 = static_cast<unsigned char>(shaderSource[2]);
//		if (b0 == 0xEF && b1 == 0xBB && b2 == 0xBF)
//		{
//			shaderSource.erase(0, 3);
//			std::cout << "Removed UTF-8 BOM from " << filepath << std::endl;
//		}
//	}
//
//	const char* shaderSrc = shaderSource.c_str();
//
//	unsigned int shaderModule = glCreateShader(module_type);
//	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
//	glCompileShader(shaderModule);
//
//	int success;
//	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		char errorLog[1024];
//		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
//		std::cout << "Shader module compiling error (" << filepath << "):\n" << errorLog << std::endl;
//		glDeleteShader(shaderModule);
//		return 0;
//	}
//
//	return shaderModule;
//}
unsigned int make_module(const std::string& filepath, unsigned int module_type)
{
	std::ifstream file(filepath);
	if (!file.is_open())
	{
		std::cout << "Failed to open shader file: " << filepath << std::endl;
		return 0;
	}

	std::stringstream bufferedLines;
	std::string line;
	while (std::getline(file, line))
	{
		bufferedLines << line << "\n";
	}
	file.close();

	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader module compiling error (" << filepath << "):\n" << errorLog << std::endl;
		glDeleteShader(shaderModule);
		return 0;
	}

	return shaderModule;
}

// Inizializza GLFW/GLAD, crea la finestra e configura viewport/clear color.
// Ritorna il puntatore alla finestra o nullptr in caso di errore.
GLFWwindow* openGLEngineInit(int width, int height, const char* title)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Couldn't load OpenGL" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return nullptr;
	}

	const GLubyte* glVer = glGetString(GL_VERSION);
	const GLubyte* glslVer = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	int major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "GL_VERSION string: " << (glVer ? (const char*)glVer : "NULL") << std::endl;
	std::cout << "GLSL version string: " << (glslVer ? (const char*)glslVer : "NULL") << std::endl;
	std::cout << "Renderer: " << (renderer ? (const char*)renderer : "NULL") << std::endl;
	std::cout << "Vendor: " << (vendor ? (const char*)vendor : "NULL") << std::endl;
	std::cout << "GL major/minor (glGetIntegerv): " << major << "." << minor << std::endl;
	int attrMajor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int attrMinor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int profile = glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE);
	std::cout << "glfw window attrib requested/actual: " << attrMajor << "." << attrMinor << " profile=" << profile << std::endl;


	int winWidth, winHeight;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	glViewport(0, 0, winWidth, winHeight);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	return window;
}