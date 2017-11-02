#pragma once

class Game
{
public:
  Game() : state(), gameOverState(state) {
    PongHelpers::resetGame(state);
  }
  ~Game() {}

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
  void checkRestartKey() {
    if (GameHelpers::isKeyDown(state.restartGame)) {
      PongHelpers::resetGame(state);
    }
  }

  void checkGameOver() {
    // Game over check
    if (gameOverState.isGameOver()) {
      // It's game over. :/
        PongHelpers::resetGame(state);
    }
  }

  void maybeAddBalls() {
    // Lel add more balls if space is down, iz fun
    if (GameHelpers::isKeyDown(KeyPress(KeyPress::spaceKey))) {
      state.balls.push_back({ {WINDOW_WIDTHF*0.5f - 16.0f,WINDOW_HEIGHTF*0.5F - 16.0f,32.0f,32.0f}, GameHelpers::getRandomColour() });
    }
  }

  void updateTimersBy(float dt) {
    state.timeSpent += dt;
  }

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

  void updateBalls(float dt) {
    for (auto& b : state.balls) {
      ballMove(b, dt);
      ballCollide(b);
      ballScore(b);
    }
  }
  
  void ballMove(Ball& b, float dt) {
    // Movement
    GameHelpers::moveHorizontal(b.body, b.speedx * dt);
    GameHelpers::moveVertical(b.body, b.speedy * dt);

    if (GameHelpers::isOutsideVertically(b.body)) {
      b.speedy *= -1;
    }
    GameHelpers::clampVertical(b.body);
  }

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

  void ballCollide(Ball& b) {
    // Lazy collisions. But comon it's just 1 ball and 2 players. At least for now ;))) jk it will always be 2 players this time. At least for LE BALLZ
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

  void updateLine(float dt) {
    state.lineY += dt * 16.0f;
    if (state.lineY >= 48.0f)  state.lineY -= 48.0f;
  }

  // Data
  GameState state;

  // Misc
  GameOverState gameOverState;
};
