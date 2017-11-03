#pragma once

#include "DataModel.h"

/*! These are more game specific and will probably have to be adjusted for future use */
namespace PongHelpers {

  /*! Bounce angle calculation between player and ball */
  float getBounceAngle(const Rectangle<float>& player, const Rectangle<float>& intersection) {
    const static float MAXBOUNCEANGLE = 5.0f * float_Pi / 12.0f;

    auto acy = player.getCentreY();
    auto bcy = intersection.getCentreY();
    auto relativeIntersectY = acy - bcy;
    auto  normalizedRelativeIntersectionY = (relativeIntersectY / (player.getHeight() * 0.5f));

    return normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
  }

  /*! Sets the movement speeds based on the bounce angles on a ball */
  void setBallWithBounceAngle(Ball& b, float bounceAngle) {
    if (b.speedx > 0) {
      b.speedx = b.realSpeed *  -cos(bounceAngle);
    } else {
      b.speedx = b.realSpeed *   cos(bounceAngle);
    }

    b.speedy = b.realSpeed * -sin(bounceAngle);
  }

  /*! Is supposed to move the ball to the edge of the player if it gets inside */
  void moveToPaddleEdge(Ball& b, Player& p) {
    if (b.body.getX() > (WINDOW_WIDTHF*0.5f)) {
      // On the right side going left
      b.body = b.body.withX(p.body.getX() - b.body.getWidth());
    } else {
      // On the left side going right
      b.body = b.body.withX(p.body.getRight());
    }
  }

  /*! Resets the ball to the middle */
  void resetBall(Ball& b) {
    b.body = Rectangle<float>((WINDOW_WIDTHF / 2.0f) - (b.body.getWidth() / 2.0f)
      , (WINDOW_HEIGHTF / 2.0f) - (b.body.getHeight() / 2.0f)
      , b.body.getWidth()
      , b.body.getHeight());
    b.realSpeed = 100.0f;
  }

  /*! Clears the gamestate */
  void resetGame(GameState& state) {
      // Reload players
    state.players.clear();
    state.players.push_back({ KeyPress('w'), KeyPress('s'),{ 48.0f,320.0f, 20.0f, 128.0f } });
    state.players.push_back({ KeyPress(KeyPress::upKey), KeyPress(KeyPress::downKey),{ WINDOW_WIDTHF - 20.0f - 48.0f, 320.0f, 20.0f, 128.0f } });

    // Reload balls
    state.balls.clear();
    state.balls.push_back({ { 0,0,32,32 } });
    PongHelpers::resetBall(state.balls[0]);

    // Reset time
    state.timeSpent = 0.0f;
  }
};
