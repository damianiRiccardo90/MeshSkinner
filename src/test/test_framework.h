#pragma once

// Standard library imports
#include <functional>
#include <string>
#include <vector>


/**
 * @brief Represents a single test case with a name and test function.
 *
 * This struct encapsulates a named test function that returns a boolean
 * indicating success or failure of the test.
 */
struct TestCase 
{
    /**
     * @brief Constructs a test case with a name and test function.
     * @param n The name of the test case.
     * @param fn The function to execute for this test case.
     */
    TestCase(const std::string& n, std::function<bool()> fn);

    std::string name;
    std::function<bool()> test_function;
};

/**
 * @brief A collection of related test cases that can be executed together.
 *
 * This class manages a group of test cases under a common name and provides
 * functionality to add tests and run the entire suite.
 */
class TestSuite 
{
public:

    /**
     * @brief Constructs a test suite with the specified name.
     * @param name The name of the test suite.
     */
    TestSuite(const std::string& name);
    
    /**
     * @brief Adds a new test case to the suite.
     * @param name The name of the test case.
     * @param test_function The function to execute for this test case.
     */
    void add_test(const std::string& name, std::function<bool()> test_function);
    
    /**
     * @brief Executes all test cases in the suite.
     * @return true if all tests pass; otherwise false.
     */
    bool run();

private:

    std::string suite_name;
    std::vector<TestCase> tests;
};
