//a class to easily manage the animation of an object 
#ifndef ANIMATION_H
#define ANIMATION_H

#include "GameObject.h"
#include "TextureRegion.h"
#include <vector>

class Animation: public GameObject
{
private:
	std::vector<TextureRegion*> _Regions;
	int _Index;				// Current texture index
	int _Count;				// Number of sprites
	float _FrameInterval;	//the duration of a frame
	float _StateTime;		//current time in one frame


public:
	Animation();
	Animation(const std::vector<TextureRegion> &regions, float frameInterval);
	~Animation();
	Animation(const Animation &ani);
	Animation& operator=(const Animation &ani);
	

	TextureRegion* GetKeyAnimation() const;

	//Go to next animation frame
	//If isSameDirection = 0, the animation will be rendered as the opposite direction  
	//If isSameDirection = 1, the animation will be rendered as the same direction  
	//Note that the animation will be automatically returned to its original direction in the next animation frame before checking isSameDirection
	//Set isSameDirection = -1 to disable the effect
	TextureRegion* Next(float deltaTime/*, int isSameDirection*/);
	void Reset();
	void SetFrameInterval(float frameInterval);

	//add texture region. Note that this method create a new texture region to add into animation. 
	//so changing the textureregion after using this method doesn't affect animation.
	void AddRegion(const TextureRegion &region);
	
	void AddRegion(const std::vector<TextureRegion> &region);

	int GetIndex();

	void SetIndex(int index);

};

#endif