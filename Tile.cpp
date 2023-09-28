#include "pch.h"
#include "Tile.h"

void opt::Tile::moveVertexes(int nbVertexes)
{
	m_vertexes->resize(m_vertexes->size() - m_tileVertexesCount + nbVertexes);

	int difference{ nbVertexes - static_cast<int>(m_tileVertexesCount) }; // On met dans cet ordre pour que si le nombre est plus petit, le vector rappetisse
	if (difference < 0 && m_tileIndex < m_beginTiles->size() - 1)
	{
		for (std::size_t i{ m_beginTiles->at(m_tileIndex + 1) }; i < m_vertexes->size(); ++i)
			m_vertexes->at(i + difference) = m_vertexes->at(i);
		m_vertexes->resize(m_vertexes->size() - m_tileVertexesCount + nbVertexes);
		for (std::size_t i{m_tileIndex + 1}; i < m_beginTiles->size(); ++i)
			m_beginTiles->at(i) += difference;
	}
	else if (difference > 0 && m_tileIndex < m_beginTiles->size() - 1)
	{
		m_vertexes->resize(m_vertexes->size() + difference);
		for (std::size_t i{ m_vertexes->size() - 1 }; i > m_beginTiles->at(m_tileIndex + 1) + difference; --i)
			m_vertexes->at(i) = m_vertexes->at(i - difference);
		for (std::size_t i{m_tileIndex + 1}; i < m_beginTiles->size(); ++i)
			m_beginTiles->at(i) += difference;
	}
	m_tileVertexesCount = nbVertexes;
}

void opt::Tile::intializeVertexes()
{
	if (!m_subTextures || m_tileRect.getSize() == sf::Vector2f())
	{
		const sf::Vector2f& position{ m_tileRect.getPosition() };
		moveVertexes(6);
		m_textureRule = TextureRule::fill_space;
		(*m_vertexes)[m_beginTiles->at(m_tileIndex)].position = position;
		(*m_vertexes)[m_beginTiles->at(m_tileIndex) + 1].position = position + sf::Vector2f(0.f, m_tileRect.height);
		(*m_vertexes)[m_beginTiles->at(m_tileIndex) + 2].position = position + sf::Vector2f(m_tileRect.width, 0.f);
		(*m_vertexes)[m_beginTiles->at(m_tileIndex) + 3].position = (*m_vertexes)[m_beginTiles->at(m_tileIndex) + 1].position;
		(*m_vertexes)[m_beginTiles->at(m_tileIndex) + 4].position = (*m_vertexes)[m_beginTiles->at(m_tileIndex) + 2].position;
		(*m_vertexes)[m_beginTiles->at(m_tileIndex) + 5].position = position + m_tileRect.getSize();
		for (unsigned long long i{ 0 }; i < m_tileVertexesCount; ++i)
			(*m_vertexes)[i + m_beginTiles->at(m_tileIndex)].color = m_colour;
		return;
	}

	sf::Vector2f coinGaucheSommet;
	std::size_t indexSommet{ m_beginTiles->at(m_tileIndex) };
	sf::Vector2f textureSize{m_subTextures->at(m_subTextureIndex).width, m_subTextures->at(m_subTextureIndex).height};
	sf::Vector2f texturePosition{m_subTextures->at(m_subTextureIndex).left, m_subTextures->at(m_subTextureIndex).top};

	if (m_textureRule == TextureRule::fill_space)
	{
		m_scale.x = m_tileRect.width / textureSize.x;
		m_scale.y = m_tileRect.height / textureSize.y;
	}

	const int nbVertexes{ static_cast<int>(std::ceil(m_tileRect.width / (m_subTextures->at(m_subTextureIndex).width * m_scale.x))
		* std::ceil(m_tileRect.height / (m_subTextures->at(m_subTextureIndex).height * m_scale.y))
		* 6) }; // Pourquoi on multiplie par 6? Car il faut 6 sommets pour faire un carré de tuile
	moveVertexes(nbVertexes);

	// En théorie, l'index devrait coïncider avec la taille
	while (coinGaucheSommet.y < m_tileRect.height)
	{
		coinGaucheSommet.x = 0.f;
		for (; coinGaucheSommet.x < m_tileRect.width; coinGaucheSommet.x += textureSize.x * m_scale.x, indexSommet += 6)
		{
			(*m_vertexes)[indexSommet].position = coinGaucheSommet;
			(*m_vertexes)[indexSommet].texCoords = sf::Vector2f(texturePosition.x, 0.f);

			if (coinGaucheSommet.y + textureSize.y * m_scale.y > m_tileRect.height)
			{
				(*m_vertexes)[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, m_tileRect.height - coinGaucheSommet.y);
				(*m_vertexes)[indexSommet + 1].texCoords = sf::Vector2f(texturePosition.x, m_tileRect.height - coinGaucheSommet.y);
			}
			else
			{
				(*m_vertexes)[indexSommet + 1].position = coinGaucheSommet + sf::Vector2f(0.f, textureSize.y * m_scale.y);
				(*m_vertexes)[indexSommet + 1].texCoords = sf::Vector2f(texturePosition.x, textureSize.y);
			}

			if (coinGaucheSommet.x + textureSize.x * m_scale.x > m_tileRect.width)
			{
				(*m_vertexes)[indexSommet + 2].position = coinGaucheSommet + sf::Vector2f(m_tileRect.width - coinGaucheSommet.x, 0.f);
				(*m_vertexes)[indexSommet + 2].texCoords = sf::Vector2f(texturePosition.x + (m_tileRect.width - coinGaucheSommet.x), 0.f);
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

	for (unsigned long long i{ 0 }; i < m_tileVertexesCount; ++i)
	{
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).position += m_tileRect.getPosition();
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).color = m_colour;
	}
}

opt::Tile::Tile() : m_subTextureIndex{ -1 }, m_textureRule{ TextureRule::repeat_texture },
m_subTextures{ nullptr }, m_beginTiles{ nullptr }, m_tileIndex{ 0ull }, m_vertexes{ nullptr }, m_tileVertexesCount{ 0ull }
{}

opt::Tile::Tile(std::vector<std::size_t>& beginTiles, std::vector<sf::Vertex>& vertices) :
	m_beginTiles{ &beginTiles }, m_vertexes{ &vertices }, m_tileVertexesCount{ 0ull }, m_tileIndex{ m_beginTiles->size() },
	m_textureRule{ TextureRule::repeat_texture }, m_subTextureIndex{ -1 }, m_colour{ sf::Color(0xFFFFFFFF) }
{
	m_beginTiles->push_back(m_vertexes->size());
}

opt::Tile::Tile(int noTuileDebutTexture, const sf::FloatRect& tileRect, TextureRule textureRule,
	const std::vector<sf::FloatRect>& subTextures, std::vector<std::size_t>& beginTiles,
	std::vector<sf::Vertex>& vertices, sf::Vector2f scale) :
	m_subTextureIndex{ noTuileDebutTexture }, m_textureRule{ textureRule }, m_scale{ scale },
	m_tileRect{ tileRect }, m_subTextures{ &subTextures }, m_tileVertexesCount{ 0ull },
	m_beginTiles{ &beginTiles }, m_vertexes{ &vertices },
	m_tileIndex{ m_beginTiles->size() }, m_colour{ sf::Color(0xFFFFFFFF) }
{
	m_beginTiles->push_back(m_vertexes->size());
	intializeVertexes();
}

float opt::Tile::height() const
{
	return m_tileRect.height;
}

float opt::Tile::width() const
{
	return m_tileRect.width;
}

const std::vector<sf::Vertex>& opt::Tile::vertexes() const
{
	return *m_vertexes;
}

std::vector<sf::Vertex>& opt::Tile::vertexes()
{
	return *m_vertexes;
}

sf::Vector2f opt::Tile::topLeftCorner() const
{
	return m_tileRect.getPosition();
}

sf::Vector2f opt::Tile::topRightCorner() const
{
	return m_tileRect.getPosition() + sf::Vector2f(this->width(), 0.f);
}

sf::Vector2f opt::Tile::bottomLeftCorner() const
{
	return m_tileRect.getPosition() + sf::Vector2f(0.f, this->height());
}

sf::Vector2f opt::Tile::bottomRightCorner() const
{
	return m_tileRect.getPosition() + m_tileRect.getSize();
}

sf::Vector2f opt::Tile::getPosition() const
{
	sf::Vector2f position {m_tileRect.getPosition()};
	position.x *= m_centerPositionScale.x;
	position.y *= m_centerPositionScale.y;
	return position;
}

void opt::Tile::setScale(const sf::Vector2f& scale)
{
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileRect.width *= scale.x;
		m_tileRect.height *= scale.y;
		break;
	case TextureRule::keep_height:
		m_scale.x *= scale.y;
		m_scale.y *= scale.y;
		m_tileRect.width *= scale.y;
		m_tileRect.height *= scale.y;
		break;
	case TextureRule::keep_width:
		m_scale.x *= scale.x;
		m_scale.y *= scale.x;
		m_tileRect.width *= scale.x;
		m_tileRect.height *= scale.x;
		break;
	case TextureRule::keep_size:
		m_scale.x *= scale.x;
		m_scale.y *= scale.y;
		break;
	case TextureRule::adjustable_size:
		m_scale.x *= scale.x;
		m_scale.y *= scale.y;
		m_tileRect.width *= scale.x;
		m_tileRect.height *= scale.y;
		break;
	}
	intializeVertexes();
}

void opt::Tile::setScale(float scale)
{
	switch (m_textureRule)
	{
	case TextureRule::keep_height:
	case TextureRule::keep_width:
	case TextureRule::adjustable_size:
		m_tileRect.height *= scale;
		m_tileRect.width *= scale;
		m_scale *= scale;
		break;
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileRect.height *= scale;
		m_tileRect.width *= scale;
		break;
	case TextureRule::keep_size:
		m_scale *= scale;
		break;
	}
	intializeVertexes();
}

void opt::Tile::setScale(float x, float y)
{
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::fill_space:
		m_tileRect.width *= x;
		m_tileRect.height *= y;
		break;
	case TextureRule::adjustable_size:
		m_tileRect.width *= x;
		m_tileRect.height *= y;
		m_scale.x *= x;
		m_scale.y *= y;
		break;
	case TextureRule::keep_height:
		m_tileRect.width *= y;
		m_tileRect.height *= y;
		m_scale.x *= y;
		m_scale.y *= y;
		break;
	case TextureRule::keep_width:
		m_tileRect.width *= x;
		m_tileRect.height *= x;
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

void opt::Tile::setScale(const sf::Vector2f& scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

void opt::Tile::setScale(float scale, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(scale);
}

void opt::Tile::setScale(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	setScale(x, y);
}

void opt::Tile::resize(const sf::Vector2f& size)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileRect.width = size.x;
		m_tileRect.height = size.y;
		break;
	case TextureRule::keep_height:
		scale = size.y / m_tileRect.height;
		m_tileRect.width *= scale;
		m_tileRect.height *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = size.x / m_tileRect.width;
		m_tileRect.width *= scale;
		m_tileRect.height *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

void opt::Tile::resize(float x, float y)
{
	float scale{};
	switch (m_textureRule)
	{
	case TextureRule::repeat_texture:
	case TextureRule::adjustable_size:
	case TextureRule::fill_space:
		m_tileRect.width = x;
		m_tileRect.height = y;
		break;
	case TextureRule::keep_height:
		scale = y / m_tileRect.height;
		m_tileRect.width *= scale;
		m_tileRect.height *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_width:
		scale = x / m_tileRect.width;
		m_tileRect.width *= scale;
		m_tileRect.height *= scale;
		m_scale *= scale;
		break;
	case TextureRule::keep_size:
		break;
	}
	intializeVertexes();
}

void opt::Tile::resize(const sf::Vector2f& size, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(size);
}

void opt::Tile::resize(float x, float y, TextureRule textureRule)
{
	m_textureRule = textureRule;
	resize(x, y);
}

void opt::Tile::changeTextureRect(int numberSubTexture)
{
	if (numberSubTexture >= 0 && numberSubTexture < m_subTextures->size())
	{
		if (m_subTextures->at(m_subTextureIndex).getSize() == m_subTextures->at(numberSubTexture).getSize())
		{
			sf::Vector2f deplacement{ m_subTextures->at(numberSubTexture).getPosition() - m_subTextures->at(m_subTextureIndex).getPosition()};
			for (int i{ 0 }; i < m_tileVertexesCount; ++i)
				(*m_vertexes)[i + m_beginTiles->at(m_tileIndex)].texCoords += deplacement;
		}
		else
		{
			intializeVertexes();
		}
		m_subTextureIndex = numberSubTexture;
		//m_texturePosition = sf::Vector2f(m_textureSize.x / *m_textureCount * m_subTextureIndex, 0.f);
	}
}

void opt::Tile::setTextureRule(TextureRule textureRule)
{
	m_textureRule = textureRule;
}

opt::TextureRule opt::Tile::getTextureRule()
{
	return m_textureRule;
}

void opt::Tile::move(const sf::Vector2f& offset)
{
	for (std::size_t i {0}; i < m_tileVertexesCount; ++i)
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).position += offset;
}

void opt::Tile::move(float offsetX, float offsetY)
{
	sf::Vector2f offset{ offsetX, offsetY };
	for (std::size_t i {0}; i < m_tileVertexesCount; ++i)
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).position += offset;
}

void opt::Tile::setPosition(const sf::Vector2f& position)
{
	sf::Vector2f deplacement{position - m_tileRect.getPosition()};
	m_tileRect.left = position.x;
	m_tileRect.top = position.y;
	for (std::size_t i {0}; i < m_tileVertexesCount; ++i)
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).position += deplacement;
}

void opt::Tile::setPosition(float x, float y)
{
	sf::Vector2f deplacement{sf::Vector2f(x, y) - m_tileRect.getPosition()};
	m_tileRect.left = x;
	m_tileRect.top = y;
	for (std::size_t i {0}; i < m_tileVertexesCount; ++i)
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).position += deplacement;
}

void opt::Tile::reloadTexture()
{
	if (!m_subTextures)
		return;
	if (m_subTextureIndex >= m_subTextures->size())
		m_subTextureIndex = m_subTextures->size() - 1;
	changeTextureRect(m_subTextureIndex);
}

opt::Tile* opt::Tile::getThis()
{
	return this;
}

std::unique_ptr<opt::Tile> opt::Tile::clone() const
{
	return std::make_unique<opt::Tile>(*this);
}

float opt::Tile::subTextureHeight() const
{
	return m_subTextures->at(m_subTextureIndex).height;
}

float opt::Tile::subTextureWidth() const
{
	return m_subTextures->at(m_subTextureIndex).width;
}

sf::Vector2f opt::Tile::subTextureSize() const
{
	return sf::Vector2f(m_subTextures->at(m_subTextureIndex).width, m_subTextures->at(m_subTextureIndex).height);
}

void opt::Tile::changeColour(const sf::Color& colour)
{
	m_colour = colour;
	for (std::size_t i {0}; i < m_tileVertexesCount; ++i)
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).color = m_colour;
}

void opt::Tile::resetColour()
{
	m_colour = sf::Color(0xFFFFFFFF);
	for (std::size_t i{0}; i < m_tileVertexesCount; ++i)
		m_vertexes->at(m_beginTiles->at(m_tileIndex) + i).color = m_colour;
}

sf::Color opt::Tile::getColour() const
{
	return m_colour;
}

sf::Vector2f opt::Tile::getSize() const
{
	return m_tileRect.getSize();
}

int opt::Tile::subTextureIndex() const
{
	return m_subTextureIndex;
}

std::size_t opt::Tile::vertexCount() const
{
	return m_tileVertexesCount;
}

void opt::Tile::changePositionCenter(float scaleX, float scaleY)
{
	m_centerPositionScale = sf::Vector2f(scaleX, scaleY);
}
