//
//  CustomLabel.h
//  MeldNumber
//
//  Created by Harry Nguyen on 11/12/22.
//

#ifndef CustomLabel_h
#define CustomLabel_h

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class CustomLabel: public Node {
private:
  static void drawCircleColor(DrawNode* node, Vec2 origin, float radius,
                              float angleDegree, Color4F fillColor, float borderWidth,
                              Color4F borderColor, unsigned int num_of_points = 100);
public:
  static Label* create(const std::string& text, const std::string& fontFile, float fontSize);
};

#endif /* CustomLabel_h */
