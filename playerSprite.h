#ifndef PLAYERSPRITE__H
#define PLAYERSPRITE__H
#include <string>
#include <vector>
#include "multisprite.h"
#include "ioManager.h"
#include "collisionStrategy.h"

class PlayerSprite : public MultiSprite {
public:
  PlayerSprite(const std::string&);
  PlayerSprite(const PlayerSprite&);
  virtual ~PlayerSprite() { } 
  void updateVelocity();
  void setKeyX(int, int);
  void setKeyY(int, int);
  float getXPos();
  float getYPos();
  int testFooting(bool *, int);
  void adjustFooting(bool *, int);
  void disableGravity(){ gravity = false; };
  void enableGravity(){ gravity = true; };
  bool collidedWith(const Drawable* d) const{
	return collider.execute(*this, *d);
  }
protected:

  float getVelX();
  float getVelY();
  const int maxXSpeed;
  const int maxYSpeed;
  const IOManager& io;
  
  //approximate pixel positions of left and right foot at standing position
  //numbers are offsets from the frame sprite position
  int leftFoot[2];
  int rightFoot[2];
  //Flags denote which keys are pressed and therefore which direction to accelerate the player sprite.
  //If the flag is not set, the player will lose momentum until it no longer moves
  //0 - key not pressed
  //1 - positive direction
  //2 - negative direction
  bool gravity;
  int footSupport;
  PerPixelCollisionStrategy collider;
  int keyPressedX[2];
  int keyPressedY[2];
  virtual void advanceFrame(Uint32 ticks);
};
#endif
