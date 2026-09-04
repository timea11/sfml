#include "tile_map.hpp"

bool TileMap::load(const std::filesystem::path& tileset, sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height)
    {

        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

	m_tiles = tiles;
	m_tilesMatrixWidth = width;
	m_tilesMatrixHeight = height;
	    

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vertices.resize(width * height * 6);

	// tiles on which player can walk
        m_walkableTiles.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_walkableTiles.resize(20 * 6);
	int walkableCnt = 0;
	

        // populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
        {
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                const int tileNumber = tiles[i + j * width];

                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

                // find its position in the tileset texture
                const int tx = tileNumber % (m_tileset.getSize().x / tileSize.x);
                const int ty = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tx * tileSize.x, ty * tileSize.y);
                triangles[1].texCoords = sf::Vector2f((tx + 1) * tileSize.x, ty * tileSize.y);
                triangles[2].texCoords = sf::Vector2f(tx * tileSize.x, (ty + 1) * tileSize.y);
                triangles[3].texCoords = sf::Vector2f(tx * tileSize.x, (ty + 1) * tileSize.y);
                triangles[4].texCoords = sf::Vector2f((tx + 1) * tileSize.x, ty * tileSize.y);
                triangles[5].texCoords = sf::Vector2f((tx + 1) * tileSize.x, (ty + 1) * tileSize.y);

                sf::Vertex* walkableTriangle = &m_walkableTiles[walkableCnt * 6];
		if (tx == 3)
		{
		    walkableTriangle[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
		    walkableTriangle[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
		    walkableTriangle[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
		    walkableTriangle[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
		    walkableTriangle[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
		    walkableTriangle[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

		    walkableCnt++;
		}
            }
        }

        return true;
    }

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

sf::VertexArray& TileMap::getWalkableTiles()
{
    return m_walkableTiles;
}

int *TileMap::getTilesMatrix()
{
	return m_tiles;
}

std::vector<int> TileMap::getTilesMatrixDim()
{
	return {m_tilesMatrixWidth, m_tilesMatrixHeight};
}

sf::VertexArray &TileMap::getMapVertices()
{
	return m_vertices;
}    
