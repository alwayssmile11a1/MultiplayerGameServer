#include "StateManager.h"



StateManager::StateManager()
{
}


StateManager::~StateManager()
{
}

void StateManager::AddCondition(const std::string &conditionName)
{
	Condition condition;
	conditionMap[conditionName] = condition;
}

void StateManager::Add(Animation *ani1, Animation *ani2,const std::initializer_list<Condition>& conditions)
{
	//Condition condition;
	//condition.conditionName = conditionName;
	//condition.type = type;
	//condition.nextAnimation = ani2;
	//condition.comparedValue = comparedValue;
	ConditionAnimation conditionAni;
	conditionAni.conditions = conditions;
	conditionAni.nextAnimation = ani2;
	aniMultiMap.insert(std::multimap< Animation*, ConditionAnimation >::value_type(ani1, conditionAni));
}

void StateManager::SetCurrentAnimation(Animation *ani)
{
	currentAnimation = ani;
}

void StateManager::Set(const std::string &conditionName, float value)
{
	// Attempt to find and return a map using provided name, else return nullptr
	std::map<std::string, Condition>::iterator it = conditionMap.find(conditionName);

	if (it != conditionMap.end())
	{
		it->second.value = value;
	}
}

Animation* StateManager::GetTargetAnimation()
{
	//find all pair that has the key "currentAnimation"
	std::pair <std::multimap<Animation*, ConditionAnimation>::iterator, std::multimap<Animation*, ConditionAnimation>::iterator> ret;
	ret = aniMultiMap.equal_range(currentAnimation);

	//loop through these keys
	for (std::multimap<Animation*, ConditionAnimation>::iterator itAni = ret.first; itAni != ret.second; ++itAni)
	{
		bool isConditionTrue = true;
		//loop through all condition 
		for (std::vector<Condition>::iterator itCon = itAni->second.conditions.begin(); itCon != itAni->second.conditions.end(); ++itCon)
		{
			if (!conditionMap[(*itCon).conditionName].IsTrue((*itCon).type, (*itCon).comparedValue))
			{
				isConditionTrue = false;
			}
		}

		if (isConditionTrue)
		{
			currentAnimation = itAni->second.nextAnimation;
			break;
		}


	}



	return currentAnimation;
}