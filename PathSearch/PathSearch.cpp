#include "PathSearch.h"

namespace fullsail_ai { namespace algorithms {

	PathSearch::PathSearch()
	{
	}

	PathSearch::~PathSearch()
	{
	}

	static int iterations;

	void PathSearch::initialize(TileMap* _tileMap)
	{
		//set tilemap
		tileMap = _tileMap;
		done = false;

		//create SearchNodes
		for (int y = 0; y < tileMap->getRowCount(); y++) {
			for (int x = 0; x < tileMap->getColumnCount(); x++) {
				SearchNode* newNode = new SearchNode();
				newNode->tile = tileMap->getTile(y, x);
				nodes[newNode->tile] = newNode;
			}
		}

		//get neighbors
		for (int y = 0; y < tileMap->getRowCount(); y++) {
			for (int x = 0; x < tileMap->getColumnCount(); x++) {
				std::vector<SearchNode*> neighbors;
				
				//get neighbors
				if (y % 2 == 0) {
					if (!(x - 1 < 0)) {
						if (!(y - 1 < 0))
							neighbors.push_back(nodes[tileMap->getTile(y - 1, x - 1)]);

						if(!(y + 1 >= tileMap->getRowCount()))
							neighbors.push_back(nodes[tileMap->getTile(y + 1, x - 1)]);

						neighbors.push_back(nodes[tileMap->getTile(y, x - 1)]);
					}

					if (!(x + 1 >= tileMap->getColumnCount()))
						neighbors.push_back(nodes[tileMap->getTile(y, x + 1)]);

					if(!(y - 1 < 0))
						neighbors.push_back(nodes[tileMap->getTile(y - 1, x)]);

					if(!(y + 1 >= tileMap->getRowCount()))
						neighbors.push_back(nodes[tileMap->getTile(y + 1, x)]);
				}
				else {
					if (!(x - 1 < 0))
						neighbors.push_back(nodes[tileMap->getTile(y, x - 1)]);

					if (!(x + 1 >= tileMap->getColumnCount())) {
						if (!(y - 1 < 0))
							neighbors.push_back(nodes[tileMap->getTile(y - 1, x + 1)]);

						if (!(y + 1 >= tileMap->getRowCount()))
							neighbors.push_back(nodes[tileMap->getTile(y + 1, x + 1)]);

						neighbors.push_back(nodes[tileMap->getTile(y, x + 1)]);
					}

					if (!(y - 1 < 0))
						neighbors.push_back(nodes[tileMap->getTile(y - 1, x)]);

					if (!(y + 1 >= tileMap->getRowCount()))
						neighbors.push_back(nodes[tileMap->getTile(y + 1, x)]);
				}

				//set neighbors
				nodes[tileMap->getTile(y, x)]->neighbors = neighbors;
			}
		}
	}

	void PathSearch::enter(int startRow, int startColumn, int goalRow, int goalColumn)
	{
		//set the goal
		goal = tileMap->getTile(goalRow, goalColumn);
		start = tileMap->getTile(startRow, startColumn);

		//set up the start node
		PlannerNode* startNode = new PlannerNode();
		startNode->parent = nullptr;
		startNode->searchNode = nodes[start];
		visited[startNode->searchNode] = startNode;

#if BreadthFirst
		q.push(start);
#endif
#if !BreadthFirst
		startNode->cost = GetCost(startNode->searchNode->tile, goal) * HeuristicWeight;

#if UniformCost
		startNode->givenCost = 0;
#endif
#if Astar
		startNode->finalCost = startNode->cost;
#endif

		pq.push(startNode);
#endif
	}

	void PathSearch::update(long timeslice)
	{
		std::cout << iterations++ << std::endl;

		//get current node
		PlannerNode* current;
#if BreadthFirst
		current = q.front();
		q.pop();
#endif
#if !BreadthFirst
		current = pq.front();
		pq.pop();
#endif

		//std::cout << "-------------------------" << std::endl;
		//std::cout << "heuristic cost: " << current->cost << std::endl;
		//std::cout << "given cost: " << current->givenCost << std::endl;
		//std::cout << "final cost: " << current->finalCost << std::endl;
		//std::cout << "-------------------------" << std::endl;

		//handle goal reached
		PlannerNode* temp = current;
		if (temp->searchNode->tile == goal) {
			//build solution vector
			while (temp != nullptr) {
				sln.push_back(temp->searchNode->tile);
				temp = temp->parent;
			}

			//set flag
			done = true;
		}

		if (!done) {
			for (int i = 0; i < current->searchNode->neighbors.size(); i++) {
				if (current->searchNode->neighbors[i]->tile->getWeight() != 0) {
					//draw each neighbor being checked
					current->searchNode->neighbors[i]->tile->setOutline(0xffff8080);

#if !BreadthFirst
					double newCost = GetCost(current->searchNode->neighbors[i]->tile, goal);

#if Astar
					newCost *= HeuristicWeight;
#endif
#endif
#if UniformCost
					double newGivenCost = current->givenCost + (GetCost(current->searchNode->neighbors[i]->tile, current->searchNode->tile) * current->searchNode->neighbors[i]->tile->getWeight());
#endif

					if (visited.count(current->searchNode->neighbors[i]) == 0) {
						PlannerNode* newNode = new PlannerNode();
						newNode->parent = current;
						newNode->searchNode = current->searchNode->neighbors[i];
						visited[newNode->searchNode] = newNode;
#if BreadthFirst
						q.push(newNode);
#endif
#if !BreadthFirst
						newNode->cost = newCost;

#if UniformCost
						newNode->givenCost = newGivenCost;
#endif
#if Astar
						newNode->finalCost = newNode->cost + newNode->givenCost;
#endif

						pq.push(newNode);
#endif
					}
#if UniformCost
					else {
						PlannerNode* node = visited[current->searchNode->neighbors[i]];

						if (node->finalCost > newGivenCost + node->cost) {
							node->givenCost = newGivenCost;
							node->parent = current;

#if Astar
							node->finalCost = node->cost + node->givenCost;
#endif

							bool check = false;
							std::vector<PlannerNode*> tNodes;
							pq.enumerate(tNodes);

							for (int i = 0; i < tNodes.size(); i++) {
								if (tNodes[i] == node) {
									check = true;
									break;
								}
							}

							if (!check) pq.push(node);
						}
					}
#endif
				}
			}
		}

		//clear drawing
		tileMap->resetTileDrawing();

		//draw current path
		temp = current;

		while (temp->parent != nullptr) {
			temp->searchNode->tile->addLineTo(temp->parent->searchNode->tile, 0xffff0000);
			temp = temp->parent;
		}

		//draw neighbors
		for (int i = 0; i < current->searchNode->neighbors.size(); i++) {
			//draw each neighbor being checked
			current->searchNode->neighbors[i]->tile->setOutline(0xffff8080);
		}

		//draw visited
		for (auto iter = visited.begin(); iter != visited.end(); iter++) {
			iter->second->searchNode->tile->setFill(0xff0000ff);
		}

		//draw open
		int counter = 0;
		unsigned int color = 0xff00ff00;

#if BreadthFirst
		std::queue<PlannerNode*> drawq = q;
		while (!drawq.empty()) {
			color = (255 << 24) + (0 << 16) + (255 - (counter * 20) << 8) + (0);
			drawq.front()->searchNode->tile->setMarker(color);
			drawq.pop();
			counter++;
		}
#endif
#if !BreadthFirst
		std::vector<PlannerNode*> drawq;
		pq.enumerate(drawq);
		for (int i = drawq.size() - 1; i >= 0; i--) {
			color = (255 << 24) + (0 << 16) + (255 - (counter * 20) << 8) + (0);
			drawq[i]->searchNode->tile->setMarker(color);
			counter++;
		}
#endif
	}

	void PathSearch::exit()
	{
		//reset fields
		done = false;
		goal = nullptr;
		visited.clear();
		sln.clear();
		pq.clear();
		while (!q.empty()) q.pop();
	}

	void PathSearch::shutdown()
	{
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			delete iter->second;
		}
	}

	bool PathSearch::isDone() const
	{
		return done;
	}

	std::vector<Tile const*> const PathSearch::getSolution() const
	{
		return sln;
	}

	double PathSearch::CalculateDistance(double ax, double ay, double bx, double by)
	{
		return sqrt(pow((bx - ax), 2) + pow((by - ay), 2));
	}

	double PathSearch::GetCost(Tile const* t, Tile const* g) {
		return CalculateDistance(t->getXCoordinate(), t->getYCoordinate(), g->getXCoordinate(), g->getYCoordinate());
	}
}}  // namespace fullsail_ai::algorithms

