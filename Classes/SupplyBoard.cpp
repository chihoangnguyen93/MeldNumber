//
//  SupplyBoard.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#include "SupplyBoard.h"
#include "GlobalConfig.h"
#include "ColorFactory.h"
#include "Cell.h"

SupplyBoard::SupplyBoard(int rows, int columns) {
  this->rows = rows;
  this->columns = columns;
}

SupplyBoard::~SupplyBoard() {
  rotation = nullptr;
  singleNumber = nullptr;
  pairNumber.first = nullptr;
  pairNumber.second = nullptr;
}

void SupplyBoard::drawBoard() {
  if(!IS_DEBUG_MODE) { return; }
  for(int column = 0; column < columns; column++) {
    for(int row = 0; row < rows; row++ ) {
      Cell* cell = new Cell(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL,
                            ColorFactory::GetInstance()->getCellColorOfSupplyBoard(row, column));
      cell->setPosition(this->convertToPosition(row, column));
      cell->setAnchorPoint(Vec2::ZERO);
      cell->setVisible(true);
      this->addChild(cell);
    }
  }
}

Vec2 SupplyBoard::convertToPosition(int row, int column) {
  float xPosition = row * WIDTH_HEIGHT_CELL;
  float yPosition = column * WIDTH_HEIGHT_CELL;
  return Vec2(xPosition, yPosition);
}

void SupplyBoard::generateSingleNumberObject(int value) {
  supplyType = SINGLE_NUMBER;
  rotation = new NumberRotatePosition(SINGLE_NUMBER);
  
  singleNumber = new NumberNode(Size(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL),
                                value,
                                ColorFactory::GetInstance()->getTextNumberColor(value),
                                ColorFactory::GetInstance()->getLayerColorOfNumber());
  singleNumber->setPosition(rotation->current->headPosition);
  this->addChild(singleNumber);
}

void SupplyBoard::generatePairNumberObject(int value1, int value2) {
  supplyType = PAIR_NUMBER;
  rotation = new NumberRotatePosition(PAIR_NUMBER);
  
  NumberNode* firstNumber = new NumberNode(Size(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL),
                                           value1,
                                           ColorFactory::GetInstance()->getTextNumberColor(value1),
                                           ColorFactory::GetInstance()->getLayerColorOfNumber());
  firstNumber->setPosition(rotation->current->headPosition);
  this->addChild(firstNumber);
  pairNumber.first = firstNumber;
  
  NumberNode* secondNumber = new NumberNode(Size(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL),
                                            value2,
                                            ColorFactory::GetInstance()->getTextNumberColor(value2),
                                            ColorFactory::GetInstance()->getLayerColorOfNumber());
  secondNumber->setPosition(rotation->current->tailPosition);
  this->addChild(secondNumber);
  pairNumber.second = secondNumber;
}

void SupplyBoard::rotatePairNumber() {
  if(supplyType == SINGLE_NUMBER) { return; }
  rotation->didRotate();
  pairNumber.first->setPosition(rotation->current->headPosition);
  pairNumber.second->setPosition(rotation->current->tailPosition);
}

bool SupplyBoard::isTouchingInsideSupplyBoard(const Vec2& touchPosition) {
  Vec2 position = Vec2(touchPosition.x - this->getPositionX(), touchPosition.y - this->getPositionY());
  switch (supplyType) {
    case SINGLE_NUMBER:
      return singleNumber->isTouchingNumber(position);
    default:
      return pairNumber.first->isTouchingNumber(position) || pairNumber.second->isTouchingNumber(position);
  }
}

void SupplyBoard::moveNumberByTouch(const Vec2& touchPosition) {
  Vec2 position = Vec2(touchPosition.x - this->getPositionX(),
                       touchPosition.y - this->getPositionY());
  
  if (supplyType == SINGLE_NUMBER) {
    singleNumber->setPosition(Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0));
  } else {
    
    Vec2 firstPos, secondPos;
    
    if(pairNumber.first->isTouchingNumber(position)) {
      pairNumber.first->isHeadOfPairNumber = true;
      pairNumber.second->isHeadOfPairNumber = false;
      
      firstPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0);
      
      if(pairNumber.first->getPositionX() == pairNumber.second->getPositionX()) {
        if(pairNumber.first->getPositionY() > pairNumber.second->getPositionY()) {
          secondPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0 - WIDTH_HEIGHT_CELL);
        } else {
          secondPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0 + WIDTH_HEIGHT_CELL);
        }
      } else {
        if(pairNumber.first->getPositionX() > pairNumber.second->getPositionX()) {
          secondPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0 - WIDTH_HEIGHT_CELL, position.y - WIDTH_HEIGHT_CELL/2.0);
        } else {
          secondPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0 + WIDTH_HEIGHT_CELL, position.y - WIDTH_HEIGHT_CELL/2.0);
        }
      }
    } else {
      pairNumber.first->isHeadOfPairNumber = false;
      pairNumber.second->isHeadOfPairNumber = true;
      
      secondPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0);
      
      if(pairNumber.first->getPositionX() == pairNumber.second->getPositionX()) {
        if(pairNumber.first->getPositionY() > pairNumber.second->getPositionY()) {
          firstPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0 + WIDTH_HEIGHT_CELL);
        } else {
          firstPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0, position.y - WIDTH_HEIGHT_CELL/2.0 - WIDTH_HEIGHT_CELL);
        }
      } else {
        if(pairNumber.first->getPositionX() > pairNumber.second->getPositionX()) {
          firstPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0 + WIDTH_HEIGHT_CELL, position.y - WIDTH_HEIGHT_CELL/2.0);
        } else {
          firstPos = Vec2(position.x - WIDTH_HEIGHT_CELL/2.0 - WIDTH_HEIGHT_CELL, position.y - WIDTH_HEIGHT_CELL/2.0);
        }
      }
    }
    pairNumber.first->setPosition(firstPos);
    pairNumber.second->setPosition(secondPos);
  }
}

void SupplyBoard::cleanUpAllNumber() {
  switch (supplyType) {
    case SINGLE_NUMBER:
      singleNumber->removeFromParent();
      singleNumber = nullptr;
      break;
    default:
      pairNumber.first->removeFromParent();
      pairNumber.second->removeFromParent();
      pairNumber.first = nullptr;
      pairNumber.second = nullptr;
      break;
  }
}

vector<int> SupplyBoard::getCurrentValue() {
  switch (supplyType) {
    case SINGLE_NUMBER:
      return vector<int> { singleNumber->getValue() };
    default:
      return vector<int> { pairNumber.first->getValue(), pairNumber.second->getValue() };
  }
}

void SupplyBoard::revertToPositionOnSupplyBoard() {
  switch (supplyType) {
    case SINGLE_NUMBER:
      singleNumber->setPosition(rotation->current->headPosition);
      break;
    default:
      pairNumber.first->setPosition(rotation->current->headPosition);
      pairNumber.second->setPosition(rotation->current->tailPosition);
      break;
  }
}

int SupplyBoard::getCurrentSupplyType() {
  return supplyType;
}

NumberNode* SupplyBoard::getCurrentSingleNumber() {
  return this->singleNumber;
}

std::pair<NumberNode*, NumberNode*> SupplyBoard::getCurrentPairNumber() {
  return this->pairNumber;
}
