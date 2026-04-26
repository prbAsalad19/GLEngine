#include "core/math/algebricOp.h"
#include "opengl/OpenGLConfig.h"
#include "core/bvh/aabb.h"

class OpenGLMesh
{
public:
    OpenGLMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        AABB aabb);
    ~OpenGLMesh();

    void draw() const;
	void drawInstanced(uint32_t instanceCount) const;
    AABB aabb;

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
};