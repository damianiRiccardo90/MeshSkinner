#pragma once

// Standard library imports
#include <string>
#include <vector>


class Mesh;

/**
 * @brief A Facade class that simplifies interactions with OBJ file operations.
 *
 * This class encapsulates the complexity of working with tinyobjloader, providing
 * a clean interface for loading and saving 3D mesh data in OBJ format. It handles
 * the parsing of vertex data, indices, and proper error handling during file operations.
 */
class ObjFacade 
{
public:

    /**
     * @brief Loads an OBJ mesh from a file path.
     * @param filePath The path to the .obj file to load.
     * @return A Mesh object containing the parsed geometry data.
     * @throws std::runtime_error if the file cannot be loaded or contains invalid data.
     *
     * This method abstracts all the complexity of working with tinyobjloader,
     * converting the library's internal representation into our own Mesh structure.
     * It handles vertex positions and triangle indices, with appropriate warnings
     * displayed for non-critical issues.
     */
    static Mesh load_obj_mesh(const std::string& filePath);

    /**
     * @brief Saves a Mesh object to an OBJ file.
     * @param filePath The path where the .obj file will be written.
     * @param mesh The Mesh object to serialize.
     * @return true if the file was saved successfully; otherwise false.
     *
     * Converts the internal Mesh representation to the OBJ file format,
     * writing vertex positions and face indices. OBJ indices are automatically
     * adjusted from 0-based (internal) to 1-based (OBJ standard) during export.
     */
    static bool save_obj_mesh(const std::string& filePath, const Mesh& mesh);
};
