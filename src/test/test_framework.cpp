#include "test_framework.h"

// Standard library imports
#include <iomanip>
#include <iostream>


TestCase::TestCase(const std::string& n, std::function<bool()> fn) 
    : name(n)
    , test_function(fn) 
{}

TestSuite::TestSuite(const std::string& name) 
    : suite_name(name) 
{}

void TestSuite::add_test(const std::string& name, std::function<bool()> test_function) 
{
    tests.emplace_back(name, test_function);
}

bool TestSuite::run() 
{
    std::cout << "\n===== Running Test Suite: " << suite_name << " =====\n";

    int passed = 0;
    for (const auto& test : tests) 
    {
        std::cout << "Test: " << std::left << std::setw(40) << test.name;
        bool result = test.test_function();
        std::cout << "=====" 
                  << (result ? " [PASSED] " : " [FAILED] ") 
                  << "=====\n";
        if (result) passed++;
    }
    
    std::cout << "\nResults: " << passed << "/" << tests.size() 
              << " tests passed in suite '" << suite_name << std::endl;
    
    return passed == tests.size();
}
