//  GameScene.cpp
//  MeldNumber
//  Created by Harry Nguyen on 15/11/22.
//
//

#include "GameScene.h"
#include "GlobalConfig.h"
#include "NumberNode.h"
#include "GameManager.h"
#include "ColorFactory.h"
#include "AdmobManager.h"

GameScene::GameScene() {
  gameManager = new GameManager();
  gameManager->setDelegate(this);
  numberSkip = 3;
}

GameScene::~GameScene() {
  rotateNumberButton = nullptr;
  gameBoard = nullptr;
  gameManager = nullptr;
}

Scene* GameScene::createScene(bool shouldShowGuidLine) {
  auto scene = Scene::create();
  auto layer = new GameScene();
  layer->init(shouldShowGuidLine);
  scene->addChild(layer);
  return scene;
}

bool GameScene::init(bool shouldShowGuidLine) {
  if (!Layer::init()) { return false; }
  
  winSize = Director::getInstance()->getWinSize();
  
  LayerColor* backgroundLayer = LayerColor::create(ColorFactory::GetInstance()->getBackgroundColor(), winSize.width, winSize.height);
  this->addChild(backgroundLayer);
  
  createGameAndSupplyBoard();
  createHeaderLayer();
  registerTouchEventHandler();
  gameManager->generateNextNumber();
  
  /// Don't show guideline screen reset and game over screen
  if(shouldShowGuidLine) {
    displayHowToPlayLayer();
  }
  return true;
}

void GameScene::createGameAndSupplyBoard() {
  /// GameBoard set up
  gameBoard = new GameBoard(TOTAL_GAME_BOARD_CELL, TOTAL_GAME_BOARD_CELL);
  Vec2 gameBoardPosition = Vec2((winSize.width - TOTAL_GAME_BOARD_CELL * WIDTH_HEIGHT_CELL)/2.0,
                                (winSize.height - TOTAL_GAME_BOARD_CELL * WIDTH_HEIGHT_CELL) /2.0
                                + DISTANCE_64_PIXEL);
  gameBoard->setPosition(gameBoardPosition);
  gameBoard->drawBoard();
  this->addChild(gameBoard);
  gameManager->setGameBoard(gameBoard);
  
  /// SupplyBoard  set up
  Vec2 supplyBoardPosition = Vec2((winSize.width - TOTAL_SUPPLY_CELL * WIDTH_HEIGHT_CELL)/2.0,
                                  gameBoard->getPositionY()
                                  - TOTAL_SUPPLY_CELL * WIDTH_HEIGHT_CELL
                                  - DISTANCE_16_PIXEL);
  SupplyBoard* supplyBoard = new SupplyBoard(TOTAL_SUPPLY_CELL, TOTAL_SUPPLY_CELL);
  supplyBoard->setPosition(supplyBoardPosition);
  supplyBoard->drawBoard();
  this->addChild(supplyBoard);
  gameManager->setSupplyBoard(supplyBoard);
  
  /// RotateNumberButton set up
  rotateNumberButton = ui::Button::create(ROTATE_BUTTON_IMAGE_NAME, ROTATE_BUTTON_IMAGE_NAME,"", ui::Widget::TextureResType::LOCAL);
  rotateNumberButton->setPosition(Vec2(winSize.width/2.0,
                                       supplyBoard->getPositionY()
                                       - rotateNumberButton->getContentSize().height/2.0
                                       - DISTANCE_16_PIXEL));
  rotateNumberButton->addClickEventListener(CC_CALLBACK_1(GameScene::handleTapOnRotateButton,this));
  rotateNumberButton->setVisible(false);
  this->addChild(rotateNumberButton);
  
  /// skipNumberButton set up
  skipNumberButton = CustomButton::create(SKIP_BUTTON_IMAGE_NAME, CC_CALLBACK_1(GameScene::handleTapOnSkipButton,this));
  skipNumberButton->setPosition(Vec2(gameBoard->getPositionX() + DISTANCE_32_PIXEL,
                                     rotateNumberButton->getPositionY()
                                     + DISTANCE_16_PIXEL)); // Equal to CUSTOM_BUTTON_OFFSET_Y
  this->addChild(skipNumberButton);
  skipNumberButton->setBadge(numberSkip, FONT_NAME_TILE_LABEL, FONT_28_SIZE);
  
  /// reset button set up
  auto resetButton = CustomButton::create(RESET_BUTTON_IMAGE_NAME, CC_CALLBACK_1(GameScene::handleTapOnResetGame,this));
  resetButton->setPosition(Vec2(gameBoard->getPositionX()
                                + TOTAL_GAME_BOARD_CELL * WIDTH_HEIGHT_CELL
                                - DISTANCE_32_PIXEL,
                                skipNumberButton->getPositionY()));
  this->addChild(resetButton);
}

void GameScene::createHeaderLayer() {
  earnPointLabel = Label::createWithTTF("Earn point", FONT_NAME_TILE_LABEL, FONT_36_SIZE);
  earnPointLabel->setAnchorPoint(Vec2(0.5, 0.5));
  earnPointLabel->setPosition(Vec2(winSize.width/2.0,
                                   gameBoard->getPositionY()
                                   + TOTAL_GAME_BOARD_CELL * WIDTH_HEIGHT_CELL
                                   + earnPointLabel->getContentSize().height/2.0
                                   + DISTANCE_48_PIXEL));
  earnPointLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  earnPointLabel->setVisible(true);
  this->addChild(earnPointLabel);
  updateEarnPointLabel(0);
  
  Label* gameTitleLabel = Label::createWithTTF(GAME_TITLE, FONT_NAME_TILE_LABEL, FONT_48_SIZE);
  gameTitleLabel->setAnchorPoint(Vec2(0.5, 0.5));
  gameTitleLabel->setPosition(Vec2(winSize.width/2.0,
                                   earnPointLabel->getPositionY()
                                   + earnPointLabel->getContentSize().height/2.0
                                   + gameTitleLabel->getContentSize().height/2.0
                                   + DISTANCE_32_PIXEL));
  gameTitleLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  this->addChild(gameTitleLabel);
}

void GameScene::registerTouchEventHandler() {
  auto lister = EventListenerTouchOneByOne::create();
  lister->setSwallowTouches(true);
  lister->onTouchBegan = CC_CALLBACK_2(GameScene::handleTouchBegan, this);
  lister->onTouchMoved = CC_CALLBACK_2(GameScene::handleTouchMove, this);
  lister->onTouchEnded = CC_CALLBACK_2(GameScene::handleTouchEnd, this);
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lister, this);
}

void GameScene::handleTapOnRotateButton(Ref* pSender) {
  if(gameManager->currentSupplyType() == SINGLE_NUMBER) { return; }
  rotateNumberButton->runAction(RotateBy::create(TIME_ROTATE_BUTTON, 90.0));
  gameManager->rotateSupplyBoard();
}

bool GameScene::handleTouchBegan(Touch* mTouch, Event* pEvent) {
  return gameManager->handleTouchBegan(mTouch->getLocation());
}

void GameScene::handleTouchMove(Touch* mTouch, Event* pEvent) {
  gameManager->handleTouchMove(mTouch->getLocation());
}

void GameScene::handleTouchEnd(Touch* mTouch, Event* pEvent) {
  gameManager->handleTouchEnd(mTouch->getLocation());
}

void GameScene::displayHowToPlayLayer() {
  /// guiderLayer set up
  LayerColor* guideLayer = createLayerColor();
  this->addChild(guideLayer);
  
  vector<string> textBuckets {
    HOW_TO_PLAY_TITLE,
    HOW_TO_PLAY_DESCRIPTION_LINE_1,
    HOW_TO_PLAY_DESCRIPTION_LINE_2,
  };
  
  float postionY = winSize.height/2.0 + DISTANCE_64_PIXEL;
  for(int index = 0; index < textBuckets.size(); index ++) {
    Label* guideLabel = Label::createWithTTF(textBuckets[index], FONT_NAME_TILE_LABEL, FONT_32_SIZE);
    guideLabel->setPosition(Vec2(winSize.width/2.0, postionY));
    guideLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
    guideLayer->addChild(guideLabel);
    postionY -= (guideLabel->getContentSize().height + DISTANCE_16_PIXEL);
  }
  
  Label* actionLabel = Label::createWithTTF(HOW_TO_PLAY_ACTION_TEXT, FONT_NAME_TILE_LABEL, FONT_24_SIZE);
  actionLabel->setPosition(Vec2(winSize.width/2.0,
                                postionY - DISTANCE_16_PIXEL));
  actionLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  guideLayer->addChild(actionLabel);
  
  /// Handle touch on game over layer
  auto touchGuideLayer = EventListenerTouchOneByOne::create();
  touchGuideLayer->setSwallowTouches(true);
  touchGuideLayer->onTouchBegan = [=](Touch* mtouch, Event* pEvent){
    guideLayer->removeFromParent();
    /// Show admod
    AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
    showFullScreenAdvertisement(AD_OPEN_GAME_SCENE_KEY, AD_OPEN_GAME_SCENE_FREQUENCY);
    return true;
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchGuideLayer, guideLayer);
}

/// MARK: GameManagerDelegate

void GameScene::displayGameOver(int totalEarnPoint) {
  this->updateHighestEarnPoint(totalEarnPoint);
  
  /// GameOverLayer set up
  LayerColor* gameOverLayer = createLayerColor();
  this->addChild(gameOverLayer);
  
  /// replayButton set up
  auto replayButton = CustomButton::create(REPLAY_BUTTON_IMAGE_NAME, CC_CALLBACK_1(GameScene::handleTapOnResetGame,this));
  replayButton->setPosition(Vec2(winSize.width/2.0, winSize.height/2.0));
  gameOverLayer->addChild(replayButton);
  
  /// highestEarnPointLabel set up
  int highestPoint = UserDefault::getInstance()->getIntegerForKey(HIGHEST_SCORE_KEY, 0);
  std::string stringValue = std::to_string(highestPoint);
  stringValue =  (highestPoint < 2 ? "Highest point: " : "Highest points: ")  + stringValue;
  Label* highestEarnPointLabel = Label::createWithTTF(stringValue, FONT_NAME_TILE_LABEL, FONT_48_SIZE);
  
  highestEarnPointLabel->setPosition(Vec2(winSize.width / 2.0,
                                          replayButton->getPositionY()
                                          + highestEarnPointLabel->getContentSize().height
                                          + DISTANCE_48_PIXEL));
  highestEarnPointLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  gameOverLayer->addChild(highestEarnPointLabel);
  
  /// Handle touch on game over layer
  auto touchGameOver = EventListenerTouchOneByOne::create();
  touchGameOver->setSwallowTouches(true);
  touchGameOver->onTouchBegan = [=](Touch* mtouch, Event* pEvent){ return true; };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchGameOver, gameOverLayer);
}

void GameScene::updateEarnPointLabel(int value) {
  std::string stringValue = std::to_string(value);
  stringValue = (value < 2 ? "Earn point: " : "Earn points: " )  + stringValue;
  earnPointLabel->setString(stringValue);
}

void GameScene::updateVisibleRotateButton(bool isVisible) {
  rotateNumberButton->setVisible(isVisible);
}

void GameScene::updateHighestEarnPoint(int newEarnPoint) {
  int currentHighestEarnPoint = UserDefault::getInstance()->getIntegerForKey(HIGHEST_SCORE_KEY, 0);
  if(newEarnPoint <= currentHighestEarnPoint) { return; }
  UserDefault::getInstance()->setIntegerForKey(HIGHEST_SCORE_KEY, newEarnPoint);
}

void GameScene::handleTapOnSkipButton(Ref* pSender) {
  numberSkip -= 1;
  skipNumberButton->updateBadge(numberSkip);
  if(numberSkip >= 0) {
    gameManager->skipCurrentNumber();
  } else {
//    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    displayShowAdToGetMoreSkipTurn();
//    #endif
  }
}

void GameScene::handleTapOnResetGame(Ref* pSender) {
  showFullScreenAdvertisement(AD_CLICK_RESET_KEY, AD_CLICK_RESET_FREQUENCY);
  Director::getInstance()->replaceScene(GameScene::createScene(false));
}

void GameScene::showFullScreenAdvertisement(const char* key, int frequency) {
  int currentCount = UserDefault::getInstance()->getIntegerForKey(key, 0);
  if(currentCount == frequency) {
    AdmobManager::getInstance()->showInterstitial([](bool result) { });
    UserDefault::getInstance()->setIntegerForKey(key, 0);
  } else {
    UserDefault::getInstance()->setIntegerForKey(key, currentCount + 1);
  }
}

void GameScene::displayShowAdToGetMoreSkipTurn() {
  if(this->getChildByTag(AD_POP_UP) != nullptr) {
    this->removeChildByTag(AD_POP_UP);
  }
  /// adLayer set up
  LayerColor* adLayer = createLayerColor();
  adLayer->setTag(AD_POP_UP);
  this->addChild(adLayer);
  
  Label* titleLabel = Label::createWithTTF(TITLE_AD_POP_UP, FONT_NAME_TILE_LABEL, FONT_32_SIZE);
  titleLabel->setPosition(Vec2(winSize.width/2.0, winSize.height/2.0));
  titleLabel->setColor(ColorFactory::GetInstance()->getTitleLabelColor());
  adLayer->addChild(titleLabel);
  
  auto noButton = CustomButton::create(NO_BUTTON_IMAGE_NAME, CC_CALLBACK_1(GameScene::handleTapOnNoButton,this));
  noButton->setPosition(Vec2(winSize.width/2.0 - DISTANCE_48_PIXEL,
                             winSize.height/2.0
                             - DISTANCE_64_PIXEL));
  noButton->setTag(NO_BUTTON);
  adLayer->addChild(noButton);
  
  auto okayButton = CustomButton::create(OKAY_BUTTON_IMAGE_NAME, CC_CALLBACK_1(GameScene::handleTapOnOkayButton,this));
  okayButton->setPosition(Vec2(winSize.width/2.0 + DISTANCE_48_PIXEL,
                               noButton->getPositionY()));
  noButton->setTag(OKAY_BUTTON);
  adLayer->addChild(okayButton);
  
  /// Handle touch on game over layer
  auto touchAdLayer = EventListenerTouchOneByOne::create();
  touchAdLayer->setSwallowTouches(true);
  touchAdLayer->onTouchBegan = [=](Touch* mtouch, Event* pEvent){ return true; };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchAdLayer, adLayer);
}

void GameScene::handleTapOnNoButton(Ref* pSender) {
  this->removeChildByTag(AD_POP_UP);
}

void GameScene::handleTapOnOkayButton(Ref* pSender) {
  this->removeChildByTag(AD_POP_UP);
  AdmobManager::getInstance()->showInterstitial([&, this](bool finished) {
    this->numberSkip = finished == true ? 1 : 0;
    this->skipNumberButton->updateBadge(this->numberSkip);
  });
}

LayerColor* GameScene::createLayerColor() {
  LayerColor* layerColor = LayerColor::create(ColorFactory::GetInstance()->getGameOverLayerColor(), winSize.width, winSize.height);
  layerColor->setPosition(Vec2(0, 0));
  return layerColor;
}
