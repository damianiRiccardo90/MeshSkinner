#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    // For testing purposes, run this block first.
    if (true)
    {
        std::cout << "Dio Merda\n";

        // Wait for user input so the window stays open.
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 0;
    }
    
    // Original functionality
    if (argc < 5) 
    {
        std::cerr << "Usage: " << argv[0] << " <input_mesh.obj> <bone_weight.json> "
                  << "<inverse_bind_pose.json> <new_pose.json>\n";
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: Cannot open " << argv[1] << "\n";
        return 1;
    }

    std::ofstream output_file(argv[2]);
    if (!output_file) {
        std::cerr << "Error: Cannot create " << argv[2] << "\n";
        return 1;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        output_file << "Processed: " << line << "\n";
    }

    std::cout << "File processed successfully!\n";
    
    // Wait for input so the console doesn't close immediately
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}
