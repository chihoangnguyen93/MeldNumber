//
//  GameManager.cpp
//  Meld
//
//  Created by Harry Nguyen on 16/11/22.
//

#include "GameManager.h"
#include "GlobalConfig.h"
#include "ColorFactory.h"
#include <queue>

GameManager::GameManager() {
  isAbleToGenerateNextTurn = true;
  totalEarnPoint = 0;
  nextNumberFactory = NextNumberFactory();
}

GameManager::~GameManager() {
  gameBoard = nullptr;
  supplyBoard = nullptr;
  delegate = nullptr;
  listNumberOnGameBoard.clear();
}

int GameManager::getCurrentPoint() {
  return totalEarnPoint;
}

void GameManager::setGameBoard(GameBoard* gameBoard) {
  this->gameBoard = gameBoard;
}

void GameManager::setSupplyBoard(SupplyBoard* supplyBoard) {
  this->supplyBoard = supplyBoard;
}

bool GameManager::handleTouchBegan(const Vec2& touchPosition) {
  if(this->isGameOver() || !isAbleToGenerateNextTurn) { return false;}
  return supplyBoard->isTouchingInsideSupplyBoard(touchPosition);
}

void GameManager::handleTouchMove(const Vec2& touchPosition) {
  supplyBoard->moveNumberByTouch(touchPosition);
}

void GameManager::handleTouchEnd(const Vec2& touchPosition) {
  bool isTouchOnGameBoard = gameBoard->isTouchingInsideGameBoard(Vec2(touchPosition.x - gameBoard->getPositionX(),
                                                                      touchPosition.y - gameBoard->getPositionY()));
  if (!isTouchOnGameBoard) {
    supplyBoard->revertToPositionOnSupplyBoard();
    return;
  }
  
  if(supplyBoard->getCurrentSupplyType() == SINGLE_NUMBER) {
    Vec2 matrix = gameBoard->getMatrixForSingleNumber(supplyBoard->getCurrentSingleNumber(), touchPosition);
    if(matrix != OUT_OF_GAME_BOARD_MATRIX && this->getIndexInListNumberOnGameBoard(matrix) == IS_EMPTY_CELL) {
      addSingNumberToGameBoard(supplyBoard->getCurrentSingleNumber(), matrix);
    } else {
      supplyBoard->revertToPositionOnSupplyBoard();
    }
  } else {
    pair<Vec2, Vec2> pairMatrix = gameBoard->getMatrixForPairNumber(supplyBoard->getCurrentPairNumber(), touchPosition);
    int headIndex = this->getIndexInListNumberOnGameBoard(pairMatrix.first);
    int tailIndex = this->getIndexInListNumberOnGameBoard(pairMatrix.second);
    if(pairMatrix.first != OUT_OF_GAME_BOARD_MATRIX &&
       pairMatrix.second != OUT_OF_GAME_BOARD_MATRIX &&
       headIndex == IS_EMPTY_CELL &&
       tailIndex == IS_EMPTY_CELL) {
      addPairNumberToGameBoard(supplyBoard->getCurrentPairNumber(), pairMatrix);
    } else {
      supplyBoard->revertToPositionOnSupplyBoard();
    }
  }
}

void GameManager::rotateSupplyBoard() {
  if(this->isGameOver() || !isAbleToGenerateNextTurn) { return; }
  supplyBoard->rotatePairNumber();
}

void GameManager::generateNextNumber() {
  if(isGameOver()) {
    delegate->displayGameOver(totalEarnPoint);
    return;
  }
  int currentMaxNumber = getMaximumNumberOnGameBoard();
  vector<int> nextNumbers = nextNumberFactory.build(currentMaxNumber, isHavingTwoSequenceEmptyCell());
  if(nextNumbers.size() == 1) {
    supplyBoard->generateSingleNumberObject(nextNumbers[0]);
    delegate->updateVisibleRotateButton(false);
  } else {
    supplyBoard->generatePairNumberObject(nextNumbers[0], nextNumbers[1]);
    delegate->updateVisibleRotateButton(true);
  }
}

void GameManager::skipCurrentNumber() {
  if(this->isGameOver() || !isAbleToGenerateNextTurn) { return ;}
  
  vector<int> currentValues = supplyBoard->getCurrentValue();
  
  int currentMaxNumber = getMaximumNumberOnGameBoard();
  
  vector<int> nextNumbers = nextNumberFactory.build(currentMaxNumber, isHavingTwoSequenceEmptyCell());
  
  while (isEqualTwoVector(currentValues, nextNumbers)) {
    nextNumbers = nextNumberFactory.build(currentMaxNumber, isHavingTwoSequenceEmptyCell());
  }
  
  supplyBoard->cleanUpAllNumber();
  
  if(nextNumbers.size() == 1) {
    supplyBoard->generateSingleNumberObject(nextNumbers[0]);
    delegate->updateVisibleRotateButton(false);
  } else {
    supplyBoard->generatePairNumberObject(nextNumbers[0], nextNumbers[1]);
    delegate->updateVisibleRotateButton(true);
  }
}

void GameManager::addPairNumberToGameBoard(pair<NumberNode*, NumberNode*> pairNumber, pair<Vec2, Vec2> pairMatrix) {
  // Meld number matrix game board
  pairNumber.first->setMatrix(pairMatrix.first);
  pairNumber.second->setMatrix(pairMatrix.second);
  
  // Supply board layer remove old pair number and clean up it
  supplyBoard->cleanUpAllNumber();
  
  // add old pair number from supply board to game board
  gameBoard->addPairNumber(pairNumber, pair<Vec2, Vec2>(gameBoard->convertToGameBoardPosition(pairMatrix.first.x, pairMatrix.first.y),
                                                        gameBoard->convertToGameBoardPosition(pairMatrix.second.x, pairMatrix.second.y)));

  // add data to game board matrix data
  listNumberOnGameBoard.push_back(pairNumber.first);
  listNumberOnGameBoard.push_back(pairNumber.second);
  
  // check game board
  isAbleToGenerateNextTurn = false;
  this->delegate->updateVisibleRotateButton(false);
  
  // Always check smaller number first
  if(pairNumber.first->getValue() < pairNumber.second->getValue()) {
    this->checkGameBoardAfterAddNewNumber(pairNumber.first, [&, this, pairNumber] {
      this->checkGameBoardAfterAddNewNumber(pairNumber.second, [&, this] {
        // Build next number
        this->generateNextNumber();
        isAbleToGenerateNextTurn = true;
      });
    });
  } else {
    this->checkGameBoardAfterAddNewNumber(pairNumber.second, [&, this, pairNumber] {
      this->checkGameBoardAfterAddNewNumber(pairNumber.first, [&, this] {
        // Build next number
        this->generateNextNumber();
        isAbleToGenerateNextTurn = true;
      });
    });
  }
}


void GameManager::addSingNumberToGameBoard(NumberNode* singleNumber, const Vec2& matrix) {
  // Meld number matrix game board
  singleNumber->setMatrix(matrix);
  
  // Supply board layer remove old single number and clean up it
  supplyBoard->cleanUpAllNumber();
  
  // add old single number from supply board to game board
  gameBoard->addSingleNumber(singleNumber, gameBoard->convertToGameBoardPosition(matrix.x, matrix.y));
  
  // add data to game board matrix data
  listNumberOnGameBoard.push_back(singleNumber);
  
  // check game board
  isAbleToGenerateNextTurn = false;
  this->delegate->updateVisibleRotateButton(false);
  this->checkGameBoardAfterAddNewNumber(singleNumber, [&, this] {
    // Build next number
    this->generateNextNumber();
    isAbleToGenerateNextTurn = true;
  });
}

int GameManager::currentSupplyType() {
  return supplyBoard->getCurrentSupplyType();
}

NumberNode* GameManager::getNumber(const Vec2& matrix) {
  for(int i = 0; i < listNumberOnGameBoard.size(); i++) {
    if(listNumberOnGameBoard.at(i)->getMatrix() == matrix) {
      return listNumberOnGameBoard.at(i);
    }
  }
  return nullptr;
}

int GameManager::getIndexInListNumberOnGameBoard(const Vec2& matrix) {
  for(int i = 0; i < listNumberOnGameBoard.size(); i++) {
    if(listNumberOnGameBoard.at(i)->getMatrix() == matrix) {
      return i;
    }
  }
  return IS_EMPTY_CELL;
}

bool GameManager::checkValidMatrixOnGameBoard(const Vec2& matrix) {
  if(matrix.x < 0 ||
     matrix.x > gameBoard->getRows() -1 ||
     matrix.y < 0 ||
     matrix.y > gameBoard->getColumns() - 1) {
    return false;
  }
  return true;
}

Vec2 GameManager::getNeighborMatrix(const Vec2& matrix, int direction) {
  if(direction == LEFT) {
    return Vec2(matrix.x - 1, matrix.y);
  } else if(direction == RIGHT) {
    return Vec2(matrix.x + 1, matrix.y);
  } else if(direction == TOP) {
    return Vec2(matrix.x, matrix.y + 1);
  } else {
    return Vec2(matrix.x, matrix.y - 1);
  }
}

NumberNode* GameManager::getChildNumber(NumberNode* number, int direction) {
  Vec2 nextMatrix = getNeighborMatrix(number->getMatrix(), direction);
  NumberNode* nextNumber = getNumber(nextMatrix);
  if(nextNumber != nullptr && nextNumber->getValue() == number->getValue()) {
    return nextNumber;
  }
  return nullptr;
}

void GameManager::visitNeighbor(NumberNode* number,
                                queue<NumberNode*> &queue,
                                vector<NumberNode*> &listVisitedNumber,
                                map<Vec2, int> &matrixVisited,
                                const Vec2& nextNeighborPosition) {
  if(number != nullptr && matrixVisited.find(number->getMatrix()) == matrixVisited.end()) {
    queue.push(number);
    number->nextNeighborPosition = nextNeighborPosition;
    listVisitedNumber.push_back(number);
    matrixVisited[number->getMatrix()] = number->getValue();
  }
}

void GameManager::checkGameBoardAfterAddNewNumber(NumberNode* number, function<void()> completion) {
  queue<NumberNode*> queue;
  vector<NumberNode*> listVisitedNumber;
  map<Vec2, int> matrixVisited;
  
  queue.push(number);
  listVisitedNumber.push_back(number);
  matrixVisited[number->getMatrix()] = number->getValue();
  
  while (!queue.empty()) {
    NumberNode *parentNumber = queue.front();
    queue.pop();
    visitNeighbor(getChildNumber(parentNumber, LEFT), queue, listVisitedNumber, matrixVisited, parentNumber->getPosition());
    visitNeighbor(getChildNumber(parentNumber, RIGHT), queue, listVisitedNumber, matrixVisited, parentNumber->getPosition());
    visitNeighbor(getChildNumber(parentNumber, TOP), queue, listVisitedNumber, matrixVisited, parentNumber->getPosition());
    visitNeighbor(getChildNumber(parentNumber, BOTTOM), queue, listVisitedNumber, matrixVisited, parentNumber->getPosition());
  }
  if(listVisitedNumber.size() < MIN_SAME_NUMBER) {
    completion();
    return;
  }
  
  this->mergeSameNumber(listVisitedNumber, [&, this, number, completion] {
    totalEarnPoint += 1;
    this->delegate->updateEarnPointLabel(totalEarnPoint);
    this->checkGameBoardAfterAddNewNumber(number, completion);
  });
}

void GameManager::deleteNumberOutOfGameBoard(NumberNode* number) {
  int index = getIndexInListNumberOnGameBoard(number->getMatrix());
  if(index == IS_EMPTY_CELL) { return; }
  listNumberOnGameBoard.erase(listNumberOnGameBoard.begin() + index);
}

void GameManager::mergeSameNumber(const vector<NumberNode*> listSameNumber, function<void()> completion) {
  float totalSequenceDelayTime = 0.0;
  
  int iterator = int(listSameNumber.size()) - 1;
  
  while (iterator > 0) {
    NumberNode* number = listSameNumber.at(iterator);
    totalSequenceDelayTime += (TIME_NUMBER_MOVE_ANIMATION + DELAY_BETWEEN_TWO_ANIMATION);
    number->moveAndHide(number->nextNeighborPosition, totalSequenceDelayTime, TIME_NUMBER_MOVE_ANIMATION, DELAY_BETWEEN_TWO_ANIMATION);
    deleteNumberOutOfGameBoard(number);
    iterator -= 1;
  }
  
  NumberNode* targetNumber = listSameNumber.at(0);
  int nextValue = targetNumber->getValue() + 1;
  
  if(nextValue > TOTAL_GAME_BOARD_CELL + 1) {
    totalSequenceDelayTime += DELAY_BETWEEN_TWO_ANIMATION * 3;
    deleteNumberOutOfGameBoard(targetNumber);
    targetNumber->disappearAnimation(totalSequenceDelayTime, completion);
  } else {
    totalSequenceDelayTime += DELAY_BETWEEN_TWO_ANIMATION;
    Color3B nextColor = ColorFactory::GetInstance()->getTextNumberColor(nextValue);
    targetNumber->updateToNextValue(nextValue, totalSequenceDelayTime, DELAY_BETWEEN_TWO_ANIMATION, nextColor, completion);
  }
}

int GameManager::getMaximumNumberOnGameBoard() {
  int result = 1;
  int count = 0;
  for(int index = 0; index < listNumberOnGameBoard.size(); index++) {
    int value = listNumberOnGameBoard[index]->getValue();
    if(value == TOTAL_GAME_BOARD_CELL + 1) { count += 1; }
    result = max(value, result);
  }
  /// This logic to make sure the number 6 will only is random when we have more than 6 number on game board
  if(count >= MIN_SAME_NUMBER - 1) { return result; }
  return std::min(result, TOTAL_GAME_BOARD_CELL);
}

bool GameManager::isHavingTwoSequenceEmptyCell() {
  for(int row = 0; row < gameBoard->getRows(); row++) {
    for(int column = 0; column < gameBoard->getColumns(); column++) {
      Vec2 matrix = Vec2(row, column);
      if(isHavingOneEmptyCell(matrix)) {
        if(isHavingOneEmptyCell(getNeighborMatrix(matrix, LEFT)) ||
           isHavingOneEmptyCell(getNeighborMatrix(matrix, RIGHT)) ||
           isHavingOneEmptyCell(getNeighborMatrix(matrix, TOP)) ||
           isHavingOneEmptyCell(getNeighborMatrix(matrix, BOTTOM))) {
          return true;
        }
      }
    }
  }
  return false;
}

bool GameManager::isHavingOneEmptyCell(const Vec2& matrix) {
  return checkValidMatrixOnGameBoard(matrix) && getIndexInListNumberOnGameBoard(matrix) == IS_EMPTY_CELL;
}

bool GameManager::isGameOver() {
  for(int row = 0; row < gameBoard->getRows(); row++) {
    for(int column = 0; column < gameBoard->getColumns(); column++) {
      Vec2 matrix = Vec2(row, column);
      if(isHavingOneEmptyCell(matrix)) {
        return false;
      }
    }
  }
  return true;
}

void GameManager::setDelegate(GameManagerDelegate* delegate) {
  this->delegate = delegate;
}

bool GameManager::isEqualTwoVector(const vector<int> v1, const vector<int> v2) {
  if(v1.size() != v2.size()) { return false; }
  int size = int(v1.size());
  int totalTrue = 0;
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      if(v1[i] ==  v2[j]) {
        totalTrue += 1;
        break;
      }
    }
  }
  return totalTrue == size;
}

bool operator<(const Vec2 &c1, const Vec2 &c2) {
  return c1.x < c2.x && c1.y < c2.y;
}
