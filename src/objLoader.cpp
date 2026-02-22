#include "config.h"
#include "objLoader.h"

std::vector<Vertex> MeshLoader::formatForOpenGL(CPUMesh mesh_in, std::vector<unsigned int>& indices)
{
    std::vector<float> vert_in = mesh_in.getVertices();
    std::vector<float> norm_in = mesh_in.getNormals();
    std::vector<float> TCoord_in = mesh_in.getTextureCoord();
    std::vector<int> face_in = mesh_in.getFaces();
    int type = mesh_in.getMeshType();

    std::vector<Vertex> vertVector;      // VBO
    std::map<Vertex, unsigned int> vertexCache; // Mappa: [Dati Vertice] -> [Indice nel VBO]

    int step = 1; // Default per type 0
    if (type == 1 || type == 2) step = 2;
    if (type == 3) step = 3;

    const int vertCount = static_cast<int>(vert_in.size() / 3);
    const int texCount = static_cast<int>(TCoord_in.size() / 2);
    const int normCount = static_cast<int>(norm_in.size() / 3);

    for (int i = 0; i + 0 < static_cast<int>(face_in.size()); i += step)
    {
        Vertex v;
        v.Vzero(); // Pulisce i dati (importante per le normali/uv se non presenti)

        // --- LOGICA DI PESCAGGIO UNIVERSALE ---
        int vIdx = face_in[i];
        if (vIdx < 0 || vIdx >= vertCount) continue; // difensivo

        v.position[0] = vert_in[vIdx * 3 + 0];
        v.position[1] = vert_in[vIdx * 3 + 1];
        v.position[2] = vert_in[vIdx * 3 + 2];

        if (type == 1) { // v/vt
            int tIdx = face_in[i + 1];
            if (tIdx >= 0 && tIdx < texCount) {
                v.uv[0] = TCoord_in[tIdx * 2 + 0];
                v.uv[1] = TCoord_in[tIdx * 2 + 1];
            }
        }
        else if (type == 2) { // v//vn
            int nIdx = face_in[i + 1];
            if (nIdx >= 0 && nIdx < normCount) {
                v.normal[0] = norm_in[nIdx * 3 + 0];
                v.normal[1] = norm_in[nIdx * 3 + 1];
                v.normal[2] = norm_in[nIdx * 3 + 2];
            }
        }
        else if (type == 3) { // v/vt/vn
            // CORRETTO: primo indice dopo v è t, poi n
            int tIdx = face_in[i + 1];
            int nIdx = face_in[i + 2];
            if (nIdx >= 0 && nIdx < normCount) {
                v.normal[0] = norm_in[nIdx * 3 + 0];
                v.normal[1] = norm_in[nIdx * 3 + 1];
                v.normal[2] = norm_in[nIdx * 3 + 2];
            }
            if (tIdx >= 0 && tIdx < texCount) {
                v.uv[0] = TCoord_in[tIdx * 2 + 0];
                v.uv[1] = TCoord_in[tIdx * 2 + 1];
            }
        }

        // --- DEDUPLICAZIONE ---
        auto it = vertexCache.find(v);
        if (it == vertexCache.end()) {
            // Il vertice è nuovo
            unsigned int newIndex = static_cast<unsigned int>(vertVector.size());
            vertVector.push_back(v);
            vertexCache[v] = newIndex;
            indices.push_back(newIndex);
        }
        else {
            // Il vertice esiste già, usa l'indice salvato
            indices.push_back(it->second);
        }
    }
    return vertVector;
}

bool MeshLoader::GetObjFileData(const std::string& filepath, CPUMesh& out_mesh) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix; // Legge la prima parola (v, vt, vn, f, o commenti #)

        if (prefix == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            out_mesh.addVertex(x, y, z);
        }
        else if (prefix == "vt") {
            float u, v;
            ss >> u >> v;
            out_mesh.addTextureCoord(u, v);
        }
        else if (prefix == "vn") {
            float x, y, z;
            ss >> x >> y >> z;
            out_mesh.addNormal(x, y, z);
        }
        else if (prefix == "f") {
            // raccogliamo tutti i token della faccia (es: "1/2/3", "4//5", ...)
            std::vector<int> vIdxs;
            std::vector<int> tIdxs;
            std::vector<int> nIdxs;

            std::string segment;
            while (ss >> segment) {
                // split per '/'
                std::vector<std::string> parts;
                std::stringstream segSS(segment);
                std::string part;
                while (std::getline(segSS, part, '/')) parts.push_back(part);

                int v = -1, t = -1, n = -1;

                // numero di elementi già presenti (per supportare indici negativi)
                int vertCount = static_cast<int>(out_mesh.getVertices().size() / 3);
                int texCount = static_cast<int>(out_mesh.getTextureCoord().size() / 2);
                int normCount = static_cast<int>(out_mesh.getNormals().size() / 3);

                if (parts.size() >= 1 && !parts[0].empty()) {
                    v = std::stoi(parts[0]);
                    if (v < 0) v = vertCount + v;
                    else v = v - 1; // 1-based -> 0-based
                }
                if (parts.size() >= 2 && !parts[1].empty()) {
                    t = std::stoi(parts[1]);
                    if (t < 0) t = texCount + t;
                    else t = t - 1;
                }
                if (parts.size() >= 3 && !parts[2].empty()) {
                    n = std::stoi(parts[2]);
                    if (n < 0) n = normCount + n;
                    else n = n - 1;
                }

                vIdxs.push_back(v);
                tIdxs.push_back(t);
                nIdxs.push_back(n);
            }

            // Triangolazione a fan per poligoni con >3 vertici
            if (vIdxs.size() >= 3) {
                for (size_t i = 1; i + 1 < vIdxs.size(); ++i) {
                    int vi[3] = { vIdxs[0], vIdxs[i], vIdxs[i + 1] };
                    int ti[3] = { (tIdxs.size() ? tIdxs[0] : -1), (tIdxs.size() ? tIdxs[i] : -1), (tIdxs.size() ? tIdxs[i + 1] : -1) };
                    int ni[3] = { (nIdxs.size() ? nIdxs[0] : -1), (nIdxs.size() ? nIdxs[i] : -1), (nIdxs.size() ? nIdxs[i + 1] : -1) };

                    for (int k = 0; k < 3; ++k) {
                        // aggiungiamo sempre l'indice del vertice (già int)
                        out_mesh.addOneFaceValue(vi[k]);
                        // aggiungiamo gli indici opzionali solo se esistono effettivamente vettori globali
                        if (!out_mesh.getTextureCoord().empty()) {
                            out_mesh.addOneFaceValue(ti[k]);
                        }
                        if (!out_mesh.getNormals().empty()) {
                            out_mesh.addOneFaceValue(ni[k]);
                        }
                    }
                }
            }
        }
    }
    return true;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int>& indices)
{
    std::vector<float> fVertices;
    for (const auto& v : vertices) {
        fVertices.push_back(v.position[0]);
        fVertices.push_back(v.position[1]);
        fVertices.push_back(v.position[2]);
        fVertices.push_back(v.normal[0]);
        fVertices.push_back(v.normal[1]);
        fVertices.push_back(v.normal[2]);
        fVertices.push_back(v.uv[0]);
        fVertices.push_back(v.uv[1]);
	}
    gpuVertices = fVertices;

    vertex_count = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, fVertices.size() * sizeof(float), fVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}