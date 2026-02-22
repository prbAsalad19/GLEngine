#pragma once
#include "config.h"
#include "algebricOp.h"

class Mesh
{
private:
	GLuint VBO, EBO, VAO, vertex_count;
	std::vector<float> gpuVertices;

public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int>& indices);
	void draw();
	~Mesh();
};

class CPUMesh
{
private:
	std::vector<float> vertices; //3 floats per vertex (positions)
	std::vector<float> textureCoord;      //2 floats 
	std::vector<float> normals;           //3 floats per normal

	int faceType = 0;
	//0 vertices only, 1 vertices + texture coords, 2 vertices + normals, 3 vertices + texture coords + normals

	std::vector<int> faces; //3/6/9 floats per face, depending on the face type (vertex indices, texture coord indices, normal indices) v,vt,vn v,vt,vn

public:
	std::vector<float> getVertices() { return vertices; }
	std::vector<float> getTextureCoord() { return textureCoord; }
	std::vector<float> getNormals() { return normals; }
	std::vector<int> getFaces() { return faces; }

	void addVertex(float x, float y, float z) { vertices.push_back(x); vertices.push_back(y); vertices.push_back(z); }
	void addTextureCoord(float u, float v) { textureCoord.push_back(u); textureCoord.push_back(v); }
	void addNormal(float x, float y, float z) { normals.push_back(x); normals.push_back(y); normals.push_back(z); }
	void addOneFaceValue(int val) { faces.push_back(val); } //because the type can vary depending on type, every val is pushed one by one and ordered by esternal logic in the code using the function

	void setVertex(int index, float x, float y, float z) { vertices[index * 3] = x; vertices[index * 3 + 1] = y; vertices[index * 3 + 2] = z; }
	void setTextureCoord(int index, float u, float v) { textureCoord[index * 2] = u; textureCoord[index * 2 + 1] = v; }
	void setNormal(int index, float x, float y, float z) { normals[index * 3] = x; normals[index * 3 + 1] = y; normals[index * 3 + 2] = z; }
	void setOneFaceValue(int index, int val) { faces[index] = val; }

	int getMeshType()
	{
		int type = 0;
		if (textureCoord.size() > 0) type += 1;
		if (normals.size() > 0) type += 2;
		return type;
	}

	void clear() { vertices.clear(); textureCoord.clear(); normals.clear(); faces.clear(); }
};

class MeshLoader
{

public:
	static bool GetObjFileData(const std::string& filepath, CPUMesh& out_mesh);
	static std::vector<Vertex> formatForOpenGL(CPUMesh inMesh, std::vector<unsigned int>& map);

};