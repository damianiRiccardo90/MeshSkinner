// Standard library imports
#include <iostream>
#include <vector>

// Local application imports
#include "test/test_framework.h"
#include "test/test_utils.h"


// Forward declarations for test suite creators
TestSuite create_mesh_tests();
TestSuite create_skeleton_tests();
TestSuite create_skinning_data_tests();
TestSuite create_skinner_tests();

int main() 
{
    // Print a colorful header for the test run
    TestUtils::set_console_color(TestUtils::ConsoleColor::Cyan);
    std::cout << "\n===================================\n";
    std::cout << "  MESH SKINNER TEST SUITE RUNNER  ";
    std::cout << "\n===================================\n\n";
    TestUtils::reset_console_color();
    
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
    
    // Print overall test summary with color
    TestUtils::set_console_color(TestUtils::ConsoleColor::Cyan);
    std::cout << "\n===== Test Summary =====\n";
    TestUtils::reset_console_color();
    
    // Test suites summary
    if (failed_suites == 0) 
    {
        TestUtils::set_console_color(TestUtils::ConsoleColor::Green);
        std::cout << test_suites.size() << "/" << test_suites.size() 
                  << " test suites passed.\n";
        TestUtils::reset_console_color();
        
        TestUtils::print_colored("\n✓ ALL TEST SUITES PASSED ✓\n", 
            TestUtils::ConsoleColor::Green);
    } 
    else 
    {
        TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
        std::cout << (test_suites.size() - failed_suites) << "/" << test_suites.size() 
                  << " test suites passed.\n";
        TestUtils::reset_console_color();
        
        TestUtils::print_colored("\n✗ SOME TEST SUITES FAILED ✗\n", 
            TestUtils::ConsoleColor::Red);
    }

    // Wait for input so the console doesn't close immediately
    TestUtils::set_console_color(TestUtils::ConsoleColor::Default);
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return failed_suites == 0 ? 0 : 1;
}
