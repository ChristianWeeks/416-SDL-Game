#ifndef TWOWAYSPRITE_H
#define TWOWAYSPRITE_H
#include "multisprite.h"

class TwoWaySprite : public MultiSprite {
public:
	TwoWaySprite(const std::string&);
	TwoWaySprite(const TwoWaySprite&);
private:
	void advanceFrame(Uint32 ticks);


};
#endif
