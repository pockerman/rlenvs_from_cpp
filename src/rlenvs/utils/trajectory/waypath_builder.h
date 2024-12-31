#ifndef WAYPATH_BUILDER_H
#define WAYPATH_BUILDER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/data_structs/boost_serial_graph.h"

#include <vector>

namespace cengine {
namespace grids {

/// \brief Build the WaypointPath from the givn
/// BoostGraph
template<int dim, typename NodeData, typename EdgeData>
void build(WaypointPath<dim, NodeData, EdgeData>& path,
           const kernel::BoostSerialGraph<NodeData, EdgeData>& graph,
           const std::vector<uint_t>& vertices){

    /// clear the path entries
    path.clear();

    path.reserve_nodes(vertices.size());

    const auto& vertex0 = graph.get_vertex(vertices[0]);
    auto* previous_node = path.add_node(vertex0.data.position);
    auto* next_node = previous_node;
    next_node = nullptr;

    for(uint_t v=1; v<vertices.size(); ++v){

        const auto& vertex = graph.get_vertex(vertices[v]);
        next_node = path.add_node(vertex.data.position);

        ///add a new element
        auto* element = path.add_element(previous_node->get_id(),
                                         next_node->get_id());

        previous_node = next_node;
    }

}

}

}

#endif // WAYPATH_BUILDER_H
