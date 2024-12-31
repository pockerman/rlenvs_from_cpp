#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/utils/geometry/geom_point.h"

namespace rlenvscpp{
namespace utils{
namespace trajectory{

	
///
/// \brief Helper class to represent a waypoint
///
template<int dim, typename Data>
struct WayPoint: public kernel::GeomPoint<dim>
{
    typedef kernel::GeomPoint<dim> position_t;
    typedef Data data_t;
    uint_t id;
    data_t data;
    bool is_active_point;

    /// constructor
    WayPoint(const position_t& p, uint_t id_, const data_t& data_=data_t())
        :
          kernel::GeomPoint<dim>(p),
          id(id_),
          data(data),
          is_active_point(true)
    {}

    /// \brief Return the id of the point
    uint_t get_id()const{return id;}

    /// \brief Writable reference to the data
    data_t& get_data(){return data;}

    /// \brief Read reference to the data
    const data_t& get_data()const{return data;}

    /// \brief Returns true if the waypoint is active
    bool is_active()const{return is_active_point;}
};


			
}
}
}