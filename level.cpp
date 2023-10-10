#include "pch.h"
#include "Level.h"
#include "Exceptions.h"
#include <cmath>

template <typename T>
T opt::parse(const std::string& line)
{
	T valeur;
	std::stringstream conversion{line};
	if (!(conversion >> valeur))
		throw ParseException("Unable to parse the line");
	return valeur;
}

std::vector<std::string> opt::splitString(std::string str, const char separator)
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

void opt::Level::reloadBuffer()
{
	m_renderVertexes.create(m_vertexes.size());
	m_renderVertexes.update(m_vertexes.data());
}

bool opt::Level::continueUpdate(std::size_t index, std::size_t itterator)
{
	if (index >= m_beginTileIndex.size() - 1)
		return itterator < m_vertexes.size();
	else
		return itterator < m_beginTileIndex[index + 1];
}


opt::Level::Level() : m_renderVertexes{ sf::Triangles, sf::VertexBuffer::Dynamic }, m_autoUpdate{ true }
{

}

opt::Level::~Level()
{
	for (auto& tile : m_tiles)
		tile.release();
}

opt::Level::Level(const std::string& pPathTexture, std::size_t pNbTextures) : m_renderVertexes{ sf::Triangles, sf::VertexBuffer::Dynamic }, m_autoUpdate{ true }
{
	if (!m_texture.loadFromFile(pPathTexture))
		throw opt::LoadException(pPathTexture);
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

opt::Tile& opt::Level::operator[](int index)
{
	return *m_tiles[index];
}

const opt::Tile& opt::Level::operator[](int index) const
{
	return *m_tiles[index];
}

opt::Level& opt::Level::operator=(opt::Level&& other) noexcept
{
	if (this != &other)
	{
		m_tiles = std::move(other.m_tiles);
		m_beginTileIndex = other.m_beginTileIndex;
		m_subTextures = other.m_subTextures;
		m_texture = other.m_texture;
		m_vertexes = other.m_vertexes;
	}
	return *this;
}

void opt::Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &m_texture;

	//states.transform = m_transformations;

	if (!m_vertexes.empty())
		target.draw(m_renderVertexes, states);
}

void opt::Level::move(float offsetX, float offsetY, std::size_t index)
{
	m_tiles[index]->move(offsetX, offsetY);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::move(const sf::Vector2f& offset, std::size_t index)
{
	m_tiles[index]->move(offset);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setPosition(float x, float y, std::size_t index)
{
	m_tiles[index]->setPosition(x, y);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setPosition(const sf::Vector2f& position, std::size_t index)
{
	m_tiles[index]->setPosition(position);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::resize(float x, float y, std::size_t index)
{
	m_tiles[index]->resize(x, y);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::resize(const sf::Vector2f& size, std::size_t index)
{
	m_tiles[index]->resize(size);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::resize(float x, float y, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->resize(x, y, textureRule);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->resize(size, textureRule);
	if (m_autoUpdate) reloadBuffer();
}

bool opt::Level::loadTexture(const std::string& path, int subTextureCount)
{
	if (!m_texture.loadFromFile(path))
		return false;
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
	if (m_autoUpdate) reloadBuffer();
	return true;
}

bool opt::Level::loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures)
{
	if (!m_texture.loadFromFile(path))
		return false;
	m_subTextures = subTextures;
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}

	if (m_autoUpdate) reloadBuffer();
	return true;
}

bool opt::Level::loadTexture(const std::string& path, const std::string& subTexturePath)
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
		return false;
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
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	if (m_autoUpdate) reloadBuffer();
	return true;
}

bool opt::Level::loadTexture(const sf::Texture& texture, int subTextureCount)
{
	if (subTextureCount <= 0)
		return false;
	m_texture = texture;
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
	if (m_autoUpdate) reloadBuffer();
	return true;
}

void opt::Level::loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures)
{
	m_texture = texture;
	m_subTextures = subTextures;
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	if (m_autoUpdate) reloadBuffer();
}

bool opt::Level::loadTexture(const sf::Texture& texture, const std::string& subTexturePath)
{
	m_texture = texture;
	std::fstream fichier{subTexturePath};
	std::vector<std::string> lignes;
	for (int i{ 0 }; fichier; ++i)
	{
		lignes.push_back("");
		std::getline(fichier, lignes[i]);
	}
	if (lignes.empty())
		return false;
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
	for (auto& tuile : m_tiles)
	{
		tuile->reloadTexture();
	}
	if (m_autoUpdate) reloadBuffer();
	return true;
}

void opt::Level::setScale(const sf::Vector2f& scale, std::size_t index)
{
	m_tiles[index]->setScale(scale);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setScale(float x, float y, std::size_t index)
{
	m_tiles[index]->setScale(x, y);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setScale(const sf::Vector2f& scale, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->setScale(scale, textureRule);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setScale(float x, float y, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->setScale(x, y, textureRule);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setScale(float scale, TextureRule textureRule, std::size_t index)
{
	m_tiles[index]->setScale(scale, textureRule);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::setScale(float scale, std::size_t index)
{
	m_tiles[index]->setScale(scale);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::resetTiles()
{
	for (auto& tuile : m_tiles)
		tuile.release();
	m_tiles.resize(0);
	m_beginTileIndex.resize(0);
	m_renderVertexes.create(0);
	m_vertexes.resize(0);
}

void opt::Level::add(const opt::Tile& tile)
{
	m_tiles.push_back(tile.clone());
	reloadBuffer();
}

void opt::Level::add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, sf::Vector2f scale)
{
	m_tiles.push_back(std::make_unique<opt::Tile>(numberSubTexture, sf::FloatRect(position, size),
		textureRule, m_subTextures, m_beginTileIndex, m_vertexes, scale));
	reloadBuffer();
}

const sf::Texture& opt::Level::getTexture() const
{
	return m_texture;
}

const std::vector<sf::FloatRect>& opt::Level::getSubTextures() const
{
	return m_subTextures;
}

void opt::Level::changeTextureRect(int numberTexture, int index)
{
	m_tiles[index]->changeTextureRect(numberTexture);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::changeColour(const sf::Color& color, int index)
{
	m_tiles[index]->changeColour(color);
	if (m_autoUpdate) reloadBuffer();
}

void opt::Level::resetColour(int index)
{
	m_tiles[index]->resetColour();
	if (m_autoUpdate) reloadBuffer();
}

sf::Color opt::Level::getColour(int index) const
{
	return m_tiles[index]->getColour();
}

sf::FloatRect opt::Level::getSubTexture(int index) const
{
	return m_subTextures[index];
}

sf::Vector2f opt::Level::getSubTextureSize(int index) const
{
	return sf::Vector2f(m_subTextures[index].width, m_subTextures[index].height);
}

std::size_t opt::Level::size() const
{
	return m_tiles.size();
}

std::vector<sf::Vertex>& opt::Level::vertices()
{
	return m_vertexes;
}

std::vector<std::size_t>& opt::Level::beginTileIndexes()
{
	return m_beginTileIndex;
}

void opt::Level::bufferAutoUpdate(bool autoUpdate)
{
	m_autoUpdate = autoUpdate;
}
