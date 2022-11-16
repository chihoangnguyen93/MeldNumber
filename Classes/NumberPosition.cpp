//
//  NumberPosition.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 19/11/22.
//

#include "NumberPosition.h"

NumberPosition::NumberPosition(const Vec2 &headPosition, const Vec2& tailPosition) {
  this->headPosition = headPosition;
  this->tailPosition = tailPosition;
  this->next = nullptr;
}

NumberPosition::~NumberPosition() {
  this->next = nullptr;
}
