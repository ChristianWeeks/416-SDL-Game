#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"
#include "viewport.h"
#include "clock.h"

class hud {
public:
	hud();
	void draw() const;
private:
	unsigned int width;
	unsigned int height;
	Vector2f pos;
	const IOManager& io;
	SDL_Surface* screen;
	Clock& clock;
	Viewport& viewport;
	const Uint32 borderColor;
};

