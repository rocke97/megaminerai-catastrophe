// AI
// This is where you build your AI

#include "ai.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
// <<-- /Creer-Merge: includes -->>
using std::cout;
using std::endl;

namespace cpp_client
{

namespace catastrophe
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Arrays Start at Zero";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
	cout << "Game start." << endl;

	cout << "Game start end." << endl;
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
	cout << "Game end." << endl;
    //<<-- /Creer-Merge: ended -->>
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    // <<-- Creer-Merge: runTurn -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // Put your game logic here for run_turn here
	cout << "Running turn." << endl;

	if (first_turn)
	{
		auto player_units = player->units;

		int counter = 0;

		for (auto unit : player_units)
		{
			cout << unit->job->title << endl;
			if (unit->job->title == "cat overlord")
				cout << "Reached cat" << endl;
			else
			{
				if (counter == 0)
				{
					if (unit->change_job("missionary"))
					{
						cout << "missionary is the Correct Name." << endl;
						counter++;
					}
					cout << "The title is: \n\n\n" << unit->job->title << endl;
				}
				else if (counter == 1)
				{
					if (unit->change_job("builder"))
					{
						cout << "builder is the Correct Name." << endl;
						counter++;
					}
					cout << "The title is: \n\n\n" << unit->job->title << endl;
				}
				else //counter == 2
				{
					if (unit->change_job("soldier"))
					{
						cout << "Created a soldier." << endl;
						counter++;
					}
					cout << "The title is: \n\n\n" << unit->job->title << endl;
				}
			}
		}

		first_turn = false;
	}

	//grab all units
	auto player_units = player->units;

	for (auto unit : player_units)
	{
		if (unit->job->title == "soldier")
		{
			//warrior_turn(unit); <-- broken
			soldier_turn(unit);
		}
		if (unit->job->title == "missionary") {
			converter_turn(unit);
		}
	}

	cout << "Ending turn." << endl;
    // <<-- /Creer-Merge: runTurn -->>
    return true;
}

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> AI::find_path(const Tile& start, const Tile& goal)
{
    // no need to make a path to here...
    if (start == goal)
    {
        return {};
    }

    // the tiles that will have their neighbors searched for 'goal'
    std::queue<Tile> fringe;

    // How we got to each tile that went into the fringe.
    std::unordered_map<Tile,Tile> came_from;

    // Enqueue start as the first tile to have its neighbors searched.
    fringe.push(start);

    // keep exploring neighbors of neighbors... until there are no more.
    while(fringe.size() > 0)
    {
        // the tile we are currently exploring.
        Tile inspect = fringe.front();
        fringe.pop();

        // Note, we are using the `auto` keyword here
        //   The compiler can discern that this is a `std::vector<Tile>` based on the return type of `getNeighbors()`
        auto neighbors = inspect->get_neighbors();

        // cycle through the tile's neighbors.
        for (unsigned i = 0; i < neighbors.size(); i++)
        {
            Tile neighbor = neighbors[i];

            // If we found the goal we've found the path!
            if (neighbor == goal)
            {
                // Follow the path backward starting at the goal and return it.
                std::deque<Tile> path;
                path.push_front(goal);

                // Starting at the tile we are currently at, insert them retracing our steps till we get to the starting tile
                for (Tile step = inspect; step != start; step = came_from[step])
                {
                    path.push_front(step);
                }

                // we want to return a vector as that's what we use for all containers in C++
                // (and they don't have push_front like we need)
                // So construct the vector-ized path here
                std::vector<Tile> vector_path;
                for (auto& tile : path)
                {
                    vector_path.push_back(tile);
                }
                return vector_path;
            }

            // if the tile exists, has not been explored or added to the fringe yet, and it is pathable
            if (neighbor && came_from.count(neighbor) == 0 && neighbor->is_pathable())
            {
                // add it to the tiles to be explored and add where it came from.
                fringe.push(neighbor);
                came_from[neighbor] = inspect;
            }

        } // for each neighbor

    } // while fringe not empty

    // if you're here, that means that there was not a path to get to where you want to go.
    //   in that case, we'll just return an empty path.
    return {};
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional methods here for your AI to call
//<<-- /Creer-Merge: methods -->>
std::vector<Tile> AI::shortest_path_to_materials(const Unit& unit)
{
	//return the shortest path to a gatherable structure, or a empty vector if theres nothing
	std::vector<std::vector<Tile> > possible_paths;
	std::vector<Tile> nodes_to_try;

	for (auto q : game->tiles) {
		if ((q->structure != NULL) && (q->structure->type == "neutral")) {
			nodes_to_try.push_back(q);
		}
	}

	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_shelter(const Unit& unit)
{
	//return the shortest path to a structure, or a empty vector if theres nothing
	std::vector<std::vector<Tile> > possible_paths;
	std::vector<Tile> nodes_to_try;
	std::vector<Structure> playerShelters;

	for (auto q : player->structures) {
		if (q->type == "shelter") {
			playerShelters.push_back(q);
		}
	}

	for (auto q : playerShelters) {
		Tile temp = q->tile;
		nodes_to_try.push_back(temp);
	}

	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_neutral_human(const Unit& unit)
{
	//return the shortest path to a neutral, or a empty vector if theres nothing
	std::vector<Tile> nodes_to_try;
	std::vector<Unit> neutralHumans;

	for (auto q : game->units) {
		if (q->owner == NULL) {
			neutralHumans.push_back(q);
		}
	}
	for (auto q : neutralHumans) {
		Tile temp = q->tile;
		nodes_to_try.push_back(temp);
	}

	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_soldier(const Unit& unit)
{
	//return the shortest path to a soldier, or a empty vector if theres nothing
	std::vector<std::vector<Tile> > possible_paths;
	std::vector<Tile> nodes_to_try;
	std::vector<Unit> ourSoldiers;
	for (auto q : player->units) {
		if (q->job->title == "soldier") {
			ourSoldiers.push_back(q);
		}
	}
	for (auto q : ourSoldiers) {
			Tile temp = q->tile;
			nodes_to_try.push_back(temp);
		}

	return find_closest_helper(nodes_to_try, unit);
}

bool AI::converter_turn(Unit& converter)
{
	if (converter->energy < 75.0) { //not enough energy so try to rest by moving to a rest point
		std::cout << "Moving to rest, converter" << endl;
		std::vector<Tile> closestStructure = find_closest_shelter(converter);

		if (closestStructure.size() == 0) {
			//No structure sooooo what do we want to do?
			return false;
		}

		if (closestStructure.size() == 1) {
			converter->rest();
			return true;
		} else {
			while((!closestStructure.empty()) && (converter->moves > 0)) {
				auto iter = closestStructure.begin();
				converter->move(*iter);
				closestStructure.erase(iter);

			}
			return true;
		}
	}

	//energy higher than 75 so lets try to convert a enemy
	std::vector<Tile> closestNeutralHuman = find_closest_neutral_human(converter);
	if ((closestNeutralHuman.empty()) || (closestNeutralHuman.size()-1 > converter->moves)) {
		std::cout << "following soldier, converter turn" << endl;
		// no enemies or none close enough to convert
		//so follow soilder instead
		std::vector<Tile> soldierToFollow = find_closest_soldier(converter);
		if (soldierToFollow.size() == 1) {
			//already at soldier
			return true;
		}
		if ((soldierToFollow.size() > 1) && (converter->moves > 0)) { //move towards soldier
			while((!soldierToFollow.empty()) && (converter->moves > 0)) {
				auto iter = soldierToFollow.begin();
				converter->move(*iter);
				soldierToFollow.erase(iter);
			}
			return true;
		} else {
			return false; //no soldier so what do we want to do?
		}
	} else { // enemy close enough to convert
		std::cout << "Going to convert neutral human, converter turn" << endl;
		while((closestNeutralHuman.size() > 1) && (converter->moves > 0)) { //move to the neutralHumans
			auto iter = closestNeutralHuman.begin();
			converter->move(*iter);
			closestNeutralHuman.erase(iter);
		}

		converter->convert(closestNeutralHuman[0]);
		//converted an enemy so lets head towards a restpoint
		std::vector<Tile> closestStructure = find_closest_shelter(converter);

		if (closestStructure.size() == 0) {
			//No structure sooooo what do we want to do?
			return false;
		}

		if (closestStructure.size() == 1) {
			converter->rest();
			return true;
		} else {
			while((!closestStructure.empty()) && (converter->moves > 0)) {
				auto iter = closestStructure.begin();
				converter->move(*iter);
				closestStructure.erase(iter);
			}
			return true;
		}
	}
	return false;
}

std::vector<Tile> AI::find_closest_enemy(const Unit& unit)
{
	//return the shortest path to a soldier, or a empty vector if theres nothing
	std::vector<Tile> nodes_to_try;
	std::vector<Unit> enemies;

	for (auto q : player->opponent->units) {
			enemies.push_back(q);
	}

	for (auto q : enemies) {
		Tile temp = q->tile;
		nodes_to_try.push_back(temp);
	}

	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_helper(const std::vector<Tile>& nodes_to_try, const Unit& unit)
{
	std::vector<std::vector<Tile>> possible_paths;

	for (auto q : nodes_to_try) {
		std::vector<Tile> temp = find_path(unit->tile, q);
		if (!temp.empty())
			possible_paths.push_back(temp);
	}

	if (possible_paths.empty()) {
		return nodes_to_try;
	}

	unsigned size = possible_paths[0].size();
	int index = 0;

	for (unsigned i = 0; i < possible_paths.size(); ++i) {
		if (possible_paths[i].size() < size) {
			size = possible_paths[i].size();
			index = i;
		}
	}

	return possible_paths[index];
}

bool AI::soldier_turn(Unit& unit)
{
	if (unit->energy <= 25.0) {//not enough energy so lets go rest
		std::vector<Tile> closestStructure = find_closest_shelter(unit);

		if (closestStructure.size() == 0) {
			std::cout << "Error, no shelter, soldier turn" << endl;
			//No structure sooooo what do we want to do?
			return false;
		}

		if (closestStructure.size() == 1) {
			std::cout << "Soldier is resting" << endl;
			unit->rest();
			return true;
		} else {
			std::cout << "Moving to shelter, to rest soldier turn" << endl;
			while((!closestStructure.empty()) && (unit->moves > 0)) {
				auto iter = closestStructure.begin();
				unit->move(*iter);
				closestStructure.erase(iter);

			}
			return true;
		}
	} else {
		std::cout << "Moving towards enemy, soldier turn" << endl;
		//have energy lets hunt for a enemy
		std::vector<Tile> closestEnemy = find_closest_enemy(unit);

		if (closestEnemy.empty()) {
			std::cout << "error no enemy, soldier turn" << endl;
			return false; // no enemy so what do you want to do?
		} else if (closestEnemy.size()-1 <= unit->moves) {
			std::cout << "soldier is moving and will attack this turn" << endl;
			//lets attack this Turn
			while(closestEnemy.size() > 1) {
				auto iter = closestEnemy.begin();
				unit->move(*iter);
				closestEnemy.erase(iter);
			}
			unit->attack(closestEnemy[0]);
			return 1;
		} else { //move towards the unit
			std::cout << "soldier is only moving towards the unit this turn" << endl;
			while(unit->moves > 0) {
				auto iter = closestEnemy.begin();
				unit->move(*iter);
				closestEnemy.erase(iter);
			}
		}
	}
	return false; // some issue
}

} // catastrophe

} // cpp_client