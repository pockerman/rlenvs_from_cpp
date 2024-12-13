#include "rlenvs/envs/connect2/connect2_env.h"
#include "rlenvs/envs/time_step_type.h"

#include <algorithm>
#include <unordered_map>

namespace rlenvs_cpp{
namespace envs{
namespace connect2{
	
const std::string Connect2::name = "Connect2";

Connect2::Connect2()
:
is_created_(false),
version_("v1"),
discount_(1.0)
board_()
{}


void 
Connect2::make(const std::string& /*version*/,
               const std::unordered_map<std::string, std::any>& /*options*/){
				   
	board.resize(4, 0);
	is_created_ = true;				   

}

time_step_type 
Connect2::step(action_type action){
	
}

time_step_type 
Connect2::reset(){
	board_ = std::vector(4, 0);
	return time_step_type(TimeStepTp::FIRST, 0.0, board_, discount_);
}

bool 
Connect2::is_win(uint_t player)const noexcept{
	
	auto player_sum = 0;
	std::for_each(board_.begin(),
	              board_.end(),
				  [&player_sum, player](auto val){
					
					 if(val == player)
						 player_sum += 1;
				  });
				  
	return player_sum == win_val_;
}
		
}
}
} 