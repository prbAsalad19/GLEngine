#include "triangle_mesh.h"
#include <glad/glad.h>
#include <vector>

TriangleMesh::TriangleMesh() {
    // posizioni (x,y,z) per vertice
    std::vector<float> positions = {
        -1.0f, -1.0f, 0.0f, // bottom left
         1.0f, -1.0f, 0.0f, // bottom right
        -1.0f,  1.0f, 0.0f,  // top left
		 1.0f,  1.0f, 0.0f 
    };

    // colori separati (r,g,b) per vertice — corrispondono alla vertex shader
    std::vector<float> colors = {
        1.0f, 0.0f, 0.0f, // rosso
        0.0f, 1.0f, 0.0f, // verde
        0.0f, 0.0f, 1.0f,  // blu
		1.0f, 1.0f, 0.0f  // giallo
    };

    std::vector<unsigned int> elementIndices = {
        0, 1, 2, 2, 1, 3
    };

    vertex_count = static_cast<unsigned int>(elementIndices.size());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(2);

    // position VBO
    glGenBuffers(1, &VBOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color VBO (separato, float per corrispondere a vec3 vertexColor)
    glGenBuffers(1, &VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 colors.size() * sizeof(float),
                 colors.data(),
                 GL_STATIC_DRAW);
    // layout location 1 nella vertex shader: vec3 vertexColor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // element buffer: bind mentre VAO è bindato
	glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 elementIndices.size() * sizeof(unsigned int),
                 elementIndices.data(),
                 GL_STATIC_DRAW);

    // ora è sicuro unbindare il VAO (l'EBO rimane associato al VAO)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBOs.data());
	glDeleteBuffers(1, &EBO);
}