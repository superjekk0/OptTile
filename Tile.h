#pragma once

#ifndef TILE_H
#define TILE_H

#include "OptTile.h"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <memory>

namespace opt
{
	enum class OPTTILE_API TextureRule {
		repeat_texture,		// The texture is repeated inside the tile. Increasing the scale only increased the size of the tile, without inflating the texture zoom
		keep_height,		// The texture is transformed by taking first the height. Eg: a tile begins with the scale 1:1 (100x100), then becomes 2:1, it will be 1:1, because height has the priority. The texture rectangle is not affected (if a texture is repeated twice, it will stay repeated twice)
		keep_width,			// The texture is transformed by taking first the width. Eg: a tile begins with the scale 1:1 (100x100), then becomes 2:1. The tile will have the scale 2:2 (200x200), as width has the priority. The texture rectangle is not affected (if a texture is repeated twice, it will stay repeated twice)
		keep_size,			// The texture will be more zoomed, but the object will keep the same size
		adjustable_size,	// No attempt is made to keep the same size. Everything will be transformed; the texture is zoomed, the tile is bigger.
		fill_space			// The texture takes the entirety of the tile, without repeat. The size has priority over its scale
	};

	class OPTTILE_API Tile {
	protected:
		std::shared_ptr<const sf::Texture> m_texture;						// The inherited texture from the class containing the object
		sf::Vector2f m_position;											// Gives the position at the top-left corner of the tile
		sf::Vector2f m_tileSize;											// Gives the tile's size
		std::vector<sf::Vertex> m_vertexes;									// Entirety of vertexes composing the tile
		TextureRule m_textureRule;											// Texture rule dictating the texture's behaviour when resized or rescaled
		sf::Vector2f m_scale{ 1.f, 1.f };									// Indicates the zoom of the subtexture (1:1 by default)
		int m_numberSubTexture;												// Indicates the subtexture number (usefull when texture is changed)
		std::shared_ptr<const std::vector<sf::FloatRect>> m_subTextures;	// Indicates subtexture's rectangles (they may be asymetrical)
		sf::Color m_color;													// Indicates the vertexes colour

		void intializeVertexes();
	public:

		/// <summary>
		/// This constructor should not be used for other reasons than changing the tiles' vector size
		/// </summary>
		Tile();

		/// <summary>
		/// This constructor puts by default the scale at 1:1 (even in case when the subtexture in not squre)
		/// </summary>
		/// <param name="texture">Reference of the utilised texture (most likely coming from the level)</param>
		/// <param name="noTuileDebutTexture">Number of the used texture</param>
		/// <param name="desiredSize">Tile size</param>
		/// <param name="position">Position at top left corner</param>
		/// <param name="subTextureCount">Number of subtextures</param>
		/// <param name="textureRule">Rule applied to the subtexture</param>
		/// <param name="subTextures">Positions and sizes of the subtextures</param>
		Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures);

		/// <summary>
		/// Constructor intializing the zoom from the scale asked
		/// </summary>
		/// <param name="texture">Reference of the utilised texture (most likely coming from the level)</param>
		/// <param name="noTuileDebutTexture">Number of the used texture</param>
		/// <param name="desiredSize">Tile size</param>
		/// <param name="position">Position at top left corner</param>
		/// <param name="subTextureCount">Number of subtextures</param>
		/// <param name="textureRule">Rule applied to the subtexture</param>
		/// <param name="scale">Zoom applied to the texture</param>
		/// <param name="subTextures">Positions and sizes of the subtextures</param>
		Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, TextureRule textureRule, const sf::Vector2f& scale, const std::vector<sf::FloatRect>& subTextures);

		/// <summary>
		/// Returns a reference of the vertexes' vector (usefull to draw)
		/// </summary>
		const std::vector<sf::Vertex>& vertexes() const;

		/// <summary>
		/// Returns the tile's height
		/// </summary>
		float height() const;

		/// <summary>
		/// Returns the tile's width
		/// </summary>
		float width() const;

		/// <summary>
		/// Returns the coordinates of the top left corner
		/// </summary>
		sf::Vector2f topLeftCorner() const;

		/// <summary>
		/// Returns the coordinates of the top right corner
		/// </summary>
		sf::Vector2f topRightCorner() const;

		/// <summary>
		/// Returns the coordinates of the bottom left corner
		/// </summary>
		sf::Vector2f bottomLeftCorner() const;

		/// <summary>
		/// Returns the coordinates of the bottom right corner
		/// </summary>
		sf::Vector2f bottomRightCorner() const;

		/// <summary>
		/// Returns the coordinates of the top left corner
		/// </summary>
		sf::Vector2f getPosition() const;

		/// <summary>
		/// Puts the tile at the specified zoom according to the texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must be copied inside a vector.
		/// </summary>
		/// <param name="scale">Zoom factor</param>
		void setScale(const sf::Vector2f& scale);

		/// <summary>
		/// Puts the tile at the specified zoom according to the texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must be copied inside a vector.
		/// </summary>
		/// <param name="scale">Zoom factor applied vertically and horizontally</param>
		void setScale(float scale);

		/// <summary>
		/// Puts the tile at the specified zoom according to the texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must be copied inside a vector.
		/// </summary>
		/// <param name="x">Horizontal zoom factor</param>
		/// <param name="y">Vertical zoom factor</param>
		void setScale(float x, float y);

		/// <summary>
		/// Changes the texture rule, then puts the tile at the specified zoom according the new texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must copied inside a vector.
		/// </summary>
		/// <param name="scale">Zoom factor</param>
		/// <param name="textureRule">New texture rule</param>
		void setScale(const sf::Vector2f& scale, TextureRule textureRule);

		/// <summary>
		/// Changes the texture rule, then puts the tile at the specified zoom according the new texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must copied inside a vector.
		/// </summary>
		/// <param name="scale">Zoom factor applied vertically and horizontally</param>
		/// <param name="textureRule">New texture rule</param>
		void setScale(float scale, TextureRule textureRule);

		/// <summary>
		/// Changes the texture rule, then puts the tile at the specified zoom according the new texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must copied inside a vector.
		/// </summary>
		/// <param name="x">Horizontal zoom factor</param>
		/// <param name="y">Vertical zoom factor</param>
		/// <param name="textureRule">New texture rule</param>
		void setScale(float x, float y, TextureRule textureRule);

		/// <summary>
		/// If the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="size">New required size</param>
		void resize(const sf::Vector2f& size);

		/// <summary>
		/// If the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="x">New required horizontal size</param>
		/// <param name="y">New required vertical size</param>
		void resize(float x, float y);

		/// <summary>
		/// The texture rule is updated, then if the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="size">New required size</param>
		/// <param name="textureRule">New texture rule</param>
		void resize(const sf::Vector2f& size, TextureRule textureRule);

		/// <summary>
		/// The texture rule is updated, then if the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="x">New required horizontal size</param>
		/// <param name="y">New required vertical size</param>
		/// <param name="textureRule">New texture rule</param>
		void resize(float x, float y, TextureRule textureRule);

		/// <summary>
		/// Changes tile's subtexture
		/// </summary>
		/// <param name="numberSubTexture">Index of the subtexture</param>
		void changeTextureRect(int numberSubTexture);

		/// <summary>
		/// Changes the texture rule
		/// </summary>
		/// <param name="textureRule">New texture rule</param>
		void setTextureRule(TextureRule textureRule);

		/// <summary>
		/// Obtains the texture rule of the tile
		/// </summary>
		TextureRule getTextureRule();

		/// <summary>
		/// Moves the tile of the equivalent of the offset
		/// </summary>
		/// <param name="offset">Move to do in 2 dimensions</param>
		void move(const sf::Vector2f& offset);

		/// <summary>
		/// Moves the tile of the equivalent of the offset
		/// </summary>
		/// <param name="offsetX">Horizontal movement</param>
		/// <param name="offsetY">Vertical movement</param>
		void move(float offsetX, float offsetY);

		/// <summary>
		/// Puts the top left corner at the position entered in parameter
		/// </summary>
		/// <param name="position">New position</param>
		void setPosition(const sf::Vector2f& position);

		/// <summary>
		/// Puts the top left corner at the position entered in parameter
		/// </summary>
		/// <param name="x">New horizontal position</param>
		/// <param name="y">New vertical position</param>
		void setPosition(float x, float y);

		/// <summary>
		/// Reloads vertexes to make sure it follows a subtexture that exists. If the texture number overwhelm the maximum value, the subtexture will be the subtexture at the last index.
		/// </summary>
		void reloadTexture();

		/// <summary>
		/// Allows to get the address of the object
		/// </summary>
		virtual Tile* getThis();

		/// <summary>
		/// Creates a clone of the object wich can contain the derived methods as well as their unique members. To clone a derived object, overload this method.
		/// </summary>
		virtual std::unique_ptr<Tile> clone() const;

		/// <summary>
		/// Gives the subtexture's height (usefull when want only a few times its height)
		/// </summary>
		float subTextureHeight() const;

		/// <summary>
		/// Gives the subtexture's width (usefull when want only a few times its width)
		/// </summary>
		float subTextureWidth() const;

		/// <summary>
		/// Gives the size of the subtexture
		/// </summary>
		sf::Vector2f subTextureSize() const;

		/// <summary>
		/// Changes the tile's colour
		/// </summary>
		/// <param name="color">New colour to apply</param>
		void changeColour(const sf::Color& color);

		/// <summary>
		/// Resets the tile's colour (puts the colour 0xFFFFFFFF)
		/// </summary>
		void resetColour();

		/// <summary>
		/// Returns the tile's colour
		/// </summary>
		sf::Color getColour() const;

		/// <summary>
		/// Returns the tile's size
		/// </summary>
		sf::Vector2f getSize() const;

		/// <summary>
		/// Returns the index used for subtexture by the tile
		/// </summary>
		int subTextureIndex() const;
	};
}
#endif 


