//
//  CustomButton.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 10/12/22.
//

#include "CustomButton.h"
#include "GlobalConfig.h"
#include "ColorFactory.h"
#include "CustomLabel.h"

namespace  {
const float CUSTOM_BUTTON_OFFSET_Y = -16.0f;
const float TOUCH_ANIMATION_SPEED = 0.04f;
}

CustomButton* CustomButton::create(const string &fileName, const function<void (Ref *)> &mainFunc) {
  CustomButton *node = new (std::nothrow)CustomButton();
  if(node) {
    node->initCustomButton(fileName);
    node->mainFunc = mainFunc;
    node->init();
    node->autorelease();
    return node;
  }
  CC_SAFE_DELETE(node);
  return nullptr;
}

bool CustomButton::init() {
  if (!Node::init()) { return false; }
  
  auto listener = EventListenerTouchOneByOne::create();
  
  listener->onTouchBegan = [=](Touch* touch, Event* event){
    if(this->getBoundingBoxOfImage().containsPoint(touch->getLocation())) {
      this->onTouchDown();
    }
    return true;
  };
  
  listener->onTouchMoved = [=](Touch* touch, Event* event){
    if(this->getBoundingBoxOfImage().containsPoint(touch->getLocation()) && didTouchOnButton == false) {
      this->onTouchDown();
    }
    if(this->getBoundingBoxOfImage().containsPoint(touch->getLocation()) == false && didTouchOnButton == true) {
      this->onTouchUp();
    }
    return true;
  };
  
  listener->onTouchEnded = [=](Touch* touch, Event* event){
    if(this->getBoundingBoxOfImage().containsPoint(touch->getLocation())) {
      this->onTouchUp();
      mainFunc(this);
    }
    if(didTouchOnButton) {
      this->onTouchUp();
    }
    return true;
  };
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
  this->scheduleUpdate();
  return true;
}

void CustomButton::initCustomButton(const std::string &fileName) {
  mainSprite = Sprite::create(fileName);
  this->addChild(mainSprite,3);
  mainSprite->setPositionY(CUSTOM_BUTTON_OFFSET_Y);
}

void CustomButton::onTouchDown() {
  mainSprite->stopAllActions();
  mainSprite->runAction(MoveTo::create(TOUCH_ANIMATION_SPEED, Point(0,CUSTOM_BUTTON_OFFSET_Y/3)));
  didTouchOnButton = true;
}

void CustomButton::onTouchUp() {
  mainSprite->stopAllActions();
  mainSprite->runAction(JumpTo::create(TOUCH_ANIMATION_SPEED * 2.5f , Point(0,CUSTOM_BUTTON_OFFSET_Y), CUSTOM_BUTTON_OFFSET_Y, 1));
  didTouchOnButton = false;
}

const Rect CustomButton::getBoundingBoxOfImage() const {
  Rect rect = Rect(this->getPosition().x + mainSprite->getPosition().x - mainSprite->getContentSize().width/2,
                   this->getPosition().y + mainSprite->getPosition().y - mainSprite->getContentSize().height/2,
                   mainSprite->getBoundingBox().size.width,
                   mainSprite->getBoundingBox().size.height);
  return rect;
}

void CustomButton::update(float dt) {}

void CustomButton::setBadge(int value, const std::string& fontFile, float fontSize) {
  badgeLabel = CustomLabel::create(std::to_string(value), FONT_NAME_TILE_LABEL, FONT_28_SIZE);
  badgeLabel->setAnchorPoint(Vec2(0.5, 0.5));
  badgeLabel->setPosition(Vec2(mainSprite->getPositionX() + mainSprite->getContentSize().width/2.0,
                               mainSprite->getPositionY() + mainSprite->getContentSize().height/2.0
                               + badgeLabel->getContentSize().height/2.0));
  badgeLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  this->addChild(badgeLabel, 100);
}

void CustomButton::updateBadge(int value) {
  if(badgeLabel == nullptr) { return; }
  if(value <= 0) {
    badgeLabel->setVisible(false);
  } else {
    badgeLabel->setVisible(true);
    badgeLabel->setString(std::to_string(value));
  }
}
