#include "OpenGLMesh.h"

OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    Transform t)
    : transform(t), indexCount(static_cast<unsigned int>(indices.size()))
{
    // Interleave vertex data: position (3) | normal (3) | uv (2)
    // std::vector<float> data;
    // data.reserve(vertices.size() * 8);
    // for (const auto& v : vertices)
    // {
    //     data.push_back(v.position[0]); data.push_back(v.position[1]); data.push_back(v.position[2]);
    //     data.push_back(v.normal[0]);   data.push_back(v.normal[1]);   data.push_back(v.normal[2]);
    //     data.push_back(v.uv[0]);       data.push_back(v.uv[1]);
    // }

    glCreateVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, vertices.size() * sizeof(Vertex), vertices.data(), 0);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
    // constexpr int stride = 8 * sizeof(float);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
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
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLMesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

OpenGLMesh::~OpenGLMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}