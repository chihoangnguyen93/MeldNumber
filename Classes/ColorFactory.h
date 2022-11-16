//
//  ColorFactory.h
//  MeldNumber
//
//  Created by Harry Nguyen on 20/11/22.
//

#ifndef ColorFactory_h
#define ColorFactory_h

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class ColorFactory {
private:
  map<int, Color3B> colorNumberBucket;
  vector<Color4B> colorCellBucket;
  static ColorFactory* instance;
public:
  ColorFactory();
  virtual ~ColorFactory();
  
  /* Singletons should not be cloneable. */
  ColorFactory(ColorFactory &other) = delete;
  
  /* Singletons should not be assignable. */
  void operator=(const ColorFactory &) = delete;
  
  static ColorFactory *GetInstance();
  
  void shuffleColor();
  Color4B getBackgroundColor();
  Color4B getGameOverLayerColor();
  Color3B getTitleLabelColor();
  Color4B getLayerColorOfNumber();
  Color3B getTextNumberColor(int number);
  Color4B getCellColorOfGameBoard(int row, int column);
  Color4B getCellColorOfSupplyBoard(int row, int column);
  Color4F getBadgeLabelColor();
};

#endif /* ColorFactory_h */
