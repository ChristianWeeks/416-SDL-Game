#include <SDL.h>
#include <vector>
#include "playerSprite.h"
#include "ioManager.h"
#include "viewport.h"
#include "world.h"
#include "hud.h"


class Manager {
public:
  Manager ();
  ~Manager ();
  void play();


private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  //screen must be initialized before viewport & world
  SDL_Surface * const screen;

  
  //viewport must be initialized before world
  Viewport &viewport;
  World world;
  hud HUD;
  bool drawHud;
  std::vector<Drawable*> sprites;
  //foreground must be declared after world
  const Frame * foreground;
  
  //Collision map of the foreground
  bool *collisionMap;
  int currentSprite;
  bool makeVideo;
  int frameCount;

  std::string username;
  int frameMax;
  const std::string TITLE;

  //Player sprite
  PlayerSprite* player;
 // CollisionStrategy * collider;

  void makeFrame();
  void draw() const;
  void update();
  bool checkForCollisions() const;
  bool isVisible(Uint32, SDL_Surface *) const;
  void createCollisionMap();


  Manager(const Manager&);
  Manager& operator=(const Manager&);

};
