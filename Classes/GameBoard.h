//
//  GameBoard.h
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#ifndef GameBoard_h
#define GameBoard_h

#include "cocos2d.h"
#include "NumberNode.h"

using namespace cocos2d;
using namespace std;

class GameBoard: public Layer {
  
private:
  int rows, columns;
  Size size;
  
public:
  GameBoard(int rows, int columns);
  virtual ~GameBoard();
  
  bool isTouchingInsideGameBoard(const Vec2& position);
  
  void drawBoard();
  void addSingleNumber(NumberNode* number, const Vec2& position);
  void addPairNumber(pair<NumberNode*, NumberNode*> pairNumber, pair<Vec2, Vec2> pairPosition);

  int getRows();
  int getColumns();
  
  Size getContentSize();
  
  Vec2 convertToGameBoardPosition(int row, int column);
  Vec2 convertToGameBoardMatrix(const Vec2& position);
  Vec2 getMatrixForSingleNumber(NumberNode* number, const Vec2& touchPosition);
  
  pair<Vec2, Vec2> getMatrixForPairNumber(pair<NumberNode*, NumberNode*> pairNumber, const Vec2& touchPosition);
};

#endif /* GameBoard_h */
