#include "twowaysprite.h"

TwoWaySprite::TwoWaySprite( const std::string& name) : MultiSprite(name)
{ }

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) : MultiSprite(s)
  { }


void TwoWaySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	Vector2f currVelocity = getVelocity();
	//If moving right, use first frame
	if(currVelocity[0] >= 0)
		currentFrame = 0;
	//else use second frame
	else
		currentFrame = 1;

}
