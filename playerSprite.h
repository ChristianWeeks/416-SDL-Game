#ifndef PLAYERSPRITE__H
#define PLAYERSPRITE__H
#include <string>
#include <vector>
#include "multisprite.h"
#include "aggroSprite.h"
#include "ioManager.h"
#include "collisionStrategy.h"
#include "collectible.h"

class PlayerSprite : public MultiSprite {
public:
  PlayerSprite(const std::string&);
  PlayerSprite(const PlayerSprite&);
  virtual ~PlayerSprite() { } 
  virtual void update(Uint32 ticks);
  void updateVelocity();
  void setKeyX(int, int);
  void setKeyY(int, int);
  float getXPos();
  float getYPos();
  int testFooting(bool *, int);
  float getHealth(){return health;}
  void setHealth(float newHealth){health = newHealth;}
  void adjustFooting(bool *, int);
  void disableGravity(){ gravity = false; };
  void enableGravity(){ gravity = true; };
  void pushSprite(AggroSprite * sprite) { spriteObservers.push_back(sprite);}
  void pushCollectible(collectible * item){ collectObservers.push_back(item);}
  void popCollected(){if(collectObservers.size() > 0) collectObservers.pop_back();}
  bool collidedWith(const Drawable* d) const{
	return collider.execute(*this, *d);
  }
  virtual void draw() const;

protected:



  float getVelX();
  float getVelY();
  float health;
  const int maxXSpeed;
  const int maxYSpeed;
  const IOManager& io;
  std::vector<AggroSprite*> spriteObservers;
  std::vector<collectible*> collectObservers;
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
