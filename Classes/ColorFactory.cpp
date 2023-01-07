//
//  ColorFactory.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 20/11/22.
//

#include "ColorFactory.h"
#include "GlobalConfig.h"
#include <algorithm>
#include <random>

ColorFactory* ColorFactory::instance= nullptr;

ColorFactory *ColorFactory::GetInstance() {
  if(instance == nullptr) { instance = new ColorFactory(); }
  return instance;
}

ColorFactory::~ColorFactory() {}

ColorFactory:: ColorFactory() {
  shuffleColor();
}

void ColorFactory::shuffleColor() {
  vector<Color3B> colorNumbers = vector<Color3B> {
    Color3B(249, 217, 35),
    Color3B(24, 116, 152),
    Color3B(54, 174, 124),
    Color3B(255, 87, 127),
    Color3B(255, 127, 63),
    Color3B(235, 83, 83),
  };
  shuffle(colorNumbers.begin(),
          colorNumbers.end(),
          std::default_random_engine(unsigned(std::chrono::system_clock::now().time_since_epoch().count())));

  colorNumberBucket[1] = colorNumbers[0];
  colorNumberBucket[2] = colorNumbers[1];
  colorNumberBucket[3] = colorNumbers[2];
  colorNumberBucket[4] = colorNumbers[3];
  colorNumberBucket[5] = colorNumbers[4];
  colorNumberBucket[6] = colorNumbers[5];
}

Color4B ColorFactory::getBackgroundColor() {
  return Color4B(179, 232, 184, 120);
}

Color4B ColorFactory::getCellColorOfGameBoard(int row, int column) {
  int index = row + column * TOTAL_GAME_BOARD_CELL;
  return index % 2 == 0 ? Color4B(52, 43, 56, 255) : Color4B(128, 189, 171, 255);
}

Color3B ColorFactory::getTitleLabelColor() {
  return Color3B(245, 242, 241);
}

Color4B ColorFactory::getLayerColorOfNumber() {
  return Color4B(233, 236, 231, 50);
}

Color4B ColorFactory::getGameOverLayerColor() {
  return Color4B(16, 7, 32, 200);
}

Color4F ColorFactory::getBadgeLabelColor() {
  return Color4F(Color3B(245, 115, 40));
}

Color3B ColorFactory::getTextNumberColor(int number) {
  if(colorNumberBucket.find(number) == colorNumberBucket.end()) { return Color3B::ORANGE; }
  return colorNumberBucket[number];
}

// MARK: For debug mode only
Color4B ColorFactory::getCellColorOfSupplyBoard(int row, int column) {
  int index = row + column * TOTAL_SUPPLY_CELL;
  if(column % 2 == 0) {
    return index % 2 == 0 ? Color4B::BLACK : Color4B::WHITE;
  } else {
    return index % 2 == 0 ? Color4B::WHITE : Color4B::BLACK;
  }
}
