#include "playerSprite.h"
#include "gamedata.h"
#include "aaline.h"
#include "ioManager.h"

PlayerSprite::PlayerSprite(const std::string& name) : MultiSprite(name),
	health(100.0),
	maxXSpeed( Gamedata::getInstance().getXmlInt("playerMaxX") ),
	maxYSpeed( Gamedata::getInstance().getXmlInt("playerMaxY") ),
	io( IOManager::getInstance() ),
	spriteObservers(),
	collectObservers(),
	gravity(false),
	footSupport(0),
        collider()
{ 
	int width = getFrame()->getWidth();
	int height = getFrame()->getHeight();
	//Offset so character stands more on the center of his feet rather than his tip toes
	int heightOffset = height - 15;
	leftFoot[0] = heightOffset;
	//left foot is ~ 3/7ths the width of the sprite
	leftFoot[1] = (width * 3) / 7;
	rightFoot[0] = heightOffset;
	rightFoot[1] = (width * 4) / 7;
	setVelocity(Vector2f(0,0));
	keyPressedX[0] = 0;
	keyPressedX[1] = 0;
	keyPressedY[0] = 0;
	keyPressedY[1] = 0; 
}

PlayerSprite::PlayerSprite(const PlayerSprite& p) : MultiSprite(p),
	health(100.0),
	maxXSpeed( p.maxXSpeed ),
	maxYSpeed( p.maxYSpeed ),
	io( IOManager::getInstance() ),
	spriteObservers(),
	collectObservers(),
	gravity(false),
	footSupport(0),
        collider()
{ setVelocity(Vector2f(0,0));
	keyPressedX[0] = 0;
	keyPressedX[1] = 0;
	keyPressedY[0] = 0;
	keyPressedY[1] = 0;  
}

void PlayerSprite::advanceFrame(Uint32 ticks) {
/*	timeSinceLastFrame += ticks;
	Vector2f currVelocity = getVelocity();
	//If moving right, use first frame
	if(currVelocity[0] > 0)
		currentFrame = 0;
	//else use second frame
	else if (currVelocity[0] < 0)
		currentFrame = 1;*/

	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		//if idle, display the idle sprite
		//std::cout << getVelX() << std::endl;
		if(getVelX() == 0)
			currentFrame = 12;
		//if moving right, display right run sprites
		else if(getVelX() > 0){
			if( currentFrame < 5)
				currentFrame += 1;
			else
				currentFrame = 0;
		}
		//if moving left, display left run sprites
		else if(getVelX() < 0){
			if(currentFrame <= 11 && currentFrame > 6)
				currentFrame -= 1;
			else
				currentFrame = 11;
		}		
		timeSinceLastFrame = 0;
	}

}

int PlayerSprite::testFooting(bool * collisionMap, int worldWidth){

	//is the player in the ground?
	bool leftFootPos = collisionMap[(((int)Y() + leftFoot[0])*worldWidth) + (int)X() + leftFoot[1]];
	bool rightFootPos = collisionMap[(((int)Y() + rightFoot[0])*worldWidth) + (int)X() + rightFoot[1]];
	//if either foot is in the collision map, they must be adjusted up
	if(rightFootPos || leftFootPos){
		footSupport = 1;
		return 1;
	}
	//is the player in the air?	
	bool isLeftFootSupported = collisionMap[(((int)Y() + leftFoot[0] + 1)*worldWidth) + (int)X() + leftFoot[1] + 1];
	bool isRightFootSupported = collisionMap[(((int)Y() + rightFoot[0] + 1)*worldWidth) + (int)X() + rightFoot[1] + 1];
	//if neither foot is on the ground, gravity must be enacted
	if(!isLeftFootSupported && !isRightFootSupported){
		footSupport = 2;   
		return 2;
	}
	footSupport = 0;
	return 0;
}
 
void PlayerSprite::adjustFooting(bool * collisionMap, int worldWidth){
	Vector2f newPos;

	while(testFooting(collisionMap, worldWidth) == 1){
		newPos = getPosition();
		newPos[1] = newPos[1] - 1 ;
		setPosition(newPos);
	}
	
}

void PlayerSprite::update(Uint32 ticks) {
  
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  updateVelocity();

  //notifying mobs and items of new position
	for(unsigned int i = 0; i < spriteObservers.size(); ++i)
		spriteObservers[i]->notify(X(), Y());
	for(unsigned int i = 0; i < collectObservers.size(); ++i)
		collectObservers[i]->notify(X(), Y());


}

void PlayerSprite::updateVelocity(){

/*	io.printMessageValueAt("Right Key Pressed: ", keyPressedX[0], 20, 200);
	io.printMessageValueAt("Left Key Pressed: ", keyPressedX[1], 20, 215);
	io.printMessageValueAt("Down Key Pressed:", keyPressedY[0], 20, 230);
	io.printMessageValueAt("Up Key Pressed: ", keyPressedY[1], 20, 245);*/
	io.printMessageValueAt("Gravity: ", (int)gravity, 20, 260);
	io.printMessageValueAt("FootSupport: ", footSupport, 20, 275);
	//stopping character if he hits the edge of the world and is moving that direction
	if((getXPos() >= worldWidth - frameWidth && keyPressedX[0]) || (getXPos() <= 0 && keyPressedX[1])){
		setVelX(0);
	}
	else{
		
		//if keys aren't pressed, bring sprite to a stop
		if(keyPressedX[0] == 0 && keyPressedX[1] == 0){
			setVelX(0);
		}
		//increase x velocity until it reaches max velocity
		//right direction
		if(keyPressedX[0] == 1 && getVelX() < maxXSpeed){
			//acceleration rate is 1.2, may become a hardcoded constant later
			setVelX(maxXSpeed);
		}
		//left direction
		if(keyPressedX[1] == 1 && getVelX() > -1*maxXSpeed){
			setVelX(-1 * maxXSpeed);
		}
	}

	if((getYPos() >= worldHeight - frameHeight && keyPressedY[0]) || (getYPos() <= 0 && keyPressedY[1])){
		setVelY(0);
	}
	else{
		if(keyPressedY[1] == 0 && footSupport == 0)
			setVelY(0);
	}
	if(gravity) setVelY(velocityY() + 100);
}

void PlayerSprite::draw() const {
	Uint32 x = static_cast<Uint32>(X());
	Uint32 y = static_cast<Uint32>(Y());
	frames[currentFrame]->draw(x, y);
	Draw_AALine(IOManager::getInstance().getScreen(), 30, 450, 30+(int)health, 450, 10.0f, 0xff, 0xff, 0xff, 0xff);
}

void PlayerSprite::setKeyX(int index, int value){
	keyPressedX[index] = value;
}

void PlayerSprite::setKeyY(int index, int value){
	keyPressedY[index] = value;
}

float PlayerSprite::getXPos(){
	Vector2f temp = getPosition();
	return temp[0];
}

float PlayerSprite::getYPos(){
	Vector2f temp = getPosition();
	return temp[1];
}

float PlayerSprite::getVelX(){
	Vector2f temp = getVelocity();
	return temp[0];
}

float PlayerSprite::getVelY(){
	Vector2f temp = getVelocity();
	return temp[1];
}
