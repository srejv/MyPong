#pragma once

/*! The Player data model. */
struct Player {
  /*! Input for moving up */
  KeyPress up;
  /*! Input for moving down */
  KeyPress down;

  /*! Player - boundingbox, renders this one right now */
  Rectangle<float> body;

  /*! How fast you can move per second */
  float speed = 300.0f;

  /*! The colour of the player. To start with they are white */
  Colour colour = Colour(240, 240, 240);

  /*! Score is kept in the player? Well... it is a personal score I guess. */
  int score = 0;

  /*! I wanted to add some juice, but had to remove because it was too advanced. */
  float scale = 1.0f;
};

/*! The Ball that bounces around the screen */
struct Ball {
  /*! The ball bounding box... and rectangle that it renders */
  Rectangle<float> body;

  /*! Colour of the ball */
  Colour colour = Colour(240, 240, 240);

  /*! Speed, the speedx, speedy is the ones used for movement, realSpeed is the multiplier thingy */
  float speedx = 100.0f;
  float speedy = 100.0f;
  float realSpeed = 200.0f;

  /*! Scale for bounce animation */
  float scale = 1.0f;
};

/*! Data model for the state of the game during the whole session. */
struct GameState {
  /*! A list of all the players. In this version it is just two. */
  vector<Player> players;
  /*! A list of all the balls. */
  vector<Ball> balls;

  /*! Time since this game session started */
  float timeSpent = 0.0f;

  /*! Restart game button. Should be in some PlayerInput struct */
  KeyPress restartGame{KeyPress::F5Key};

  /*! Extra GameState for background line. */
  float lineY = 0.0f; // Middle line for animation
};

/*! An object to check if the game over conditions have been met.
 * You can see this as a part of the game state, but I like to see it as separate.
 */
struct GameOverState {
  enum class GameMode : int { Timed = 0, MaxScore };

  GameOverState() {}

  /*! Use this to check if we have reached game over conditions */
  bool isGameOver(const GameState& state)
  {
    switch (mode)
    {
    case GameMode::Timed:
      return state.timeSpent >= matchTime;
    case GameMode::MaxScore:
      for (auto& p : state.players) {
        if (p.score >= maxScore)
          return true;
      }
    }
    return false;
  }

  GameOverState::GameMode mode = GameOverState::GameMode::Timed;
  int maxScore = 10;
  float timeLeft = 0.0f;
  float matchTime = 60.0f;
};
