#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include"../drawable/Animation.h"
#include <map>
#include <string>

class Condition
{
public:
	enum ConditionType { Equal, Greater, Less };

private:
	std::string conditionName;
	float value;
	float comparedValue;
	ConditionType type;

	friend class StateManager;
public:
	
	bool IsTrue(ConditionType type,float comparedValue)
	{
		switch (type)
		{
		case Equal:
			return value == comparedValue;
			break;
		case Greater:
			return value > comparedValue;
			break;
		case Less:
			return value < comparedValue;
			break;
		}
	}
	Condition()
	{

	}

	Condition(const std::string &name, Condition::ConditionType type, float comparedValue)
	{
		conditionName = name;
		this->type = type;
		this->comparedValue = comparedValue;
	}

};

class ConditionAnimation
{
private:
	friend class StateManager;

	Animation *nextAnimation;
	std::vector<Condition> conditions;

};

class StateManager
{
private:

	std::map<std::string, Condition> conditionMap;
	std::multimap<Animation*, ConditionAnimation> aniMultiMap;
	Animation* currentAnimation;


public:
	StateManager();
	~StateManager();

	void AddCondition(const std::string &conditionName);

	void Add(Animation *ani1, Animation *ani2, const std::initializer_list<Condition>& conditions);

	void SetCurrentAnimation(Animation *ani);
	void Set(const std::string &conditionName, float value);

	Animation* GetTargetAnimation();


};





#endif