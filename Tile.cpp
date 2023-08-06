#include "pch.h"
#include "Tile.h"
#include "Tile.h"

inline void opt::Tile::intializeVertexes()
{
	//m_vertexes->resize(6);

	if (!m_texture || !m_subTextures)
	{
		m_textureRule = TextureRule::fill_space;
		(*m_vertexes)[0].position = m_position;
		(*m_vertexes)[1].position = m_position + sf::Vector2f(0.f, m_tileSize.y);
		(*m_vertexes)[2].position = m_position + sf::Vector2f(m_tileSize.x, 0.f);
		(*m_vertexes)[3].position = (*m_vertexes)[1].position;
		(*m_vertexes)[4].position = (*m_vertexes)[2].position;
		(*m_vertexes)[5].position = m_position + m_tileSize;
		for (auto& sommet : *m_vertexes)
			sommet.color = m_colour;
		return;
	}

	int nbVertexes{ static_cast<int>(std::ceil(m_tileSize.x / m_subTextures->at(m_subTextureIndex).width * m_scale.x)
	 * std::ceil(m_tileSize.y / m_subTextures->at(m_subTextureIndex).height * m_scale.y)
	 * 6) }; //Pourquoi 6? Car il s'agit du nombre de points pour faire une tuile en carr�.
	int difference{ nbVertexes - static_cast<int>(m_tileVertexesCount) }; // On met dans cet ordre pour que si le nombre est plus petit, le vector rappetisse
	if (difference < 0 && m_tileIndex < m_beginTiles->size() - 1)
	{
		for (std::size_t i{ m_beginTiles->at(m_tileIndex + 1) }; i < m_vertexes->size(); ++i)
			m_vertexes->at(i + difference) = m_vertexes->at(i);
		m_vertexes->resize(m_vertexes->size() - m_tileVertexesCount + nbVertexes);
		m_tileVertexesCount = nbVertexes;
	}
	else if (difference > 0 && m_tileIndex < m_beginTiles->size() - 1)
	{
		m_vertexes->resize(m_vertexes->size() + difference);
		for (std::size_t i{ m_vertexes->size() }; i > m_beginTiles->at(m_tileIndex + 1); --i)
			m_vertexes->at(i) = m_vertexes->at(i - difference);
		m_tileVertexesCount = nbVertexes;
	}
	sf::Vector2f coinGaucheSommet;
	std::size_t indexSommet{ m_beginTiles->at(m_tileIndex) };
	sf::Vector2f textureSize{m_subTextures->at(m_subTextureIndex).width, m_subTextures->at(m_subTextureIndex).height};
	sf::Vector2f texturePosition{m_subTextures->at(m_subTextureIndex).left, m_subTextures->at(m_subTextureIndex).top};

	if (m_textureRule == TextureRule::fill_space)
	{
		m_scale.x = m_tileSize.x / textureSize.x;
		m_scale.y = m_tileSize.y / textureSize.y;
	}

	// En th�orie, l'index devrait co�ncider avec la taille
	while (coinGaucheSommet.y < m_tileSize.y)
	{
		coinGaucheSommet.x = 0.f;
		for (; coinGaucheSommet.x < m_tileSize.x; coinGaucheSommet.x += textureSize.x * m_scale.x, indexSommet += 6)
		{
			(*m_vertexes)[indexSommet].position = coinGaucheSommet;
			(*m_vertexes)[indexSommet].texCoords = sf::Vector2f(texturePosition.x, 0.f);

			if (coinGaucheSommet.y + textureSize.y * m_scale.y > m_tileSize.y)
			{
				(*m_vertexes)[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, m_tileSize.y - coinGaucheSommet.y);
				(*m_vertexes)[indexSommet + 1].texCoords = sf::Vector2f(texturePosition.x, m_tileSize.y - coinGaucheSommet.y);
			}
			else
			{
				(*m_vertexes)[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, textureSize.y * m_scale.y);
				(*m_vertexes)[indexSommet + 1].texCoords = sf::Vector2f(texturePosition.x, textureSize.y);
			}

			if (coinGaucheSommet.x + textureSize.x * m_scale.x > m_tileSize.x)
			{
				(*m_vertexes)[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(m_tileSize.x - coinGaucheSommet.x, 0.f);
				(*m_vertexes)[indexSommet + 2].texCoords = sf::Vector2f(texturePosition.x + (m_tileSize.x - coinGaucheSommet.x), 0.f);
			}
			else
			{
				(*m_vertexes)[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(textureSize.x * m_scale.x, 0.f);
				(*m_vertexes)[indexSommet + 2].texCoords = sf::Vector2f(texturePosition.x + textureSize.x - 1.f, 0.f);
			}

			(*m_vertexes)[indexSommet + 3] = (*m_vertexes)[indexSommet + 1];
			(*m_vertexes)[indexSommet + 4] = (*m_vertexes)[indexSommet + 2];

			(*m_vertexes)[indexSommet + 5].position = sf::Vector2f((*m_vertexes)[indexSommet + 4].position.x, (*m_vertexes)[indexSommet + 3].position.y);
			(*m_vertexes)[indexSommet + 5].texCoords = sf::Vector2f((*m_vertexes)[indexSommet + 4].texCoords.x, (*m_vertexes)[indexSommet + 3].texCoords.y);

		}
		coinGaucheSommet.y += textureSize.y * m_scale.y;
	}

	for (int i{ 0 }; i < m_tileVertexesCount; ++i)
	{
		m_vertexes->at(m_beginTiles->at(m_tileIndex)).position += m_position;
		m_vertexes->at(m_beginTiles->at(m_tileIndex)).color = sf::Color::White;
	}
}

inline opt::Tile::Tile() : m_texture{ nullptr }, m_subTextureIndex{ 0 }, m_textureRule{ TextureRule::repeat_texture },
m_subTextures{ nullptr }, m_beginTiles{ nullptr }, m_tileIndex{ 0ull }, m_vertexes{ nullptr }
{}

inline opt::Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, TextureRule textureRule, const sf::Vector2f& scale, const	std::vector<sf::FloatRect>& subTextures) :
	m_texture{ &texture }, m_subTextureIndex{ noTuileDebutTexture }, m_textureRule{ textureRule },
	m_scale{ scale }, m_tileSize{ desiredSize }, m_subTextures{ &subTextures }, m_position{ position }
{
	intializeVertexes();
}


inline opt::Tile::Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize,
	const sf::Vector2f& position, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures) :
	m_texture{ &texture }, m_subTextureIndex{ noTuileDebutTexture }, //m_texturePosition{ texture.getSize().x / static_cast<float>(subTextureCount) * noTuileDebutTexture , 0.f },
	//m_textureSize{ static_cast<float>(texture.getSize().x / subTextureCount) , static_cast<float>(texture.getSize().y) },
	m_textureRule{ textureRule }, m_scale{ 1.f, 1.f }, m_tileSize{ desiredSize }, m_subTextures{ &subTextures }, m_position{ position }
{
	intializeVertexes();
}

inline float opt::Tile::height() const
{
	return m_tileSize.y;
}

inline float opt::Tile::width() const
{
	return m_tileSize.x;
}

inline const std::vector<sf::Vertex>& opt::Tile::vertexes() const
{
	return *m_vertexes;
}

inline std::vector<sf::Vertex>& opt::Tile::vertexes()
{
	return *m_vertexes;
}

inline sf::Vector2f opt::Tile::topLeftCorner() const
{
	return m_position;
}

inline sf::Vector2f opt::Tile::topRightCorner() const
{
	return m_position + sf::Vector2f(this->width(), 0.f);
}

inline sf::Vector2f opt::Tile::bottomLeftCorner() const
{
	return m_position + sf::Vector2f(0.f, this->height());
}

inline sf::Vector2f opt::Tile::bottomRightCorner() const
{
	return m_position + this->m_tileSize;
}

inline sf::Vector2f opt::Tile::getPosition() const
{
	return topLeftCorner();
}

inline void opt::Tile::setScale(const sf::Vector2f& scale)
{
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileSize.x *= scale.x;
		m_tileSize.y *= scale.y;
		break;
	case TextureRule::keep_height:
		m_scale.x *= scale.y;
		m_scale.y *= scale.y;
		m_tileSize.x *= scale.y;
		m_tileSize.y *= scale.y;
		break;
	case TextureRule::keep_width:
		m_scale.x *= scale.x;
		m_scale.y *= scale.x;
		m_tileSize.x *= scale.x;
		m_tileSize.y *= scale.x;
		break;
	case TextureRule::keep_size:
		m_scale.x *= scale.x;
		m_scale.y *= scale.y;
		break;
	case TextureRule::adjustable_size:
		m_scale.x *= scale.x;
		m_scale.y *= scale.y;
		m_tileSize.x *= scale.x;
		m_tileSize.y *= scale.y;
		break;
	}
	intializeVertexes();
}

inline void opt::Tile::setScale(float scale)
{
	switch (m_textureRule)
	{
	case TextureRule::keep_height:
	case TextureRule::keep_width:
	case TextureRule::adjustable_size:
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileSize *= scale;
		break;
	case TextureRule::keep_size:
		m_scale *= scale;
		break;
	}
	intializeVertexes();
}

inline void opt::Tile::setScale(float x, float y)
{
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileSize.x *= x;
		m_tileSize.y *= y;
		break;
	case TextureRule::adjustable_size:
		m_tileSize.x *= x;
		m_tileSize.y *= y;
		m_scale.x *= x;
		m_scale.y *= y;
		break;
	case TextureRule::keep_height:
		m_tileSize.x *= y;
		m_tileSize.y *= y;
		m_scale.x *= y;
		m_scale.y *= y;
		break;
	case TextureRule::keep_width:
		m_tileSize.x *= x;
		m_tileSize.y *= x;
		m_scale.x *= x;
		m_scale.y *= x;
		break;
	case TextureRule::keep_size:
		m_scale.x *= x;
		m_scale.y *= y;
		break;
	}
	intializeVertexes();
}

inline void opt::Tile::setScale(const sf::Vector2f& scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

inline void opt::Tile::setScale(float scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

inline void opt::Tile::setScale(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(x, y);
}

inline void opt::Tile::resize(const sf::Vector2f& size)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileSize = size;
		break;
	case TextureRule::keep_height:
		scale = size.y / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = size.x / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

inline void opt::Tile::resize(float x, float y)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileSize = sf::Vector2f(x, y);
		break;
	case TextureRule::keep_height:
		scale = y / m_tileSize.y;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = x / m_tileSize.x;
		m_tileSize *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

inline void opt::Tile::resize(const sf::Vector2f& size, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(size);
}

inline void opt::Tile::resize(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(x, y);
}

inline void opt::Tile::changeTextureRect(int numberSubTexture)
{
	if (numberSubTexture >= 0 && numberSubTexture < m_subTextures->size())
	{
		m_subTextureIndex = numberSubTexture;
		//m_texturePosition = sf::Vector2f(m_textureSize.x / *m_textureCount * m_subTextureIndex, 0.f);
		intializeVertexes();
	}
}

inline void opt::Tile::setTextureRule(TextureRule textureRule)
{
	m_textureRule = textureRule;
}

inline opt::TextureRule opt::Tile::getTextureRule()
{
	return m_textureRule;
}

inline void opt::Tile::move(const sf::Vector2f& offset)
{
	for (auto& sommet : *m_vertexes)
		sommet.position += offset;
}

inline void opt::Tile::move(float offsetX, float offsetY)
{
	sf::Vector2f offset{ offsetX, offsetY };
	for (auto& sommet : *m_vertexes)
		sommet.position += offset;
}

inline void opt::Tile::setPosition(const sf::Vector2f& position)
{
	sf::Vector2f deplacement{position - m_position};
	m_position = position;
	for (auto& sommet : *m_vertexes)
		sommet.position += deplacement;
}

inline void opt::Tile::setPosition(float x, float y)
{
	sf::Vector2f deplacement{sf::Vector2f(x, y) - m_position};
	m_position = sf::Vector2f(x, y);
	for (auto& sommet : *m_vertexes)
		sommet.position += deplacement;
	//m_position = sf::Vector2f(x, y);
	//intializeVertexes();
}

inline void opt::Tile::reloadTexture()
{
	if (m_subTextureIndex >= m_subTextures->size())
		m_subTextureIndex = m_subTextures->size() - 1;
	changeTextureRect(m_subTextureIndex);
}

inline opt::Tile* opt::Tile::getThis()
{
	return this;
}

inline std::unique_ptr<opt::Tile> opt::Tile::clone() const
{
	return std::make_unique<opt::Tile>(*this);
}

inline float opt::Tile::subTextureHeight() const
{
	return m_subTextures->at(m_subTextureIndex).height;
}

inline float opt::Tile::subTextureWidth() const
{
	return m_subTextures->at(m_subTextureIndex).width;
}

inline sf::Vector2f opt::Tile::subTextureSize() const
{
	return sf::Vector2f(m_subTextures->at(m_subTextureIndex).width, m_subTextures->at(m_subTextureIndex).height);
}

inline void opt::Tile::changeColour(const sf::Color& colour)
{
	m_colour = colour;
	for (auto& sommet : *m_vertexes)
		sommet.color = m_colour;
}

inline void opt::Tile::resetColour()
{
	m_colour = sf::Color(0xFFFFFFFF);
	for (auto& sommet : *m_vertexes)
		sommet.color = m_colour;
}

inline sf::Color opt::Tile::getColour() const
{
	return m_colour;
}

inline sf::Vector2f opt::Tile::getSize() const
{
	return m_tileSize;
}

inline int opt::Tile::subTextureIndex() const
{
	return m_subTextureIndex;
}

std::size_t opt::Tile::vertexCount() const
{
	return m_tileVertexesCount;
}