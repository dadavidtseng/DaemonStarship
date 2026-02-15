//----------------------------------------------------------------------------------------------------
// ScoreBoardHandler.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/ScoreBoardHandler.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/GameCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/SimpleTriangleFont.hpp"
#include "Engine/Renderer/Renderer.hpp"

//----------------------------------------------------------------------------------------------------
ScoreBoardHandler::ScoreBoardHandler() = default;

//----------------------------------------------------------------------------------------------------
ScoreBoardHandler::~ScoreBoardHandler() = default;

// #TODO: refactor
void ScoreBoardHandler::SaveScoreboardToFile(const PlayerScore scoreboard[], const int size, const std::string& filename)
{
	FILE*         file;
	const errno_t err = fopen_s(&file, filename.c_str(), "w");

	if (err == 0 && file != nullptr)
	{
		for (int i = 0; i < size; ++i)
		{
			fprintf(file, "%d. %s - %d\n", scoreboard[i].rank, scoreboard[i].name.c_str(), scoreboard[i].score);
		}
		fclose(file);

		printf("Scoreboard saved to file.\n");
	}
	else
	{
		printf("Error opening file for writing!\n");
	}
}

bool ScoreBoardHandler::FileExists(const std::string& filename)
{
	FILE*         file;
	const errno_t err = fopen_s(&file, filename.c_str(), "r");

	if (err == 0 && file != nullptr)
	{
		fclose(file);

		return true; // file exist
	}
	return false; // file doesn't exist
}

void ScoreBoardHandler::CreateEmptyScoreboardFile(const std::string& filename)
{
	FILE*         file;
	const errno_t err = fopen_s(&file, filename.c_str(), "w");

	if (err == 0 && file)
	{
		// create an empty scoreboard if there isn't one
		fprintf(file, "No scores yet\n");

		fclose(file);

		printf("Scoreboard file created: %s\n", filename.c_str());
	}
	else
	{
		printf("Error creating file: %s\n", filename.c_str());
	}
}

void ScoreBoardHandler::LoadScoreboardFromFile(PlayerScore scoreboard[], int& size, const std::string& filename)
{
	FILE* file = nullptr;

	fopen_s(&file, filename.c_str(), "r");

	if (file == nullptr)
	{
		size = 0;
		return;
	}

	size = 0;
	char nameBuffer[100];
	int  score = 0;
	int  rank  = 0;

	while (fscanf_s(file, "%d. %99s - %d", &rank, nameBuffer, static_cast<unsigned>(_countof(nameBuffer)), &score) == 3)
	{
		scoreboard[size].name  = std::string(nameBuffer);
		scoreboard[size].score = score;
		scoreboard[size].rank  = rank;
		++size;

		printf("Loaded: %s - %d size: %d\n", scoreboard[size - 1].name.c_str(), scoreboard[size - 1].score, size);

		if (size >= MAX_PLAYERS)
			break;
	}

	fclose(file);
}

void ScoreBoardHandler::AddScore(PlayerScore scoreboard[], int& currentSize, const std::string& playerName,
                                 const int   score)
{
	if (currentSize < MAX_PLAYERS)
	{
		scoreboard[currentSize].name  = playerName;
		scoreboard[currentSize].score = score;
		scoreboard[currentSize].rank  = currentSize + 1;
		++currentSize;

		SortScoreboard(scoreboard, currentSize);

		for (int i = 0; i < currentSize; ++i)
		{
			scoreboard[i].rank = i + 1;
		}
	}
	else
	{
		printf("Scoreboard is full!\n");
	}
}

void ScoreBoardHandler::SortScoreboard(PlayerScore scoreboard[], const int size)
{
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = 0; j < size - i - 1; ++j)
		{
			if (scoreboard[j].score < scoreboard[j + 1].score)
			{
				const PlayerScore temp = scoreboard[j];

				scoreboard[j]     = scoreboard[j + 1];
				scoreboard[j + 1] = temp;
			}
		}
	}
}

void ScoreBoardHandler::DisplayScoreboard(const PlayerScore scoreboard[], const int size)
{
	const int maxDisplayCount = size < 10 ? size : 10;

	constexpr float startY = SCREEN_SIZE_Y / 2.f;
	const Rgba8     textColor(150, 255, 150);

	std::vector<Vertex_PCU> textVerts;

	for (int i = 0; i < maxDisplayCount; ++i)
	{
		constexpr float textSize = 50.f;

		std::string scoreLine = "Rank " + std::to_string(scoreboard[i].rank) + ". " + scoreboard[i].name + " - " +
			std::to_string(scoreboard[i].score);

		AddVertsForTextTriangles2D(textVerts,
		                           scoreLine,
		                           Vec2(0.f, startY - i * textSize),
		                           textSize,
		                           textColor,
		                           1.f, true, 0.3f);
	}

	g_renderer->DrawVertexArray(static_cast<int>(textVerts.size()), textVerts.data());

	printf("Scoreboard (Top 10):\n");

	for (int i = 0; i < maxDisplayCount; ++i)
	{
		printf("Rank %d. %s - %d\n", scoreboard[i].rank, scoreboard[i].name.c_str(), scoreboard[i].score);
	}
}

int ScoreBoardHandler::GetHighScore(const PlayerScore scoreboard[], const int size)
{
	if (size == 0)
	{
		printf("No scores available.\n");
		return -1; // Return -1 if there are no scores.
	}

	int highScore = scoreboard[0].score;

	for (int i = 1; i < size; ++i)
	{
		if (scoreboard[i].score > highScore)
		{
			highScore = scoreboard[i].score;
		}
	}

	printf("The highest score is: %d\n", highScore);
	return highScore;
}
