#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "../src/world/Body.h"
#include "HanabiMath.h"

class QuadTree
{
private:
	//
	//Elements
	//

	//Max levels of the quadtree
	int MAX_Levels;
	//Max objects in 1 node of the quadtree
	int MAX_Bodies;
	//Current level of this node
	int _Level;
	//List of objects this node
	std::vector<Body*> _ListBodies;
	//Size of this node
	Vector2 _Size;
	//Position of this node
	Vector2 _Position;
	//List of children of this node
	std::vector<QuadTree*> _SubQuadTrees;

	//
	//Some varieties and methods but in private
	//

	//An int which returns to the children node's order number (0 to 3) of this node where the object belongs to if it can fit in the children node's size
	int getIndexFitted(Body *body);
	//A boolean which returns to True or False of the question "Does this node's area contain any parts of the object?" (use this in case of the object cannot fit in any children node)
	bool getIndexNotFitted(QuadTree *quadrant, Body *body);
	//A function which adds 4 children nodes into this node's vector<*QuadTree> _SubQuadTrees and provides level, size and position for the children nodes
	void Split();
public:
	//
	//Constructor & Destructor
	//
	QuadTree();
	QuadTree(int level, Vector2 size, Vector2 position);
	~QuadTree();

	//
	//Methods
	//

	//A function which delete this node's objects and it's children node
	void Clear();
	//A function which adds an object into the "right" node of the quadtree
	void Insert(Body *body);
	//A vector which contains a list of objects which could collide with the object
	void Retrieve(std::vector<Body*>  &returnBodies, Body *body);
	//A function to update the starting position and refesh the quadtree
	void Update(Vector2 size, Vector2 position);
};

#endif