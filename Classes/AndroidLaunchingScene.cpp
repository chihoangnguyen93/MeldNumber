
//  AndroidLaunchingScene.h
//  MeldNumber
//  Created by Harry Nguyen on 15/11/22.
//
//

#include "AndroidLaunchingScene.h"
#include "GameScene.h"
#include "GlobalConfig.h"
#include "ColorFactory.h"

AndroidLaunchingScene::AndroidLaunchingScene() {}

AndroidLaunchingScene::~AndroidLaunchingScene() {}

Scene* AndroidLaunchingScene::createScene() {
  auto newScene = Scene::create();
  auto layer = new AndroidLaunchingScene();
  layer->init();
  newScene->addChild(layer);
  return newScene;
}

bool AndroidLaunchingScene::init(){
  if(!Layer::init()){ return false; }
  Size winSize = Director::getInstance()->getWinSize();
  LayerColor* gameOverLayer = LayerColor::create(ColorFactory::GetInstance()->getBackgroundColor(), winSize.width, winSize.height);
  this->addChild(gameOverLayer);

  Label* gameTitleLabel = Label::createWithTTF(GAME_TITLE, FONT_NAME_TILE_LABEL, FONT_48_SIZE);
  gameTitleLabel->setAnchorPoint(Vec2(0.5, 0.5));
  gameTitleLabel->setPosition(Vec2(winSize.width/2.0, winSize.height/2.0));
  gameTitleLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  this->addChild(gameTitleLabel);
  
  this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&AndroidLaunchingScene::update), 1.0f);
  return true;
}

void AndroidLaunchingScene::update(float dt){
  Director::getInstance()->replaceScene(GameScene::createScene(true));
}
