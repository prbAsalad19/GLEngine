#pragma once
#include "config.h"
#include "algebricOp.h"

// ─────────────────────────────────────────────
//  Mesh  — GPU-side geometry (VAO / VBO / EBO)
// ─────────────────────────────────────────────

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

// ─────────────────────────────────────────────
//  CPUMesh  — raw parsed data, lives only on CPU
// ─────────────────────────────────────────────

class CPUMesh
{
public:
    // Accessors
    const std::vector<float>& getVertices()     const { return vertices; }
    const std::vector<float>& getTextureCoord() const { return textureCoord; }
    const std::vector<float>& getNormals()      const { return normals; }
    const std::vector<unsigned int>&   getFaces()        const { return faces; }

    // 0 = v only | 1 = v/vt | 2 = v//vn | 3 = v/vt/vn
    int getMeshType() const
    {
        int type = 0;
        if (!textureCoord.empty()) type += 1;
        if (!normals.empty())      type += 2;
        return type;
    }

    void addVertex(float x, float y, float z)       { vertices.push_back(x); vertices.push_back(y); vertices.push_back(z); }
    void addTextureCoord(float u, float v)           { textureCoord.push_back(u); textureCoord.push_back(v); }
    void addNormal(float x, float y, float z)        { normals.push_back(x); normals.push_back(y); normals.push_back(z); }
    void addFaceValue(int val)                       { faces.push_back(val); }

    void clear() { vertices.clear(); textureCoord.clear(); normals.clear(); faces.clear(); }

private:
    std::vector<float> vertices;
    std::vector<float> textureCoord;
    std::vector<float> normals;
    std::vector<unsigned int>   faces;
};

// ─────────────────────────────────────────────
//  MeshLoader  — loads OBJ files and converts
//               CPUMesh to GPU-ready data
// ─────────────────────────────────────────────

class MeshLoader
{
public:
    // Parses an OBJ file into a CPUMesh. Returns false on failure.
    static bool loadOBJ(const std::string& filepath, CPUMesh& out);

    // Converts a CPUMesh to a deduplicated vertex array + index array.
    static std::vector<Vertex> toVertexArray(const CPUMesh& mesh,
                                             std::vector<unsigned int>& outIndices);
};

struct Scene
{
    std::vector<CPUMesh> CPUMeshes;
    std::vector<Mesh> meshes;
    std::vector<Transform> Transforms;


    struct RenderObject
    {
        unsigned int mesh;
        unsigned int tranform;
        unsigned int texture; //-1 for no texture
    };

    std::vector<RenderObject> objects;
};
