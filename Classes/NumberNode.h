//
//  NumberNode.h
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#ifndef NumberNode_h
#define NumberNode_h

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class NumberNode: public Node {
private:
  int value;
  Vec2 matrix;
  Size size;
  Label *numberLabel;
  LayerColor* bgColorLayer;
public:
  bool isHeadOfNumber;      /// use only for pair number to decided which is head of pair number
  Vec2 parentPosition;      /// use when run animation of same value number
  
  NumberNode(Size size, int value, Color3B textNumberColor, Color4B colorLayer);
  virtual ~NumberNode();
  bool isTouchingNumber(const Vec2& position);
  int getValue();
  Vec2 getMatrix();
  void setMatrix(const Vec2& matrix);
  void moveAndHide(const Vec2& position,
                   float totalDelay,
                   float timeMoveAnimation,
                   float delayBetweenMoveAndHide);
  void updateToNextValue(int newValue,
                         float totalDelay,
                         float delayBetweenUpdateAndComplete,
                         Color3B newColor,
                         function<void()> completion);
  void disappearAnimation(float totalDelay, function<void()> completion);
  void removeBackgroundColor();
};

#endif /* NumberNode_h */
