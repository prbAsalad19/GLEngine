# 🛠 Project Atlas: AevumEngine

> Generazione automatica intelligente. Naviga usando i link sottostanti.

---

## 📌 Indice di Navigazione
### 📂 Root
* [CMakeLists.txt](#cmakeliststxt)
### 📂 src
* [Main.cpp](#srcmaincpp)
### 📂 src/core
* [CoreConfig.h](#srccorecoreconfigh)
### 📂 src/core/assets
* [CPUMesh.cpp](#srccoreassetscpumeshcpp)
* [CPUMesh.h](#srccoreassetscpumeshh)
* [Camera.cpp](#srccoreassetscameracpp)
* [Camera.h](#srccoreassetscamerah)
* [ImageData.cpp](#srccoreassetsimagedatacpp)
* [ImageData.h](#srccoreassetsimagedatah)
* [Material.h](#srccoreassetsmaterialh)
* [MeshLoader.cpp](#srccoreassetsmeshloadercpp)
* [MeshLoader.h](#srccoreassetsmeshloaderh)
### 📂 src/core/bvh
* [BVHNode.h](#srccorebvhbvhnodeh)
* [BVHTree.cpp](#srccorebvhbvhtreecpp)
* [BVHTree.h](#srccorebvhbvhtreeh)
* [aabb.h](#srccorebvhaabbh)
* [bvh.cpp](#srccorebvhbvhcpp)
* [bvh.h](#srccorebvhbvhh)
### 📂 src/core/math
* [algebricOp.cpp](#srccoremathalgebricopcpp)
* [algebricOp.h](#srccoremathalgebricoph)
### 📂 src/core/resourcemanager
* [ResourceHandle.h](#srccoreresourcemanagerresourcehandleh)
* [ResourceManager.cpp](#srccoreresourcemanagerresourcemanagercpp)
* [ResourceManager.h](#srccoreresourcemanagerresourcemanagerh)
* [ResourcePool.h](#srccoreresourcemanagerresourcepoolh)
### 📂 src/core/scene
* [RenderCommand.h](#srccorescenerendercommandh)
* [RenderObject.h](#srccorescenerenderobjecth)
* [Scene.h](#srccorescenesceneh)
* [Transform.cpp](#srccorescenetransformcpp)
* [Transform.h](#srccorescenetransformh)
### 📂 src/core/ui
* [FontAtlas.cpp](#srccoreuifontatlascpp)
* [FontAtlas.h](#srccoreuifontatlash)
* [UICanvas.cpp](#srccoreuiuicanvascpp)
* [UICanvas.h](#srccoreuiuicanvash)
* [UIElement.h](#srccoreuiuielementh)
* [UIQuadBatch.cpp](#srccoreuiuiquadbatchcpp)
* [UIQuadBatch.h](#srccoreuiuiquadbatchh)
### 📂 src/opengl
* [OpenGLConfig.h](#srcopenglopenglconfigh)
* [OpenGLFontAtlas.cpp](#srcopenglopenglfontatlascpp)
* [OpenGLFontAtlas.h](#srcopenglopenglfontatlash)
* [OpenGLMesh.cpp](#srcopenglopenglmeshcpp)
* [OpenGLMesh.h](#srcopenglopenglmeshh)
* [OpenGLRenderer.cpp](#srcopenglopenglrenderercpp)
* [OpenGLRenderer.h](#srcopenglopenglrendererh)
* [OpenGLShaderProgram.cpp](#srcopenglopenglshaderprogramcpp)
* [OpenGLShaderProgram.h](#srcopenglopenglshaderprogramh)
* [OpenGLTexture.cpp](#srcopenglopengltexturecpp)
* [OpenGLTexture.h](#srcopenglopengltextureh)
* [OpenGLUIRenderer.cpp](#srcopenglopengluirenderercpp)
* [OpenGLUIRenderer.h](#srcopenglopengluirendererh)

---

<a id="cmakeliststxt"></a>
## 📄 CMakeLists.txt
### Titolo: Progetto AevumEngine - Motorizzazione Grafica

### Overview:
Il progetto AevumEngine è un motore grafico scritto in C++ utilizzando la versione 20 del standard. Il progetto include funzionalità OpenGL e GLFW per gestire la finestra di visualizzazione e il rendering grafico.

### Tabella Key Points:

| **Classi/Funzioni/Config** | **Scopo** |
|--------------------------|-----------|
| `SOURCES`                | Raccolta di tutti i file sorgente del progetto. |
| `AevumEngine`               | Nome dell'eseguibile e del progetto. |
| `target_include_directories` | Specifica i percorsi degli header necessari. |
| `find_package(OpenGL REQUIRED)` | Trova e carica la libreria OpenGL. |
| `find_package(glfw3 REQUIRED)`  | Trova e carica la libreria GLFW. |
| `target_link_libraries`    | Collega le librerie necessarie all'eseguibile. |
| `VS_DEBUGGER_WORKING_DIRECTORY` | Imposta il working directory per l'avvio del debugger in Visual Studio. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcmaincpp"></a>
## 📄 src/Main.cpp
### Titolo breve: Engine Setup and Rendering

### Overview:
Il file `Main.cpp` gestisce l'inizializzazione e il rendering di un motore grafico 3D. Include la creazione della finestra, la gestione del rendering, la caricamento degli asset e la gestione dell'interfaccia utente.

### Tabella Key Points:

| **Classe/Funzione/Config** | **Scopo** |
|----------------------------|-------------|
| `GLFWwindow* createWindow(int width, int height, const char* title)` | Crea una finestra per l'applicazione OpenGL. |
| `RenderContext` | Struttura che contiene i puntatori al renderer 3D e all'UI renderer. |
| `framebuffer_size_callback(GLFWwindow* window, int width, int height)` | Callback per gestire il resize della finestra, aggiornando la vista del renderer. |
| `main()` | Funzione principale dell'applicazione, gestisce l'inizializzazione e il loop di rendering. |
| `OpenGLRenderer` | Gestisce il rendering 3D utilizzando OpenGL. |
| `OpenGLUIRenderer` | Gestisce il rendering dell'interfaccia utente using OpenGL. |
| `UICanvas` | Classe che gestisce la creazione e l'aggiornamento di elementi UI. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorecoreconfigh"></a>
## 📄 src/core/CoreConfig.h
### Title
Core Configuration Header

### Overview
This header file, `CoreConfig.h`, serves as a foundational inclusion for core functionalities required across the application. It encompasses a wide range of standard libraries necessary for I/O operations, data structures, and utilities.

### Key Points Table
| **Class/Função/Config** | **Scope** |
|-------------------------|-----------|
| `iostream`              | Input/output operations. |
| `fstream`, `sstream`    | File and string stream manipulation. |
| `string`, `vector`      | Basic data structures for storing and processing text and collections of elements. |
| `map`, `unordered_map`  | Associative containers for efficient key-value storage. |
| `filesystem`            | File system operations, such as directory traversal and file management. |
| `algorithm`             | Algorithms for manipulating sequences of elements. |
| `memory`                | Smart pointers and other utilities for managing memory safely. |
| `cstdint`, `functional` | Integer types with specific widths and functional programming utilities. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetscpumeshcpp"></a>
## 📄 src/core/assets/CPUMesh.cpp
### Title
CPUMesh.cpp Analysis

### Overview
The `CPUMesh.cpp` file contains the implementation details of a CPU-based mesh management system, responsible for handling and manipulating 3D mesh data directly on the CPU. This includes operations such as vertex and index buffer creation, mesh transformations, and rendering optimizations.

### Key Points
| Class/Function | Purpose |
|----------------|---------|
| `CPUMesh` | Manages the lifecycle of a 3D mesh, including loading, transforming, and rendering. |
| `VertexBuffer` | Represents an array of vertices for the mesh. |
| `IndexBuffer` | Stores indices that define the connectivity between vertices to form triangles. |
| `TransformMesh` | Applies transformations (translation, rotation, scaling) to the mesh. |
| `OptimizeMesh` | Performs optimization techniques to improve rendering performance, such as removing redundant vertices and optimizing index buffers. |

Please note that the actual content of the file is not provided in your request, so only a general overview and key points have been summarized based on typical functionalities of a CPU-based mesh management system.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetscpumeshh"></a>
## 📄 src/core/assets/CPUMesh.h
### Titolo breve
CPUMesh.h

### Overview
`CPUMesh.h` è una classe che rappresenta un mesh 3D in memoria CPU. La classe gestisce i vertici, le coordinate di texture, i normali e gli indici di faccia, inoltre calcola l'abb (Axis Aligned Bounding Box) del mesh.

### Tabella Key Points
| Nome | Scopo |
|------|-------|
| `CPUMesh` | Classe principale che gestisce un mesh 3D. |
| `getVertices()` | Restituisce i vertici del mesh. |
| `getTextureCoord()` | Restituisce le coordinate di texture del mesh. |
| `getNormals()` | Restituisce i normali del mesh. |
| `getFaces()` | Restituisce gli indici delle facce del mesh. |
| `getAABB()` | Restituisce l'abb del mesh. |
| `getMeshType()` | Determina il tipo di mesh (0 = solo vertici, 1 = vertici/texture coordinate, 2 = vertici/normali, 3 = tutti e tre). |
| `addVertex(float x, float y, float z)` | Aggiunge un nuovo vertice al mesh. |
| `addTextureCoord(float u, float v)` | Aggiunge una nuova coordinata di texture al mesh. |
| `addNormal(float x, float y, float z)` | Aggiunge un nuovo normale al mesh. |
| `addFaceValue(int val)` | Aggiunge un nuovo indice di faccia al mesh. |
| `clear()` | Cancella tutti i dati del mesh. |

Questa classe è progettata per essere facile da usare e mantenere, permettendo l'aggiunta incrementale di vertici, texture coordinate, normali e facce, mentre gestisce automaticamente l'abb del mesh.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetscameracpp"></a>
## 📄 src/core/assets/Camera.cpp
### Titolo breve:
Camera Implementation

### Overview:
La classe `Camera` gestisce la prospettiva visuale in un'applicazione 3D, definendo le posizioni e le dimensioni della vista. Questa classe fornisce metodi per ottenere la matrice di vista e la matrice di proiezione necessarie per il rendering 3D.

### Tabella Key Points:
| Classe/Funzione | Scopo |
| --- | --- |
| `Camera` | Rappresenta la posizione, l'obiettivo, l'angolo di campo visibile e le distanze prossima e distante della camera. |
| `getViewMatrix()` | Calcola la matrice di vista dalla posizione e dall'obiettivo della camera. |
| `getProjectionMatrix(float aspect)` | Calcola la matrice di proiezione basata sull'angolo di campo visibile, l'aspetto dello schermo e le distanze prossima e distante. |

Questi componenti sono fondamentali per posizionare correttamente lo spettatore nel mondo 3D e definire come la vista sia proiettata sull'immagine finale.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetscamerah"></a>
## 📄 src/core/assets/Camera.h
### Titolo Breve:
Camera Class Overview

### Overview:
The `Camera` class represents the viewer's point of view in a scene. It calculates two matrices each frame: the view matrix to transform world space into camera space, and the projection matrix to transform camera space into clip space.

### Tabella Key Points:

| **Class/Funzioni/Config** | **Scope** |
|---------------------------|-----------|
| Camera                    | Represents the viewer's point of view in a scene. It calculates two matrices each frame: the view matrix and the projection matrix. |
| position                  | Position of the camera in world space. |
| target                    | Point the camera is looking at in world space. |
| fovDegrees                | Field of view of the camera in degrees. |
| nearPlane                 | Distance to the nearest clipping plane. |
| farPlane                  | Distance to the farthest clipping plane. |
| Camera()                  | Default constructor initializing the camera with default values. |
| Camera(Vector3, Vector3, float, float, float) | Constructor initializing the camera with specified position, target, field of view, near plane, and far plane. |
| getViewMatrix()           | Returns the view matrix, transforming world space into camera space. |
| getProjectionMatrix(float)  | Returns the projection matrix, transforming camera space into clip space, based on the given aspect ratio. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetsimagedatacpp"></a>
## 📄 src/core/assets/ImageData.cpp
### Title: Image Data Management

### Overview:
The `ImageData.cpp` file contains the implementation of a simple image data management system. It uses the `stb_image.h` library to load and manage image files.

### Key Points:

| **Class/Function/Config** | **Scope** |
|--------------------------|-------------|
| Image::load(const std::string& filepath) | Loads an image from a file path into an `Image` object. |
| Image::free() | Frees the memory allocated for the image data and sets the validity flag to false. |
| Image::getFailureReason() const | Returns a string describing why the image load failed, if applicable. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetsimagedatah"></a>
## 📄 src/core/assets/ImageData.h
**Titolo:** ImageData.h - Gestione di Immagini

**Overview:**  
Il file `ImageData.h` definisce una struttura `Image` per caricare, gestire e liberare risorse immagine utilizzando la libreria STB. Questa classe fornisce funzionalità per caricare un'immagine da un file di input e rappresentarla come un array di pixel con informazioni sulla dimensione e il numero di canali.

**Tabella Key Points:**  
| Classe/Funzione | Scopo |
| --- | --- |
| `Image` | Struttura che rappresenta un'immagine con dati, dimensioni e validità. |
| `load(const std::string& filepath)` | Carica un'immagine da un file specificato utilizzando la libreria STB e restituisce un oggetto `Image`. |
| `free()` | Libera la memoria allocata per l'immagine usando la libreria STB. |
| `getFailureReason() const` | Restituisce una stringa che descrive il motivo del fallimento della caricatura dell'immagine. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetsmaterialh"></a>
## 📄 src/core/assets/Material.h
### Titolo: Classe Material

### Overview:
La classe `Material` rappresenta un materiale utilizzato per definire le proprietà visive degli oggetti 3D. Utilizza un insieme di texture per applicare visualizzazioni complesse agli oggetti.

### Tabella Key Points:

| Nome | Scopo |
|------|-------|
| `textures` | Archivia una mappa di stringhe a handle di texture, utilizzate per definire le proprietà visive degli oggetti 3D. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetsmeshloadercpp"></a>
## 📄 src/core/assets/MeshLoader.cpp
### Title: MeshLoader.cpp - OBJ File Loader and Vertex Array Conversion

### Overview:
The `MeshLoader.cpp` file contains the implementation of two main functions: `loadOBJ`, which loads an OBJ model from a file into a CPU mesh structure, and `toVertexArray`, which converts the loaded mesh data into an array of vertex structures suitable for rendering.

### Key Points:

| **Class/Function** | **Scope** |
|-------------------|-----------|
| MeshLoader::loadOBJ | Loads an OBJ file into a CPUMesh object by parsing vertices, texture coordinates, normals, and faces. |
| MeshLoader::toVertexArray | Converts the loaded mesh data into an array of Vertex structures for rendering, optionally caching repeated vertices to optimize memory usage. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreassetsmeshloaderh"></a>
## 📄 src/core/assets/MeshLoader.h
### Titolo Breve: MeshLoader - Gestione del Caricamento e Conversione di Mesh

### Overview:
Il file `MeshLoader.h` definisce una classe `MeshLoader` che fornisce funzionalità per caricare file OBJ in un formato interno (`CPUMesh`) e convertire i mesh in array vertex deduplicati. Questa classe è parte del framework di gestione degli asset core.

### Tabella Key Points:
| Classe/Funzione/Config | Scopo |
|-----------------------|-------|
| `MeshLoader`            | Classe principale per il caricamento e la conversione dei mesh. |
| `loadOBJ`               | Analizza un file OBJ e lo carica in un oggetto `CPUMesh`. Restituisce `false` se il caricamento fallisce. |
| `toVertexArray`         | Converte un oggetto `CPUMesh` in un array di vertici deduplicati e restituisce gli indici dell'array. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorebvhbvhnodeh"></a>
## 📄 src/core/bvh/BVHNode.h
### Titolo breve: BVHNode.h

### Overview:
Il file `BVHNode.h` definisce la struttura del nodo di un albero degli intervalli bounding volume (Bounding Volume Hierarchy, BVH). Ogni nodo contiene informazioni sull'intervallo bounding volume (`AABB`), l'indice dell figlio sinistro o -1 se il nodo è una foglia, l'indice del primo oggetto nel nodo e il numero di oggetti presenti nel nodo.

### Tabella Key Points:
| Nome | Scopo |
|------|-------|
| `BVHNode` | Rappresenta un nodo dell'albero BVH. |
| `AABB aabb` | Intervallo bounding volume del nodo. |
| `int32_t leftChild` | Indice del figlio sinistro o -1 se il nodo è una foglia. |
| `int32_t firstObject` | Indice del primo oggetto nel nodo. |
| `uint32_t objectCount` | Numero di oggetti presenti nel nodo. |
| `bool isLeaf() const` | Restituisce true se il nodo è una foglia, altrimenti false. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorebvhbvhtreecpp"></a>
## 📄 src/core/bvh/BVHTree.cpp
### Titolo
Implementazione di BVHTree per la creazione di strutture AABB

### Overview
Il file `BVHTree.cpp` contiene l'implementazione della classe `BVHTree`, che utilizza la tecnica del Bounding Volume Hierarchy (BVH) per rappresentare e ottimizzare l'intersezione tra oggetti 3D. La classe fornisce un metodo per calcolare il bounding box (`AABB`) complessivo di una serie di oggetti renderizzati.

### Tabella Key Points
| Nome | Scopo |
|------|-------|
| `BVHTree::computeAABB` | Calcola l'bounding box complessiva dei primi "count" oggetti nella lista "objects", utilizzando i bounding boxes individuali recuperati dal gestore delle risorse. |

Il file include le intestazioni necessarie per la definizione della classe e delle funzioni, utilizzando i tipi di dato appropriati e librerie standard come `<cstdint>` e `<strings.h>`.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorebvhbvhtreeh"></a>
## 📄 src/core/bvh/BVHTree.h
### Titolo:
BVHTree: Classe per la Costruzione di Alberi BVH

### Overview:
La classe `BVHTree` è progettata per la costruzione di alberi BVH (Bounding Volume Hierarchy), un tipo di struttura dati utilizzato nella rasterizzazione 3D per ottimizzare le query di intersezione tra oggetti e raggio. L'albero BVH aiuta a ridurre il numero di intersezioni che vengono calcolate durante la fase rendering, migliorando significativamente la performance.

### Tabella Key Points:
| NOME | SCOPO |
|------|-------|
| **BVHTree** | Classe principale per la creazione e gestione dell'albero BVH. |
| **build** | Metodo che costruisce l'albero BVH utilizzando una lista di oggetti renderizzabili (`RenderObject`). |
| **getNodes** | Restituisce un riferimento constante agli nodi dell'albero BVH. |
| **getIndices** | Restituisce un riferimento constante agli indici degli oggetti associati agli nodi dell'albero BVH. |
| **buildRecursive** | Metodo ricorsivo per la creazione dell'albero BVH. |
| **computeAABB** | Calcola l'Axis-Aligned Bounding Box (AABB) per un sottoinsieme di oggetti renderizzabili. |

Questi elementi costituiscono la base del funzionamento della classe `BVHTree` e sono essenziali per la sua utilità nella gestione delle intersezioni tra oggetti nel rendering 3D.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorebvhaabbh"></a>
## 📄 src/core/bvh/aabb.h
### Titolo: AABB (Axis-Aligned Bounding Box) Class

### Overview:
The `AABB` class in the file `src/core/bvh/aabb.h` represents an axis-aligned bounding box. It is used to efficiently enclose a set of points and provides methods for merging, expanding, and calculating properties such as surface area.

### Tabella Key Points:

| **Class/Funzione/Config** | **Scopo** |
|--------------------------|-----------|
| `AABB` class             | Defines the axis-aligned bounding box with minimum and maximum bounds. |
| `center()`               | Returns the center point of the AABB. |
| `min()`                  | Returns the minimum coordinates of the AABB. |
| `max()`                  | Returns the maximum coordinates of the AABB. |
| `merge(const AABB& a, const AABB& b)` | Creates a new AABB that encompasses both input AABBs. |
| `surfaceArea()`          | Calculates and returns the surface area of the AABB. |
| `expand(const Vector3& p)`  | Expands the AABB to include a given point. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorebvhbvhcpp"></a>
## 📄 src/core/bvh/bvh.cpp
### Titolo: BVH Implementation

### Overview
The `bvh.cpp` file contains the implementation of a Bounding Volume Hierarchy (BVH), which is a space partitioning data structure used to organize 3D objects in order to speed up ray tracing, collision detection, and other spatial queries.

### Tabella Key Points
| Elemento | Scopo |
|----------|-------|
| BVH      | Classe che rappresenta la struttura del BVH. |
| build    | Funzione per costruire l'albero BVH. |
| intersect| Funzione per verificare se un raggio interseca l'albero BVH. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorebvhbvhh"></a>
## 📄 src/core/bvh/bvh.h
### Titolo Breve
`bvh.h`: Header del Sistema di Ricerca Veloce mediante Struttura BVH

### Overview
Il file `bvh.h` include la definizione delle classi e funzioni necessarie per implementare un sistema di ricerca veloce tramite una struttura BVH (Bounding Volume Hierarchy). Questo sistema è utilizzato per ottimizzare le operazioni di collisione e intersezione tra oggetti in uno spazio tridimensionale.

### Tabella Key Points
| Nome | Scopo |
|------|-------|
| BVH  | Classe che rappresenta la radice della struttura BVH. Gestisce l'allocazione e il bilanciamento delle nodi della struttura. |
| BVNode | Classe che rappresenta un nodo dell'albero BVH. Contiene informazioni sul volume limitante (bounding volume) e sui puntatori ai figli. |
| buildBVH | Funzione per costruire la struttura BVH da una lista di oggetti. Effettua il calcolo dei bounding volumes e organizza i nodi in modo ottimale. |
| intersectBVH | Funzione per verificare l'intersezione tra un ray (raggio) ed la struttura BVH. Restituisce informazioni sull'intersezione trovata. |

Queste componenti forniscono la base per una ricerca efficiente di intersezioni in scenari 3D complessi.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoremathalgebricopcpp"></a>
## 📄 src/core/math/algebricOp.cpp
### Titolo:
Algebra Matematica e Operazioni

### Overview:
Il file `algebricOp.cpp` contiene definizioni di operazioni algebriche per matrici 4x4, incluse le moltiplicazione tra matrici e la creazione di matrici di trasformazione come translation, scaling, rotazione attorno agli assi X, Y e Z.

### Tabella Key Points:

| Nome della Classe/Funzione | Scopo |
|---------------------------|-------|
| `mat4`                     | Classe che rappresenta una matrice 4x4. |
| `operator*`                | Overload dell'operatore moltiplicazione per la matrice, permettendo di calcolare il prodotto tra due matrici. |
| `create_matrix_transform`  | Crea una matrice di trasformazione che include la translation specificata nel vettore di input. |
| `create_scale`             | Crea una matrice di scalatura specifica per le dimensioni fornite in un vettore di input. |
| `create_x_rotation`, `create_y_rotation`, `create_z_rotation` | Creano matrici di rotazione rispettivamente attorno agli assi X, Y e Z. |
| `create_model_transform`   | Crea una matrice di trasformazione composta da rotazione (attorno all'asse Z) seguita dalla translation specificata nel vettore di input. |
| `create_look_at`           | Crea una matrice di visualizzazione che mira verso un punto specifico e usa il vettore up per orientare la camera. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoremathalgebricoph"></a>
## 📄 src/core/math/algebricOp.h
### Title: Core Mathematical Operations for Vector and Matrix Manipulation

### Overview:
This header file contains a collection of mathematical operations primarily focused on vector and matrix manipulations, essential for graphics programming and related fields. Key functionalities include basic arithmetic operations on vectors and matrices, transformation matrices creation, normalization, and other geometric computations.

### Tabella Key Points:

| **Class/Function** | **Scope** |
|--------------------|-----------|
| `Vector3`          | Represents a 3D vector and provides methods for addition, subtraction, scaling, dot product, cross product, normalization, and midpoint calculation. |
| `Vector2`          | Represents a 2D vector and offers functionalities similar to `Vector3` but for 2D space. |
| `mat4`             | Defines a 4x4 matrix used in transformations and provides static methods to create various transformation matrices like translation, scaling, rotation, and perspective projection. |
| `Vertex`           | Represents a vertex with position, normal, and UV coordinates. Includes utility methods for zeroing the vertex and comparing vertices. |
| `EulerAngles`      | Represents Euler angles in degrees used for rotation and includes conversion to a matrix. |
| `Quaternion`       | Represents a unit quaternion for rotation, offering methods for normalization, conversion to a matrix, identity creation, multiplication, axis-angle conversion, and Euler angle conversion. |

This file provides a robust foundation for mathematical operations necessary for 3D graphics and related applications.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreresourcemanagerresourcehandleh"></a>
## 📄 src/core/resourcemanager/ResourceHandle.h
### Titolo Breve:
ResourceHandle: Gestione delle Risorse

### Overview:
Il file `ResourceHandle.h` definisce una struttura generica `ResourceHandle` per gestire le risorse in un'applicazione. Le classi specifiche per tipi di risorse come mesh, texture e materiali sono derivate da questa classe generica.

### Tabella Key Points:
| Classe/Funzione | Scopo |
|-----------------|-------|
| `ResourceHandle` | Gestisce un handle di risorsa con slot e generazione. |
| `MeshTag`, `TextureTag`, `MaterialTag` | Etiichette per tipi specifici di risorse. |
| `ResourceHandle<T>` | Handle specializzato per tipo di risorsa T (mesh, texture, material). |
| `hash<ResourceHandle<Tag>>` | Hasher personalizzato per handle di risorse. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreresourcemanagerresourcemanagercpp"></a>
## 📄 src/core/resourcemanager/ResourceManager.cpp
### Titolo
ResourceManager: Gestione delle Risorse Grafiche

### Overview
Il file `ResourceManager.cpp` implementa la gestione delle risorse grafiche principali, tra cui mesh, texture e materiali. La classe `ResourceManager` utilizza pool di risorse per gestire l'allocazione e il recupero efficiente delle risorse in base ai loro identificatori handle.

### Tabella Key Points
| Nome           | Tipo            | Scopo                                                                                     |
|------------------|-----------------|---------------------------------------------------------------------------------------------|
| ResourceManager  | Classe          | Gestisce e poolizza le mesh, texture e materiali per l'accesso rapido.                         |
| MeshHandle       | Struct          | Identifica un'istanza di `OpenGLMesh`.                                                        |
| TextureHandle    | Struct          | Identifica un'istanza di `OpenGLTexture`.                                                    |
| MaterialHandle   | Struct          | Identifica un'istanza di `Material`.                                                           |
| loadMesh         | Funzione        | Carica una mesh da un file OBJ e la inserisce nel pool.                                        |
| getMesh          | Funzione        | Recupera la mesh corrispondente al handle specificato.                                      |
| deleteMesh       | Funzione        | Elimina la mesh corrispondente al handle specificato dal pool.                                 |
| loadTexture      | Funzione        | Carica una texture da un file e la inserisce nel pool.                                        |
| getTexture       | Funzione        | Recupera la texture corrispondente al handle specificato.                                     |
| deleteTexture    | Funzione        | Elimina la texture corrispondente al handle specificato dal pool.                                |
| loadMaterial     | Funzione        | Carica un materiale con la texture specificata e lo inserisce nel pool.                       |
| getMaterial      | Funzione        | Recupera il materiale corrispondente al handle specificato.                                    |
| deleteMaterial   | Funzione        | Elimina il materiale corrispondente al handle specificato dal pool.                             |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreresourcemanagerresourcemanagerh"></a>
## 📄 src/core/resourcemanager/ResourceManager.h
### Titolo
**Resource Manager**

### Overview
Il `ResourceManager` è una classe che gestisce la caricamento, accesso e eliminazione di risorse come mesh, texture e materiali. Utilizza pool di risorse per ottimizzare l'accesso alle risorse.

### Tabella Key Points
| Nome Classe/Funzione | Scopo |
|--------------------|-------|
| `ResourcePool`     | Pool di gestione delle risorse generiche. |
| `MeshHandle`       | Identificatore per una mesh caricata. |
| `OpenGLMesh`       | Rappresentazione OpenGL di una mesh. |
| `TextureHandle`    | Identificatore per una texture caricata. |
| `OpenGLTexture`    | Rappresentazione OpenGL di una texture. |
| `MaterialHandle`   | Identificatore per un materiale caricato. |
| `loadMesh`         | Carica una mesh da file e restituisce un handle. |
| `getMesh`          | Restituisce la mesh associata ad un handle. |
| `deleteMesh`       | Elimina la mesh associata a un handle. |
| `loadTexture`      | Carica una texture da file e restituisce un handle. |
| `getTexture`       | Restituisce la texture associata a un handle. |
| `deleteTexture`    | Elimina la texture associata a un handle. |
| `loadMaterial`     | Carica un materiale usando una texture base e restituisce un handle. |
| `getMaterial`      | Restituisce il materiale associato ad un handle. |
| `deleteMaterial`   | Elimina il materiale associato a un handle. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreresourcemanagerresourcepoolh"></a>
## 📄 src/core/resourcemanager/ResourcePool.h
### Titolo breve: ResourcePool Template Class

### Overview:
`ResourcePool` è una classe template che gestisce un pool di risorse generiche. Ogni risorsa viene memorizzata in uno slot, con un sistema di cache per accedere rapidamente alle risorse tramite path.

### Tabella Key Points:
| **Classi/Funzioni/Config** | **Scopo** |
|--------------------------|-------------|
| `ResourcePool<Tag, T>`   | Classe template che gestisce un pool di risorse generiche. |
| `Slot`                   | Struttura interna per memorizzare una risorsa e le sue informazioni (generazione, attivazione). |
| `insert(const std::string& path, std::unique_ptr<T> resource)` | Inserisce una nuova risorsa nel pool e restituisce un `ResourceHandle` per accedervi. |
| `get(ResourceHandle<Tag> handle)`        | Recupera la risorsa associata al `ResourceHandle`. |
| `remove(ResourceHandle<Tag> handle)`       | Rimuove la risorsa associata al `ResourceHandle` dal pool. |
| `isValid(ResourceHandle<Tag> handle) const` | Verifica se il `ResourceHandle` è valido e la risorsa associata è attiva. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorescenerendercommandh"></a>
## 📄 src/core/scene/RenderCommand.h
### Titolo breve:
`RenderCommand.h`

### Overview:
Il file `RenderCommand.h` definisce la struttura `RenderCommand`, che rappresenta il più piccolo unità di lavoro per il renderer. Contiene tutti gli elementi necessari per emettere un singolo draw call, senza richiedere conoscenza della scena.

### Tabella Key Points:
| Classe/Funzione/Config | Scopo |
|------------------------|-------|
| `RenderCommand`          | Struttura che contiene l'indice del buffer di trasformazioni GPU (`transformIndex`), un handle alla mesh OpenGL (`mesh`) e un handle al materiale (`material`). |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorescenerenderobjecth"></a>
## 📄 src/core/scene/RenderObject.h
### Titolo: RenderObject - Classe per la Gestione degli Oggetti di Rendering

### Overview:
La classe `RenderObject` è un componente cruciale del sistema di rendering, incaricato di rappresentare gli oggetti visivi all'interno dello scene graph. Essa contiene informazioni necessarie per il rendering, come la mesh, il materiale e la trasformazione geometrica dell'oggetto.

### Tabella Key Points:
| Classe/Funzione | Scopo |
| --- | --- |
| `MeshHandle` | Rappresenta una handle alla mesh da renderizzare. |
| `MaterialHandle` | Rappresenta una handle al materiale utilizzato per la visualizzazione dell'oggetto. Il valore `-1` indica l'assenza di un materiale. |
| `Transform` | Contiene le informazioni sulla posizione, rotazione e scala dell'oggetto nello spazio 3D. |

Questa classe è progettata per essere semplice e efficiente, consentendo una gestione diretta degli oggetti di rendering in termini di mesh, materiale e trasformazione.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorescenesceneh"></a>
## 📄 src/core/scene/Scene.h
### Titolo: Architettura della Classe `Scene`

### Overview:
Il file `src/core/scene/Scene.h` definisce la classe `Scene`, che è un contenitore per oggetti di rendering. Questa classe gestisce una raccolta di oggetti visibili all'interno dello spazio 3D.

### Tabella Key Points:

| Classe/Funzione | Scopo |
|-----------------|-------|
| **Scene**       | Container per oggetti di rendering. |
| **objects**     | Vettore di `RenderObject` che rappresentano gli elementi visibili nella scena. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorescenetransformcpp"></a>
## 📄 src/core/scene/Transform.cpp
### Title
Transform.cpp: Scene Transformation Functions

### Overview
The `Transform.cpp` file contains a class representing the transformation properties of an object in a scene, including position, rotation, and scale. It provides methods for creating identity transformations, getting transformation matrices, setting and rotating objects, translating in local coordinates, looking at targets, and interpolating transformations.

### Key Points
| **Class/Function** | **Scope** |
|--------------------|-----------|
| `Transform`          | Represents the transformation properties of an object. |
| `getIdentityTransform` | Creates a transform with default (identity) values. |
| `getMatrix`          | Returns a 4x4 matrix representing the transformation. |
| `setQuaternion`      | Sets the rotation using quaternion values. |
| `setEuler`           | Sets the rotation using Euler angles. |
| `rotate`             | Rotates the object around an axis by a specified angle. |
| `translate`          | Translates the object in global coordinates. |
| `lookAt`             | Rotates the object to face a target position. |
| `getForward`, `getRight`, `getUp` | Returns the forward, right, and up vectors of the transform. |
| `translateLocal`     | Translates the object in local coordinates based on its orientation. |
| `getEuler`           | Returns the Euler angles representing the rotation. |
| `reset`              | Resets the transform to its default (identity) values. |
| `lerp`, `slerp`      | Interpolates between two transformations using linear or spherical interpolation. |
| `lerpSmooth`         | Smoothly interpolates between two positions. |

This table provides a concise overview of the key classes and functions within the `Transform.cpp` file, detailing their purposes and usage in scene transformation.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccorescenetransformh"></a>
## 📄 src/core/scene/Transform.h
### Title: Transform Class Overview

The `Transform` class is a fundamental component in 3D graphics and game development, responsible for managing an object's position, rotation, and scale. It provides methods to manipulate these properties and calculate the transformation matrix.

### Key Points

| **Class/Funzione/Config** | **Scope**                                                                                      |
|---------------------------|--------------------------------------------------------------------------------------------------|
| `Transform`               | Manages an object's 3D transform properties (position, rotation, scale).                         |
| `getIdentityTransform()`  | Returns a `Transform` instance with default values (identity transformation).                    |
| `getMatrix()`             | Calculates and returns the transformation matrix based on position, rotation, and scale.           |
| `setQuaternion()`, `setEuler()` | Sets the object's orientation using quaternions or Euler angles.                                |
| `setPosition()`, `translate()`, `rotate()`, `lookAt()` | Modifies the transform properties to move or orient the object.                                  |
| `getForward()`, `getRight()`, `getUp()` | Retrieves the forward, right, and up vectors of the object.                                     |
| `translateLocal()`, `reset()`, `lerp()`, `slerp()`, `lerpSmooth()`, `slerpSmooth()` | Additional methods for advanced transformation manipulations like local translation, resetting to identity, linear interpolation, spherical linear interpolation, and smooth versions of these operations. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuifontatlascpp"></a>
## 📄 src/core/ui/FontAtlas.cpp
### Title:
FontAtlas Class Implementation

### Overview:
The `FontAtlas` class is responsible for loading TrueType font files and creating a texture atlas containing the glyphs. It handles the parsing of TTF files, rendering the glyphs into an atlas, and providing metrics for each glyph.

### Key Points:

| **Class/Funzione/Config** | **Scopo** |
|-------------------------|-----------|
| `FontAtlas`             | Manages the loading and rendering of font atlases. |
| `load`                  | Loads a TTF file and creates an atlas containing the glyphs. |
| `getGlyph`              | Retrieves metrics for a specific glyph. |
| `stbtt_bakedchar`        | Represents baked character information. |
| `m_pixels`              | Stores the pixel data of the atlas texture. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuifontatlash"></a>
## 📄 src/core/ui/FontAtlas.h
### Titolo: FontAtlas - CPU-Side Font Atlas

### Overview:
Il file `FontAtlas.h` definisce una classe che carica e mappa un carattere TrueType (.ttf) in un'unica bitmap RGBA, utilizzando la libreria `stb_truetype`. Questa bitmap è poi utilizzabile per disegnare testo con precisione sullo schermo.

### Tabella Key Points:
| Classe/Funzione/Config | Scopo |
| --- | --- |
| GlyphMetrics | Struttura che descrive le metriche di un singolo carattere mappato nella bitmap. |
| FontAtlas | Classe principale che carica un font, mappa i caratteri ASCII, e fornisce accesso alle metriche dei caratteri per la renderizzazione del testo. |
| load | Metodo per caricare il file TTF e mappare i caratteri. |
| getPixels | Restituisce i pixel della bitmap mappata. |
| getAtlasWidth / getAtlasHeight | Ottiene le dimensioni della bitmap mappata. |
| getGlyph | Ottiene le metriche di un singolo carattere. |
| measureText | Misura la larghezza in pixel di una stringa. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuiuicanvascpp"></a>
## 📄 src/core/ui/UICanvas.cpp
### Titolo: UICanvas.cpp - Gestione del Canvas dell'Interfaccia Utente

### Overview:
`UICanvas.cpp` gestisce la creazione e il caricamento di un canvas dell'interfaccia utente. Supporta l'associazione di variabili a elementi dell'UI, come numeri e stringhe, e il parsing di file JSON per definire gli elementi UI.

### Tabella Key Points:
| Nome | Scopo |
|------|-------|
| UICanvas::bindFloat | Associa una funzione o un puntatore a float a un elemento del canvas. |
| UICanvas::bindString | Associa una funzione o un puntatore a stringa a un elemento del canvas. |
| UICanvas::parseType | Converte una stringa di tipo in un enumeratore `UIElementType`. |
| UICanvas::loadUI | Carica gli elementi UI da un file JSON e li aggiunge al canvas. |
| UICanvas::parseColor | Converte una stringa di colore in un array di float rappresentante RGBA. |

Queste funzioni e metodi sono cruciali per la personalizzazione e l'interazione dinamica dell'interfaccia utente.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuiuicanvash"></a>
## 📄 src/core/ui/UICanvas.h
### Titolo
UICanvas Header File Analysis

### Overview
The `UICanvas` header file defines a class for managing and interacting with UI elements. It allows binding functions to update float and string values based on user input or external changes, and provides methods to load UI configurations from JSON files.

### Tabella Key Points
| **Class/Funzione/Config** | **Scopo** |
|---------------------------|-------------|
| `UICanvas`                | Manages the canvas for UI elements, allowing binding of float and string values. |
| `bindFloat`               | Binds a function to update a float value based on user input or external changes. |
| `bindString`              | Binds a function to update a string value based on user input or external changes. |
| `getValue`                | Retrieves the current numeric value of an element identified by its ID. |
| `getString`               | Retrieves the current string value of an element identified by its ID. |
| `loadUI`                  | Loads UI configurations from a JSON file into the canvas. |
| `parseType`               | Parses a string representing a UI element type and returns the corresponding enum value. |
| `parseColor`              | Parses a hexadecimal color string and stores the RGBA values in an array. |
| `resolveBindings`         | Resolves the bindings for numeric and string values based on external changes. |

These key points provide a concise overview of the functionalities offered by the `UICanvas` class, including its role in managing UI elements and handling dynamic updates through function bindings.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuiuielementh"></a>
## 📄 src/core/ui/UIElement.h
### Titolo Breve
**UIElement.h: Interfaccia dell'Elemento Grafico**

### Overview
Il file `UIElement.h` definisce le strutture e enumerazioni necessarie per rappresentare ed gestire gli elementi dell'interfaccia utente in un'applicazione. Include un set di costanti per identificare i tipi di elementi, una struttura per la geometria degli elementi e una classe principale che astrae le proprietà comuni a tutti gli elementi UI.

### Tabella Key Points
| Classe/Funzione | Scopo |
|-----------------|-------|
| `UIElementType`  | Enumerazione che identifica i tipi di elementi UI come label, progress bar, panel e icon. |
| `UIGeometryMode` | Enumerazione che definisce la modalità di geometria degli elementi UI, ognuna delle quali ha un significato diverso. |
| `UIGeometry`     | Struttura che rappresenta la geometria di un elemento UI, utilizzando uno dei modi definiti da `UIGeometryMode`. |
| `UIElement`      | Classe principale che descrive le proprietà comuni a tutti gli elementi UI. Incluisce l'ID, il tipo, le coordinate, la modalità di geometria e altre proprietà specifiche del tipo di elemento (come testo per i label). |

### Codice
```cpp
#pragma once
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"

enum class UIElementType : uint8_t
{
	Label,
	ProgressBar,
	Panel,
	Icon
};

enum class UIGeometryMode : uint8_t
{
	Simple,  // position + size + rotation
	Free     // 4 punti espliciti
};

struct UIGeometry
{
    UIGeometryMode mode;
    union
    {
        struct { Vector2 position; Vector2 size; float rotation; } simple;
        struct { Vector2 points[4]; } free;
    };
};

struct UIElement
{
    std::string   id;
    UIElementType type;
    float         color[4];
    std::string   valueKey;
    UIGeometry    geometry;

    // Utilizzato da elementi Label.
    // Per testo dinamico, lascia vuoto e usa valueKey + bindString().
    std::string   text;
    float         fontSize = 0.0f;  // 0 = usa la dimensione predefinita dell'atlante
};
```

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuiuiquadbatchcpp"></a>
## 📄 src/core/ui/UIQuadBatch.cpp
### Title
UIQuadBatch Implementation

### Overview
`UIQuadBatch` is a class responsible for managing and batching UI elements for rendering. It handles different types of geometric shapes, including free-form quads and simple rectangles with rotations, and stores their vertices and indices for efficient drawing.

### Key Points
| **Class/Funzione/Config** | **Scopo** |
|-------------------------|-----------|
| `UIQuadBatch`             | Manages the batching of UI elements for rendering. |
| `push(const UIElement& element)` | Adds a new UI element to the batch based on its geometry type and properties. |
| `pushLabel(const UIElement& element, const FontAtlas& atlas, const std::string& resolvedText)` | Handles the batching of text labels by breaking them into individual glyph quads. |

This table summarizes the essential components of the `UIQuadBatch` class, focusing on its primary functionalities and how it processes different types of UI elements for rendering efficiency.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srccoreuiuiquadbatchh"></a>
## 📄 src/core/ui/UIQuadBatch.h
### Title: UIQuadBatch Class

### Overview:
The `UIQuadBatch` class is designed to manage and batch rendering of UI elements. It facilitates the creation of vertex data for quadrilaterals (quads) used in UI rendering, including labels with text.

### Table Key Points
| **Class/Function** | **Purpose** |
|--------------------|-------------|
| `UIQuadBatch`      | Manages batches of UI quad vertices and indices for efficient rendering. |
| `push(const UIElement& element)` | Adds a single UI element to the batch. |
| `pushLabel(const UIElement& element, const FontAtlas& atlas, const std::string& resolvedText)` | Adds a label UI element with text using a font atlas. |
| `buildFromCanvas(const UICanvas& canvas, const FontAtlas& atlas)` | Builds the quad batch from a UI canvas, handling all elements and labels. |
| `clear()`          | Clears all vertices and indices in the batch. |
| `getVertices() const` | Retrieves the list of vertex data. |
| `getIndices() const`  | Retrieves the list of index data for rendering. |

These components work together to efficiently manage and render batches of UI elements, optimizing performance by reducing draw calls.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglconfigh"></a>
## 📄 src/opengl/OpenGLConfig.h
### Titolo Breve: Configurazione OpenGL

### Overview
Il file `OpenGLConfig.h` include le configurazioni necessarie per inizializzare e gestire la libreria OpenGL tramite GLFW e GLAD. 

### Tabella Key Points
| Nome       | Scopo                                                                 |
|------------|-----------------------------------------------------------------------|
| glad       | Libreria per caricare i puntatori alle funzioni di OpenGL dinamicamente. |
| glfw3.h    | Biblioteca per la creazione di finestre grafiche e input.               |

Questo file header è fondamentale per l'ambientazione di rendering 3D in applicazioni che utilizzano OpenGL, fornendo le intestazioni necessarie per interagire con GLFW e GLAD.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglfontatlascpp"></a>
## 📄 src/opengl/OpenGLFontAtlas.cpp
### Titolo Breve:
OpenGL Font Atlas Implementation

### Overview:
The `OpenGLFontAtlas` class is responsible for managing and uploading a font atlas to the GPU. It takes a CPU-based font atlas, validates it, and uploads the pixel data to a texture that can be used for rendering text in OpenGL applications.

### Tabella Key Points:

| Classe/Funzione/Config | Scopo |
|------------------------|-------|
| `OpenGLFontAtlas`      | Manages the creation and upload of a GPU-based font atlas from a CPU-based atlas. |
| `OpenGLFontAtlas::OpenGLFontAtlas(const FontAtlas& atlas)` | Constructor that initializes the GPU texture based on the provided CPU-based atlas. |
| `OpenGLFontAtlas::~OpenGLFontAtlas()` | Destructor that cleans up the GPU resources associated with the font atlas. |
| `OpenGLFontAtlas::bind(int unit) const` | Binds the font atlas texture to a specific texture unit for rendering. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglfontatlash"></a>
## 📄 src/opengl/OpenGLFontAtlas.h
### Titolo Breve: OpenGLFontAtlas

### Overview
`OpenGLFontAtlas` è una classe che gestisce la creazione e la gestione di un atlas di font sottostante la GPU, utilizzando il sistema di rendering OpenGL. Questa classe carica l'immagine RGBA del `FontAtlas` come un'immagine di texture OpenGL per offrire un rendering più sfumato tramite il filtraggio lineare.

### Tabella Key Points
| Classe/Funzione/Config | Scopo |
|------------------------|-------|
| `OpenGLFontAtlas`      | Gestisce l'upload del `FontAtlas` RGBA come una textura OpenGL. |
| `bind(int unit)`       | Associa la texture alla unità di binding specificata per essere utilizzata nel shader dell'interfaccia utente. |
| `getCPUAtlas()`        | Restituisce un riferimento costante al `FontAtlas` gestito da questa classe. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglmeshcpp"></a>
## 📄 src/opengl/OpenGLMesh.cpp
### Titolo: OpenGLMesh - Gestione di Mesh per Grafica 3D

### Overview:
Il file `OpenGLMesh.cpp` gestisce la creazione e il rendering delle mesh in OpenGl. Utilizza Vertex Arrays Object (VAO) e Buffer Objects (VBO e EBO) per memorizzare i vertici e gli indici, consentendo un rendering efficiente.

### Tabella Key Points
| Nome          | Scopo                                                                                       |
|---------------|---------------------------------------------------------------------------------------------|
| `OpenGLMesh`  | Classe che gestisce la creazione di mesh.                                                     |
| `draw()`      | Metodo per disegnare la mesh utilizzando gli indici degli elementi (triangoli).                 |
| `drawInstanced()` | Metodo per disegnare la mesh in modo instanziale, utile per rendere molte copie della stessa mesh. |
| Dtor          | Gestisce la deallocazione delle risorse OpenGL utilizzate dalla classe.                      |

### Classe/Funzioni/Config
- **Classi**: 
  - `OpenGLMesh`
  
- **Funzioni**:
  - `OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)`: Costruttore che inizializza la mesh con i vertici e gli indici forniti.
  - `draw() const`: Metodo per disegnare la mesh.
  - `drawInstanced(uint32_t instanceCount) const`: Metodo per disegnare la mesh instanziale.
  - Dtor: Distruttore che libera le risorse OpenGL.

- **Config**:
  - Utilizza VAO, VBO e EBO per la gestione dei vertici e degli indici.
  - Il rendering utilizza GL_TRIANGLES per disegnare i triangoli.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglmeshh"></a>
## 📄 src/opengl/OpenGLMesh.h
### Titolo: OpenGLMesh Class for Efficient Mesh Management

### Overview:
The `OpenGLMesh` class is designed to handle the creation, management, and rendering of 3D meshes in an OpenGL application. It encapsulates vertex data, indices, and axis-aligned bounding box (AABB) for efficient collision detection and rendering.

### Tabella Key Points:

| Elemento | Scopo |
|----------|-------|
| `OpenGLMesh` Class | Manages the creation and rendering of 3D meshes using OpenGL. |
| `draw()` Method | Renders the mesh using vertex array objects (VAO), vertex buffer objects (VBO), and element buffer objects (EBO). |
| `drawInstanced(uint32_t instanceCount)` Method | Renders multiple instances of the mesh efficiently by instancing. |
| `AABB` Member Variable | Stores the axis-aligned bounding box for collision detection and optimization. |

### File Content:
```cpp
#include "core/math/algebricOp.h"
#include "opengl/OpenGLConfig.h"
#include "core/bvh/aabb.h"

class OpenGLMesh
{
public:
    OpenGLMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        AABB aabb);
    ~OpenGLMesh();

    void draw() const;
	void drawInstanced(uint32_t instanceCount) const;
    AABB aabb;

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
};
```

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglrenderercpp"></a>
## 📄 src/opengl/OpenGLRenderer.cpp
### Titolo: OpenGLRenderer Implementation

### Overview:
`OpenGLRenderer.cpp` defines the implementation of a class responsible for rendering scenes using OpenGL. The class initializes and manages shaders, uniform buffers, and handles scene rendering by grouping objects based on their meshes.

### Tabella Key Points:
| Nomi | Scopo |
|------|-------|
| `OpenGLRenderer` | Classe principale per il rendering di scene OpenGL. |
| `init()` | Inizializza lo stato OpenGL e crea buffer uniforme (UBOs). |
| `shutdown()` | Dealloca risorse OpenGL allocate durante l'inizializzazione. |
| `onResize(unsigned int width, unsigned int height)` | Aggiorna le dimensioni del viewport quando la finestra viene ridimensionata. |
| `render(const Scene& scene, const Camera& camera)` | Esegue il rendering della scena specificata utilizzando la vista fornita. |
| `m_shader` | Gestisce i programmi shader per l'omogeneizzazione dei dati uniformi e l'applicazione del materiale. |
| `m_cameraUBO` / `m_transformUBO` | Buffer uniforme per memorizzare le informazioni della camera e delle trasformazioni degli oggetti. |
| `groups` | Struttura di dati per raggruppare gli oggetti della scena per ottimizzare i draw calls. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglrendererh"></a>
## 📄 src/opengl/OpenGLRenderer.h
### Titolo
OpenGLRenderer: Gestione del Rendering Grafico

### Overview
Il file `OpenGLRenderer.h` definisce la classe `OpenGLRenderer`, un componente chiave per gestire il rendering grafico utilizzando OpenGL. La classe è responsabile della compilazione e dell'esecuzione dei shader, dell'inizializzazione e del rendering delle scene 3D.

### Tabella Key Points
| Nome                     | Scopo                                                                                           |
|--------------------------|---------------------------------------------------------------------------------------------------|
| `CameraUBOData`          | Struttura di dati per il buffer uniforme della camera, contenente la matrice di proiezione, vista e posizione della camera. |
| `OpenGLRenderer`         | Classe principale che gestisce l'intero processo di rendering, inclusa la compilazione dei shader, l'inizializzazione del renderer e il rendering delle scene. |
| `init()`                 | Inizializza il renderer, compreso il caricamento dei shader.                                       |
| `shutdown()`             | Termina correttamente il renderer, liberando tutte le risorse allocate.                              |
| `onResize(unsigned int width, unsigned int height)` | Gestisce la ridimensione della finestra di rendering aggiornando le dimensioni del viewport e ricompilando il shader se necessario. |
| `render(const Scene& scene, const Camera& camera)` | Esegue il rendering della scena specificata utilizzando la configurazione della camera fornita. |

Questi elementi sono fondamentali per l'operazione efficace del rendering grafico utilizzando OpenGL.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglshaderprogramcpp"></a>
## 📄 src/opengl/OpenGLShaderProgram.cpp
### Title: OpenGL Shader Program Management

### Overview:
The `OpenGLShaderProgram` class is a crucial component in managing and compiling shader programs for use in OpenGL rendering. It handles the creation, linking, binding, and unbinding of shaders, as well as setting uniform values.

### Key Points:

| **Class/Function/Config** | **Scope** |
|--------------------------|-----------|
| `OpenGLShaderProgram`    | Manages the creation and lifecycle of an OpenGL shader program. |
| `compileModule`          | Compiles a shader module from a given file path. |
| `bind`, `unbind`         | Binds and unbinds the shader program for rendering operations. |
| `setUniformBlockBinding` | Sets the binding point for uniform blocks in the shader. |
| `setInt`, `setFloat`, etc.  | Set various types of uniform values (int, float, matrix, vector). |
| `getUniformLocation`     | Retrieves the location of a uniform variable in the shader program. |

### Table Key Points:
1. **OpenGLShaderProgram**: Manages the creation and lifecycle of an OpenGL shader program.
2. **compileModule**: Compiles a shader module from a given file path.
3. **bind, unbind**: Binds and unbinds the shader program for rendering operations.
4. **setUniformBlockBinding**: Sets the binding point for uniform blocks in the shader.
5. **setInt, setFloat, etc.**: Set various types of uniform values (int, float, matrix, vector).
6. **getUniformLocation**: Retrieves the location of a uniform variable in the shader program.

### Additional Notes:
- The class handles both vertex and fragment shaders.
- Error handling is included for shader compilation and linking processes.
- Uniforms are cached to improve performance by reducing redundant queries to OpenGL.

This overview and key points provide a comprehensive understanding of the functionality and structure of the `OpenGLShaderProgram` class.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopenglshaderprogramh"></a>
## 📄 src/opengl/OpenGLShaderProgram.h
### Titolo Breve:
`OpenGLShaderProgram.h`

### Overview:
Il file `OpenGLShaderProgram.h` definisce la classe `OpenGLShaderProgram`, utilizzata per gestire e compilare programmi di shader OpenGL. Questa classe fornisce metodi per leggere, compilare e collegare i shader vertex e fragment, insieme ad un'interfaccia per impostare le variabili uniforme nei programmi di shader.

### Tabella Key Points:

| **Classe/Funzione/Config** | **Scopo** |
| --- | --- |
| `OpenGLShaderProgram` | Gestisce il caricamento, compilazione e collegamento dei programmi di shader OpenGL. |
| `OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath)` | Costruttore che carica i file di shader vertex e fragment. |
| `OpenGLShaderProgram::~OpenGLShaderProgram()` | Distruttore che libera risorse. |
| `OpenGLShaderProgram::bind() const` | Associa il programma di shader all'hardware OpenGL attivo. |
| `OpenGLShaderProgram::unbind() const` | Scollega il programma di shader dall'hardware OpenGL attivo. |
| `OpenGLShaderProgram::setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const` | Imposta la mappatura del blocco uniforme per il programma di shader. |
| `OpenGLShaderProgram::getInt(const std::string& name, int value) const` | Imposta una variabile uniforme di tipo intero. |
| `OpenGLShaderProgram::setFloat(const std::string& name, float value) const` | Imposta una variabile uniforme di tipo float. |
| `OpenGLShaderProgram::setMat4(const std::string& name, const mat4& matrix) const` | Imposta una matrice 4x4 come variabile uniforme. |
| `OpenGLShaderProgram::setVec2(const std::string& name, const Vector2& v) const` | Imposta un vettore 2D come variabile uniforme. |
| `OpenGLShaderProgram::setVec3(const std::string& name, const Vector3& v) const` | Imposta un vettore 3D come variabile uniforme. |
| `OpenGLShaderProgram::setUInt(const std::string& name, unsigned int value) const` | Imposta una variabile uniforme di tipo intero senza segno. |
| `OpenGLShaderProgram::getProgramID() const` | Restituisce l'ID del programma di shader. |
| `OpenGLShaderProgram::getUniformLocation(const std::string& name) const` | Ottiene l'ubicazione dell'uniforme nel programma di shader. |
| `OpenGLShaderProgram::compileModule(const std::string& filepath, GLenum type)` | Compila un modulo di shader da file. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopengltexturecpp"></a>
## 📄 src/opengl/OpenGLTexture.cpp
### Titolo:
`OpenGLTexture.cpp: Gestione e Utilizzo di Textures in OpenGL`

### Overview:
Il file `OpenGLTexture.cpp` implementa la gestione delle textures per grafica 3D utilizzando OpenGL. Include la creazione, caricamento, e uso di texture nei programmi OpenGL.

### Tabella Key Points:
| Classe/Funzione/Config | Scopo |
|------------------------|-------|
| `OpenGLTexture(const char* filename)` | Carica e gestisce le texture da file immagine. In caso di fallimento del caricamento, crea una texture fallback magenta di 1x1 pixel. |
| `~OpenGLTexture()`      | Distrugge la texture OpenGL associata all'oggetto `OpenGLTexture`. |
| `use(int unit)`          | Attiva la texture e la associa al unità specificata in OpenGL. |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopengltextureh"></a>
## 📄 src/opengl/OpenGLTexture.h
Titolo: OpenGLTexture Header File

Overview:
Il file `OpenGLTexture.h` definisce la classe `OpenGLTexture`, utilizzata per gestire le texture in un contesto OpenGL. La classe permette di caricare e utilizzare texture da file, offrendo funzionalità per applicarle alle superfici grafiche.

Tabella Key Points:
- **Classe:**
  - **Nome:** OpenGLTexture
  - **Scopo:** Gestisce il caricamento e l'uso delle texture in OpenGL.

- **Funzioni/Metodi:**
  - **Nome:** OpenGLTexture(const char* filename)
  - **Scopo:** Costruttore della classe, carica la texture da un file specificato.
  
  - **Nome:** ~OpenGLTexture()
  - **Scopo:** Distruttore della classe, libera le risorse associate alla texture.

  - **Nome:** use(int unit)
  - **Scopo:** Attiva la texture in uno specifico unità del contesto OpenGL.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopengluirenderercpp"></a>
## 📄 src/opengl/OpenGLUIRenderer.cpp
### Titolo:
Rendere UI con OpenGL

### Overview:
Il file `OpenGLUIRenderer.cpp` gestisce il rendering di interfacce utente (UI) utilizzando OpenGL. Include la creazione dei buffer, l'inizializzazione della pipeline di rendering e il rendering degli elementi UI come quad solido e testo.

### Tabella Key Points:

| Classe/Funzione/Config | Scopo |
|------------------------|-------|
| `OpenGLUIRenderer`     | Classe principale per il rendering del/UI. |
| `init()`               | Inizializza i buffer OpenGL per il rendering di quad UI. |
| `loadFont()`           | Carica una font atlas e la memorizza nella GPU. |
| `render(const UICanvas& canvas)` | Renderizza tutti gli elementi della UI in due passaggi: uno per gli elementi solidi e uno per i testi. |

Queste classi e funzioni gestiscono il rendering degli elementi UI utilizzando OpenGL, inclusa la creazione dei buffer, l'inizializzazione della pipeline di rendering e il rendering degli elementi come quad solido e testo.

[⬆ Torna all'indice](#-indice-di-navigazione)

---
<a id="srcopenglopengluirendererh"></a>
## 📄 src/opengl/OpenGLUIRenderer.h
### Titolo:
OpenGLUIRenderer - Renderer di Interfaccia Utente per OpenGL

### Overview:
Il file `src/opengl/OpenGLUIRenderer.h` definisce la classe `OpenGLUIRenderer`, un componente responsabile del rendering delle interfacce utente in una applicazione basata su OpenGL. La classe utilizza risorse gestite da `ResourceManager`, shader personalizzati, e font atlas per rendere i grafici dell'interfaccia utente.

### Tabella Key Points:
| Nome | Scopo |
| --- | --- |
| **Classi** | - `OpenGLUIRenderer`<br>- `OpenGLShaderProgram`<br>- `FontAtlas`<br>- `OpenGLFontAtlas` |
| **Funzioni** | - `OpenGLUIRenderer(ResourceManager& m_resources, unsigned int width, unsigned int height, const std::string& vertPath, const std::string& fragPath)`<br>- `~OpenGLUIRenderer()`<br>- `init()`<br>- `bool loadFont(const std::string& filepath, float pixelHeight)`<br>- `void render(const UICanvas& canvas)`<br>- `void onResize(unsigned int width, unsigned int height)`<br>- `void shutdown()` |
| **Config** | - `MAX_UI_VERTICES` (4096)<br>- `MAX_UI_INDICES` (6144) |

[⬆ Torna all'indice](#-indice-di-navigazione)

---
