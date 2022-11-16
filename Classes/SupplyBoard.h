//
//  SupplyBoard.h
//  Meld
//
//  Created by Harry Nguyen on 15/11/22.
//

#ifndef SupplyBoard_h
#define SupplyBoard_h

#include "cocos2d.h"
#include "NumberNode.h"
#include "NumberPosition.h"
#include "NumberRotatePosition.h"

using namespace cocos2d;
using namespace std;

class SupplyBoard: public Layer {
private:
  int rows, columns, supplyType;
  NumberRotatePosition *rotation;
  NumberNode *singleNumber;
  pair<NumberNode*, NumberNode*> pairNumber;
  
public:
  SupplyBoard(int rows, int columns);
  
  virtual ~SupplyBoard();
  
  /**
   @brief The function check user touch on supply board or not
   */
  bool isTouchingInsideSupplyBoard(const Vec2& touchPosition);
  
  /**
   @brief The function be called when  draw supply board
   */
  void drawBoard();
  
  /**
   @brief The function be called when  user tap to rotate pair number
   */
  void rotatePairNumber();
  
  /**
   @brief clean up meld number
   */
  void cleanUpAllNumber();
  
  /**
   @brief The function move meld number
   */
  void moveNumberByTouch(const Vec2& touchPosition);
  
  /**
   @brief revert to current position on supply board
   */
  void revertToPositionOnSupplyBoard();
  
  /**
   @brief The function builds single number
   */
  void generateSingleNumberObject(int value);
  
  /**
   @brief The function builds pair numbers
   */
  void generatePairNumberObject(int value1, int value2);
  
  /**
   @brief get current supply type
   */
  int getCurrentSupplyType();
  
  /**
   @brief The function will convert row, column to screen position
   */
  Vec2 convertToPosition(int row, int column);
  
  /**
   @brief get current single number
   */
  NumberNode* getCurrentSingleNumber();
  
  /**
   @brief get current pair number
   */
  pair<NumberNode*, NumberNode*> getCurrentPairNumber();
  
  vector<int> getCurrentValue();
};

#endif /* SupplyBoard_h */
