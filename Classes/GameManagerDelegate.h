//
//  GameManagerDelegate.h
//  MeldNumber
//
//  Created by Harry Nguyen on 20/11/22.
//

#ifndef GameManagerDelegate_h
#define GameManagerDelegate_h

class GameManagerDelegate {
public:
  virtual void updateEarnPointLabel(int value) = 0;
  virtual void displayGameOver(int totalEarnPoint) = 0;
  virtual void updateVisibleRotateButton(bool isVisible) = 0;
};

#endif /* GameManagerDelegate_h */
