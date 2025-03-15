// Standard library imports
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"

// Local application imports
#include "facade/json_facade.h"
#include "facade/math_facade.h"
#include "model/skeleton.h"
#include "test/test_framework.h"
#include "test/test_utils.h"


TestSuite create_skeleton_tests() 
{
    TestSuite suite("Skeleton");
    
    // Basic file loading tests
    suite.add_test("Load Valid Bind Pose", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/inverse_bind_pose.json");
            const Skeleton skeleton = Skeleton::from_json(json_data);

            TestUtils::print_colored("Loading bind pose: Success\n", 
                TestUtils::ConsoleColor::Green);

            std::cout << "Loaded skeleton with " << skeleton.joints.size() 
                      << " joints\n";

            return true;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading bind pose failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    suite.add_test("Load Valid New Pose", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/output_pose.json");
            const Skeleton skeleton = Skeleton::from_json(json_data);

            TestUtils::print_colored("Loading output pose: Success\n", 
                TestUtils::ConsoleColor::Green);

            std::cout << "Loaded skeleton with " << skeleton.joints.size() 
                      << " joints\n";

            return true;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading output pose failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    suite.add_test("Handle Invalid Pose File", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/nonexistent_pose.json");
            const Skeleton skeleton = Skeleton::from_json(json_data);

            TestUtils::print_colored("Failed to reject nonexistent pose file\n", 
                TestUtils::ConsoleColor::Red);
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected nonexistent pose file\n", 
                TestUtils::ConsoleColor::Green);
            return true;
        }
    });
    
    // Test with malformed JSON file
    suite.add_test("Handle Malformed JSON File", []() 
    {
        // Create temporary malformed JSON file
        const std::string tempFilePath = "asset/temp_malformed.json";
        std::ofstream tempFile(tempFilePath);
        tempFile << "{ this is not valid JSON }";

        try 
        {
            const Json json_data = JsonFacade::load_from_file(tempFilePath);
            const Skeleton skeleton = Skeleton::from_json(json_data);

            TestUtils::print_colored("Failed to reject malformed JSON file\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected malformed JSON file\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return true;
        }
    });
    
    // Bone transformation tests
    suite.add_test("Bone Transformation Matrix Calculation", []() 
    {
        TestUtils::print_colored("Testing bone transformation matrices...\n", 
            TestUtils::ConsoleColor::Default);
        
        // Create a simple test skeleton with two joints
        Skeleton skeleton;
        
        // Create root joint
        Joint root_joint;
        root_joint.parent_id = -1; // No parent
        root_joint.local_transform = MathFacade::create_identity();
        
        // Create child joint
        Joint child_joint;
        child_joint.parent_id = 0; // Parent is root
        child_joint.local_transform = MathFacade::translate(1.f, 0.f, 0.f);
        
        // Add joints to skeleton
        skeleton.joints.push_back(root_joint);
        skeleton.joints.push_back(child_joint);
        
        // Calculate global transforms
        std::vector<HMM_Mat4> global_transforms;
        skeleton.calculate_global_transforms(global_transforms);
        
        // Verify correct number of transforms
        if (global_transforms.size() != 2) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Incorrect number of global transforms: " 
                      << global_transforms.size() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
        
        // Root should still be identity
        const bool root_correct = TestUtils::approx_equal_mat4(
            global_transforms[0], 
            MathFacade::create_identity()
        );
        
        // Child should have parent's transform * local transform
        const bool child_correct = TestUtils::approx_equal_mat4(
            global_transforms[1],
            MathFacade::translate(1.f, 0.f, 0.f)
        );
        
        TestUtils::set_console_color(root_correct ? 
            TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Root bone transform correct: " 
                  << (root_correct ? "Yes" : "No") << std::endl;
        TestUtils::reset_console_color();
        
        TestUtils::set_console_color(child_correct ? 
            TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Child bone transform correct: " 
                  << (child_correct ? "Yes" : "No") << std::endl;
        TestUtils::reset_console_color();
        
        if (root_correct && child_correct) 
        {
            TestUtils::print_colored("Bone transformation test passed!\n", 
                TestUtils::ConsoleColor::Green);
        } 
        else 
        {
            TestUtils::print_colored("Bone transformation test failed.\n", 
                TestUtils::ConsoleColor::Red);
        }
        
        return root_correct && child_correct;
    });
    
    suite.add_test("Inverse Bind Pose Calculation", []() 
    {
        TestUtils::print_colored("Testing inverse bind pose calculation...\n", 
            TestUtils::ConsoleColor::Default);
        
        // Set up a simple bind pose skeleton
        Skeleton bind_pose;
        
        // Create root joint
        Joint root_joint;
        root_joint.parent_id = -1;
        root_joint.local_transform = MathFacade::create_identity();
        
        // Create child joint
        Joint child_joint;
        child_joint.parent_id = 0;
        child_joint.local_transform = MathFacade::translate(0.f, 1.f, 0.f);
        
        // Add joints to skeleton
        bind_pose.joints.push_back(root_joint);
        bind_pose.joints.push_back(child_joint);
        
        // Calculate global transforms
        std::vector<HMM_Mat4> global_transforms;
        bind_pose.calculate_global_transforms(global_transforms);
        
        // Calculate inverse bind pose
        std::vector<HMM_Mat4> inverse_bind_matrices;
        for (const auto& transform : global_transforms) 
        {
            inverse_bind_matrices.push_back(MathFacade::inverse(transform));
        }
        
        // Check that inverse_bind * global_transform = identity
        const HMM_Mat4 result1 = MathFacade::multiply(
            inverse_bind_matrices[0],
            global_transforms[0]
        );
        
        const HMM_Mat4 result2 = MathFacade::multiply(
            inverse_bind_matrices[1],
            global_transforms[1]
        );
        
        const bool root_inverse_correct = 
            TestUtils::approx_equal_mat4(result1, MathFacade::create_identity());
        const bool child_inverse_correct = 
            TestUtils::approx_equal_mat4(result2, MathFacade::create_identity());
        
        TestUtils::set_console_color(
            root_inverse_correct ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Root inverse bind pose correct: " 
                  << (root_inverse_correct ? "Yes" : "No") << std::endl;
        TestUtils::reset_console_color();
        
        TestUtils::set_console_color(
            child_inverse_correct ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Child inverse bind pose correct: " 
                  << (child_inverse_correct ? "Yes" : "No") << std::endl;
        TestUtils::reset_console_color();
        
        if (root_inverse_correct && child_inverse_correct) 
        {
            TestUtils::print_colored("Inverse bind pose test passed!\n", 
                TestUtils::ConsoleColor::Green);
        } 
        else 
        {
            TestUtils::print_colored("Inverse bind pose test failed.\n", 
                TestUtils::ConsoleColor::Red);
        }
        
        return root_inverse_correct && child_inverse_correct;
    });
    
    // Test complex hierarchy
    suite.add_test("Complex Bone Hierarchy Transformation", []() 
    {
        TestUtils::print_colored("Testing complex bone hierarchy...\n", 
            TestUtils::ConsoleColor::Default);
        
        // Create a more complex hierarchy
        // Root -> Child1 -> Child2
        //      -> Child3
        Skeleton skeleton;
        
        // Create joints
        Joint root_joint;
        root_joint.parent_id = -1;
        root_joint.local_transform = MathFacade::create_identity();
        
        Joint child1_joint;
        child1_joint.parent_id = 0; // Parent is root
        child1_joint.local_transform = MathFacade::translate(1.f, 0.f, 0.f);
        
        Joint child2_joint;
        child2_joint.parent_id = 1; // Parent is child1
        child2_joint.local_transform = MathFacade::translate(0.f, 1.f, 0.f);
        
        Joint child3_joint;
        child3_joint.parent_id = 0; // Parent is root
        child3_joint.local_transform = MathFacade::translate(0.f, 0.f, 1.f);
        
        // Add joints to skeleton
        skeleton.joints.push_back(root_joint);
        skeleton.joints.push_back(child1_joint);
        skeleton.joints.push_back(child2_joint);
        skeleton.joints.push_back(child3_joint);
        
        // Calculate global transforms
        std::vector<HMM_Mat4> global_transforms;
        skeleton.calculate_global_transforms(global_transforms);
        
        if (global_transforms.size() != 4) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Incorrect number of global transforms: " 
                      << global_transforms.size() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
        
        // Expected global transforms:
        // Root: Identity
        // Child1: Identity * Translate(1,0,0) = Translate(1,0,0)
        // Child2: Translate(1,0,0) * Translate(0,1,0) = Translate(1,1,0)
        // Child3: Identity * Translate(0,0,1) = Translate(0,0,1)

        // Create expected matrices for comparison
        const HMM_Mat4 expected_child2 = MathFacade::translate(1.f, 1.f, 0.f);

        // Child3's global transform is its parent's global transform * its local transform
        // Since its parent is Root with identity transform, Child3's global = Child3's local
        const HMM_Mat4 expected_child3 = MathFacade::translate(0.f, 0.f, 1.f);

        const bool child2_correct = TestUtils::approx_equal_mat4(
            global_transforms[2],
            expected_child2
        );

        const bool child3_correct = TestUtils::approx_equal_mat4(
            global_transforms[3],
            expected_child3
        );
        
        TestUtils::set_console_color(
            child2_correct ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Child2 global transform correct: " 
                  << (child2_correct ? "Yes" : "No") << std::endl;
        TestUtils::reset_console_color();
        
        TestUtils::set_console_color(
            child3_correct ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Child3 global transform correct: " 
                  << (child3_correct ? "Yes" : "No") << std::endl;
        TestUtils::reset_console_color();
        
        if (child2_correct && child3_correct) 
        {
            TestUtils::print_colored("Complex bone hierarchy test passed!\n", 
                TestUtils::ConsoleColor::Green);
        } 
        else 
        {
            TestUtils::print_colored("Complex bone hierarchy test failed.\n", 
                TestUtils::ConsoleColor::Red);
        }
        
        return child2_correct && child3_correct;
    });
    
    return suite;
}
