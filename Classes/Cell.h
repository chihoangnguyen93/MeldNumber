//
//  Cell.h
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#ifndef Cell_h
#define Cell_h

#include "cocos2d.h"

using namespace cocos2d;

class Cell: public Node {
public:
  Cell(float width, float height, Color4B color);
  virtual ~Cell();
};
#endif /* Cell_h */
