//
//  NextNumberFactory.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 4/12/22.
//

#include "NextNumberFactory.h"
#include "GlobalConfig.h"

vector<int> probability = vector<int> {
  SINGLE_NUMBER, PAIR_NUMBER,
  SINGLE_NUMBER, SINGLE_NUMBER,
  PAIR_NUMBER, SINGLE_NUMBER,
  PAIR_NUMBER, SINGLE_NUMBER,
  SINGLE_NUMBER, PAIR_NUMBER,
};

NextNumberFactory::NextNumberFactory() {
  totalCurrentSingleNumberOnScreen = 0;
  buckets = probability;
}

vector<int> NextNumberFactory::build(int currentMaxNumber, bool havePairEmptySquare) {
  int maxNumber = this->adjustMaximumNumber(currentMaxNumber);
  if (havePairEmptySquare && this->shouldGeneratePairNumber(maxNumber, havePairEmptySquare)) {
    int middle = maxNumber % 2 == 0 ? (maxNumber / 2) : ((maxNumber / 2) + 1);
    int firstNumber = getRandomNumber(1, middle);
    int secondNumber = getRandomNumber(middle + 1, maxNumber);
    
    if (isOddNumberWhenRandom()) {
      return vector<int> { firstNumber, secondNumber };
    } else {
      return vector<int> { secondNumber, firstNumber };
    }
  } else {
    totalCurrentSingleNumberOnScreen += 1;
    return vector<int>{ getRandomNumber(1, maxNumber) };
  }
}

bool NextNumberFactory::shouldGeneratePairNumber(int maxNumber, bool havePairEmptySquare) {
  if(maxNumber <= 1 || !havePairEmptySquare) { return false; }
  return probabilityNumberType() == PAIR_NUMBER ? true : false;
}

int NextNumberFactory::adjustMaximumNumber(int currentMaxNumber) {
  if(currentMaxNumber > 1) { return currentMaxNumber; }
  return totalCurrentSingleNumberOnScreen == 0 ? 1 : 2;
}

bool NextNumberFactory::isOddNumberWhenRandom() {
  srand( static_cast<unsigned int>(time(0)));
  return rand() % 2 != 0;
}

int NextNumberFactory::getRandomNumber(int from, int to) {
  int minValue = min(from, to);
  int maxValue = max(from, to);
  srand( static_cast<unsigned int>(time(0)));
  return rand() % (maxValue - minValue + 1) + minValue;
}

int NextNumberFactory::probabilityNumberType() {
  if(buckets.empty()) { buckets = probability; }
  int index = getRandomNumber(0, int(buckets.size()) - 1);
  int probabilityType = buckets[index];
  buckets.erase(buckets.begin() + index);
  return probabilityType;
}
