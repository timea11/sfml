#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{

private:
	int*            m_tiles;
	sf::Texture     m_tileset;
	int m_tilesMatrixWidth  = 0;
	int m_tilesMatrixHeight = 0;
	sf::VertexArray m_vertices;
	sf::VertexArray m_walkableTiles;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	bool load(const std::filesystem::path& tileset, sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height);
	sf::VertexArray& getWalkableTiles();
	int* getTilesMatrix();
	std::vector<int> getTilesMatrixDim();
	sf::VertexArray& getMapVertices();
};

#endif
