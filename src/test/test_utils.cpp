#include "test_utils.h"

// Standard library imports
#include <cmath>
#include <iostream>

// Local application imports
#include "model/mesh.h"

// Platform-specific includes
#ifdef _WIN32
#include <windows.h>
#endif


namespace TestUtils {
bool approx_equal(float a, float b, float epsilon/*= .0001f*/) 
{
    return std::fabs(a - b) < epsilon;
}

bool approx_equal_vec3(const HMM_Vec3& a, const HMM_Vec3& b, float epsilon/*= .0001f*/) 
{
    return approx_equal(a.X, b.X, epsilon) && 
           approx_equal(a.Y, b.Y, epsilon) && 
           approx_equal(a.Z, b.Z, epsilon);
}

bool approx_equal_mat4(const HMM_Mat4& a, const HMM_Mat4& b, float epsilon/*= .0001f*/)
{
    for (size_t row = 0; row < 4; row++) 
    {
        for (size_t col = 0; col < 4; col++) 
        {
            if (!approx_equal(a.Elements[row][col], b.Elements[row][col], epsilon)) 
            {
                return false;
            }
        }
    }

    return true;
}

bool is_vertex_connected(const std::vector<Face>& faces, size_t vertex_index)
{
    for (const auto& face : faces)
    {
        for (const auto& index : face.indices)
        {
            if (index == vertex_index)
            {
                return true;
            }
        }
    }

    return false;
}

std::pair<HMM_Vec3, HMM_Vec3> calculate_mesh_bounds(const std::vector<HMM_Vec3>& vertices)
{
    if (vertices.empty())
    {
        return {HMM_V3(0.f, 0.f, 0.f), HMM_V3(0.f, 0.f, 0.f)};
    }
    
    HMM_Vec3 min_bounds = vertices[0];
    HMM_Vec3 max_bounds = vertices[0];
    
    for (const auto& vertex : vertices)
    {
        min_bounds.X = std::min(min_bounds.X, vertex.X);
        min_bounds.Y = std::min(min_bounds.Y, vertex.Y);
        min_bounds.Z = std::min(min_bounds.Z, vertex.Z);
        
        max_bounds.X = std::max(max_bounds.X, vertex.X);
        max_bounds.Y = std::max(max_bounds.Y, vertex.Y);
        max_bounds.Z = std::max(max_bounds.Z, vertex.Z);
    }
    
    return {min_bounds, max_bounds};
}

void set_console_color(ConsoleColor color)
{
#ifdef _WIN32
    // Windows implementation
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD colorAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // Default

    switch (color)
    {
        case ConsoleColor::Red:
            colorAttribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::Green:
            colorAttribute = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::Yellow:
            colorAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::Blue:
            colorAttribute = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::Magenta:
            colorAttribute = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::Cyan:
            colorAttribute = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::White:
            colorAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::Default:
        default:
            colorAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
    }

    SetConsoleTextAttribute(hConsole, colorAttribute);
#else
    // ANSI escape codes for Unix-like systems
    switch (color)
    {
        case ConsoleColor::Red:
            std::cout << "\033[1;31m";
            break;
        case ConsoleColor::Green:
            std::cout << "\033[1;32m";
            break;
        case ConsoleColor::Yellow:
            std::cout << "\033[1;33m";
            break;
        case ConsoleColor::Blue:
            std::cout << "\033[1;34m";
            break;
        case ConsoleColor::Magenta:
            std::cout << "\033[1;35m";
            break;
        case ConsoleColor::Cyan:
            std::cout << "\033[1;36m";
            break;
        case ConsoleColor::White:
            std::cout << "\033[1;37m";
            break;
        case ConsoleColor::Default:
        default:
            std::cout << "\033[0m";
            break;
    }
#endif
}

void reset_console_color()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    std::cout << "\033[0m";
#endif
}

void print_colored(const std::string& text, ConsoleColor color)
{
    set_console_color(color);
    std::cout << text;
    reset_console_color();
}

} // namespace TestUtils
