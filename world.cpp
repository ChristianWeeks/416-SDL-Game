#include <iostream>
#include "world.h"
#include "frameFactory.h"



World::World(const std::string& name, int fact) : 
//arg1 (const std::string& name) corresponds to the tag in the xmlfile under which all of the world background info is held
  io( IOManager::getInstance() ),
  layers( Gamedata::getInstance().getXmlInt("backLayers") ),
  backLayers( FrameFactory::getInstance().getBackLayers(name) ),
  factor(fact),
    frameWidth( backLayers[0]->getWidth() ),
    worldWidth( Gamedata::getInstance().getXmlInt("worldWidth") ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{}

const Frame * World::getForeground() const{
	//Hardcorded for now...  Will figure out later how to dynamically denote the foreground layer
	return backLayers[3];
}

//updates the current viewport location
void World::update() {
  viewX = static_cast<int>(view.X());
  viewY = view.Y();
}

void World::draw() const {
  unsigned int viewWidth = Gamedata::getInstance().getXmlInt("viewWidth");
  unsigned int center = (worldWidth / 2); //- (viewWidth / 2);
  float perspectiveFactor;
  float compensateForViewWidth;
  for(unsigned int i = 0; i < layers; ++i){

	//everything should line up at half the width of the screen.
	perspectiveFactor = ((float)(i+1) / 4) - 1;
	if(viewX == 0)
		compensateForViewWidth = 0;
	else
		compensateForViewWidth = perspectiveFactor * viewWidth * (viewX / worldWidth);
	//std::cout << "perspectiveFactor: " << perspectiveFactor << std::endl;
	backLayers[i]->draw((viewX - perspectiveFactor*(center - viewX))+(perspectiveFactor*center) + (int)compensateForViewWidth, viewY, 0, 0);
  //frame->draw(viewX, viewY, 0, 0); 
  // backLayers[i]->draw(0, viewY, frameWidth-viewX, 0); 
  }
}

