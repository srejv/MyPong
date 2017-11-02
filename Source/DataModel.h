#pragma once

struct Player {
  // Input
  KeyPress up;
  KeyPress down;

  // Player - boundingbox, renders this one right now
  Rectangle<float> body;

  // Movement Speed
  float speed = 300.0f;

  // Colour
  Colour colour = Colour(240, 240, 240);

  // Score is kept in the player? Well... it is a personal score I guess.
  int score = 0;

  // Uniform scale of the player
  float scale = 1.0f;
};

struct Ball {
  // Bounding box - and right now render box
  Rectangle<float> body;

  // Colour
  Colour colour = Colour(240, 240, 240);

  // Speed, the speedx, speedy is the ones used for movement, realSpeed is the multiplier thingy
  float speedx = 100.0f;
  float speedy = 100.0f;
  float realSpeed = 200.0f;

  // Scale for bounce animation
  float scale = 1.0f;
};

struct GameState {
  // Game Objects
  vector<Player> players;
  vector<Ball> balls;

  // Data
  float timeSpent = 0.0f;

  // Input
  KeyPress restartGame{KeyPress::F5Key};

  // Extra
  float lineY = 0.0f; // Middle line for animation
};

// Hehe turned the game over state into its own object for checking
// if it's game over or not. Should be a functor tbh.
struct GameOverState {
  enum class GameMode : int { Timed = 0, MaxScore };

  GameOverState(GameState& state) : state(state) {}

  bool isGameOver()
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

  // Need access to the state. hu....
  GameState& state;

  // Data we need?
  GameOverState::GameMode mode = GameOverState::GameMode::Timed;
  int maxScore = 10;
  float timeLeft = 0.0f;
  float matchTime = 60.0f;
};
