#include "hud.h"

hud::hud() :
	width( Gamedata::getInstance().getXmlInt("hudWidth")),
	height( Gamedata::getInstance().getXmlInt("hudHeight")),
	pos(
		Gamedata::getInstance().getXmlInt("hudX"),
		Gamedata::getInstance().getXmlInt("hudY")
		),
	io( IOManager::getInstance() ),
	screen( io.getScreen() ),
	clock( Clock::getInstance() ),
	viewport( Viewport::getInstance() ),
	borderColor( Gamedata::getInstance().getXmlInt("hudBorderColor"))
{}

void hud::draw() const {
	//transparant background
	Draw_AALine(screen, pos[0], pos[1]+height/2,
						pos[0]+width, pos[1]+height/2,
						height, 0xff, 0xff, 0xff, 0xff/3);
	//top border
	Draw_AALine(screen, pos[0], pos[1],
						pos[0]+width, pos[1], 1.0f, borderColor);
	//bottom border
	Draw_AALine(screen, pos[0], pos[1]+height,
						pos[0]+width, pos[1]+height, 1.0f, borderColor);
	//right side
	Draw_AALine(screen, pos[0]+width, pos[1],
						pos[0]+width, pos[1]+height, 1.0f, borderColor);
	//left side
	Draw_AALine(screen, pos[0], pos[1],
						pos[0], pos[1]+height, 1.0f, borderColor);
	io.printMessageValueAt("Seconds: ", clock.getSeconds(), pos[0]+10, pos[1]+30);
	io.printMessageValueAt("fps: ", clock.getFps(), pos[0]+10, pos[1]+50);	
	io.printMessageAt("Press r to reset game", pos[0]+10, pos[1]+70);
	io.printMessageAt("Press Space to turn off music", pos[0]+10, pos[1]+90);
	io.printMessageAt("Press F1 to turn off HUD", pos[0]+10, pos[1]+110);
}


