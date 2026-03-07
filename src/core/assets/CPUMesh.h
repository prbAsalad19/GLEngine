#include "core/CoreConfig.h"

class CPUMesh
{
public:
    // Accessors
    const std::vector<float>& getVertices()     const { return vertices; }
    const std::vector<float>& getTextureCoord() const { return textureCoord; }
    const std::vector<float>& getNormals()      const { return normals; }
    const std::vector<unsigned int>& getFaces()        const { return faces; }

    // 0 = v only | 1 = v/vt | 2 = v//vn | 3 = v/vt/vn
    int getMeshType() const
    {
        int type = 0;
        if (!textureCoord.empty()) type += 1;
        if (!normals.empty())      type += 2;
        return type;
    }

    void addVertex(float x, float y, float z) { vertices.push_back(x); vertices.push_back(y); vertices.push_back(z); }
    void addTextureCoord(float u, float v) { textureCoord.push_back(u); textureCoord.push_back(v); }
    void addNormal(float x, float y, float z) { normals.push_back(x); normals.push_back(y); normals.push_back(z); }
    void addFaceValue(int val) { faces.push_back(val); }

    void clear() { vertices.clear(); textureCoord.clear(); normals.clear(); faces.clear(); }

private:
    std::vector<float> vertices;
    std::vector<float> textureCoord;
    std::vector<float> normals;
    std::vector<unsigned int>   faces;
};