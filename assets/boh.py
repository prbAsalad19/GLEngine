import numpy as np

def add_uv_to_obj(input_path, output_path):
    vertices = []
    faces = []

    # 1. Leggi il file originale
    with open(input_path, 'r') as f:
        for line in f:
            if line.startswith('v '):
                vertices.append([float(x) for x in line.split()[1:]])
            elif line.startswith('f '):
                faces.append(line.split()[1:])

    # 2. Calcola le coordinate UV (Proiezione Sferica)
    # Formula: u = 0.5 + arctan2(z, x) / 2pi | v = 0.5 - arcsin(y/r) / pi
    uvs = []
    for v in vertices:
        x, y, z = v
        r = np.sqrt(x**2 + y**2 + z**2)
        u = 0.5 + (np.arctan2(z, x) / (2 * np.pi))
        v_coord = 0.5 - (np.arcsin(y / r) / np.pi) if r != 0 else 0.5
        uvs.append((u, v_coord))

    # 3. Scrivi il nuovo file con le sezioni 'vt'
    with open(output_path, 'w') as f:
        f.write("# File generato con coordinate UV sferiche\n")
        
        # Scrivi i vertici
        for v in vertices:
            f.write(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}\n")
        
        # Scrivi le texture coordinates
        for uv in uvs:
            f.write(f"vt {uv[0]:.6f} {uv[1]:.6f}\n")
        
        # Scrivi le facce mappando i vertici alle rispettive UV
        # Nota: in questo caso semplice, l'indice vt corrisponde all'indice v
        for face in faces:
            new_face = []
            for index in face:
                idx = index.split('/')[0]
                new_face.append(f"{idx}/{idx}")
            f.write(f"f {' '.join(new_face)}\n")

add_uv_to_obj('assets/teapot.obj', 'assets/teapot_with_uv.obj')