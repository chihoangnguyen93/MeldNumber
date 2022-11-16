//
//  CustomLabel.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 11/12/22.
//

#include "CustomLabel.h"
#include "ColorFactory.h"

Label* CustomLabel::create(const std::string& text,
                           const std::string& fontFile,
                           float fontSize) {
  Label* label = Label::createWithTTF(text, fontFile, fontSize);
  
  //Draw node
  Size size = label->getContentSize();
  Vec2 position = Vec2(size.width/2.0, size.height/2.0);
  Color4F fillColor = ColorFactory::GetInstance()->getBadgeLabelColor();
  Color4F borderColor = ColorFactory::GetInstance()->getBadgeLabelColor();
  float radius = (size.width + size.height)/2.0;
  
  auto drawNode = DrawNode::create();
  CustomLabel::drawCircleColor(drawNode, position, radius, 360.0, fillColor, 1.0, borderColor);
  label->addChild(drawNode, -1);
  return label;
}

void CustomLabel::drawCircleColor(DrawNode* node,
                                  Vec2 origin,
                                  float radius,
                                  float angleDegree,
                                  Color4F fillColor,
                                  float borderWidth,
                                  Color4F borderColor,
                                  unsigned int num_of_points) {
  if (!node) { return; }
  const auto angle_step = 2 * M_PI * angleDegree / 360.f / num_of_points;
  std::vector<cocos2d::Point> circle;
  
  circle.emplace_back(origin);
  for (int i = 0; i <= num_of_points; i++) {
    auto rads = angle_step * i;
    auto x = origin.x + radius * cosf(rads);
    auto y = origin.y + radius * sinf(rads);
    circle.emplace_back(x, y);
  }
  node->drawPolygon(circle.data(), int(circle.size()), fillColor, borderWidth, borderColor);
}
