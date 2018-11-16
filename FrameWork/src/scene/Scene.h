#ifndef SCENE_H
#define SCENE_H

//scene is not automatically released, you have to release it yourself
class Scene
{
private:


public:
	Scene();
	~Scene();
	virtual void Create() = 0;
	virtual void Update(float dt) = 0;
	virtual void Release() = 0;

};

#endif