// Standard library imports
#include <iostream>
#include <vector>

// Local application imports
#include "test/test_framework.h"


// Forward declarations for test suite creators
TestSuite create_mesh_tests();
TestSuite create_skeleton_tests();
TestSuite create_skinning_data_tests();
TestSuite create_skinner_tests();

int main() 
{
    std::vector<TestSuite> test_suites = {
        create_mesh_tests(),
        create_skeleton_tests(),
        create_skinning_data_tests(),
        create_skinner_tests()
    };
    
    int failed_suites = 0;
    for (auto& suite : test_suites) 
    {
        if (!suite.run()) 
        {
            failed_suites++;
        }
    }
    
    std::cout << "\n===== Test Summary =====\n";
    std::cout << (test_suites.size() - failed_suites) << "/" << test_suites.size() 
              << " test suites passed.\n";
              
    return failed_suites == 0 ? 0 : 1;
}
