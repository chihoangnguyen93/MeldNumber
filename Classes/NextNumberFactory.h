//
//  NextNumberFactory.h
//  MeldNumber
//
//  Created by Harry Nguyen on 4/12/22.
//

#ifndef NextNumberFactory_h
#define NextNumberFactory_h

#include "cocos2d.h"
using namespace std;

class NextNumberFactory {
private:
  int totalCurrentSingleNumberOnScreen;
  vector<int> buckets;
  bool isOddNumberWhenRandom();
  bool shouldGeneratePairNumber(int maxNumber, bool havePairEmptySquare);
  int getRandomNumber(int from, int to);
  int adjustMaximumNumber(int currentMaxNumber);
  int probabilityNumberType();
public:
  NextNumberFactory();
  vector<int> build(int currentMaxNumber, bool havePairEmptySquare);
};

#endif /* NextNumberFactory_h */
