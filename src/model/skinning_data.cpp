#include "skinning_data.h"

// Standard library imports
#include <stdexcept>

// Local application imports
#include "facade/json_facade.h"


std::vector<std::vector<JointInfluence>> SkinningData::from_json(const Json& json_obj)
{
    // Check for "vertex_weights" presence.
    if (!json_obj.contains("vertex_weights"))
    {
        throw std::runtime_error("SkinningData JSON missing 'vertex_weights' field");
    }

    // Retrieve the vertex_weights array
    const Json vertices_weights = json_obj["vertex_weights"];

    std::vector<std::vector<JointInfluence>> result;
    result.reserve(vertices_weights.size());

    // Parse each vertex's influences
    for (size_t i = 0; i < vertices_weights.size(); i++)
    {
        std::vector<JointInfluence> influences;
        const Json vertex_influences = vertices_weights.at(i);
        
        // Parse each joint influence
        for (size_t j = 0; j < vertex_influences.size(); j++)
        {
            const Json influence = vertex_influences.at(j);
            JointInfluence joint_influence;
            joint_influence.joint_id = influence["joint_id"].as_int();
            joint_influence.weight = influence["weight"].as_float();
            influences.push_back(joint_influence);
        }
        result.push_back(std::move(influences));
    }

    return result;
}
