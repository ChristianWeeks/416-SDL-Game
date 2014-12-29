#include <stdio.h>
#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"


FrameFactory::~FrameFactory() {
  std::cout << "Deleting FrameFactory" << std::endl;
  std::map<std::string, SDL_Surface*>::iterator itSurf = surfaces.begin();
  while ( itSurf != surfaces.end() ) {
    SDL_FreeSurface( itSurf->second );
    ++itSurf;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::iterator 
    surfaces = multiSurfaces.begin();
  while ( surfaces != multiSurfaces.end() ) {
    for (unsigned int i = 0; i < surfaces->second.size(); ++i) {
      SDL_FreeSurface( surfaces->second[i] );
    }
    ++surfaces;
  }
  std::map<std::string, Frame*>::iterator frame = frames.begin();
  while ( frame != frames.end() ) {
    delete frame->second;
    ++frame;
  }
  std::map<std::string, std::vector<Frame*> >::iterator 
    frames = multiFrames.begin();
  while ( frames != multiFrames.end() ) {
    for (unsigned int i = 0; i < frames->second.size(); ++i) {
      delete frames->second[i];
    }
    ++frames;
  }
	//Deleting background frames
  std::map<std::string, std::vector<Frame*> >::iterator 
    backframes = backgroundFrames.begin();
  while ( backframes != backgroundFrames.end() ) {
    for (unsigned int i = 0; i < backframes->second.size(); ++i) {
      delete backframes->second[i];
    }
    ++backframes;
  }
	//deleting background surfaces
  std::map<std::string, std::vector<SDL_Surface*> >::iterator 
    backsurfaces = backgroundSurfaces.begin();
  while ( backsurfaces != backgroundSurfaces.end() ) {
    for (unsigned int i = 0; i < backsurfaces->second.size(); ++i) {
      SDL_FreeSurface( backsurfaces->second[i] );
    }
    ++backsurfaces;
  }
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {

	//checking to see if a frame of this name has already been created.
	//if no, create frame. else, return that frame
    std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
			//Instantiating a surface based on the filename and transparancy
          gdata.getXmlStr(name+"File"),
          gdata.getXmlBool(name+"Transparency"));
	
	//stores the surface in a map
    surfaces[name] = surface;

	//instantiates a frame from that surface
    Frame * const frame =new Frame(name, surface);
    frames[name] = frame;
	//returns that frame
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"File"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"Frames");
	//creating a vector of frames for this single multisprite
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
	//retrieving data fro mXML
  Uint16 srcX = gdata.getXmlInt(name+"SrcX");
  Uint16 srcY = gdata.getXmlInt(name+"SrcY");
  Uint16 width = gdata.getXmlInt(name+"Width");
  Uint16 height = gdata.getXmlInt(name+"Height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
	//calling extractsurface to cut up the multisprite and place it in the vector of frames
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}


/*
	getBackLayers - Unique factory needed for the background, which is a single object consisting of multiple layered images
*/
std::vector<Frame*> FrameFactory::getBackLayers(const std::string& name) {
  // First search map to see if we've already made it:
  //The background should only need to be made once, but I'll keep this in here for now in case of future additions
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = backgroundFrames.find(name); 
  if ( pos != backgroundFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* currentLayer;
  unsigned numberOfLayers = gdata.getXmlInt(name+"Layers");
	//creating a vector of frames for this single multisprite
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfLayers);
  //All background layers have same width and height and starting location.  This will probably be changed in the future for efficiency.

  char layerNumber;
  for (unsigned i = 0; i < numberOfLayers; ++i) {
	layerNumber = i + 49;
	currentLayer = IOManager::getInstance().loadAndSet(gdata.getXmlStr(name + "Layer" + layerNumber + "File"), false);
	//calling extractsurface to cut up the multisprite and place it in the vector of frames
   surfaces.push_back(currentLayer); 
   frames.push_back( new Frame(name, currentLayer) );
  }

  backgroundSurfaces[name] = surfaces;
  backgroundFrames[name] = frames;
  return frames;
}
