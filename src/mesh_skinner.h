// Standard library imports
#include <string>

// Local application imports
#include "model/mesh.h"
#include "model/skeleton.h"
#include "model/skinning_data.h"


class MeshSkinner
{
public:
        
    // Core workflow methods
    bool load_mesh(const std::string& mesh_path);
    bool load_weights(const std::string& weights_path);
    bool load_bind_pose(const std::string& bind_pose_path);
    bool load_new_pose(const std::string& new_pose_path);
    
    // Process the skinning operation
    bool perform_skinning();
    
    // Output method
    bool save_skinned_mesh(const std::string& output_path);
    
    // Getter for the resulting mesh
    const Mesh& get_skinned_mesh() const;

protected:
    
    void calculate_vertex_transformations();
    void apply_vertex_transformations();

    static const float WEIGHT_THRESHOLD;
    
private:

    Mesh original_mesh;
    Mesh skinned_mesh;
    SkinningData skin_data;
    Skeleton bind_pose;
    Skeleton new_pose;
};
