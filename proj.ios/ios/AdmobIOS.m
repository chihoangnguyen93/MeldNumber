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
@property (nonatomic, strong) GADBannerView* topBannerView;
@property (nonatomic, strong) GADBannerView* bottomBannerView;
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

- (void)showBanner {
  if (self.bannerId == nil || self.bannerId.length == 0) { return; }
  [self showTopBannerView];
  [self showBottomBannerView];
}

- (void)showTopBannerView{
  if (self.topBannerView == nil) {
    CGRect vFrame = [self.getWindow rootViewController].view.frame;
    GADAdSize size =  GADInlineAdaptiveBannerAdSizeWithWidthAndMaxHeight(vFrame.size.width, 50);
    self.topBannerView = [[GADBannerView alloc] initWithAdSize: size origin: CGPointZero];
    [self.topBannerView setAdUnitID: self.bannerId];
    [self.topBannerView setDelegate: self];
    [self.topBannerView setRootViewController: [self.getWindow rootViewController]];
    [[self.getWindow rootViewController].view addSubview:  self.topBannerView];
    GADRequest* request = [GADRequest request];
    [self.topBannerView loadRequest: request];
  }
  [self.topBannerView setFrame: [self getFrameFromPosition: TopCenter]];
  [self.topBannerView setHidden: NO];
}

- (void)showBottomBannerView{
  if (self.bottomBannerView == nil) {
    CGRect vFrame = [self.getWindow rootViewController].view.frame;
    GADAdSize size =  GADInlineAdaptiveBannerAdSizeWithWidthAndMaxHeight(vFrame.size.width, 50);
    self.bottomBannerView = [[GADBannerView alloc] initWithAdSize: size origin: CGPointZero];
    [self.bottomBannerView setAdUnitID: self.bannerId];
    [self.bottomBannerView setDelegate: self];
    [self.bottomBannerView setRootViewController: [self.getWindow rootViewController]];
    [[self.getWindow rootViewController].view addSubview:  self.bottomBannerView];
    GADRequest* request = [GADRequest request];
    [self.bottomBannerView loadRequest: request];
  }
  [self.bottomBannerView setFrame: [self getFrameFromPosition: BottomCenter]];
  [self.bottomBannerView setHidden: NO];
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
  CGRect bannerFrame = self.topBannerView.frame;
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
