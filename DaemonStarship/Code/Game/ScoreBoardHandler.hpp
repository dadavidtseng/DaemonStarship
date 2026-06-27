//----------------------------------------------------------------------------------------------------
// ScoreBoardHandler.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include <string>

struct PlayerScore;
constexpr int MAX_PLAYERS = 100;

//----------------------------------------------------------------------------------------------------
class ScoreBoardHandler
{
public:
	ScoreBoardHandler();
	~ScoreBoardHandler();

	void SaveScoreboardToFile(const PlayerScore scoreboard[], int size, const std::string& filename);
	bool FileExists(const std::string& filename);
	void CreateEmptyScoreboardFile(const std::string& filename);
	void LoadScoreboardFromFile(PlayerScore scoreboard[], int& size, const std::string& filename);
	void AddScore(PlayerScore scoreboard[], int& currentSize, const std::string& playerName, int score);
	void SortScoreboard(PlayerScore scoreboard[], int size);
	void DisplayScoreboard(const PlayerScore scoreboard[], int size);
	int  GetHighScore(const PlayerScore scoreboard[], int size);
};

struct PlayerScore
{
	std::string name;
	int         score;
	int         rank;
};
