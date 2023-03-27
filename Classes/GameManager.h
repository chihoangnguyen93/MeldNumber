//
//  GameManager.h
//  MeldNumber
//
//  Created by Harry Nguyen on 16/11/22.
//

#ifndef GameManager_h
#define GameManager_h

#include "cocos2d.h"
#include "GameBoard.h"
#include "SupplyBoard.h"
#include "GameManagerDelegate.h"
#include "NextNumberFactory.h"

using namespace std;

class GameManager {
private:
  bool isAbleToGenerateNextTurn;
  int totalEarnPoint;
  vector<NumberNode*> listNumberOnGameBoard;
  NextNumberFactory nextNumberFactory;
  SupplyBoard *supplyBoard;
  GameBoard *gameBoard;
  GameManagerDelegate* delegate;
  
  Vec2 getNeighborMatrix(const Vec2& matrix, int direction);
  
  NumberNode* getChildNumber(NumberNode* number, int direction);
  NumberNode* getNumber(const Vec2& matrix);
  
  bool isGameOver();
  bool isHavingTwoSequenceEmptyCell();
  bool isHavingOneEmptyCell(const Vec2& matrix);
  bool checkValidMatrixOnGameBoard(const Vec2& matrix);
  bool isEqualTwoVector(const vector<int> v1, const vector<int> v2);
  
  void deleteNumberOutOfGameBoard(NumberNode* number);
  void addPairNumberToGameBoard(pair<NumberNode*, NumberNode*> pairNumber, pair<Vec2, Vec2> pairMatrix);
  void addSingNumberToGameBoard(NumberNode* singleNumber, const Vec2& matrix);
  void visitNeighbor(NumberNode* number, queue<NumberNode*> &queue,
                     vector<NumberNode*> &listVisitedNumber,
                     map<Vec2, int> &matrixVisited,
                     const Vec2& nextNeighborPosition);
  void mergeSameNumber(const vector<NumberNode*> listSameNumber, function<void()> completion);
  
  int getIndexInListNumberOnGameBoard(const Vec2& matrix);
  int getMaximumNumberOnGameBoard();
public:
  GameManager();
  virtual ~GameManager();
  bool handleTouchBegan(const Vec2& touchPosition);
  void handleTouchMove(const Vec2& touchPosition);
  void handleTouchEnd(const Vec2& touchPosition);
  void rotateSupplyBoard();
  void generateNextNumber();
  void skipCurrentNumber();
  void setGameBoard(GameBoard* gameBoard);
  void setDelegate(GameManagerDelegate* delegate);
  void setSupplyBoard(SupplyBoard* SupplyBoard);
  void checkGameBoardAfterAddNewNumber(NumberNode* number, function<void()> completion);
  int currentSupplyType();
  int getCurrentPoint();
};

#endif /* GameManager_h */
