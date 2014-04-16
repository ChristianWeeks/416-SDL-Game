#include "playerSprite.h"
#include "gamedata.h"

PlayerSprite::PlayerSprite(const std::string& name) : MultiSprite(name),
	maxXSpeed( Gamedata::getInstance().getXmlInt("playerMaxX") ),
	maxYSpeed( Gamedata::getInstance().getXmlInt("playerMaxY") ),
	io( IOManager::getInstance() ),
        collider()
{ setVelocity(Vector2f(0,0));
	keyPressedX[0] = 0;
	keyPressedX[1] = 0;
	keyPressedY[0] = 0;
	keyPressedY[1] = 0; 
}

PlayerSprite::PlayerSprite(const PlayerSprite& p) : MultiSprite(p),
	maxXSpeed( p.maxXSpeed ),
	maxYSpeed( p.maxYSpeed ),
	io( IOManager::getInstance() ),
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

void PlayerSprite::updateVelocity(){

	io.printMessageValueAt("Right Key Pressed: ", keyPressedX[0], 20, 200);
	io.printMessageValueAt("Left Key Pressed: ", keyPressedX[1], 20, 215);
	io.printMessageValueAt("Down Key Pressed:", keyPressedY[0], 20, 230);
	io.printMessageValueAt("Up Key Pressed: ", keyPressedY[1], 20, 245);
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
		if(keyPressedY[0] == 0 && keyPressedY[1] == 0)
			setVelY(0);
		//increase y velocity until it reaches max velocity
		//down
		if(keyPressedY[0] == 1 && getVelY() < maxYSpeed){
			setVelY(maxYSpeed);
		}
		//up
		if(keyPressedY[1] == 1 && getVelY() > -1 * maxYSpeed){
			setVelY(-1 * maxYSpeed);
		}
	}
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
