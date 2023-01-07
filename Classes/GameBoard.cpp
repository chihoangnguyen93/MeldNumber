//
//  GameBoard.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#include "GameBoard.h"
#include "Cell.h"
#include "GlobalConfig.h"
#include "ColorFactory.h"

GameBoard::GameBoard(int rows, int columns) {
  this->rows = rows;
  this->columns = columns;
}

GameBoard::~GameBoard() {}

void GameBoard::drawBoard() {
  for(int column = 0; column < columns; column++) {
    for(int row = 0; row < rows; row++ ) {
      Cell* cell = new Cell(WIDTH_HEIGHT_CELL, WIDTH_HEIGHT_CELL, ColorFactory::GetInstance()->getCellColorOfGameBoard(row, column));
      cell->setPosition(this->convertToGameBoardPosition(row, column));
      this->addChild(cell);
    }
  }
  size = Size(rows* WIDTH_HEIGHT_CELL, columns* WIDTH_HEIGHT_CELL);
}

Vec2 GameBoard::convertToGameBoardPosition(int row, int column) {
  float xPosition = row * WIDTH_HEIGHT_CELL;
  float yPosition = column * WIDTH_HEIGHT_CELL;
  return Vec2(xPosition, yPosition);
}

Vec2 GameBoard::convertToGameBoardMatrix(const Vec2& position) {
  int x = position.x / WIDTH_HEIGHT_CELL;
  int y = position.y / WIDTH_HEIGHT_CELL;
  return Vec2(x, y);
}

Vec2 GameBoard::getMatrixForSingleNumber(NumberNode* number, const Vec2& touchPosition) {
  Vec2 pos = Vec2(touchPosition.x - this->getPositionX(), touchPosition.y - this->getPositionY());
  if (!isTouchingInsideGameBoard(pos)) { return OUT_OF_GAME_BOARD_MATRIX; }
  return convertToGameBoardMatrix(pos);
}

pair<Vec2, Vec2> GameBoard::getMatrixForPairNumber(pair<NumberNode*, NumberNode*> pairNumber, const Vec2& touchPosition) {
  Vec2 position = Vec2(touchPosition.x - this->getPositionX(), touchPosition.y - this->getPositionY());
  Vec2 firstPos, secondPos;
  
  if(pairNumber.first->isHeadOfPairNumber) {
    firstPos = Vec2(position.x, position.y);
    if(pairNumber.first->getPositionX() == pairNumber.second->getPositionX()) {
      if(pairNumber.first->getPositionY() > pairNumber.second->getPositionY()) {
        secondPos = Vec2(position.x, position.y - WIDTH_HEIGHT_CELL);
      } else {
        secondPos = Vec2(position.x, position.y + WIDTH_HEIGHT_CELL);
      }
    } else {
      if(pairNumber.first->getPositionX() > pairNumber.second->getPositionX()) {
        secondPos = Vec2(position.x - WIDTH_HEIGHT_CELL, position.y);
      } else {
        secondPos = Vec2(position.x + WIDTH_HEIGHT_CELL, position.y);
      }
    }
  } else {
    secondPos = Vec2(position.x, position.y);
    if(pairNumber.first->getPositionX() == pairNumber.second->getPositionX()) {
      if(pairNumber.first->getPositionY() > pairNumber.second->getPositionY()) {
        firstPos = Vec2(position.x, position.y + WIDTH_HEIGHT_CELL);
      } else {
        firstPos = Vec2(position.x, position.y - WIDTH_HEIGHT_CELL);
      }
    } else {
      if(pairNumber.first->getPositionX() > pairNumber.second->getPositionX()) {
        firstPos = Vec2(position.x + WIDTH_HEIGHT_CELL, position.y);
      } else {
        firstPos = Vec2(position.x - WIDTH_HEIGHT_CELL, position.y);
      }
    }
  }
  
  if (!isTouchingInsideGameBoard(firstPos) || !isTouchingInsideGameBoard(secondPos)) {
    return pair<Vec2, Vec2>(OUT_OF_GAME_BOARD_MATRIX, OUT_OF_GAME_BOARD_MATRIX);
  }
  return pair<Vec2, Vec2>(convertToGameBoardMatrix(firstPos), convertToGameBoardMatrix(secondPos));
}

void GameBoard::addSingleNumber(NumberNode* number, const Vec2& position) {
  number->removeBackgroundColor();
  number->setPosition(position);
  this->addChild(number);
}

void GameBoard::addPairNumber(pair<NumberNode*, NumberNode*> pairNumber, pair<Vec2, Vec2> pairPosition) {
  pairNumber.first->removeBackgroundColor();
  pairNumber.first->setPosition(pairPosition.first);
  this->addChild(pairNumber.first);
  
  pairNumber.second->removeBackgroundColor();
  pairNumber.second->setPosition(pairPosition.second);
  this->addChild(pairNumber.second);
}

bool GameBoard::isTouchingInsideGameBoard(const Vec2& position) {
  if(position.x < 0 ||
     position.x > size.width ||
     position.y < 0 ||
     position.y > size.height) {
    return false;
  } else {
    return true;
  }
}

int GameBoard::getRows() {
  return rows;
}

int GameBoard::getColumns() {
  return columns;
}

Size GameBoard::getContentSize() {
  return size;
}
