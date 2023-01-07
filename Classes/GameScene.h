//  GameScene.h
//  MeldNumber
//  Created by Harry Nguyen on 15/11/22.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "GameBoard.h"
#include "SupplyBoard.h"
#include "GameManager.h"
#include "ui/CocosGUI.h"
#include "CustomButton.h"

using namespace cocos2d;

class GameScene : public cocos2d::Layer, GameManagerDelegate {
  
private:
  int numberSkip;
  Size winSize;
  ui::Button *rotateNumberButton;
  Label *earnPointLabel;
  CustomButton *skipNumberButton;
  GameBoard *gameBoard;
  GameManager *gameManager;
private:
  bool handleTouchBegan(Touch* mTouch, Event* pEvent);
  
  void createHeaderLayer();
  void createGameAndSupplyBoard();
  void registerTouchEventHandler();
  
  void handleTouchMove(Touch* mTouch, Event* pEvent);
  void handleTouchEnd(Touch* mTouch, Event* pEvent);
  
  void handleTapOnRotateButton(Ref* pSender);
  void handleTapOnResetGame(Ref* pSender);
  void handleTapOnSkipButton(Ref* pSender);
  
  void updateHighestEarnPoint(int newEarnPoint);
  void showFullScreenAdvertisement(const char* key, int frequency);
  void displayHowToPlayLayer();
  
  LayerColor* createLayerColor();
public:
  static Scene* createScene(bool shouldShowGuidLine);
  GameScene();
  virtual ~GameScene();
  virtual bool init(bool shouldShowGuidLine);
  
  virtual void displayGameOver(int totalEarnPoint);
  virtual void updateEarnPointLabel(int value);
  virtual void updateVisibleRotateButton(bool isVisible);
};

#endif /* GameScene_h */
