// Standard library imports
#include <iostream>

// Local application imports
#include "mesh_skinner.h"


int main(int argc, char* argv[])
{
    // Display ultra-cool ASCII art banner
    std::cout << R"(                       
     __  __           _      ____  _    _                       
    |  \/  | ___  ___| |__  / ___|| | _(_)_ __  _ __   ___ _ __ 
    | |\/| |/ _ \/ __| '_ \ \___ \| |/ / | '_ \| '_ \ / _ \ '__|
    | |  | |  __/\__ \ | | | ___) |   <| | | | | | | |  __/ |   
    |_|  |_|\___||___/_| |_||____/|_|\_\_|_| |_|_| |_|\___|_|   
                                                                   
    )" << std::endl;

    // Ensure the num of input params is correct
    if (argc < 5) 
    {
        std::cerr << "Usage: " << argv[0] << " <input_mesh.obj> <bone_weight.json> "
                  << "<inverse_bind_pose.json> <output_pose.json>\n";
        return 1;
    }

    MeshSkinner skinner;

    // Load input data
    if (!skinner.load_mesh(argv[1])) return 1;
    if (!skinner.load_weights(argv[2])) return 1;
    if (!skinner.load_bind_pose(argv[3])) return 1;
    // Perform the skinning operation
    if (!skinner.perform_skinning()) return 1;
    // Save the result
    if (!skinner.save_skinned_mesh(argv[4])) return 1;
    
    // Wait for input so the console doesn't close immediately
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}
