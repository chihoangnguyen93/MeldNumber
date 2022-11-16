//
//  AdmobIOS.m
//  MeldNumber
//
//  Created by Harry Nguyen on 11/12/22.
//

#import <Foundation/Foundation.h>
#import "AdmobIOS.h"
#import <GoogleMobileAds/GADBannerView.h>
#import <GoogleMobileAds/GADInterstitialAd.h>

typedef enum : NSUInteger {
  TopCenter = 1,
  BottomCenter = 2
} AdPosition;

@interface AdmobIOS() <GADBannerViewDelegate, GADFullScreenContentDelegate>
@property (nonatomic, strong) GADBannerView* bannerView;
@property (nonatomic, strong) GADInterstitialAd* interstitialAd;
@end

@implementation AdmobIOS

+ (AdmobIOS*)sharedAdmob {
  static AdmobIOS* sAdmobManager = nil;
  @synchronized (self) {
    if (sAdmobManager == nil) {
      sAdmobManager = [[self alloc] init];
    }
  }
  return sAdmobManager;
}

- (void)showBannerInPosition:(int) position {
  if (self.bannerId == nil || self.bannerId.length == 0) { return; }
  
  if (self.bannerView == nil) {
    CGRect vFrame = [self.getWindow rootViewController].view.frame;
    GADAdSize size =  GADInlineAdaptiveBannerAdSizeWithWidthAndMaxHeight(vFrame.size.width, 50);
    self.bannerView = [[GADBannerView alloc] initWithAdSize: size origin: CGPointZero];
    [self.bannerView setAdUnitID: self.bannerId];
    [self.bannerView setDelegate: self];
    [self.bannerView setRootViewController: [self.getWindow rootViewController]];
    [[self.getWindow rootViewController].view addSubview:  self.bannerView];
    
    GADRequest* request = [GADRequest request];
    [self.bannerView loadRequest: request];
  }
  [self.bannerView setFrame: [self getFrameFromPosition: position]];
  [self.bannerView setHidden: NO];
}

- (void)loadInterstitial:(void (^)(BOOL finished))completionHandler {
  if (self.interstitialId == nil || self.interstitialId.length == 0) {
    completionHandler(false);
    return;
  }
  GADRequest *request = [GADRequest request];
  [GADInterstitialAd loadWithAdUnitID: self.interstitialId
                              request: request
                    completionHandler:^(GADInterstitialAd *ad, NSError *error) {
    if (error) {
      completionHandler(false);
    } else {
      self.interstitialAd = ad;
      self.interstitialAd.fullScreenContentDelegate = self;
      completionHandler(true);
    }
  }];
}

- (void)showInterstitial:(void (^)(BOOL finished))completionHandler {
  self.completionHandler = completionHandler;
  if (self.interstitialAd != nil) {
    [self.interstitialAd presentFromRootViewController: [self.getWindow rootViewController]];
    completionHandler(true);
    self.interstitialAd = nil;
  } else {
    [self loadInterstitial:^(BOOL finished) {
      if(finished) {
        [self showInterstitial:^(BOOL finished) { completionHandler(finished); }];
      } else {
        completionHandler(false);
      }
    }];
  }
}

/// MARK: GADFullScreenContentDelegate
- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
  self.completionHandler(false);
}

/// MARK: GADBannerViewDelegate
- (void)bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView {}

- (void)bannerView:(nonnull GADBannerView *)bannerView didFailToReceiveAdWithError:(nonnull NSError *)error {}

- (CGRect)getFrameFromPosition:(int)position {
  CGRect rootViewFrame = [self.getWindow rootViewController].view.frame;
  CGRect bannerFrame = self.bannerView.frame;
  CGPoint point;
  switch (position) {
    case TopCenter:
      point = CGPointMake( (rootViewFrame.size.width - bannerFrame.size.width)/2.0, 0);
      break;
    case BottomCenter:
      point = CGPointMake((rootViewFrame.size.width - bannerFrame.size.width)/2.0,
                          (rootViewFrame.size.height - bannerFrame.size.height));
      break;
    default: break;
  }
  bannerFrame.origin = point;
  return bannerFrame;
}

- (UIWindow*) getWindow {
  UIWindow* window = [[UIApplication sharedApplication] keyWindow];
  if (!window) {
    NSArray* windows = [[UIApplication sharedApplication] windows];
    window = [windows objectAtIndex: 0];
  }
  return window;
}
@end
