package org.cocos2dx.cpp;

import android.app.Activity;
import android.graphics.Color;
import android.view.View;
import android.widget.FrameLayout;
import androidx.annotation.NonNull;

import com.boring.meldnumber.R;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;

public class AndroidAdManager {
    private Activity activity;
    private static AdView bannerAdView;
    private static boolean bannerAdViewShowing = false;
    private String bannerId;
    private String interstitialId;

    public AndroidAdManager(Activity activity) {
        this.activity = activity;
        bannerId = activity.getBaseContext().getString(R.string.bannerId);
        interstitialId = activity.getBaseContext().getString(R.string.interstitialId);
        setupAdBannerView();
    }

    public void onDestroy() {
        bannerAdView.destroy();
    }

    public void showBannerAd() {
        if (bannerAdView != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (!bannerAdViewShowing) {
                        bannerAdView.loadAd(new AdRequest.Builder().build());
                        bannerAdViewShowing = true;
                    }
                    if (!bannerAdView.isEnabled()) {
                        bannerAdView.setEnabled(true);
                    }
                    if (bannerAdView.getVisibility() == View.INVISIBLE) {
                        bannerAdView.setVisibility(View.VISIBLE);
                    }
                }
            });
        }
    }

    public void showFullscreenAd() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                InterstitialAd.load(activity, interstitialId, new AdRequest.Builder().build(), new InterstitialAdLoadCallback() {
                    @Override
                    public void onAdLoaded(@NonNull InterstitialAd interstitialAd) {
                        interstitialAd.show(activity);
                    }

                    @Override
                    public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                    }
                });
            }
        });
    }

    private void setupAdBannerView() {
        bannerAdView = new AdView(activity);
        bannerAdView.setAdSize(AdSize.FULL_BANNER);
        bannerAdView.setAdUnitId(bannerId);
        FrameLayout.LayoutParams topLayoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, android.view.Gravity.TOP | android.view.Gravity.CENTER_HORIZONTAL);
        bannerAdView.setBackgroundColor(Color.BLACK);
        bannerAdView.setBackgroundColor(0);
        activity.addContentView(bannerAdView, topLayoutParams);
        hideBannerAd();
    }

    private void hideBannerAd() {
        if (bannerAdView != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (bannerAdView.isEnabled()) {
                        bannerAdView.setEnabled(false);
                    }
                    if (bannerAdView.getVisibility() != View.INVISIBLE) {
                        bannerAdView.setVisibility(View.INVISIBLE);
                    }
                }
            });
        }
    }
}