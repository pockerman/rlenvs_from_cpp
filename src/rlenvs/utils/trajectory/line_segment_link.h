#include "rlenvs/utils/trajectory/waypoint.h"

namespace rlenvscpp{
namespace utils{
namespace trajectory{


template<int dim, typename NodeData, typename SegmentData>
class LineSegmentLink: private kernel::kernel_detail::generic_line_base<WayPoint<dim, NodeData>>
{
public:

    static const int dimension = dim;
    typedef NodeData node_data_t;
    typedef SegmentData segment_data_t;
    typedef WayPoint<dim, NodeData> w_point_t;
    typedef kernel::kernel_detail::generic_line_base<w_point_t> base;
    typedef typename base::node_type node_type;

    using base::start;
    using base::end;
    using base::get_id;
    using base::set_id;
    using base::has_valid_id;

    /// \brief Constructor
    LineSegmentLink(uint_t id, const w_point_t& v1,
                const w_point_t& v2, const segment_data_t& data);


    /// \brief Constructor
    LineSegmentLink(uint_t id, const w_point_t& v1,
                const w_point_t& v2);


    /// \brief Returns the v-th vertex of the segment
    const w_point_t& get_vertex(uint_t v)const;

    /// \brief Returns true if the segment is active
    bool is_active()const{return is_active_;}

    /// \brief deactive the segment
    void deactivate(){is_active_ = false;}

    /// \brief Activate the segment
    void make_active(){is_active_ = true;}

    /// \brief Returns the orientation of the
    /// segment with respect to some global frame
    real_t get_orientation()const{return data_.theta;}

    /// \brief Returns the angular velocity on the
    /// segment
    real_t get_angular_velocity()const{return data_.w;}

    /// \brief Returns the linear velocity on the
    /// segment
    real_t get_velocity()const{return data_.v;}

    /// \brief Returns the Euclidean distance between
    /// the start and end vertices of the segmen
    real_t length()const{return this->end().distance(this->start());}

private:

    /// \brief list of internal points of
    /// the segment.
    std::vector<w_point_t> internal_points_;

    /// \brief The data asociated with the segmen
    segment_data_t data_;

    /// \brief Flag indicating if the segment is active
    bool is_active_;
};

template<int dim, typename NodeData, typename SegmentData>
LineSegmentLink<dim, NodeData, 
                SegmentData>::LineSegmentLink(uint_t id,
                                              const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v1,
                                              const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v2,
                                              const typename LineSegment<dim, NodeData, SegmentData>::segment_data_t& data)
    :
     kernel::kernel_detail::generic_line_base<WayPoint<dim, NodeData>>(v1, v2, id),
     internal_points_(),
     data_(data),
     is_active_(true)
{}

template<int dim, typename NodeData, typename SegmentData>
LineSegmentLink<dim, NodeData, SegmentData>::LineSegmentLink(uint_t id,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v1,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v2)
    :
    LineSegmentLink<dim, NodeData, SegmentData>(id, v1, v2, 
	                                            typename LineSegmentLink<dim, NodeData, SegmentData>::segment_data_t())
{}


template<int dim, typename NodeData, typename SegmentData>
const typename LineSegmentLink<dim, NodeData, SegmentData>::w_point_t&
LineSegmentLink<dim, NodeData, SegmentData>::get_vertex(uint_t v)const{

    if( v == 0 ){
        return this->start();
    }
    else if(v == 1){
        return this->end();
    }

    throw std::logic_error("Vertex index not in [0,1]");
}
			

	
}
}
}