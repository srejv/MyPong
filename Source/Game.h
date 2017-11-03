#pragma once

/*!
 * The Game class is the core of the execution of game logic. It's the area
 * where it reacts to input and what happens after. It can get updated any time,
 * but it's supposed to be updated each frame with a low dt (deltatime, time between frames. delta is usually used in math to denote difference. difference as we all know is x - y = difference)
 */
class Game
{
public:
  /*!
   * Contructor. Initializes the gameOverState using the state object.
   * Resets all the variables.
   */
  Game() {
    PongHelpers::resetGame(state);
  }
  ~Game() {}

  /*!
   * The update method. dt stands for delta time. Time difference.
   */
  void update(float dt) {
    updateTimersBy(dt);
    updatePlayers(dt);
    updateBalls(dt);

    updateLine(dt);

    maybeAddBalls();
    checkGameOver();
    checkRestartKey();
  }

  GameState& getState() {
    return state;
  }

private:
  /*! Used to check if we want to restart the game. */
  void checkRestartKey() {
    if (GameHelpers::isKeyDown(state.restartGame)) {
      PongHelpers::resetGame(state);
    }
  }

  /*! Used to check if game over conditions have been met. */
  void checkGameOver() {
    // Game over check
    if (gameOverState.isGameOver(state)) {
      // It's game over. :/
        PongHelpers::resetGame(state);
    }
  }

  /*! Check for add ball input. */
  void maybeAddBalls() {
    // Lel add more balls if space is down, iz fun
    if (GameHelpers::isKeyDown(KeyPress(KeyPress::spaceKey))) {
      state.balls.push_back({ {WINDOW_WIDTHF*0.5f - 16.0f,WINDOW_HEIGHTF*0.5F - 16.0f,32.0f,32.0f}, GameHelpers::getRandomColour() });
    }
  }

  /*! Update all the timers by dt */
  void updateTimersBy(float dt) {
    state.timeSpent += dt;
  }

  /*! Update all the players by dt, using the input */
  void updatePlayers(float dt) {
    for (auto& p : state.players) {
      if (GameHelpers::isKeyDown(p.up)) {
        GameHelpers::moveVertical(p.body, -p.speed * dt);
        GameHelpers::clampVertical(p.body);
      }
      if (GameHelpers::isKeyDown(p.down)) {
        GameHelpers::moveVertical(p.body, p.speed * dt);
        GameHelpers::clampVertical(p.body);
      }
    }
  }

  /*! Update the balls, position, collision etc */
  void updateBalls(float dt) {
    for (auto& b : state.balls) {
      ballMove(b, dt);
      ballCollide(b);
      ballScore(b);
    }
  }

  /*! Helper to move a ball */
  void ballMove(Ball& b, float dt) {
    // Movement
    GameHelpers::moveHorizontal(b.body, b.speedx * dt);
    GameHelpers::moveVertical(b.body, b.speedy * dt);

    if (GameHelpers::isOutsideVertically(b.body)) {
      b.speedy *= -1;
    }
    GameHelpers::clampVertical(b.body);
  }

  /*! Check if a ball has scored */
  void ballScore(Ball& b) {
    // Score?
    if (GameHelpers::isOutsideHorizontally(b.body)) {
      if (b.body.getX() < 0.0f) {
        // Hehehe how else do we check?
        state.players[1].score++;
      } else {
        state.players[0].score++;
      }

      PongHelpers::resetBall(b);
    }
  }

  /*! Check if a ball has collided with a player */
  void ballCollide(Ball& b) {
    // Lazy collisions. But comon it's just 1 ball and 2 players.
    for (auto& p : state.players) {
      auto intersectionBody(p.body);
      if (b.body.intersectRectangle(intersectionBody)) {
        // Hacky way to check if it came in from down under or what's goin on hehehehehe
        if (intersectionBody.getWidth() < intersectionBody.getHeight()) {
          b.realSpeed += 15.0f;

          auto bounceAngle = PongHelpers::getBounceAngle(p.body, intersectionBody);
          PongHelpers::setBallWithBounceAngle(b, bounceAngle);
          PongHelpers::moveToPaddleEdge(b, p);
        }
      }
    }
  }

  /*! Animate the middle line */
  void updateLine(float dt) {
    state.lineY += dt * 16.0f;
    if (state.lineY >= 48.0f)  state.lineY -= 48.0f;
  }

//========================================================================
  // Data
  /*! The GameState object. */
  GameState state;
  /*! The GameOverState object for checking if the "GameOver" conditions have been met. */
  GameOverState gameOverState;
};
