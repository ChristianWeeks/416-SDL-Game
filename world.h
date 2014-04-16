#include <string>
#include <vector>
#include "ioManager.h"
#include "frame.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  // ~World() { ; } // The frame will be deleted by the FrameFactory
  void update();
  void draw() const;
  const Frame * getForeground() const;
private:
  const IOManager& io;
  unsigned int layers;
	//array of frames containing each layer of the background
  const std::vector<Frame *> backLayers;
  int factor;

  unsigned frameWidth;
  unsigned worldWidth;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
