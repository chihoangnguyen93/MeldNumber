//
//  AdmobManager.h
//  MeldNumber
//
//  Created by Harry Nguyen on 11/12/22.
//

#ifndef AdmobManager_h
#define AdmobManager_h

#include <string>
class AdmobManager {
private:
  static AdmobManager* instance;
public:
  static AdmobManager* getInstance();
  void init(const std::string &bannerId, const std::string &interstitialId);
  void showBanner();
  void showInterstitial(std::function<void(bool)> completion);
};

#endif /* AdmobManager_hpp */
