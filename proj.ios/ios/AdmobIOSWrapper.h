//
//  AdmobIOSWrapper.h
//  MeldNumber
//
//  Created by Harry Nguyen on 11/12/22.
//

#ifndef AdmobIOSWrapper_h
#define AdmobIOSWrapper_h

#include "cocos2d.h"

class AdmobIOSWrapper {
public:
  static AdmobIOSWrapper* getInstance();
  void init(const std::string &bannerId, const std::string &interstitialId);
  void showBanner(int position);
  void showInterstitial(std::function<void(bool)> completion);
};

#endif /* AdmobIOSWrapper_h */
