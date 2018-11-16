#include "QuadTree.h"

QuadTree::QuadTree()
{
	MAX_Bodies = 20;
	MAX_Levels = 5;
	_Level = 0;
}

QuadTree::QuadTree(int level, Vector2 size, Vector2 position)
{
	MAX_Bodies = 20;
	MAX_Levels = 5;
	_Level = level;
	_Size = size;
	_Position = Vector2(position.x - size.x / 2, position.y - size.y / 2);
	_ListBodies.clear();
	_SubQuadTrees.clear();
}


QuadTree::~QuadTree()
{
}


void QuadTree::Clear()
{
	_ListBodies.clear();

	//if (_SubQuadTrees.empty() == false)
	//{
	for (int i = 0; i < _SubQuadTrees.size(); i++)
	{
		if (_SubQuadTrees[i] != NULL)
		{
			_SubQuadTrees[i]->Clear();
			_SubQuadTrees[i] = NULL;
		}
	}
	//}
}

void QuadTree::Split()
{
	int subWidth = (int)(_Size.x / 2);
	int subHeight = (int)(_Size.y / 2);
	int PositionX = (int)(_Position.x);
	int PositionY = (int)(_Position.y);

	Vector2 subSize;
	subSize.Set((float)subWidth, (float)subHeight);

	Vector2 subPos0;
	Vector2 subPos1;
	Vector2 subPos2;
	Vector2 subPos3;
	subPos0.Set((float)(PositionX + subWidth), (float)PositionY);
	subPos1.Set((float)PositionX, (float)PositionY);
	subPos2.Set((float)PositionX, (float)(PositionY + subHeight));
	subPos3.Set((float)(PositionX + subWidth), (float)(PositionY + subHeight));


	_SubQuadTrees.push_back(new QuadTree(_Level + 1, subSize, subPos0));
	_SubQuadTrees.push_back(new QuadTree(_Level + 1, subSize, subPos1));
	_SubQuadTrees.push_back(new QuadTree(_Level + 1, subSize, subPos2));
	_SubQuadTrees.push_back(new QuadTree(_Level + 1, subSize, subPos3));
}

int QuadTree::getIndexFitted(Body *body)
{
	int index = -1;

	float verticalMidpoint = _Position.x + (_Size.x / 2);
	float horizontalMidpoint = _Position.y + (_Size.y / 2);

	Vector2 Position = body->GetPosition();
	Vector2 Size = body->GetSize();

	Position.Set(Position.x - Size.x / 2, Position.y - Size.y / 2);

	bool bottomQuadrant = (Position.y < horizontalMidpoint && Position.y + Size.y < horizontalMidpoint);
	bool topQuadrant = (Position.y > horizontalMidpoint);
	bool leftQuadrant = (Position.x < verticalMidpoint && Position.x + Size.x < verticalMidpoint);
	bool rightQuadrant = (Position.x > verticalMidpoint);

	if (topQuadrant && leftQuadrant)
		index = 1;
	else if (topQuadrant && rightQuadrant)
		index = 0;
	else if (bottomQuadrant && leftQuadrant)
		index = 2;
	else if (bottomQuadrant && rightQuadrant)
		index = 3;

	return index;
}

bool QuadTree::getIndexNotFitted(QuadTree *quadrant, Body *body)
{
	Vector2 Position = body->GetPosition();
	Vector2 Size = body->GetSize();

	Position.Set(Position.x - Size.x / 2, Position.y - Size.y / 2);

	bool CheckX = (Position.x + Size.x > quadrant->_Position.x) && (Position.x < quadrant->_Position.x + quadrant->_Size.x);

	bool CheckY = (Position.y + Size.y > quadrant->_Position.y) && (Position.y < quadrant->_Position.y + quadrant->_Size.y);

	return (CheckX && CheckY);
}

void QuadTree::Insert(Body *body)
{
	if (_SubQuadTrees.empty() == false)
	{
		int index = getIndexFitted(body);

		if (index != -1)
		{
			_SubQuadTrees[index]->Insert(body);

			return;
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (getIndexNotFitted(_SubQuadTrees[i], body))
					_SubQuadTrees[i]->Insert(body);
			}

			return;
		}
	}

	_ListBodies.push_back(body);

	if (_ListBodies.size() > MAX_Bodies && _Level < MAX_Levels)
	{
		if (_SubQuadTrees.empty() == true)
		{
			Split();
		}

		for (int i = 0; i < _ListBodies.size(); i++)
		{
			int index = getIndexFitted(_ListBodies[i]);

			if (index != -1)
			{
				_SubQuadTrees[index]->Insert(_ListBodies[i]);
				_ListBodies.erase(_ListBodies.begin() + i);
				i--;
			}
			else
			{
				for (int x = 0; x < 3; x++)
				{
					if (getIndexNotFitted(_SubQuadTrees[x], _ListBodies[i]))
						_SubQuadTrees[x]->Insert(_ListBodies[i]);
				}

				_ListBodies.erase(_ListBodies.begin() + i);
				i--;

			}
		}
	}
}

void QuadTree::Retrieve(std::vector<Body*> &returnBodies, Body *body)
{
	int index = getIndexFitted(body);
	if (index != -1 && _SubQuadTrees.empty() == false)
	{
		_SubQuadTrees[index]->Retrieve(returnBodies, body);
	}
	else if (index == -1 && _SubQuadTrees.empty() == false)
	{
		for (int i = 0; i < 3; i++)
		{
			if (getIndexNotFitted(_SubQuadTrees[i], body))
			{
				_SubQuadTrees[i]->Retrieve(returnBodies, body);
			}
		}
	}

	for (int i = 0; i < _ListBodies.size(); i++)
	{
		if (_ListBodies[i] != body)
		{
			returnBodies.push_back(_ListBodies[i]);
		}
	}

	return;
}

void QuadTree::Update(Vector2 size, Vector2 position)
{
	_Position = Vector2(position.x - size.x / 2, position.y - size.y / 2);
	_ListBodies.clear();
	_SubQuadTrees.clear();
}