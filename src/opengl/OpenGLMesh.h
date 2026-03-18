#include "core/math/algebricOp.h"
#include "opengl/OpenGLConfig.h"

class OpenGLMesh
{
public:
    OpenGLMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices);
    ~OpenGLMesh();

    void draw() const;
	unsigned int getIndexCount() const { return indexCount; }

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
};