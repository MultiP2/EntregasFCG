#pragma once

#include "Tile.h"

class TileMap
{
public:

private:
	const std::string& tileSetPath;
	std::vector<Tile> tiles;
};