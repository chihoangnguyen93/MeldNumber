//
//  NumberNode.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 15/11/22.
//

#include "NumberNode.h"
#include "GlobalConfig.h"

NumberNode::NumberNode(Size size, int value, Color3B textNumberColor, Color4B colorLayer) {
  this->size = size;
  this->value = value;
  
  numberLabel = Label::createWithTTF(to_string(value), FONT_NAME_NUMBER_LABEL, FONT_SIZE_NUMBER_LABEL);
  numberLabel->setPosition(Vec2(size.width/2.0, size.height/2.0));
  numberLabel->setColor(textNumberColor);
  numberLabel->setVisible(true);
  this->addChild(numberLabel, 2);
  
  bgColorLayer = LayerColor::create(colorLayer, size.width, size.height);
  this->addChild(bgColorLayer);
}

NumberNode::~NumberNode() {
  bgColorLayer = nullptr;
  numberLabel = nullptr;
}

int NumberNode::getValue() {
  return value;
}

Vec2 NumberNode::getMatrix() {
  return matrix;
}

void NumberNode::setMatrix(const Vec2& matrix) {
  this->matrix = matrix;
}

bool NumberNode::isTouchingNumber(const Vec2& position) {
  if(position.x > this->getPositionX() &&
     position.x < this->getPositionX() + size.width &&
     position.y > this->getPositionY() &&
     position.y < this->getPositionY() + size.height) {
    return true;
  }
  return false;
}

void NumberNode::updateToNextValue(int newValue,
                                   float totalDelay,
                                   float delayBetweenUpdateAndComplete,
                                   Color3B newColor,
                                   function<void()> completion) {
  this->value = newValue;
  CallFunc* updateLabelAnimation = CallFunc::create([=]() {
    numberLabel->setString(to_string(value));
    numberLabel->setColor(newColor);
  });
  CallFunc* completionAction = CallFunc::create([=]() { completion(); });
  runAction(cocos2d::Sequence::create(DelayTime::create(totalDelay),
                                      updateLabelAnimation,
                                      DelayTime::create(delayBetweenUpdateAndComplete),
                                      completionAction,
                                      NULL));
}

void NumberNode::moveAndHide(const Vec2& position,
                             float totalDelay,
                             float timeMoveAnimation,
                             float delayBetweenMoveAndHide) {
  CallFunc* moveAnimation = CallFunc::create([=]() { runAction(MoveTo::create(timeMoveAnimation, position)); });
  CallFunc* disappearAnimation = CallFunc::create([=]() { this->removeFromParent(); });
  runAction(cocos2d::Sequence::create(DelayTime::create(totalDelay),
                                      moveAnimation,
                                      DelayTime::create(delayBetweenMoveAndHide),
                                      disappearAnimation,
                                      NULL));
}

void NumberNode::disappearAnimation(float totalDelay,
                                    function<void()> completion) {
  CallFunc* disappearAnimation = CallFunc::create([=]() { this->removeFromParent(); });
  CallFunc* completionAction = CallFunc::create([=]() { completion(); });
  runAction(cocos2d::Sequence::create(DelayTime::create(totalDelay),
                                      disappearAnimation,
                                      completionAction,
                                      NULL));
}

void NumberNode::removeBackgroundColor() {
  bgColorLayer->removeFromParent();
}
