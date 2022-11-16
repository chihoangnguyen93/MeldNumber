
//  AndroidLaunchingScene.h
//  MeldNumber
//  Created by Harry Nguyen on 15/11/22.
//

#ifndef AndroidLaunchingScene_h
#define AndroidLaunchingScene_h

#include "cocos2d.h"
using namespace cocos2d;

class AndroidLaunchingScene: public Layer {
public:
  AndroidLaunchingScene();
  virtual ~AndroidLaunchingScene();
  static Scene* createScene();
  virtual bool init();
  void update(float dt);
};

#endif /* AndroidLaunchingScene_h */
