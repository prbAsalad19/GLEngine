#include "config.h"
#include "objLoader.h"

// ─────────────────────────────────────────────
//  MeshLoader
// ─────────────────────────────────────────────

bool MeshLoader::loadOBJ(const std::string& filepath, CPUMesh& out)
{
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            out.addVertex(x, y, z);
        }
        else if (prefix == "vt")
        {
            float u, v;
            ss >> u >> v;
            out.addTextureCoord(u, v);
        }
        else if (prefix == "vn")
        {
            float x, y, z;
            ss >> x >> y >> z;
            out.addNormal(x, y, z);
        }
        else if (prefix == "f")
        {
            std::vector<int> vIdx, tIdx, nIdx;

            std::string token;
            while (ss >> token)
            {
                std::vector<std::string> parts;
                std::stringstream ts(token);
                std::string part;
                while (std::getline(ts, part, '/')) parts.push_back(part);

                int v = -1, t = -1, n = -1;
                int vertCount = static_cast<int>(out.getVertices().size()     / 3);
                int texCount  = static_cast<int>(out.getTextureCoord().size() / 2);
                int normCount = static_cast<int>(out.getNormals().size()      / 3);

                if (parts.size() >= 1 && !parts[0].empty())
                {
                    v = std::stoi(parts[0]);
                    v = (v < 0) ? vertCount + v : v - 1;
                }
                if (parts.size() >= 2 && !parts[1].empty())
                {
                    t = std::stoi(parts[1]);
                    t = (t < 0) ? texCount + t : t - 1;
                }
                if (parts.size() >= 3 && !parts[2].empty())
                {
                    n = std::stoi(parts[2]);
                    n = (n < 0) ? normCount + n : n - 1;
                }

                vIdx.push_back(v);
                tIdx.push_back(t);
                nIdx.push_back(n);
            }

            // Fan triangulation for polygons with more than 3 vertices
            for (size_t i = 1; i + 1 < vIdx.size(); ++i)
            {
                int vi[3] = { vIdx[0], vIdx[i], vIdx[i+1] };
                int ti[3] = { tIdx[0], tIdx[i], tIdx[i+1] };
                int ni[3] = { nIdx[0], nIdx[i], nIdx[i+1] };

                for (int k = 0; k < 3; ++k)
                {
                    out.addFaceValue(vi[k]);
                    if (!out.getTextureCoord().empty()) out.addFaceValue(ti[k]);
                    if (!out.getNormals().empty())      out.addFaceValue(ni[k]);
                }
            }
        }
    }
    return true;
}

std::vector<Vertex> MeshLoader::toVertexArray(const CPUMesh& mesh,
                                               std::vector<unsigned int>& outIndices)
{
    const auto& pos  = mesh.getVertices();
    const auto& norm = mesh.getNormals();
    const auto& uv   = mesh.getTextureCoord();
    const auto& face = mesh.getFaces();
    const int   type = mesh.getMeshType();

    const int posCount  = static_cast<int>(pos.size()  / 3);
    const int uvCount   = static_cast<int>(uv.size()   / 2);
    const int normCount = static_cast<int>(norm.size() / 3);

    int step = 1;
    if (type == 1 || type == 2) step = 2;
    if (type == 3)               step = 3;

    std::vector<Vertex> vertices;
    std::map<Vertex, unsigned int> cache;

    for (int i = 0; i < static_cast<int>(face.size()); i += step)
    {
        Vertex v;
        v.zero();

        int vIdx = face[i];
        if (vIdx < 0 || vIdx >= posCount) continue;

        v.position[0] = pos[vIdx * 3 + 0];
        v.position[1] = pos[vIdx * 3 + 1];
        v.position[2] = pos[vIdx * 3 + 2];

        if (type == 1)  // v/vt
        {
            int tIdx = face[i + 1];
            if (tIdx >= 0 && tIdx < uvCount)
            {
                v.uv[0] = uv[tIdx * 2 + 0];
                v.uv[1] = uv[tIdx * 2 + 1];
            }
        }
        else if (type == 2)  // v//vn
        {
            int nIdx = face[i + 1];
            if (nIdx >= 0 && nIdx < normCount)
            {
                v.normal[0] = norm[nIdx * 3 + 0];
                v.normal[1] = norm[nIdx * 3 + 1];
                v.normal[2] = norm[nIdx * 3 + 2];
            }
        }
        else if (type == 3)  // v/vt/vn
        {
            int tIdx = face[i + 1];
            int nIdx = face[i + 2];
            if (tIdx >= 0 && tIdx < uvCount)
            {
                v.uv[0] = uv[tIdx * 2 + 0];
                v.uv[1] = uv[tIdx * 2 + 1];
            }
            if (nIdx >= 0 && nIdx < normCount)
            {
                v.normal[0] = norm[nIdx * 3 + 0];
                v.normal[1] = norm[nIdx * 3 + 1];
                v.normal[2] = norm[nIdx * 3 + 2];
            }
        }

        auto it = cache.find(v);
        if (it == cache.end())
        {
            unsigned int idx = static_cast<unsigned int>(vertices.size());
            vertices.push_back(v);
            cache[v] = idx;
            outIndices.push_back(idx);
        }
        else
        {
            outIndices.push_back(it->second);
        }
    }
    return vertices;
}

// ─────────────────────────────────────────────
//  Mesh
// ─────────────────────────────────────────────

Mesh::Mesh(const std::vector<Vertex>& vertices,
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

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
