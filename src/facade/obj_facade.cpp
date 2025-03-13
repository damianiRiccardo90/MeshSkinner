#include "obj_facade.h"

// Standard library imports
#include <fstream>
#include <iostream>
#include <stdexcept>

// Third-party imports
#include "tiny_obj_loader/tiny_obj_loader.h"

// Local application imports
#include "model/Mesh.h"


Mesh ObjFacade::load_obj_mesh(const std::string& filePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    // Attempt to load the OBJ file
    bool ret = tinyobj::LoadObj(
        &attrib, &shapes, &materials, &warn, &err,
        filePath.c_str(), 
        nullptr, // use default material file resolver
        true     // triangulate
    );

    if (!warn.empty()) 
    {
        std::cerr << "[TinyOBJLoader Warning] " << warn << std::endl;
    }
    if (!err.empty()) 
    {
        std::cerr << "[TinyOBJLoader ERROR] " << err << std::endl;
    }
    if (!ret) 
    {
        throw std::runtime_error("Failed to load OBJ from " + filePath + ": " + err);
    }

    // Convert the loaded data into our Mesh structure
    Mesh mesh;
    
    // Process vertex positions
    // TinyOBJ stores vertices as a flat array [x1, y1, z1, x2, y2, z2, ...]
    mesh.vertices.reserve(attrib.vertices.size() / 3);
    for (size_t v = 0; v < attrib.vertices.size(); v += 3) 
    {
        Vertex vert;
        vert.x = attrib.vertices[v + 0];
        vert.y = attrib.vertices[v + 1];
        vert.z = attrib.vertices[v + 2];
        mesh.vertices.push_back(vert);
    }
    
    // Process indices from all shapes
    // Collect unique vertices and build index array
    for (const auto& shape : shapes) 
    {
        // For each face
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
        {
            int fv = shape.mesh.num_face_vertices[f];
            
            // For each vertex in the face (should be 3 after triangulation)
            for (size_t v = 0; v < fv; v++) 
            {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                mesh.indices.push_back(static_cast<unsigned int>(idx.vertex_index));
            }
            index_offset += fv;
        }
    }
    
    return mesh;
}

bool ObjFacade::save_obj_mesh(const std::string& filePath, const Mesh& mesh)
{
    std::ofstream ofs(filePath);
    if (!ofs) 
    {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    // Write OBJ file header with some metadata
    ofs << "# OBJ file created by MeshSkinner\n";
    ofs << "# Vertices: " << mesh.vertices.size() << "\n";
    ofs << "# Faces: " << mesh.indices.size() / 3 << "\n\n";

    // Write vertex positions
    for (const auto& vert : mesh.vertices) 
    {
        ofs << "v " 
            << vert.x << " "
            << vert.y << " "
            << vert.z << "\n";
    }

    // Write face indices
    // OBJ indices are 1-based, so we add 1 to each index
    // Each face is a triangle (3 indices)
    for (size_t i = 0; i < mesh.indices.size(); i += 3) 
    {
        ofs << "f "
            << (mesh.indices[i]     + 1) << " "
            << (mesh.indices[i + 1] + 1) << " "
            << (mesh.indices[i + 2] + 1) << "\n";
    }

    ofs.close();
    return true;
}
