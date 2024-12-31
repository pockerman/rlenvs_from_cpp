#ifndef WAYPOINT_PATH_H
#define WAYPOINT_PATH_H

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/utils/geometry/geom_point.h"
#include "rlenvs/utils/geometry/generic_line.h"

#include <vector>
#include <stdexcept>

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


struct LineSegmentData
{
    /// \brief The maximum velocity
    /// allowed on the edge
    real_t Vmax{0.0};

    /// \brief The orientation of the
    /// segment with respect to the global coordinate
    /// frame. This may also dictate the orientation
    /// that a reference vehicle may have on the segment
    real_t theta{0.0};

    /// \brief The angular velocity on the segment
    real_t w{0.0};

    /// \brief The angular velocity on the segement
    real_t v{0.0};

};

template<int dim, typename NodeData, typename SegmentData>
class LineSegment: private kernel::kernel_detail::generic_line_base<WayPoint<dim, NodeData>>
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
    LineSegment(uint_t id, const w_point_t& v1,
                const w_point_t& v2, const segment_data_t& data);


    /// \brief Constructor
    LineSegment(uint_t id, const w_point_t& v1,
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
LineSegment<dim, NodeData, SegmentData>::LineSegment(uint_t id,
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
LineSegment<dim, NodeData, SegmentData>::LineSegment(uint_t id,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v1,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v2)
    :
    LineSegment<dim, NodeData, SegmentData>(id, v1, v2, typename LineSegment<dim, NodeData, SegmentData>::segment_data_t())
{}


template<int dim, typename NodeData, typename SegmentData>
const typename LineSegment<dim, NodeData, SegmentData>::w_point_t&
LineSegment<dim, NodeData, SegmentData>::get_vertex(uint_t v)const{

    if( v == 0 ){
        return this->start();
    }
    else if(v == 1){
        return this->end();
    }

    throw std::logic_error("Vertex index not in [0,1]");
}

/// \brief class WaypointPath models a path formed
/// by line segments and way points. The Data
/// template parameter represents the data held
/// at the waypoints of the path
template<int dim, typename PointData, typename EdgeData>
class WaypointPath
{
public:

    static const int dimension = dim;

    typedef PointData w_point_data_t;
    typedef WayPoint<dim, w_point_data_t> w_point_t;
    typedef kernel::GeomPoint<dim> point_t;
    typedef EdgeData segment_data_t;
    typedef LineSegment<dim, w_point_data_t, segment_data_t> segment_t;
    typedef segment_t element_t;

    /// \brief point iteration
    typedef typename std::vector<w_point_t*>::iterator node_iterator_impl;
    typedef typename std::vector<w_point_t*>::const_iterator cnode_iterator_impl;

    /// \brief Line segment  iteration
    typedef typename std::vector<segment_t*>::iterator element_iterator_impl;
    typedef typename std::vector<segment_t*>::const_iterator celement_iterator_impl;

    /// \brief Constructor
    WaypointPath();

    /// \brief Destructor
    ~WaypointPath();

    /// \brief How many waypoints the pah has
    uint_t n_nodes()const{return waypoints_.size();}

    /// \brief How many segments the path has
    uint_t n_elements()const{return segments_.size();}

    /// \brief Reserve space for waypoints
    void reserve_nodes(uint_t n);

    /// \brief Reserve space for
    void reserve_elements(uint_t n);

    /// \brief clear the memory allocated for points and
    /// edges
    void clear();

    /// \brief Add a new waypoint in the path
    /// and get a writable pointer default
    /// waypoint data is assumed
    w_point_t* add_node(const kernel::GeomPoint<dim>& position)
    {return add_node(position, w_point_data_t());}


    /// \brief Add a new waypoint in the path and get back
    /// a writable reference of the newly added point
    w_point_t* add_node(const kernel::GeomPoint<dim>& position,
                        const w_point_data_t& data);

    /// \brief Add a new segment in the path and get back
    /// a writable reference of the newly added segment.
    /// Throws std::logic_error if vid0 and vid1 are not
    /// in the path
    segment_t* add_element(uint_t vid0, uint_t vid1,
                            const segment_data_t& data);

    /// \brief Add a new segment in the path and get back
    /// a writable reference of the newly added segment.
    /// Throws std::logic_error if vid0 and vid1 are not
    /// in the path
    segment_t* add_element(uint_t vid0, uint_t vid1)
    {return add_element(vid0, vid1, segment_data_t());}

    /// \brief read/write access to the n-th segment
    segment_t* element(uint_t e);

    /// \brief read access to the n-th segment
    const segment_t* element(uint_t e)const;

    /// \brief Raw node iteration
    node_iterator_impl nodes_begin(){return waypoints_.begin();}
    node_iterator_impl nodes_end(){return waypoints_.end();}

    /// \brief Raw node iteration
    cnode_iterator_impl nodes_begin()const{return waypoints_.begin();}
    cnode_iterator_impl nodes_end()const{return waypoints_.end();}

    /// \brief Raw elements iteration
    element_iterator_impl elements_begin(){return segments_.begin();}
    element_iterator_impl elements_end(){return segments_.end();}

    /// \brief Raw elements iteration
    celement_iterator_impl elements_begin()const{return segments_.begin();}
    celement_iterator_impl elements_end()const{return segments_.end();}

private:

    /// \brief The Waypoints of the path
    std::vector<w_point_t*> waypoints_;

    /// \brief The segments of the path
    std::vector<segment_t*> segments_;

};

template<int dim, typename PointData, typename EdgeData>
WaypointPath<dim, PointData, EdgeData>::WaypointPath()
    :
      waypoints_(),
      segments_()
{}

template<int dim, typename PointData, typename EdgeData>
WaypointPath<dim, PointData, EdgeData>::~WaypointPath(){
    clear();
}

template<int dim, typename PointData, typename EdgeData>
void
WaypointPath<dim, PointData, EdgeData>::clear(){

    for(uint_t i=0; i<waypoints_.size(); ++i){
        if(waypoints_[i] != nullptr){
            delete waypoints_[i];
            waypoints_[i] = nullptr;
        }
    }

    for(uint_t i=0; i<segments_.size(); ++i){
        if(segments_[i] != nullptr){
            delete segments_[i];
            segments_[i] = nullptr;
        }
    }
}

template<int dim, typename PointData, typename EdgeData>
typename WaypointPath<dim, PointData, EdgeData>::w_point_t*
WaypointPath<dim, PointData, EdgeData>::add_node(const kernel::GeomPoint<dim>& position,
                                                      const typename WaypointPath<dim, PointData, EdgeData>::w_point_data_t& data){

    uint_t id = waypoints_.size();
    WaypointPath<dim, PointData, EdgeData>::w_point_t* p = new WaypointPath<dim, PointData, EdgeData>::w_point_t(position, id, data);
    waypoints_.push_back(p);
    return waypoints_[id];
}

template<int dim, typename PointData, typename EdgeData>
typename WaypointPath<dim, PointData, EdgeData>::segment_t*
WaypointPath<dim, PointData, EdgeData>::add_element(uint_t vid0, uint_t vid1,
                            const typename WaypointPath<dim, PointData, EdgeData>::segment_data_t& data){

    if(vid0 >= waypoints_.size() ||
            vid1 >= waypoints_.size()){

        throw std::logic_error("Waypoint id given not in range [0, " +
                               std::to_string(waypoints_.size()) + ")");

    }

    if(vid0 == vid1){
       throw std::logic_error("Cannot create segment having identical vertices");
    }

    auto id = segments_.size();
    auto v0 = waypoints_[vid0];
    auto v1 = waypoints_[vid1];
    WaypointPath<dim, PointData, EdgeData>::segment_t* seg = new WaypointPath<dim, PointData, EdgeData>::segment_t(id, *v0, *v1, data);
    segments_.push_back(seg);
    return segments_[id];
}

template<int dim, typename PointData, typename EdgeData>
void
WaypointPath<dim, PointData, EdgeData>::reserve_nodes(uint_t n){
    waypoints_.reserve(n);
}

template<int dim, typename PointData, typename EdgeData>
void
WaypointPath<dim, PointData, EdgeData>::reserve_elements(uint_t n){
    segments_.reserve(n);
}

template<int dim, typename PointData, typename EdgeData>
typename WaypointPath<dim, PointData, EdgeData>::segment_t*
WaypointPath<dim, PointData, EdgeData>::element(uint_t e){

    if(e >= segments_.size()){
        throw std::logic_error("Invalid segment id: "+
                               std::to_string(e)+
                               "not in [0, "+
                               std::to_string(segments_.size()));
    }

    return segments_[e];
}

template<int dim, typename PointData, typename EdgeData>
const typename WaypointPath<dim, PointData, EdgeData>::segment_t*
WaypointPath<dim, PointData, EdgeData>::element(uint_t e)const{
    if(e >= segments_.size()){
        throw std::logic_error("Invalid segment id: "+
                               std::to_string(e)+
                               "not in [0, "+
                               std::to_string(segments_.size()));
    }

    return segments_[e];
}

}
}
}
#endif // WAYPOINT_PATH_H
