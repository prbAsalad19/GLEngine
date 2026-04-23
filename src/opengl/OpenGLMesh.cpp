#include "OpenGLMesh.h"

OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    AABB aabb)
    : indexCount(static_cast<unsigned int>(indices.size()))
{
    std::cout << "OpenGLMesh created | vertices: " << vertices.size()
        << " | indices: " << indices.size() << "\n";
    glCreateVertexArrays(1, &VAO);

    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, vertices.size() * sizeof(Vertex), vertices.data(), 0);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribBinding(VAO, 0, 0);

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribBinding(VAO, 1, 0);

    glEnableVertexArrayAttrib(VAO, 2);
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
    glVertexArrayAttribBinding(VAO, 2, 0);

    glCreateBuffers(1, &EBO);
    glNamedBufferStorage(EBO, indices.size() * sizeof(unsigned int), indices.data(), 0);

    glVertexArrayElementBuffer(VAO, EBO);
}

void OpenGLMesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void OpenGLMesh::drawInstanced(uint32_t instanceCount) const
{
    glBindVertexArray(VAO);
    glDrawElementsInstanced(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0,
        instanceCount
    );
}

OpenGLMesh::~OpenGLMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}