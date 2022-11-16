//
//  ButtonAnimation.h
//  MeldNumber
//
//  Created by Harry Nguyen on 10/12/22.
//

#ifndef CustomButton_h
#define CustomButton_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;

class CustomButton: public cocos2d::Node {
public:  
  static CustomButton* create(const string &fileName, const function<void (Ref *)> &yesFunc);
  void setBadge(int value, const std::string& fontFile, float fontSize);
  void updateBadge(int value);
private:
  bool didTouchOnButton = false;
  function<void (Ref *)> mainFunc;
  Sprite *mainSprite = nullptr;
  Label* badgeLabel;
  
  virtual bool init();
  void initCustomButton(const string &fileName);
  void update(float dt);
  void onTouchDown();
  void onTouchUp();
  const Rect getBoundingBoxOfImage() const;
};
#endif /* CustomButton_h */
