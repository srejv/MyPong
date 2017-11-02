/*
  ==============================================================================

    Author: srejv

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

//==============================================================================
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 600;

const float WINDOW_WIDTHF = 1024.0f;
const float WINDOW_HEIGHTF = 600.0f;

using namespace std;

#include "GameHelpers.h"

#include "DataModel.h"
#include "PongHelpers.h"

#include "Game.h"
#include "GameUI.h"

// By using the MainContentComponent like this we can "trick" the JIT
//   to work for us in the JUCE 5 live build engine.
class MainContentComponent   : public AnimatedAppComponent
{
public:
  //==============================================================================
  MainContentComponent() : game(), gameUI(game.getState()) {
    addAndMakeVisible(gameUI);
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    setFramesPerSecond (60);
  }

  ~MainContentComponent() { }

  void update() override {
    game.update(getMillisecondsSinceLastUpdate() / 1000.0f);
  }

  void paint (Graphics& g) override {}

  void resized() override {
    gameUI.setBounds(getLocalBounds());
  }

private:
  //==============================================================================
  Game game;
  GameUI gameUI;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }
