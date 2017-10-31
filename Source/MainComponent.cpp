/*
  ==============================================================================

    Author: srejv

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include <vector>

//==============================================================================
/*
*/
// SCREEN DATA
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 600;

const int WINDOW_WIDTHF = 1024.0f;
const int WINDOW_HEIGHTF = 600.0f;

using namespace std;

// GAME DATA
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


// GAME SPECIFIC!! WOA
namespace PongHelpers {

    float getBounceAngle(const Rectangle<float>& player, const Rectangle<float>& intersection) {
		const static float MAXBOUNCEANGLE = 5.0f * float_Pi / 12.0f;

		auto acy = player.getCentreY();
		auto bcy = intersection.getCentreY();
		auto relativeIntersectY = acy - bcy;
		auto  normalizedRelativeIntersectionY = (relativeIntersectY / (player.getHeight() * 0.5f));

		return normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
	}

	void setBallWithBounceAngle(Ball& b, float bounceAngle) {
		if (b.speedx > 0) {
			b.speedx = b.realSpeed *  -cos(bounceAngle);
		} else {
			b.speedx = b.realSpeed *   cos(bounceAngle);
		}

		b.speedy = b.realSpeed * -sin(bounceAngle);
	}

	void moveToPaddleEdge(Ball& b, Player& p) {
		if (b.body.getX() > (WINDOW_WIDTHF*0.5f)) {
			// On the right side going left
			b.body = b.body.withX(p.body.getX() - b.body.getWidth());
		} else {
			// On the left side going right
			b.body = b.body.withX(p.body.getRight());
		}
	}

	void resetBall(Ball& b) {
		b.body = Rectangle<float>((WINDOW_WIDTHF / 2.0f) - (b.body.getWidth() / 2.0f)
			, (WINDOW_HEIGHTF / 2.0f) - (b.body.getHeight() / 2.0f)
			, b.body.getWidth()
			, b.body.getHeight());
		b.realSpeed = 100.0f;
	}

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

class Game
{
public:
	Game() : state(), gameOverState(state) {
		PongHelpers::resetGame(state);
	}
	~Game() {}

	void update() {
		if (GameHelpers::isKeyDown(state.restartGame)) {
			PongHelpers::resetGame(state);
		}

		// Need time since last update.
		float dt = 1.0f / 60.0f;
		updateTimersBy(dt);
		
		updatePlayers(dt);
		updateBalls(dt);
		animateLine(dt);

		// Lel add more balls
		if (GameHelpers::isKeyDown(KeyPress(KeyPress::spaceKey))) {
			state.balls.push_back({ {WINDOW_WIDTHF*0.5f - 16.0f,WINDOW_HEIGHTF*0.5F - 16.0f,32.0f,32.0f}, GameHelpers::getRandomColour() });
		}

		// Game over check
		if (gameOverState.isGameOver()) {
			// It's game over. :/
		    PongHelpers::resetGame(state);
		}
	}

	GameState& getState() {
		return state;
	}
private:
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
	
	void animateLine(float dt) {
		state.lineY += dt * 16.0f;
		if (state.lineY >= 48.0f)  state.lineY -= 48.0f;
	}

	// Data
	GameState state;

	// Misc
	GameOverState gameOverState;
};

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
			g.setColour(p.colour);
			g.saveState();
			auto pos = p.body.getPosition();
			g.addTransform(AffineTransform::translation(pos));
			g.addTransform(AffineTransform::scale(p.scale));

			g.fillRect(p.body.withPosition(-p.body.getWidth() / 2, -p.body.getHeight() / 2));
			g.restoreState();
		}

		// Balls
		for (auto & b : state.balls) {
			g.setColour(b.colour);
			g.saveState();
			auto pos = b.body.getPosition();
			g.addTransform(AffineTransform::translation(pos));
			g.addTransform(AffineTransform::scale(b.scale));

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
		g.fillAll(Colour(30, 30, 30));

		g.setColour(Colour(240, 240, 240));
		const Line<float> line(WINDOW_WIDTHF / 2.0f, -48.0f + state.lineY, WINDOW_WIDTHF / 2.0f, WINDOW_HEIGHTF + 48.0f);
		float dashLengths[2] = { 16.0f, 32.0f };
		g.drawDashedLine(line, &dashLengths[0], 2, 16.0f);
	}
	
	GameState& state;
};

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
		game.update();
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
