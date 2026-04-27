# Deferred Rendering – G-Buffer Design (Engine Specification)

## 📌 Overview

The renderer uses a **deferred shading pipeline** based on:

- Depth reconstruction (no world position buffer)
- View-space lighting pipeline
- Octahedral normal encoding
- Separate albedo buffer
- Packed material parameter buffer

---

## 🧠 Coordinate Space Strategy

### Geometry Pass
- All vertices are transformed into **view space**
- The fragment shader outputs only data required for the lighting pass

### Lighting Pass
- All lighting computations are performed in **view space**
- Camera matrices are used for required transformations
- Position is reconstructed from depth (not stored directly)

---

## 📦 G-Buffer Layout

### Buffer 0 – Normal (Encoded)
- Format: RG16F / RGBA16F (implementation dependent)
- Encoding: **Octahedral Encoding**
- Space: View Space

#### Contents:
- Encoded normal stored in 2 channels (RG)

---

### Buffer 1 – Albedo
- Format: RGBA8
- Source: Texture sampling (primary)
- Optional: Vertex color blending (future extension)

#### Contents:
- RGB = Base Color
- A = Optional mask / transparency (future use)

---

### Buffer 2 – Material Parameters (Packed)
- Format: RGBA8

#### Channel layout:
- R → Roughness (0–1)
- G → Metallic (0–1)
- B → Ambient Occlusion (0–1)
- A → Emissive Mask (0–1, reserved for future use)

---

### Depth Buffer (Separate)
- Format: D24 or D32F
- Used for:
  - Depth reconstruction
  - View-space position reconstruction

---

## 📐 Depth Reconstruction

### Concept

Each pixel position is reconstructed using:
- Depth buffer value
- Inverse projection matrix
- Inverse view matrix (if world-space conversion is needed)

### Pipeline:

1. Read depth value from depth buffer
2. Convert from screen space to NDC
3. Reconstruct view-space position
4. Optionally transform to world space using inverse view matrix

---

## 🧮 Octahedral Normal Encoding

### Encoding:
- Input: normalized 3D normal vector (x, y, z)
- Output: 2D vector stored in RG channels

### Decoding:
- Performed in the lighting pass to reconstruct full 3D normal

### Benefits:
- Reduces bandwidth (3 floats → 2 floats)
- Standard in modern deferred rendering pipelines
- Efficient GPU memory usage

---

## 🎨 Material System Integration

### Albedo Handling:
- Primary source: texture sampling
- Optional extension: vertex color blending

### Material Parameters:
- Always fetched from packed material buffer
- Used directly in PBR lighting calculations

---

## 💡 Lighting Space Definition

- Lighting is computed entirely in **view space**
- Requires:
  - Reconstructed view-space position
  - Decoded view-space normals
  - Camera-space light transformations

---

## ⚙️ Design Philosophy

- Minimize G-buffer bandwidth usage
- Maximize flexibility in lighting pass
- Separate surface appearance from geometric reconstruction
- Maintain scalability for future rendering features

---

## 🚀 Future Extensions (Non-Breaking)

- Emissive lighting (already reserved in material buffer)
- Motion vectors (TAA / motion blur support)
- Additional material layers (clear coat, SSS, etc.)
- Clustered / tiled lighting system
- SSAO / SSR integration