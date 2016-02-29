#ifndef GAMES_${underscore(game_name).upper()}_FWD_H
#define GAMES_${underscore(game_name).upper()}_FWD_H

// ${header}

//include declarations for all of the internal classes in the game's namespace
#include <memory>

namespace cpp_client
{

namespace ${underscore(game_name)}
{

% for game_obj_key in sort_dict_keys(game_objs):
<% obj_name = underscore(game_obj_key).capitalize() %>
class ${obj_name}_;
using ${obj_name} = std::shared_ptr<${obj_name}_>;
% endfor

}

}

#endif // GAMES_${underscore(game_name).upper()}_H