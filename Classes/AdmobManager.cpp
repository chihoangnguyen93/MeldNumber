//
//  AdmobManager.cpp
//  MeldNumber
//
//  Created by Harry Nguyen on 11/12/22.
//
#include "AdmobManager.h"
#include "cocos2d.h"
using namespace std;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AdmobIOSWrapper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

AdmobManager* AdmobManager::instance= nullptr;

AdmobManager* AdmobManager::getInstance() {
  if(instance == nullptr) { instance = new AdmobManager(); }
  return instance;
}

void AdmobManager::init(const std::string &bannerId, const std::string &interstitialId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->init(bannerId, interstitialId);
#endif
}

void AdmobManager::showBanner() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->showBanner();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  cocos2d::JniMethodInfo minfo;
  if (cocos2d::JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showBanner","()V")) {
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
  }
#endif
}

void AdmobManager::showInterstitial(function<void(bool)> completion) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->showInterstitial(completion);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  cocos2d::JniMethodInfo minfo;
  if (cocos2d::JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showInterstitial","()V")) {
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
  }
#endif
}
