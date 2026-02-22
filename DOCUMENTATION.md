# GLEngine – Documentazione API (classi e funzioni custom)

Documentazione di tutte le classi e funzioni definite nel progetto (header: `include/`, implementazioni: `src/`).  
OpenGL, GLFW e GLAD non sono documentati qui.

---

## 1. Config (`include/config.h`)

Non definisce classi o funzioni custom: raccoglie gli include comuni del progetto.

- **Include:** `glad/glad.h`, `GLFW/glfw3.h`, `iostream`, `vector`, `fstream`, `sstream`, `string`, `map`

---

## 2. Algebra e matematica (`include/algebricOp.h`, `src/algebricOp.cpp`)

### Costante

| Nome | Descrizione |
|------|-------------|
| `PI` | 3.14159265358979323846f (macro) |

---

### Classe `Vector3`

Vettore 3D con componenti in `entries[0]` (x), `entries[1]` (y), `entries[2]` (z).

| Metodo | Descrizione |
|--------|-------------|
| `static float dot(Vector3 u, Vector3 v)` | Prodotto scalare tra `u` e `v`. |
| `static Vector3 normalize(Vector3 v)` | Restituisce una copia di `v` normalizzata (lunghezza 1). |
| `void normalize()` | Normalizza il vettore in-place. |
| `static Vector3 cross(Vector3 u, Vector3 v)` | Prodotto vettoriale `u × v`. |

---

### Struct `mat4`

Matrice 4×4 in **column-major** (compatibile con `glUniformMatrix4fv(..., GL_FALSE, ...)`).  
I 16 float sono in `entries[0]` … `entries[15]`.

Tutte le funzioni statiche restituiscono una `mat4`; gli angoli sono in **gradi** (convertiti internamente in radianti).

| Funzione | Descrizione |
|----------|-------------|
| `create_matrix_transform(Vector3 translation)` | Matrice di sola **traslazione** (identità + translation in entries 12,13,14). |
| `create_z_rotation(float angle)` | Rotazione attorno all’**asse Z** (piano XY). Angolo in gradi, senso antiorario (mano destra). |
| `create_x_rotation(float angle)` | Rotazione attorno all’**asse X** (piano YZ). Angolo in gradi. |
| `create_y_rotation(float angle)` | Rotazione attorno all’**asse Y** (piano XZ). Angolo in gradi. |
| `create_model_transform(Vector3 pos, float angle)` | Matrice combinata: **rotazione Z** + **traslazione** `pos`. Usata tipicamente come matrice modello (oggetto che ruota nel piano e viene spostato). |
| `create_look_at(Vector3 from, Vector3 to)` | Matrice **view** “look-at”: camera in `from`, guarda verso `to`. **Up** del mondo fissato a `(0, 0, 1)` (asse Z verso l’alto). |
| `create_prospective_projection(float fovy, float aspect, float near, float far)` | Matrice di **proiezione prospettica**. `fovy` = campo visivo verticale in gradi; `aspect` = width/height; `near` e `far` = piani di clipping (distanze positive, la camera guarda lungo -Z). |

**Nota:** Non esiste (ancora) una moltiplicazione tra matrici `mat4 * mat4`; per combinare più trasformazioni (es. rotazione X poi Z poi traslazione) andrebbe implementata.

---

### Struct `Vertex`

Vertice con posizione, normale e UV per il pipeline OpenGL / OBJ.

| Campo | Tipo | Descrizione |
|-------|------|-------------|
| `position` | `float[3]` | Posizione (x, y, z). |
| `normal` | `float[3]` | Normale (x, y, z). |
| `uv` | `float[2]` | Coordinate di texture (u, v). |

| Metodo | Descrizione |
|--------|-------------|
| `void Vzero()` | Azzera tutti i campi (memset a 0). |
| `bool operator==(const Vertex& other)` | Confronto per uguaglianza (memcmp). |
| `bool operator<(const Vertex& other)` | Ordinamento less-than (memcmp), usato per deduplicazione nei mesh. |

---

## 3. Materiale / texture (`include/material.h`, `src/material.cpp`)

### Classe `Material`

Carica un’immagine da file (tramite stb_image) e crea una texture OpenGL 2D.  
Formato interno: RGBA. Wrap: REPEAT; min filter: NEAREST; mag filter: LINEAR.

| Metodo | Descrizione |
|--------|-------------|
| `Material(const char* filename)` | Carica l’immagine da `filename` e crea la texture. |
| `~Material()` | Rilascia la texture OpenGL. |
| `void use(int unit)` | Attiva la texture sull’unità di texture `unit` (es. 0 o 1) e la binda su `GL_TEXTURE_2D`. |

---

## 4. Mesh triangoli (`include/triangle_mesh.h`, `src/triangle_mesh.cpp`)

### Classe `TriangleMesh`

Mesh OpenGL **predefinita**: un quad (2 triangoli) con 4 vertici.  
Attributi: **posizioni** (location 0) e **colori** (location 1).  
**Nota:** gli shader attuali si aspettano pos + normal + UV; questa mesh non fornisce normali/UV, quindi è adatta solo a shader che usano pos (e eventualmente colore).

| Metodo | Descrizione |
|--------|-------------|
| `TriangleMesh()` | Crea VAO, 2 VBO (posizioni e colori) ed EBO; il quad è in z=0, da (-1,-1) a (1,1). |
| `void draw()` | Esegue `glDrawElements(GL_TRIANGLES, ...)` sul VAO. |
| `~TriangleMesh()` | Elimina VAO, VBO ed EBO. |

---

## 5. Loader OBJ e mesh GPU (`include/objLoader.h`, `src/objLoader.cpp`)

### Classe `Mesh`

Mesh renderizzabile con vertici in formato **posizione (3) + normale (3) + UV (2)** = 8 float per vertice.  
Usa un unico VBO (stride 8 float) e un EBO. Layout: location 0 = pos, 1 = normal, 2 = UV.

| Metodo | Descrizione |
|--------|-------------|
| `Mesh(std::vector<Vertex> vertices, std::vector<unsigned int>& indices)` | Costruisce VAO, VBO ed EBO dai vertici e dagli indici. |
| `void draw()` | Esegue `glDrawElements(GL_TRIANGLES, ...)`. |
| `~Mesh()` | Rilascia VAO, VBO ed EBO. |

---

### Classe `CPUMesh`

Contenitore in RAM per dati OBJ “grezzi”: liste di vertici, coordinate di texture, normali e facce (indici).  
Il **tipo** di mesh è derivato da quali liste sono presenti: 0 = solo vertici, 1 = vertici + UV, 2 = vertici + normali, 3 = vertici + UV + normali.

| Metodi getter | Restituiscono |
|---------------|----------------|
| `getVertices()`, `getTextureCoord()`, `getNormals()`, `getFaces()` | Le rispettive liste (copia). |

| Metodi add | Descrizione |
|------------|-------------|
| `addVertex(x, y, z)` | Aggiunge un vertice. |
| `addTextureCoord(u, v)` | Aggiunge una coppia UV. |
| `addNormal(x, y, z)` | Aggiunge una normale. |
| `addOneFaceValue(val)` | Aggiunge un indice alla lista facce (ordine gestito dal chiamante in base al tipo di faccia OBJ). |

| Metodi set | Descrizione |
|------------|-------------|
| `setVertex(index, x, y, z)` | Imposta il vertice all’indice dato. |
| `setTextureCoord(index, u, v)` | Imposta l’UV all’indice dato. |
| `setNormal(index, x, y, z)` | Imposta la normale all’indice dato. |
| `setOneFaceValue(index, val)` | Imposta l’indice di faccia alla posizione data. |

| Altro | Descrizione |
|-------|-------------|
| `int getMeshType()` | 0 = solo v, 1 = v+vt, 2 = v+vn, 3 = v+vt+vn. |
| `void clear()` | Svuota tutte le liste. |

---

### Classe `MeshLoader`

Funzioni statiche per caricare OBJ e convertire i dati in formato adatto a `Mesh`.

| Metodo | Descrizione |
|--------|-------------|
| `static bool GetObjFileData(const std::string& filepath, CPUMesh& out_mesh)` | Legge il file OBJ e riempie `out_mesh` (v, vt, vn, f). Supporta facce in formato `v`, `v/vt`, `v//vn`, `v/vt/vn` e triangola i poligoni con più di 3 vertici (fan). Restituisce `false` se il file non si apre. |
| `static std::vector<Vertex> formatForOpenGL(CPUMesh inMesh, std::vector<unsigned int>& map)` | Converte `inMesh` in un vettore di `Vertex` e riempie `map` con gli indici per l’EBO. Deduplica i vertici (stesso pos+normal+UV → stesso indice). `map` è l’array di indici da passare al costruttore di `Mesh`. |

---

## 6. Funzioni helper in `Main.cpp`

Definite e usate in `src/Main.cpp` (dichiarate in cima al file).

| Funzione | Descrizione |
|----------|-------------|
| `unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath)` | Carica i sorgenti da due file, crea vertex e fragment shader, li linka in un program e restituisce l’handle. Restituisce **0** in caso di errore (compilazione o link). I moduli shader vengono eliminati dopo il link. |
| `unsigned int make_module(const std::string& filepath, unsigned int module_type)` | Carica il sorgente da `filepath`, compila uno shader di tipo `module_type` (es. `GL_VERTEX_SHADER` o `GL_FRAGMENT_SHADER`) e restituisce l’handle. Restituisce **0** se il file non si apre o la compilazione fallisce. |
| `GLFWwindow* openGLEngineInit(int width, int height, const char* title)` | Inizializza GLFW, crea una finestra OpenGL 3.3 Core con le dimensioni e il titolo dati, carica GLAD, stampa in console versione OpenGL/GLSL e attributi della finestra, imposta **viewport** a 640×480 e **clear color** a (0.07, 0.13, 0.17, 1). Restituisce il puntatore alla finestra o **nullptr** in caso di errore. |

---

## Riepilogo dipendenze

- **algebricOp:** solo C++ standard + cmath, cstring.
- **Material:** config (OpenGL), stb_image.
- **TriangleMesh:** OpenGL, vector.
- **objLoader / Mesh / CPUMesh / MeshLoader:** config, algebricOp (Vertex, ecc.).
- **Main (make_shader, make_module, openGLEngineInit):** config, GLFW, GLAD, iostream, fstream, sstream.
