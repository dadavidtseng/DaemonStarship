//-----------------------------------------------------------------------------------------------
// App.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------------------------
class UIHandler;
class Game;

//-----------------------------------------------------------------------------------------------
class App
{
public:
	App();
	~App();
	void Startup();
	void Shutdown();
	void RunFrame();

	void RunMainLoop();
	bool IsQuitting() const;

private:
	void BeginFrame() const;
	void Update(float deltaSeconds);
	void Render() const;
	void EndFrame() const;

	void HandleKeyPressed();
	void HandleKeyReleased();
	void HandleQuitRequested();
	void AdjustForPauseAndTimeDistortion(float& deltaSeconds) const;
	void DeleteAndCreateNewGame();

	bool  m_isQuitting = false;
	bool  m_isPaused = true;
	bool  m_isSlowMo = false;
	float m_timeLastFrameStart = 0.f;
	Game* m_theGame = nullptr;
};
