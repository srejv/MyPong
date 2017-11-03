#pragma once

/*!
  A collection of functions to clean up the game logic
 */
namespace GameHelpers {
  /*! Clamp a rectangle the Screen vertically */
  void clampVertical(Rectangle<float>& r) {
    float oldY = r.getY();
    r = r.withY(juce::jlimit<float>(0.0f, WINDOW_HEIGHTF - r.getHeight(), oldY));
  }

  /*! Clamp a rectangle to the Screen horizontally */
  void clampHorizontal(Rectangle<float>& r) {
    float oldX = r.getX();
    r = r.withX(juce::jlimit<float>(0.0f, WINDOW_WIDTHF - r.getWidth(), oldX));
  }

  /*! Moves a rectangle horizontally by an amount */
  void moveHorizontal(Rectangle<float>& r, const float amount) {
    r = r.withX(r.getX() + amount);
  }

  /*! Moves a rectangle vertically by an amount */
  void moveVertical(Rectangle<float>& r, const float amount) {
    r = r.withY(r.getY() + amount);
  }

  /*! This should actually be called is touching outside. */
  // When a rectangle is just touching outside the screen vertically
  bool isOutsideVertically(Rectangle<float>& r) {
    if (r.getY() < 0) return true;
    if (r.getY() >= (WINDOW_HEIGHTF - r.getHeight())) return true;

    return false;
  }

  /*! This one checks if the rectangle is outside of teh screen
  // When a rectangle IS wholly outside the screen horizontally
  bool isOutsideHorizontally(Rectangle<float> &r) {
    if (r.getX() < -r.getWidth()) return true;
    if (r.getX() >= (WINDOW_WIDTHF + r.getWidth())) return true;

    return false;
  }

  /!* Check if a key is currently down */
  bool isKeyDown(const KeyPress& k)
  {
    return KeyPress::isKeyCurrentlyDown(k.getKeyCode());
  }

  /*! Get a random number between 0.0 and 1.0 */
  float getRandom() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  }

  /*! Get a random colour */
  Colour getRandomColour() {
    return Colour::fromHSV(getRandom(), 0.90f, 1.0f, 1.0f);
  }
};
