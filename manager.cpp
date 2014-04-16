#include <iostream>
#include <string>
#include <iomanip>
#include "twowaysprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "playerSprite.h"
#include "gamedata.h"
#include "manager.h"
#include "sound.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

/*
	CONSTRUCTOR INSTANTIATES ALL MANAGES ALL SPRITES AND GAMEDATA
*/
Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  viewport( Viewport::getInstance() ),
  world("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  HUD(),
  drawHud( Gamedata::getInstance().getXmlBool("hudShowAtStart") ),
  sprites(),
  foreground(world.getForeground()),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance().getXmlStr("screenTitle") ),
  player( new PlayerSprite("player"))
  //collider( new PerPixelCollisionStrategy)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);

	//collider = new PerPixelCollisionStrategy;
	sprites.reserve(7);
	sprites.push_back( player );
	sprites.push_back( new TwoWaySprite("evilSpiritTwoWay"));
	sprites.push_back( new TwoWaySprite("evilSpiritTwoWay"));
	sprites.push_back( new TwoWaySprite("evilSpiritTwoWay"));
	sprites.push_back( new MultiSprite("evilSpirit"));
	sprites.push_back( new MultiSprite("evilSpirit"));
	sprites.push_back( new MultiSprite("evilSpirit"));


	viewport.setObjectToTrack(sprites[0]);
}

/* 
	CHECKS FOR COLLISIONS
*/
bool Manager::checkForCollisions() const {
  std::vector<Drawable*>::const_iterator sprite = sprites.begin() + 1;
  while(sprite != sprites.end() ){
    if ( player->collidedWith(*sprite) ) return true;
    ++sprite;
  }
  return false;
}

/*
	DRAWS ITEMS TO THE SCREEN
*/
void Manager::draw() const {
  world.draw();
  viewport.draw();
  for(unsigned int i = 0; i < sprites.size(); ++i)
		sprites[i]->draw();
  io.printMessageCenteredAt(TITLE, 10);
  if(drawHud)
  	HUD.draw();
  player->draw();
  player->updateVelocity();
  std::stringstream strm;
  int playerX = static_cast<int>(player->getXPos());
  int playerY = static_cast<int>(player->getYPos());
  strm << "Player at: (" << playerX << ", " << playerY << ")";
  io.printMessageAt(strm.str(), 320, 10);

  if ( checkForCollisions() ) {
    io.printMessageAt("*** Oops ***, collision!", 320, 60);
    io.printMessageAt("***", playerX-viewport.X(), playerY-viewport.Y()-10);
    io.printMessageAt("***", playerX-viewport.X(), playerY-viewport.Y()+20);
  }
  else {
    io.printMessageAt("No Collision.", 320, 60);
  }
 /* world.draw();
  viewport.draw();
  for(unsigned int i = 0; i < sprites.size(); ++i)
		sprites[i]->draw();
  io.printMessageCenteredAt(TITLE, 10);
  if(drawHud)
  		HUD.draw();
  player->draw();
  player->updateVelocity();
  SDL_Flip(screen);*/
  SDL_Flip(screen);
}

/*
	WRITES FRAMES TO FILE
*/
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "video/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
} 	

/*
	UPDATES THE GAME CLOCK, KEEPS TRACK OF TIME, VIEWPORT LOCATION
*/
void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  for(unsigned int i = 0; i < sprites.size(); ++i)
		sprites[i]->update(ticks);
  if ( makeVideo && frameCount < frameMax ) {
		makeFrame();
  }

  world.update();
  viewport.update();
  //player->updateVelocity();
}

/*
	RUNS THE EVENT LOOP
*/
void Manager::play() {
  SDL_Event event;
  SDLSound sound;
  bool done = false;
  bool keyCatch = false;
  while ( not done ) {


    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) {
			switch(event.key.keysym.sym){
				case SDLK_LEFT:	player->setKeyX(1, 0); break; 
				case SDLK_RIGHT: player->setKeyX(0, 0); break;
				case SDLK_UP:	player->setKeyY(1, 0); break; 
				case SDLK_DOWN:	player->setKeyY(0, 0); break;
				
				default: break;
			}
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
		//Player1 movements
		if(keystate[SDLK_UP]){
			keyCatch = true;
			player->setKeyY(1, 1);
		}
		if(keystate[SDLK_DOWN]){
			keyCatch = true;
			player->setKeyY(0, 1);
		}
		if(keystate[SDLK_LEFT]){
			keyCatch = true;
			player->setKeyX(1, 1);	
		}
		if(keystate[SDLK_RIGHT]){
			keyCatch = true;
			player->setKeyX(0, 1);
		}
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
		std::cout << "Quitting Game\n";
		done = true;
        break;
      }
      if(keystate[SDLK_SPACE] && !keyCatch){
		keyCatch = true;
		sound.toggleMusic();
      }
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        currentSprite = (currentSprite+1) % sprites.size();
        viewport.setObjectToTrack(sprites[currentSprite]);
      }
      if (keystate[SDLK_s] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
      if (keystate[SDLK_p] && !keyCatch) {
        keyCatch = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
      }
      if (keystate[SDLK_F4] && !makeVideo) {
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }
		if (keystate[SDLK_F1] && !keyCatch){
			keyCatch = true;
			drawHud = !drawHud;
		}
    }

    draw();
    update();
  }
}
