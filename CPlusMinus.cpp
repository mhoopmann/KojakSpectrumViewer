/*
Copyright 2016, Michael R. Hoopmann, Institute for Systems Biology

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "CPlusMinus.h"

CPlusMinus::CPlusMinus(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  active = true;
  posX = 0;
  posY = 0;
  szX = 0;
  szY = 0;
  min = 0;
  max = 0;
  value = 0;
  fontSize = 12;

}

CPlusMinus::~CPlusMinus(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

int CPlusMinus::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){

  if (!active) return 0;

  //clicked minus button
  if (buttonMinus.logic(mouseX, mouseY, mouseButton)){
    activeFocus->focus = this;
    value--;
    if(value<min)value=min;
    return 1;
  }

  //clicked plus button
  if (buttonPlus.logic(mouseX, mouseY, mouseButton)){
    activeFocus->focus = this;
    value++;
    if (value>max)value = max;
    return 2;
  }

  //if mouse is clicked anywhere in space, but there is no function, just do nothing
  if (mouseButton1 && mouseX >= posX && mouseX <= posX + szX){
    if (mouseY >= posY && mouseY <= posY + szY) {
      activeFocus->focus = this;
      return 3;
    }
  }

  return 0;
}

void CPlusMinus::render(){
  SDL_Rect r;
  char s[32];
  int f = font->fontSize;
  font->fontSize = fontSize;

  //Draw background
  r.h = szY;
  r.w = szX;
  r.x = posX;
  r.y = posY;
  SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(display->renderer, &r);

  //Draw border
  SDL_SetRenderDrawColor(display->renderer, colors[0].r, colors[0].g, colors[0].b, 255);
  SDL_RenderDrawRect(display->renderer, &r);

  //Draw buttons
  buttonMinus.posX = posX + szX - 2*szY;
  buttonMinus.posY = posY;
  buttonMinus.szX = szY;
  buttonMinus.szY = szY;
  buttonMinus.render();
  buttonPlus.posX = posX + szX - szY;
  buttonPlus.posY = posY;
  buttonPlus.szX = szY;
  buttonPlus.szY = szY;
  buttonPlus.render();

  //Draw text
  sprintf(s,"%d",value);
  int w=font->getStringWidth(s);
  font->render(posX + szX-2*szY-w-4, posY + 2, s, txtColor);

  //reset text size
  font->fontSize = f;

}

void CPlusMinus::setDisplay(CDisplay* d){
  display = d;
  buttonMinus.setDisplay(d);
  buttonPlus.setDisplay(d);
  colors[0] = display->pal.buttons[0];
  colors[1] = display->pal.buttons[1];
  txtColor = display->pal.txtCheckbox;
}

void CPlusMinus::setFocus(CActiveFocus* f){
  activeFocus = f;
  buttonMinus.setFocus(f);
  buttonPlus.setFocus(f);
}

void CPlusMinus::setFont(CFont* f){
  font = f;
  buttonMinus.setFont(f);
  buttonMinus.setCaption("-");
  buttonPlus.setFont(f);
  buttonPlus.setCaption("+");
}

void CPlusMinus::setFontSize(int sz){
  if (sz<6)   sz = 6;
  if (sz>20)  sz = 20;
  fontSize = sz;
  szY = sz + 4;
}
