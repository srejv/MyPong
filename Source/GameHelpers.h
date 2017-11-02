#pragma once

namespace GameHelpers {
  // *  Positioning functions
  // Clamp a rectangle to window vertically
  void clampVertical(Rectangle<float>& r) {
    float oldY = r.getY();
    r = r.withY(juce::jlimit<float>(0.0f, WINDOW_HEIGHTF - r.getHeight(), oldY));
  }

  // Clamp a rectangle to window horizontally
  void clampHorizontal(Rectangle<float>& r) {
    float oldX = r.getX();
    r = r.withX(juce::jlimit<float>(0.0f, WINDOW_WIDTHF - r.getWidth(), oldX));
  }

  // Move a rectangle horizontally
  void moveHorizontal(Rectangle<float>& r, const float amount) {
    r = r.withX(r.getX() + amount);
  }

  // Move a rectangle vertically
  void moveVertical(Rectangle<float>& r, const float amount) {
    r = r.withY(r.getY() + amount);
  }

  // When a rectangle is just touching outside the screen vertically
  bool isOutsideVertically(Rectangle<float>& r) {
    if (r.getY() < 0) return true;
    if (r.getY() >= (WINDOW_HEIGHTF - r.getHeight())) return true;

    return false;
  }

  // When a rectangle IS wholly outside the screen horizontally
  bool isOutsideHorizontally(Rectangle<float> &r) {
    if (r.getX() < -r.getWidth()) return true;
    if (r.getX() >= (WINDOW_WIDTHF + r.getWidth())) return true;

    return false;
  }

  // Is a keypress active
  bool isKeyDown(const KeyPress& k)
  {
    return KeyPress::isKeyCurrentlyDown(k.getKeyCode());
  }

  float getRandom() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  }

  Colour getRandomColour() {
    return Colour::fromHSV(getRandom(), 0.90f, 1.0f, 1.0f);
  }
};
