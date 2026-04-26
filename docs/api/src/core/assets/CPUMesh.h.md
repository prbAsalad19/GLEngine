### CPUMesh.h

#### Brief Overview
The `CPUMesh` class represents a mesh structure in CPU memory. It provides methods to add vertices, texture coordinates, normals, and faces to the mesh, as well as accessors for retrieving this data. The class also maintains an Axis-Aligned Bounding Box (AABB) for collision detection and other spatial queries.

#### Table of Classes/Structs/Enums

- **Class**: `CPUMesh`
  - Description: Represents a mesh structure in CPU memory.
  - Member Variables:
    - `vertices`: Stores the vertex positions as a vector of floats.
    - `textureCoord`: Stores the texture coordinates as a vector of floats.
    - `normals`: Stores the normals as a vector of floats.
    - `faces`: Stores the face indices as a vector of unsigned integers.
    - `aabb`: Axis-Aligned Bounding Box representing the mesh's bounds.

#### Detailed Methods Section

##### Method: `getVertices()`
- **Full Signature**: `const std::vector<float>& getVertices() const;`
- **Return Type Explanation**: Returns a constant reference to the vector of vertex positions.
- **Internal Logic/Purpose**: Allows access to the vertices without modifying them.

##### Method: `getTextureCoord()`
- **Full Signature**: `const std::vector<float>& getTextureCoord() const;`
- **Return Type Explanation**: Returns a constant reference to the vector of texture coordinates.
- **Internal Logic/Purpose**: Allows access to the texture coordinates without modifying them.

##### Method: `getNormals()`
- **Full Signature**: `const std::vector<float>& getNormals() const;`
- **Return Type Explanation**: Returns a constant reference to the vector of normals.
- **Internal Logic/Purpose**: Allows access to the normals without modifying them.

##### Method: `getFaces()`
- **Full Signature**: `const std::vector<unsigned int>& getFaces() const;`
- **Return Type Explanation**: Returns a constant reference to the vector of face indices.
- **Internal Logic/Purpose**: Allows access to the faces without modifying them.

##### Method: `getAABB()`
- **Full Signature**: `const AABB& getAABB() const;`
- **Return Type Explanation**: Returns a constant reference to the Axis-Aligned Bounding Box (AABB) of the mesh.
- **Internal Logic/Purpose**: Allows access to the AABB without modifying it.

##### Method: `getMeshType()`
- **Full Signature**: `int getMeshType() const;`
- **Return Type Explanation**: Returns an integer indicating the type of the mesh.
  - `0`: Vertex-only (v)
  - `1`: Vertex and Texture Coordinates (v/vt)
  - `2`: Vertex and Normals (v//vn)
  - `3`: Vertex, Texture Coordinates, and Normals (v/vt/vn)
- **Internal Logic/Purpose**: Determines the type of the mesh based on whether texture coordinates and normals are present.

##### Method: `addVertex()`
- **Full Signature**: `void addVertex(float x, float y, float z);`
- **Return Type Explanation**: None
- **Internal Logic/Purpose**: Adds a vertex to the mesh and updates the AABB.

##### Method: `addTextureCoord()`
- **Full Signature**: `void addTextureCoord(float u, float v);`
- **Return Type Explanation**: None
- **Internal Logic/Purpose**: Adds texture coordinates to the mesh.

##### Method: `addNormal()`
- **Full Signature**: `void addNormal(float x, float y, float z);`
- **Return Type Explanation**: None
- **Internal Logic/Purpose**: Adds a normal to the mesh.

##### Method: `addFaceValue()`
- **Full Signature**: `void addFaceValue(int val);`
- **Return Type Explanation**: None
- **Internal Logic/Purpose**: Adds a face index to the mesh.

##### Method: `clear()`
- **Full Signature**: `void clear();`
- **Return Type Explanation**: None
- **Internal Logic/Purpose**: Clears all data from the mesh, including vertices, texture coordinates, normals, and faces.