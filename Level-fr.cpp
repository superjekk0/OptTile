#include "pch.h"
#include "Level-fr.h"
#include "LoadException-fr.h"

template <typename T>
inline T parse(const std::string& line)
{
	T valeur;
	std::stringstream conversion{line};
	if (!(conversion >> valeur))
		throw "Unable to parse the line";
	return valeur;
}

/// <summary>
/// Retourne une liste de cha�nes de caract�res apr�s une filtration
/// </summary>
/// <param name="str">Cha�ne de caract�res de base</param>
/// <param name="separator">Caract�re servant � s�parer</param>
inline std::vector<std::string> splitString(std::string str, const char separator)
{
	std::vector<std::string> listStrings{};
	listStrings.reserve(4);
	for (size_t indexSeparateur{ str.find(separator) }; indexSeparateur != str.npos; indexSeparateur = str.find(separator))
	{
		listStrings.push_back(str.substr(0, indexSeparateur));
		str = str.substr(indexSeparateur + 1, str.size() - indexSeparateur);
	}
	if (str != "")
		listStrings.push_back(str);
	return listStrings;
}

inline void opt::fr::Level::reloadVertexes()
{
	m_vertexes.resize(0);
	m_beginTileIndex.resize(m_tiles.size());
	for (int i{ 0 }; i < m_tiles.size(); ++i)
	{
		m_beginTileIndex[i] = m_vertexes.size();
		auto& sommets{ m_tiles[i]->vertexes() };
		for (auto& sommet : sommets)
		{
			m_vertexes.push_back(sommet);
		}
	}
}

inline bool opt::fr::Level::continueUpdate(std::size_t index, std::size_t itterator)
{
	if (index >= m_beginTileIndex.size() - 1)
		return itterator < m_vertexes.size();
	else
		return itterator < m_beginTileIndex[index + 1];
}

inline opt::fr::Level::Level()// : m_nbTexture{ 0 }
{

}

inline opt::fr::Level::Level(const std::string& pPathTexture, std::size_t pNbTextures) //:
//m_nbTexture{ pNbTextures }
{
	if (!m_texture.loadFromFile(pPathTexture))
		throw opt::fr::LoadException(pPathTexture);
	m_tiles.resize(0);
	m_subTextures.resize(pNbTextures);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / pNbTextures;
		m_subTextures[i].top = 0.f;
		m_subTextures[i].left = m_subTextures[i].width * i;
	}
}

inline opt::fr::Tile* const opt::fr::Level::operator[](int index)
{
	return m_tiles[index].get();
}

opt::fr::Level& opt::fr::Level::operator=(opt::fr::Level&& other) noexcept
{
	if (this != &other)
	{
		m_tiles = std::move(other.m_tiles);
	}
	return *this;
}

inline void opt::fr::Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &m_texture;

	//states.transform = m_transformations;

	if (m_vertexes.size() > 0)
		target.draw(&m_vertexes[0], m_vertexes.size(), sf::Triangles, states);
}

inline void opt::fr::Level::move(float offsetX, float offsetY, std::size_t index)
{
	m_tiles[index]->move(offsetX, offsetY);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += sf::Vector2f(offsetX, offsetY);
	}
}

inline void opt::fr::Level::move(const sf::Vector2f& offset, std::size_t index)
{
	m_tiles[index]->move(offset);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += offset;
	}
}

void opt::fr::Level::setPosition(float x, float y, std::size_t index)
{
	m_tiles[index]->setPosition(x, y);
	sf::Vector2f deplacement{sf::Vector2f(x, y) - m_tiles[index]->getPosition()};
	for (std::size_t i{m_beginTileIndex[index]}; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += deplacement;
	}
}

void opt::fr::Level::setPosition(const sf::Vector2f& position, std::size_t index)
{
	m_tiles[index]->setPosition(position);
	sf::Vector2f deplacement{position - m_tiles[index]->getPosition()};
	for (std::size_t i{m_beginTileIndex[index]}; continueUpdate(index, i); ++i)
	{
		m_vertexes[i].position += deplacement;
	}
}

inline void opt::fr::Level::resize(float x, float y, std::size_t index)
{
	m_tiles[index]->resize(x, y);
	reloadVertexes();
}

inline void opt::fr::Level::resize(const sf::Vector2f& size, std::size_t index)
{
	m_tiles[index]->resize(size);
	reloadVertexes();
}

inline void opt::fr::Level::resize(float x, float y, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->resize(x, y, textureRule);
	reloadVertexes();
}

inline void opt::fr::Level::resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->resize(size, textureRule);
	reloadVertexes();
}

inline bool opt::fr::Level::loadTexture(const std::string& path, int subTextureCount)
{
	if (!m_texture.loadFromFile(path))
		return false;
	//m_nbTexture = subTextureCount;
	m_subTextures.resize(subTextureCount);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / subTextureCount;
		m_subTextures[i].left = m_texture.getSize().x / subTextureCount * i;
		m_subTextures[i].top = m_texture.getSize().y;
	}

	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	reloadVertexes();
	return true;
}

inline bool opt::fr::Level::loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures)
{
	if (!m_texture.loadFromFile(path))
		return false;
	//m_nbTexture = subTextures.size();
	/*m_subTextures.resize(subTextureCount);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / subTextureCount;
		m_subTextures[i].left = m_texture.getSize().x / subTextureCount * i;
		m_subTextures[i].top = m_texture.getSize().y;
	}*/

	m_subTextures = subTextures;
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	reloadVertexes();
	return true;
}

inline bool opt::fr::Level::loadTexture(const std::string& path, const std::string& subTexturePath)
{
	if (!m_texture.loadFromFile(path))
		return false;
	std::fstream fichier{subTexturePath};
	std::vector<std::string> lignes;
	for (int i{ 0 }; fichier; ++i)
	{
		lignes.push_back("");
		std::getline(fichier, lignes[i]);
	}
	if (lignes.empty())
		throw opt::fr::LoadException(subTexturePath);
	if (lignes[lignes.size() - 1] == "")
		lignes.pop_back();
	m_subTextures.resize(lignes.size());
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		std::vector<std::string> donnees{splitString(lignes[i], ',') };
		m_subTextures[i].left = parse<float>(donnees[0]);
		m_subTextures[i].top = parse<float>(donnees[1]);
		m_subTextures[i].width = parse<float>(donnees[2]);
		m_subTextures[i].height = parse<float>(donnees[3]);
	}
	return true;
}

inline void opt::fr::Level::loadTexture(const sf::Texture& texture, int subTextureCount)
{
	m_texture = texture;
	//m_nbTexture = subTextureCount;
	m_subTextures.resize(subTextureCount);
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		m_subTextures[i].height = m_texture.getSize().y;
		m_subTextures[i].width = m_texture.getSize().x / subTextureCount;
		m_subTextures[i].left = m_texture.getSize().x / subTextureCount * i;
		m_subTextures[i].top = m_texture.getSize().y;
	}
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	reloadVertexes();
}

inline void opt::fr::Level::loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures)
{
	m_texture = texture;
	//m_nbTexture = subTextures.size();
	m_subTextures = subTextures;
}

inline void opt::fr::Level::loadTexture(const sf::Texture& texture, const std::string& subTexturePath)
{
	m_texture = texture;
	std::fstream fichier{subTexturePath};
	std::vector<std::string> lignes;
	for (int i{ 0 }; fichier; ++i)
	{
		lignes.push_back("");
		std::getline(fichier, lignes[i]);
	}
	if (lignes[lignes.size() - 1] == "")
		lignes.pop_back();
	if (lignes.empty())
		throw "No data was within the file";
	m_subTextures.resize(lignes.size());
	for (int i{ 0 }; i < m_subTextures.size(); ++i)
	{
		std::vector<std::string> donnees{splitString(lignes[i], ',') };
		m_subTextures[i].left = parse<float>(donnees[0]);
		m_subTextures[i].top = parse<float>(donnees[1]);
		m_subTextures[i].width = parse<float>(donnees[2]);
		m_subTextures[i].height = parse<float>(donnees[3]);
	}
	//m_nbTexture = m_subTextures.size();
}

inline void opt::fr::Level::resetTiles()
{
	for (auto& tuile : m_tiles)
		tuile.release();
	m_tiles.resize(0);
	m_vertexes.resize(0);
}

inline void opt::fr::Level::add(const opt::fr::Tile& tile)
{
	m_tiles.push_back(tile.clone());
	m_beginTileIndex.push_back(m_vertexes.size());
	for (const sf::Vertex& sommet : tile.vertexes())
		m_vertexes.push_back(sommet);
}

inline void opt::fr::Level::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule)
{
	m_tiles.push_back(std::make_unique<opt::fr::Tile>(m_texture, numberSubTexture, size, position, textureRule, m_subTextures));
	m_beginTileIndex.push_back(m_vertexes.size());
	for (const sf::Vertex& sommet : m_tiles[m_tiles.size() - 1]->vertexes())
		m_vertexes.push_back(sommet);
}

inline void opt::fr::Level::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, const sf::Vector2f& scale)
{
	m_tiles.push_back(std::make_unique<opt::fr::Tile>(m_texture, numberSubTexture, size, position, textureRule, scale, m_subTextures));
	m_beginTileIndex.push_back(m_vertexes.size());
	for (const sf::Vertex& sommet : m_tiles[m_tiles.size() - 1]->vertexes())
		m_vertexes.push_back(sommet);
}

inline const sf::Texture& opt::fr::Level::getTexture() const
{
	return m_texture;
}

//inline const int& opt::fr::Level::getSubTextureCount() const
//{
//	return m_nbTexture;
//}

inline const std::vector<sf::FloatRect>& opt::fr::Level::getSubTextures() const
{
	return m_subTextures;
}

inline void opt::fr::Level::changeTextureRect(int numberTexture, int index)
{
	m_tiles[index]->changeTextureRect(numberTexture);
	reloadVertexes();
}

inline void opt::fr::Level::changeColor(const sf::Color& color, int index)
{
	m_tiles[index]->changeColor(color);
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(static_cast<std::size_t>(index), i); ++i)
		m_vertexes[i].color = color;
}

inline void opt::fr::Level::resetColor(int index)
{
	m_tiles[index]->resetColor();
	for (std::size_t i{ m_beginTileIndex[index] }; continueUpdate(static_cast<std::size_t>(index), i); ++i)
		m_vertexes[i].color = sf::Color(0xFFFFFFFF);
}

inline sf::Color opt::fr::Level::getColor(int index) const
{
	return m_tiles[index]->getColor();
}

inline sf::FloatRect opt::fr::Level::getSubTexture(int index) const
{
	return m_subTextures[index];
}

inline sf::Vector2f opt::fr::Level::getSubTextureSize(int index) const
{
	return sf::Vector2f(m_subTextures[index].width, m_subTextures[index].height);
}

inline std::size_t opt::fr::Level::size() const
{
	return m_tiles.size();
}