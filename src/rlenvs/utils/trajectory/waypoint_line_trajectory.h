#ifndef WAYPOINT_LINE_TRAJECTORY_H
#define WAYPOINT_LINE_TRAJECTORY_H
#include "rlenvs/rlenvs_types_v2.h"

#include <vector>
#include <utility>

namespace rlenvscpp{
namespace utils{
namespace trajectory{

///
/// \brief A container that represents a collection
/// of way points linked together via a LinkType
/// The LinkType defines how smooth the trajectory
/// will look like
///
template<typename LinkType>
class WaypointTrajectory
{
public:
	
	typedef LinkType link_type;
	
	typedef typename link_type::waypoint_type waypoint_type;
	
	///
	/// \brief point iteration
	///
    typedef typename std::vector<link_type>::iterator link_iterator;
    typedef typename std::vector<link_type>::const_iterator const_link_iterator;
	
	///
	/// \brief Constructor
	///
	WaypointTrajectory();
	
	///
	/// \brief Constructor
	///
	explicit WaypointTrajectory(uint_t n);
	
	///
	/// \brief Computes the minimum distance of the 
	/// given point from the trajectory
	///
	std::pair<real_t, link_type> distance(const waypoint_type& p)const;
	
	///
	/// \brief How many waypoints the pah has
	///
    uint_t size()const{return links_.size();}
	
	///
	/// \brief Reserve space for waypoints
	///
    void reserve(uint_t n){links_.reserve(n)};
	
	///
	/// \brief clear the memory allocated for points and
    /// edges
    void clear(){links_.clear()};
	
	///
	/// \brief Returns true if the trajectory is empty
	///
	bool empty()const{return links_.empty();}
	
	///
	/// \brief Push a new link
	///
	void push(const link_type& link){links_.push_back(link);}
	
	///
	/// \brief Returns a read/write reference of the i-th link
	///
	link_type& operator[](uint_t i){return links_[i];}
	
	///
	/// \brief Returns a read reference of the i-th link
	///
	const link_type& operator[](uint_t i)const{return links_[i];}
	
	///
	/// \brief Resize the underlying links
	///
	void resize(uint_t n){links_.resize(n);}
	
	///
	/// \brief Raw node iteration
	///
    link_iterator links_begin(){return links_.begin();}
    link_iterator links_end(){return links_.end();}

	///
    /// \brief Raw node iteration
	///
    const_link_iterator links_begin()const{return links_.begin();}
    const_link_iterator links_end()const{return links_.end();}
	
private:
	
	///
	/// \brief The Waypoints of the path
	///
    //std::vector<waypoint_type> waypoints_;

	///
    /// \brief The segments of the path
	///
    std::vector<link_type> links_;
};

template<typename LinkType>
WaypointTrajectory<LinkType>::WaypointTrajectory()
:
links_()
{}

template<typename LinkType>
WaypointTrajectory<LinkType>::WaypointTrajectory(uint_t n)
:
links_(n)
{}

template<typename LinkType>
std::pair<real_t, typename WaypointTrajectory<LinkType>::link_type> 
WaypointTrajectory<LinkType>::distance(const waypoint_type& p)const{
	
	real_t = dist_;
	link_type link_;
	
	for(const auto& link: links_){
		
		auto dist = link.distance(p);
		
		if(dist < dist_){
			dist_ = dist;
			link_ = link;
		}
	}
	
	return std::make_pair(dist_, links_);
}
			
}
}
}

#endif