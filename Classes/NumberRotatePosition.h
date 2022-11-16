//
//  NumberRotatePosition.h
//  MeldNumber
//
//  Created by Harry Nguyen on 9/12/22.
//

#ifndef NumberRotatePosition_h
#define NumberRotatePosition_h

#include <stdio.h>
#include "NumberPosition.h"

class NumberRotatePosition {
public:
  NumberPosition *current;
  NumberRotatePosition(int supplyType);
  virtual ~NumberRotatePosition();
  void didRotate();
};

#endif /* NumberRotatePosition_h */
