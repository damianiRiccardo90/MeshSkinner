#pragma once

// Standard library imports
#include <functional>
#include <string>
#include <vector>


struct TestCase 
{
    TestCase(const std::string& n, std::function<bool()> fn);

    std::string name;
    std::function<bool()> test_function;
};

class TestSuite 
{
public:

    TestSuite(const std::string& name);
    
    void add_test(const std::string& name, std::function<bool()> test_function);
    
    bool run();

private:

    std::string suite_name;
    std::vector<TestCase> tests;
};
