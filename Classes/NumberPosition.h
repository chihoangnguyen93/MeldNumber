//
//  NumberPosition.h
//  MeldNumber
//
//  Created by Harry Nguyen on 19/11/22.
//

#ifndef NumberPosition_h
#define NumberPosition_h

#include "cocos2d.h"

using namespace cocos2d;

class NumberPosition {
public:
  Vec2 headPosition, tailPosition;
  NumberPosition *next;
  NumberPosition(const Vec2 &headPosition, const Vec2& tailPosition);
  virtual ~NumberPosition();
};

#endif /* NumberPosition_h */
