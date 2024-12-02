#pragma once
struct LevelData
{
	int beetleCount;
	int waspCount;
	int asteroidCount;
};

constexpr int MAX_LEVEL_NUM = 5;
extern LevelData const LEVEL_DATA[MAX_LEVEL_NUM];
