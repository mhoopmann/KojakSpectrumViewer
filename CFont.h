#ifndef _CFONT_H
#define _CFONT_H

#include <string>
#include <SDL_ttf.h>

using namespace std;

class CFont {
public:
  CFont();
  ~CFont();

  int fontSize;
  
  int getFontHeight ();
  int getStringWidth(char* str);
  int getStringWidth(string str);
  bool loadFont     (char* fname);
  void setFontSize  (int sz);
  void setRenderer  (SDL_Renderer* renderer);
  bool setText      (char c, SDL_Texture*& dest, int color=0);
  void render       (int x, int y, char* str, int color=0, bool rotate=false);
  void render       (int x, int y, string s, int color=0, bool rotate=false);

private:
  SDL_Renderer* rend;
  TTF_Font*     font;

  SDL_Texture* texture_8[2][128];
  SDL_Texture* texture_10[2][128];
  SDL_Texture* texture_12[2][128];
  SDL_Texture* texture_16[2][128];

  SDL_Rect rect_8[128];
  SDL_Rect rect_10[128];
  SDL_Rect rect_12[128];
  SDL_Rect rect_16[128];
  
  int height;
  int width;

};

#endif