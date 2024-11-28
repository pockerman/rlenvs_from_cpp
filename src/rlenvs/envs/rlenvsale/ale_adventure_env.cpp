#include "rlenvs/envs/rlenvsale/ale_adventure_env.h"

#ifdef USE_ALE_ENS

namespace rlenvs_cpp{
namespace envs{
namespace rlenvsale{
	
const std::string ALEAdventure::name = "Adventure";
	
	
ALEAdventure::ALEAdventure()
{}
	
	
void 
ALEAdventure::make(const std::string&,
			       const std::unordered_map<std::string, std::any>&){
					
	ale_.setInt("random_seed", 123);
	ale_.loadROM("adventure.bin");
					   
}
	
}
}
}

#endif