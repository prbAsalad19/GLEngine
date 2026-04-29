#include "MeshLoader.h"

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
                int vertCount = static_cast<int>(out.getVertices().size() / 3);
                int texCount = static_cast<int>(out.getTextureCoord().size() / 2);
                int normCount = static_cast<int>(out.getNormals().size() / 3);

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
                int vi[3] = { vIdx[0], vIdx[i], vIdx[i + 1] };
                int ti[3] = { tIdx[0], tIdx[i], tIdx[i + 1] };
                int ni[3] = { nIdx[0], nIdx[i], nIdx[i + 1] };

                for (int k = 0; k < 3; ++k)
                {
                    out.addFaceValue(vi[k]);
                    if (!out.getTextureCoord().empty()) out.addFaceValue(ti[k]);
                    if (!out.getNormals().empty())      out.addFaceValue(ni[k]);
                }
            }
        }
    }

    // ── Generate flat normals if the OBJ file had none ──────────────────────
    if (out.getNormals().empty() && !out.getVertices().empty())
    {
        const auto& pos  = out.getVertices();
        const auto& face = out.getFaces();

        // Determine current face stride (1 = v only, 2 = v/vt)
        int oldStep = 1;
        if (!out.getTextureCoord().empty()) oldStep = 2;

        // Rebuild face array with normal indices included
        std::vector<unsigned int> oldFaces(face.begin(), face.end());
        out.clearFaces();

        for (size_t i = 0; i + oldStep * 3 - 1 < oldFaces.size(); i += oldStep * 3)
        {
            // Get the three vertex indices of this triangle
            int v0 = oldFaces[i];
            int v1 = oldFaces[i + oldStep];
            int v2 = oldFaces[i + oldStep * 2];

            // Read positions
            float p0[3] = { pos[v0*3+0], pos[v0*3+1], pos[v0*3+2] };
            float p1[3] = { pos[v1*3+0], pos[v1*3+1], pos[v1*3+2] };
            float p2[3] = { pos[v2*3+0], pos[v2*3+1], pos[v2*3+2] };

            // Compute edges
            float e1[3] = { p1[0]-p0[0], p1[1]-p0[1], p1[2]-p0[2] };
            float e2[3] = { p2[0]-p0[0], p2[1]-p0[1], p2[2]-p0[2] };

            // Cross product → face normal
            float nx = e1[1]*e2[2] - e1[2]*e2[1];
            float ny = e1[2]*e2[0] - e1[0]*e2[2];
            float nz = e1[0]*e2[1] - e1[1]*e2[0];

            // Normalize
            float len = sqrtf(nx*nx + ny*ny + nz*nz);
            if (len > 0.0f) { nx /= len; ny /= len; nz /= len; }
            else            { nx = 0.0f; ny = 0.0f; nz = 1.0f; }

            // Add this normal to the CPUMesh
            int nIdx = static_cast<int>(out.getNormals().size() / 3);
            out.addNormal(nx, ny, nz);

            // Re-emit face with v/vt/vn (new step = 3)
            for (int k = 0; k < 3; ++k)
            {
                int base = static_cast<int>(i) + k * oldStep;
                out.addFaceValue(oldFaces[base]);               // vertex index
                if (oldStep >= 2)
                    out.addFaceValue(oldFaces[base + 1]);       // texcoord index
                else
                    out.addFaceValue(-1);                       // no texcoord placeholder
                out.addFaceValue(nIdx);                         // normal index (same for all 3 verts)
            }
        }

        std::cout << "[MeshLoader] Generated " << (out.getNormals().size() / 3)
                  << " flat normals for mesh without vn data\n";
    }

    return true;
}

std::vector<Vertex> MeshLoader::toVertexArray(const CPUMesh& mesh,
    std::vector<unsigned int>& outIndices)
{
    const auto& pos = mesh.getVertices();
    const auto& norm = mesh.getNormals();
    const auto& uv = mesh.getTextureCoord();
    const auto& face = mesh.getFaces();
    const int   type = mesh.getMeshType();

    const int posCount = static_cast<int>(pos.size() / 3);
    const int uvCount = static_cast<int>(uv.size() / 2);
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