#include "core/math/algebricOp.h"
#include "opengl/OpenGLConfig.h"

class OpenGLMesh
{
public:
    OpenGLMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices);
    ~OpenGLMesh();

    void draw() const;


private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
};