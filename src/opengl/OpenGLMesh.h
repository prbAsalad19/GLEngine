#include "core/math/algebricOp.h"
#include "core/scene/Transform.h"
#include "opengl/OpenGLConfig.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        Transform transform = Transform::getIdentityTransform());
    ~Mesh();

    void draw() const;

    Transform transform;

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
};