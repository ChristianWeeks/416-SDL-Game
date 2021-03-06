#ifndef SINGLE__H
#define SINGLE__H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include "gamedata.h"

class IOManager {
public:
  static IOManager& getInstance();
  SDL_Surface * getScreen() const { return screen; }
  ~IOManager() { TTF_CloseFont(font); }

  SDL_Surface* loadAndSet(const std::string& filename, 
                          bool setcolorkey) const;

  void printMessageAt(const std::string& msg, Uint32 x, Uint32 y) const;
  void printMessageCenteredAt(const std::string& msg, Uint32 y) const;
  void printStringAfterMessage(const std::string&, 
                               Uint32 x, Uint32 y) const;
  template <typename T>
  void printMessageValueAt(const std::string& msg, T value, 
         Uint32 x, Uint32 y) const;

         
  void buildString(SDL_Event);
  void clearString() { inputString = ""; }
  const std::string& getString() const { return inputString; }
private:
  IOManager();
  IOManager(const IOManager&);
  IOManager& operator=(const IOManager&);
  const Gamedata& gdata;
  int viewWidth;
  int viewHeight;
  const unsigned MAX_STRING_SIZE;
  SDL_Surface * screen;
  TTF_Font *font;
  SDL_Color color;
  std::string title;
  std::string inputString;
};
#endif
