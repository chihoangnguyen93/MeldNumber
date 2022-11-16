//
//  Cell.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#include "Cell.h"

Cell::Cell(float width, float height, Color4B color) {
  LayerColor* bgColor = LayerColor::create(color, width, height);
  this->addChild(bgColor);
}

Cell::~Cell() {}
