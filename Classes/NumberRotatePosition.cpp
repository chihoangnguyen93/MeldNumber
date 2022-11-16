//
//  NumberRotatePosition.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 9/12/22.
//

#include "NumberRotatePosition.h"
#include "GlobalConfig.h"

NumberRotatePosition::NumberRotatePosition(int supplyType) {
  switch (supplyType) {
    case SINGLE_NUMBER:
      current = new NumberPosition(Vec2(WIDTH_HEIGHT_CELL/2.0, WIDTH_HEIGHT_CELL/2.0),
                                   Vec2(WIDTH_HEIGHT_CELL/2.0, WIDTH_HEIGHT_CELL/2.0));
      current->next = nullptr;
      break;
    default:
      NumberPosition* leftRight = new NumberPosition(Vec2(0.0, WIDTH_HEIGHT_CELL/2.0),
                                                     Vec2(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL/2.0));
      
      NumberPosition* topBottom = new NumberPosition(Vec2(WIDTH_HEIGHT_CELL/2.0, WIDTH_HEIGHT_CELL),
                                                     Vec2(WIDTH_HEIGHT_CELL/2.0, 0.0));
      
      NumberPosition* rightLeft = new NumberPosition(Vec2(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL/2.0),
                                                     Vec2(0.0, WIDTH_HEIGHT_CELL/2.0));
      
      NumberPosition* bottomTop = new NumberPosition(Vec2(WIDTH_HEIGHT_CELL/2.0, 0.0),
                                                     Vec2(WIDTH_HEIGHT_CELL/2.0, WIDTH_HEIGHT_CELL));
      
      leftRight->next = topBottom;
      topBottom->next = rightLeft;
      rightLeft->next = bottomTop;
      bottomTop->next = leftRight;
      current = leftRight;
      break;
  }
}

NumberRotatePosition::~NumberRotatePosition() {
  this->current = nullptr;
}

void NumberRotatePosition::didRotate() {
  current = current ->next;
}
