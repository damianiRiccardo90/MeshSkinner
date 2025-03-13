// Local application imports
#include "test_framework.h"
#include "MeshSkinner.h"


TestSuite create_mesh_loading_tests() 
{
    TestSuite suite("Mesh Loading");
    
    suite.add_test("Load Valid OBJ File", []() {
        MeshSkinner skinner;
        return skinner.load_mesh("test_data/cube.obj");
    });
    
    suite.add_test("Handle Nonexistent File", []() {
        MeshSkinner skinner;
        return !skinner.load_mesh("test_data/nonexistent.obj");
    });
    
    suite.add_test("Validate Mesh Vertex Count", []() {
        MeshSkinner skinner;
        if (!skinner.load_mesh("test_data/cube.obj")) return false;
        return skinner.get_skinned_mesh().vertices.size() == 8; // Assuming a cube has 8 vertices
    });
    
    return suite;
}
