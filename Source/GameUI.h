#pragma once

class GameUI : public Component {
public:
  GameUI(GameState& gameState) : state(gameState) {}

  void paint(Graphics& g) override {
    paintBackground(g);
    paintGame(g);
    paintHUD(g);
  }

private:
  void paintGame(Graphics& g) {
    // Players
    for (auto & p : state.players) {
      g.saveState();
      auto pos = p.body.getPosition();
      g.addTransform(AffineTransform::translation(pos));
      g.addTransform(AffineTransform::scale(p.scale));

      g.setColour(p.colour);
      g.fillRect(p.body.withPosition(-p.body.getWidth() / 2, -p.body.getHeight() / 2));
      g.restoreState();
    }

    // Balls
    for (auto & b : state.balls) {
      g.saveState();
      auto pos = b.body.getPosition();
      g.addTransform(AffineTransform::translation(pos));
      g.addTransform(AffineTransform::scale(b.scale));

      g.setColour(b.colour);
      g.fillRect(b.body.withPosition(-b.body.getWidth() / 2, -b.body.getHeight() / 2));
      g.restoreState();
    }
  }

  void paintHUD(Graphics& g) {
    String p1Score(state.players[0].score);
    String p2Score(state.players[1].score);
    String nBalls(state.balls.size());

    auto row(getLocalBounds().removeFromTop(32));
    //g.setColour(Colour(30,30,30));
    //g.fillRect(row);

    g.setColour(Colour(240, 240, 240));
    g.drawText(p1Score, row.removeFromLeft(200).withTrimmedLeft(8), Justification::centredLeft);
    g.drawText(p2Score, row.removeFromRight(200).withTrimmedRight(8), Justification::centredRight);
    g.drawText(nBalls, row, Justification::centred);
  }

  void paintBackground(Graphics &g) {
    g.fillAll(Colour(30, 30, 30)); // Clear

    // Draw a dashed line in the middle
    g.setColour(Colour(240, 240, 240));
    const Line<float> line(WINDOW_WIDTHF / 2.0f, -48.0f + state.lineY, WINDOW_WIDTHF / 2.0f, WINDOW_HEIGHTF + 48.0f);
    float dashLengths[2] = { 16.0f, 32.0f };
    g.drawDashedLine(line, &dashLengths[0], 2, 16.0f);
  }

  // Reference (guaranteed pointer?) to the GameState. 
  GameState& state;
};
