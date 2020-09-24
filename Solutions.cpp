/*
used Stack ADT in a DFS Breadth First Search algorithm
and Queue ADT in a BFS Depth First Search algorithm to solve the maze
also used an unordered map to map out MazeNodes and included algorithm library to reverse a vector
Also included a helper function to find neighboring nodes for BFS search, doesn't work for DFS search
Used Queue as well to solve DEF Search by adding all the dead-ends to the stack
no custom alogirthm
*/

#include "Maze.cpp"
#include <stack>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

namespace solutions
{

int getNumberOfWalls(MazeNode *a_node)
{
    int wall_counter = 0;
    for (directions::nesw dir = directions::NORTH; dir < directions::EAST; dir = directions::nesw(dir + 1))
    {
        if (a_node->getDirectionNode(dir) == nullptr || a_node->getDirectionNode(dir)->isWall() || a_node->getDirectionNode(dir)->isVisited())
        {
            wall_counter++;
        }
    }
    return wall_counter;
}

bool canTravel(MazeNode *a_node)
{
	//nullptr check added
	if(a_node == nullptr)
    {
    	return false;
	}
    if (a_node->isVisited() || a_node->isWall())
    {
        return false;
    }
    return true;
}

/* 
    uses DFS(STACK) to return a vector with the shortest solution to the end of the maze.
    @param a_maze input
    @pre: a_maze input a random maze
    @post: return an ordered vector that holds the path from start point to end point
    return: transversalOrder vector with the connecting MazeNodes 
*/
std::vector<MazeNode> solveDFS(Maze &a_maze)
{
    //set maze node originVertex to first node of maze
    MazeNode *originVertex = a_maze.getFirstNode();

    vector<MazeNode> transversalOrder; //vector for the resulting transversal order
    stack<MazeNode*> vertexStack; //stack to hold MazeNodes as they are visited

    //mark first MazeNode as visited and push it onto stack and vector
    transversalOrder.push_back(*originVertex);
    vertexStack.push(originVertex);
    originVertex->setVisited();

    while (!vertexStack.empty()) //while the stack isn't empty
    {
        //mazeNode topVertex pointing to top of the stack
        MazeNode *topVertex = vertexStack.top();
        if(topVertex == a_maze.getLastNode())
        {
        	break;
		}
		
		//topVertex will check neighboring nodes and see if it can travel starting with North, then East, then South, then West (nesw)
		//CASE NORTH
		if(canTravel(topVertex->getDirectionNode(directions::NORTH)))
		{
			//push the node onto the vector
			transversalOrder.push_back(*(topVertex->getDirectionNode(directions::NORTH)));
			//push the node onto the stack
			vertexStack.push(topVertex->getDirectionNode(directions::NORTH));
			//set the node as visited
			topVertex->getDirectionNode(directions::NORTH)->setVisited();
		}
		//CASE EAST
		else if(canTravel(topVertex->getDirectionNode(directions::EAST)))
		{
			//push node onto the vector
			transversalOrder.push_back(*(topVertex->getDirectionNode(directions::EAST)));
			//push node onto the stack
			vertexStack.push(topVertex->getDirectionNode(directions::EAST));
			//set node as visited
			topVertex->getDirectionNode(directions::EAST)->setVisited();
		}
		//CASE SOUTH
		else if(canTravel(topVertex->getDirectionNode(directions::SOUTH)))
		{
			//push node onto the vector
			transversalOrder.push_back(*(topVertex->getDirectionNode(directions::SOUTH)));
			//push node onto the stack
			vertexStack.push(topVertex->getDirectionNode(directions::SOUTH));
			//set node as visited
			topVertex->getDirectionNode(directions::SOUTH)->setVisited();
		}
		//CASE WEST
		else if(canTravel(topVertex->getDirectionNode(directions::WEST)))
		{
			//push node onto the vector
			transversalOrder.push_back(*(topVertex->getDirectionNode(directions::WEST)));
			//push node onto stack
			vertexStack.push(topVertex->getDirectionNode(directions::WEST));
			//set node as visited
			topVertex->getDirectionNode(directions::WEST)->setVisited();
		}
		//if no nodes are available
		else
		{
			//pop the current node and test the next neighboring nodes
			transversalOrder.pop_back();
			vertexStack.pop();
		}
		//topVertex travels till it reaches the last node of the maze
    }
    return transversalOrder; //tranversalOrder will have the maze path after using DFS
}

/*
helper function for BFS algorithm. Checks to see if node has any unvisited neighbors.
somehow doesnt work for DFS algorithm
@param MazeNode to be checked to see if node can travel
@pre MazeNode can either go N,E,S,W
@post boolean value if MazeNode can travel
return true if MazeNode can travel, false if not
*/
bool unvisited_neighbors(MazeNode *node)
{
	//easier way to travel across the maze for the BFS algorithm
	for(directions::nesw dir = directions::NORTH; dir <= directions::WEST; dir = directions::nesw(dir + 1))
    {
    	if(canTravel(node->getDirectionNode(dir)))
        {
            return true;
        }
    }
    return false;
}

/*
function that uses Queue ADT to implement BFS Breadth First Search to find the shortest path from beginning to end of the maze.
Includes Unordered_map that stores MazeNodes which is used then to push nodes onto the vector
the vector is reversed so use reverse from algorithm library
@pre maze with an unordered vector
@param a_maze input
@post vector with an ordered path from beginning to end of the maze
*/
std::vector<MazeNode> solveBFS(Maze &a_maze)
{
  	std::vector<MazeNode> path;
  	std::queue<MazeNode*> queue;
  
  	//find the first node of the maze and set it visited
  	MazeNode *first_node = a_maze.getFirstNode();
  	first_node->setVisited();
  	//push it onto the queue
  	queue.push(first_node);
  
  	//unordered map to keep track of the parent nodes
  	unordered_map<MazeNode *, MazeNode *> parents;
  	
  	//travel node will search through the maze
  	MazeNode *travel_node;
  	
  	//boolean to determine if there are any unvisited neighbors
	bool neighborsNotVisited;
	
	//while there are still nodes on the queue to be checked
  	while(!queue.empty())
  	{
  		//set travel node as front of the queue
  		travel_node = queue.front();
    	queue.pop();
    	
    	//determines if travel node has any unvisited nieghbors
        neighborsNotVisited = unvisited_neighbors(travel_node);
        
        //if there are unvisited neighbors
    	if(neighborsNotVisited == true)
		{ 	
      		for(directions::nesw dir = directions::NORTH; dir <= directions::WEST; dir = directions::nesw(dir + 1))
          	{
            	if(canTravel(travel_node->getDirectionNode(dir)))
              	{
              		//set node as visited
                	travel_node->getDirectionNode(dir)->setVisited();
                	//set node coordinates onto the unordered map
                	parents[travel_node->getDirectionNode(dir)] = travel_node;
                	//push the node onto the queue to check for other nieghbors
                	queue.push(travel_node->getDirectionNode(dir));
              	}
              	//once the end of the maze is found break
              	if(queue.front() == a_maze.getLastNode())
              	{
                	break;
              	}
          	}
    	}
    	//no more nieghbors to visit so get a new value for travel node
    	neighborsNotVisited = false;
  	}
  	//construct the path vector by backtracking through parent nodes to beginning
  	MazeNode *reverse_node = a_maze.getLastNode();
  	//search the maze from last to first
  	while(reverse_node != a_maze.getFirstNode())
	{
    	path.push_back(*reverse_node);
    	reverse_node = parents[reverse_node];
  	}
  	//push first node of the maze onto the path
  	path.push_back(*a_maze.getFirstNode());
  	//now the path is from last to first order
	//reverse order of vector path
  	reverse(path.begin(),path.end());
  	return path;
}

/*
	implement dead-end filling algorithm
	@pre a_maze that needs all dead-ends walled off
	@post by using a queue, all dead-end locations can be found and walled off
	return a vector with clear path to the end of the maze
*/
std::vector<MazeNode> solveDEF(Maze &a_maze)
{
	vector<MazeNode> path;
	queue<MazeNode*> dead_ends;
	
	MazeNode* travel_node;
	
	//this loop finds all the dead-ends located on the maze and then adds it to the stack
	for(auto &search_node: a_maze.getNodes())
	{
		//if the smart pointer node finds a node surrounded by three walls and its not the first or last node, set it visited and push it onto the stack
		if(search_node.isWall() == false 
		&& getNumberOfWalls(&search_node) == 3 
		&& a_maze.getLastNode()->getStrPos() != search_node.getStrPos() 
		&& a_maze.getFirstNode()->getStrPos() != search_node.getStrPos()) 
		{
			//push dead end onto the stack
			dead_ends.push(a_maze.contains(search_node.getPos()));
			//set dead end node to visited
			a_maze.contains(search_node.getPos())->setVisited();
		}
	}
	
	travel_node = dead_ends.front();
	
	//now that the dead ends are found, go through the stack and wall them off while looking for new dead ends that might be created
	while(!dead_ends.empty())
	{
		travel_node = dead_ends.front();
		dead_ends.pop();
		
		//set wall on travel_node
		travel_node->setWall();
		
		//starting from North now that the dead_end has been walled up, check if there is a new dead end
		if(getNumberOfWalls(travel_node->getDirectionNode(directions::NORTH)) == 3 && canTravel(travel_node->getDirectionNode(directions::NORTH)))
		{
			//push the node before the dead end is now a dead end
			dead_ends.push(travel_node->getDirectionNode(directions::NORTH));
		}
		//east
		if(getNumberOfWalls(travel_node->getDirectionNode(directions::EAST)) == 3 && canTravel(travel_node->getDirectionNode(directions::EAST)))
		{
			dead_ends.push(travel_node->getDirectionNode(directions::EAST));
		}
		//south
		if(getNumberOfWalls(travel_node->getDirectionNode(directions::SOUTH)) == 3 && canTravel(travel_node->getDirectionNode(directions::SOUTH)))
		{
			dead_ends.push(travel_node->getDirectionNode(directions::SOUTH));
		}
		//west
		if(getNumberOfWalls(travel_node->getDirectionNode(directions::WEST)) == 3 && canTravel(travel_node->getDirectionNode(directions::WEST)))
		{
			dead_ends.push(travel_node->getDirectionNode(directions::WEST));
		}
	}
	
	//once dead_ends stack is empty, now the vector can safely find the path to the end of the maze
	path = solveDFS(a_maze);
	return path;
}

//no custom algorithm, just DFS algorithm
std::vector<MazeNode> solveCustom(Maze &a_maze)
{
	return solveDFS(a_maze);
}
} // namespace solutions
