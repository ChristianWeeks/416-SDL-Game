#include <iostream>
#include <string>
#include <iomanip>
#include "twowaysprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "playerSprite.h"
#include "aggroSprite.h"
#include "effectSprite.h"
#include "collectible.h"
#include "gamedata.h"
#include "manager.h"
#include "sound.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  delete collisionMap;
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

/*
	CONSTRUCTOR INSTANTIATES AND MANAGES ALL SPRITES AND GAMEDATA
*/
Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  collected(false),
  screen( io.getScreen() ),
  viewport( Viewport::getInstance() ),
  world("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  HUD(),
  drawHud( Gamedata::getInstance().getXmlBool("hudShowAtStart") ),
  sprites(),
  foreground(world.getForeground()),
  collisionMap(),
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
	//creating the collision map
	createCollisionMap();	
	//collider = new PerPixelCollisionStrategy;
	AggroSprite * currSprite;
	sprites.reserve(12);
	sprites.push_back( player );
	collectible * egg = new collectible("egg");

	sprites.push_back(egg);
	sprites.push_back( new EffectSprite("shiningLight", *egg));

	sprites.push_back( new TwoWaySprite("evilSpiritTwoWay"));
	sprites.push_back( new TwoWaySprite("evilSpiritTwoWay"));
	sprites.push_back( new TwoWaySprite("evilSpiritTwoWay"));



	currSprite = new AggroSprite("evilSpirit", 1000, 200);
	sprites.push_back(currSprite);
	player->pushSprite(currSprite);

	currSprite = new AggroSprite("evilSpirit", 2300, 250);
	sprites.push_back(currSprite);
	player->pushSprite(currSprite);

	currSprite = new AggroSprite("evilSpirit", 2550, 300);
	sprites.push_back(currSprite);
	player->pushSprite(currSprite);

	currSprite = new AggroSprite("evilSpirit", 4550, 300);
	sprites.push_back(currSprite);
	player->pushSprite(currSprite);

	currSprite = new AggroSprite("evilSpirit", 4750, 350);
	sprites.push_back(currSprite);
	player->pushSprite(currSprite);

	currSprite = new AggroSprite("evilSpirit", 4900, 300);
	sprites.push_back(currSprite);
	player->pushSprite(currSprite);

	viewport.setObjectToTrack(sprites[0]);
}

/* 
	CHECKS FOR COLLISIONS
*/
void Manager::checkForCollisions(Uint32 ticks) {
	std::vector<Drawable*>::const_iterator sprite = sprites.begin() + 1;
	if(!collected){
		collectible * egg = dynamic_cast<collectible*>(*sprite);
		if(egg && player->collidedWith(*sprite)){
			egg->updateState();
			player->pushCollectible(egg);
			collected = true;
		}
	}
	sprite += 2;
  while(sprite != sprites.end() ){
    if ( player->collidedWith(*sprite) ) 
	player->setHealth(player->getHealth() - (static_cast<float>(ticks) * 0.005));
    ++sprite;
  }
}

/*
	DRAWS ITEMS TO THE SCREEN
*/
void Manager::draw() {
  world.draw();
//  viewport.draw();
  for(unsigned int i = 0; i < sprites.size(); ++i)
		sprites[i]->draw();
  //io.printMessageCenteredAt(TITLE, 10);
  if(drawHud)
  	HUD.draw();
  player->draw();

/*  std::stringstream strm, strm2;
  int playerX = static_cast<int>(player->getXPos());
  int playerY = static_cast<int>(player->getYPos());
  strm << "Player at: (" << playerX << ", " << playerY << ")";
  io.printMessageAt(strm.str(), 320, 10);
  strm2 << "Viewport at: (" << viewport.X() << ", " << viewport.Y() << ")";
  io.printMessageAt(strm2.str(), 320, 25);
  if ( checkForCollisions() ) {
    io.printMessageAt("*** Oops ***, collision!", 320, 60);
    io.printMessageAt("***", playerX-viewport.X(), playerY-viewport.Y()-10);
    io.printMessageAt("***", playerX-viewport.X(), playerY-viewport.Y()+20);
  }
  else {
    io.printMessageAt("No Collision.", 320, 60);
  }*/
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
  checkForCollisions(ticks);
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
 * TESTS IF A PIXEL IS VISIBLE
 */ 
bool Manager::isVisible(Uint32 pixel, SDL_Surface *surface) const {
	Uint32 temp;
	SDL_PixelFormat* format = surface->format;
	//performing same routine as isvisible method in collisionstrategy to create map	
	if(format->BitsPerPixel== 32){
		temp=pixel&format->Amask; /* isolate alpha component */
		temp=temp>>format->Ashift;/* shift it down to 8-bit */
		temp=temp<<format->Aloss; /* expand to a full 8-bit number */
		if(temp == 0) return false;
	}
	if(pixel == surface->format->colorkey) return false;
	return true;
}

/*
 * Creates the collision map of the foreground for the player to walk on 
 */
void Manager::createCollisionMap(){
	SDL_Surface* foreSurface = foreground->getSurface();
	SDL_LockSurface(foreSurface);
	Uint32 *pixels = static_cast<Uint32 *>(foreSurface->pixels);
	unsigned pixel;
	//initializing collision map
	int width = foreground->getWidth(); 
	int height = foreground->getHeight();
	collisionMap = new bool[width * height];
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			pixel = pixels[(i * width) + j];
			collisionMap[(i*width) + j] = isVisible(pixel, foreSurface);
		}
	}
	SDL_UnlockSurface(foreSurface);
}


bool Manager::checkVictory(){
	if(collected && (player->X() < 300))
		return true;
	return false;
}

bool Manager::checkDefeat() {
	if(player->getHealth() <= 0)
		return true;
	return false;
}	

void Manager::reset(){
	collected = false;
	collectible * temp = dynamic_cast<collectible*>(sprites[1]);
	if(temp) temp->resetState();
	sprites[1]->X(Gamedata::getInstance().getXmlInt("eggX"));
	sprites[1]->Y(Gamedata::getInstance().getXmlInt("eggY"));
	player->X(Gamedata::getInstance().getXmlInt("playerSrcX"));
	player->Y(Gamedata::getInstance().getXmlInt("playerSrcY"));
	player->popCollected();

}
	
/*
	RUNS THE EVENT LOOP
*/
void Manager::play() {
  SDL_Event event;
  SDLSound sound;
  bool done = false;
  bool keyCatch = false;
  int footStatus = 0;
  while ( not done ) {


	footStatus = player->testFooting(collisionMap, foreground->getWidth());
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
			if(footStatus == 0) {
				player->velocityY(-1100);
			keyCatch = true;			}
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
	if( keystate[SDLK_r] && !keyCatch){
		keyCatch = true;
		reset();
	}
    }
	if(footStatus == 1){
		player->disableGravity();
	   	player->adjustFooting(collisionMap, foreground->getWidth()); 
	}
	else if(footStatus == 2) player->enableGravity();
	else player->disableGravity();

	if(checkDefeat()){
		io.printMessageAt("YOU LOSE!!!", 300, 300);
		SDL_Flip(screen);
	}
	else if(checkVictory()){
		io.printMessageAt("YOU WIN!!!", 300, 300);
		SDL_Flip(screen);
	}	
	else{
    		draw();
    		update();
	}
  }
}
