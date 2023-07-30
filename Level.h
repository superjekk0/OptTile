#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include "OptTile.h"
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

namespace opt
{
	class OPTTILE_API Level : public sf::Drawable {
		private:
			std::vector<std::unique_ptr<Tile>> m_tiles;
			sf::Texture m_texture;						// Texture used by all tiles
			std::vector<std::size_t> m_beginTileIndex;	// Indicates the beginning index of vertexes of each tile
			std::vector<sf::Vertex> m_vertexes;			// Vector of the vertexes (by value copy) of the tiles. Should be used only for draw method and anything else that helps the rendering
			std::vector<sf::FloatRect> m_subTextures;	// Indicates every possible subtextures
		
			/// <summary>
			/// Indicates if we continue to update vertexes
			/// </summary>
			/// <param name="index">Index of the tile</param>
			/// <param name="itterator">Index inside the vector of vertexes</param>
			bool continueUpdate(std::size_t index, std::size_t itterator);
		
		public:
		
			/// <summary>
			/// Reloads the vertexes vector and the beginning index of the tiles for their vertexes. Usefull when many tiles have been modified without being updated by the level
			/// </summary>
			void reloadVertexes();
		
			/// <summary>
			/// Loads inside memory the desired texture et initialize the tile vector at 0 tile
			/// </summary>
			/// <param name="pPathTexture">Path used for the texture loading</param>
			/// <param name="NbTextures">Divides the texture in subtextures of same width and height</param>
			Level(const std::string& pPathTexture, std::size_t pNbTextures);
		
			/// <summary>
			/// Default constructor. Usefull when creating a structure that contains a Level
			/// </summary>
			Level();

			/// <summary>
			/// Returns a raw pointer of the tile at the specificated index
			/// </summary>
			/// <param name="index">Index de la tuile</param>
			opt::Tile* const operator[](int index);

			/// <summary>
			/// Modifies the level to make it become the other Level. Copies everything and move every unique_ptr
			/// </summary>
			/// <param name="other">The other Level</param>
			opt::Level& operator=(opt::Level&& other) noexcept;
		
			/// <summary>
			/// Draws the level on the SFML target element
			/// </summary>
			/// <param name="target">SFML target element of the render</param>
			/// <param name="states">States to add to the vertexes</param>
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
			/// <summary>
			/// Moves the specified tile by the index to the specified movement indicated in parameter, then updates vertexes 
			/// </summary>
			/// <param name="x">Horizontal movement</param>
			/// <param name="y">Vertical movement</param>
			/// <param name="index">Tile index</param>
			void move(float offsetX, float offsetY, std::size_t index);
		
			/// <summary>
			/// Moves the specified tile by the index to the specified movement indicated in parameter, then updates vertexes
			/// </summary>
			/// <param name="offset">Movement to do</param>
			/// <param name="index">Tile index</param>
			void move(const sf::Vector2f& offset, std::size_t index);

			/// <summary>
			/// Moves the specified tile by the index to the position indicated in parameter, then updates vertexes
			/// </summary>
			/// <param name="x">New horizontal position</param>
			/// <param name="y">New vertical position</param>
			/// <param name="index">Tile index</param>
			void setPosition(float x, float y, std::size_t index);

			/// <summary>
			/// Moves the specified tile by the index to the position indicated in parameter, then updates vertexes
			/// </summary>
			/// <param name="position">New tile position</param>
			/// <param name="">Tile index</param>
			void setPosition(const sf::Vector2f& position, std::size_t index);
		
			/// <summary>
			/// Resizes the tile, then updates vertexes for rendering
			/// </summary>
			/// <param name="x">New horizontal size</param>
			/// <param name="y">New vertical size</param>
			/// <param name="index">Tile index</param>
			void resize(float x, float y, std::size_t index);
		
			/// <summary>
			/// Resizes the tile, then updates vertexes for rendering
			/// </summary>
			/// <param name="size">New tile size</param>
			/// <param name="index">Tile index</param>
			void resize(const sf::Vector2f& size, std::size_t index);
		
			/// <summary>
			/// Resizes the tile, changes the texture rule, then updates vertexes for rendering
			/// </summary>
			/// <param name="x">New horizontal size</param>
			/// <param name="y">New vertical size</param>
			/// <param name="textureRule">New tile's texture rule</param>
			/// <param name="index">Tile index</param>
			void resize(float x, float y, TextureRule textureRule, std::size_t index);
		
			/// <summary>
			/// Resizes the tile, changes the texture rule, then updates vertexes for rendering
			/// </summary>
			/// <param name="size">New size</param>
			/// <param name="textureRule">New tile's texture rule</param>
			/// <param name="index">Tile index</param>
			void resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index);
		
			/// <summary>
			/// Reloads the global texture at the indicated path and redefines the number of subtextures in equal tiles of same height and width 
			/// </summary>
			/// <param name="path">Texture's path</param>
			/// <param name="subTextureCount">Number of subtextures inside the texture</param>
			void loadTexture(const std::string& path, int subTextureCount);
		
			/// <summary>
			/// Reloads the global texture at the indicated path and the subtexture's rectangles
			/// </summary>
			/// <param name="path">Texture path</param>
			/// <param name="subTextures">Subtexture rectangles</param>
			void loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures);
		
			/// <summary>
			/// Reloads the global texture at the indicated path as well as the subtexture's rectangles
			/// </summary>
			/// <param name="path">Texture path</param>
			/// <param name="subTexturePath">Subtexture path. The subtexture rectangle must be formatted this way: "left,top,width,height"</param>
			void loadTexture(const std::string& path, const std::string& subTexturePath);
		
			/// <summary>
			/// Reloads the global texture at the indicated path and redefines the number of subtextures in equals subtexture's rectangles
			/// </summary>
			/// <param name="texture">New global texture</param>
			/// <param name="subTextureCount">Number of subtextures inside the new global texture</param>
			void loadTexture(const sf::Texture& texture, int subTextureCount);
		
			/// <summary>
			/// Reloads the global texture from the indicated texture and reloads subtexture's rectangles from an existing vector
			/// </summary>
			/// <param name="texture">New global texture</param>
			/// <param name="subTextures">Rectangles defining the subtextures</param>
			void loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures);
		
			/// <summary>
			/// Reloads the global from an already present texture insie memory and reads dimensions of subtexture's rectangles from a file
			/// </summary>
			/// <param name="texture">Texture reference used to copy</param>
			/// <param name="subTexturePath">Subtexture file path. The file must be formatted that way: "left,top,width,height"</param>
			void loadTexture(const sf::Texture& texture, const std::string& subTexturePath);
		
			/// <summary>
			/// Resets the vertexes' vector (for rendering) and the tile vector
			/// </summary>
			void resetTiles();
		
			/// <summary>
			/// Adds a tile to the tiles' vector. It allows to add derived objects inside the vector
			/// </summary>
			/// <param name="tile">Tile to add</param>
			void add(const Tile& tile);
		
			/// <summary>
			/// Constructs and add a new Tile inside the tiles' vector
			/// </summary>
			/// <param name="size">Size of the new tile</param>
			/// <param name="position">Position of the new tile</param>
			/// <param name="numberSubTexture">Number of subtexture</param>
			/// <param name="textureRule">Texture rule applied to the tile. Check documentation for more details</param>
			void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule);
		
			/// <summary>
			/// Constructs and add a new tile inside the tiles' vector
			/// </summary>
			/// <param name="size">Size of the new tile</param>
			/// <param name="position">Position of the new tile</param>
			/// <param name="numberSubTexture">Number of subtexture</param>
			/// <param name="textureRule">Texture rule applied to the tile. Check documentation for more details</param>
			/// <param name="scale">Scale applied to the texture</param>
			void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, const sf::Vector2f& scale);
		
			/// <summary>
			/// Returns a pointer of a derived object
			/// </summary>
			/// <typeparam name="T">Type of the derived object</typeparam>
			/// <param name="index">Tile index</param>
			/// <returns>If null, the object is not this type</returns>
			template <class T>
			T* const derivedPointer(int index);
		
			/// <summary>
			/// Returns a constant reference of the utilised texture for the level
			/// </summary>
			const sf::Texture& getTexture() const;
		
			/// <summary>
			/// Returns a reference of the subtextures' vector. Should only be used to make derived objects
			/// </summary>
			const std::vector<sf::FloatRect>& getSubTextures() const;
		
			/// <summary>
			/// Changes subtexture rectangle (for example, animate) and updates vertexes. It might be possible that the method is not optimised
			/// </summary>
			/// <param name="numberTexture">Number of the new subtexture</param>
			/// <param name="index">Tile index</param>
			void changeTextureRect(int numberTexture, int index);
		
			/// <summary>
			/// Changes the color of vertexes of a tile
			/// </summary>
			/// <param name="color">New tile color (to apply a colour effect)</param>
			/// <param name="index">Tile index</param>
			void changeColor(const sf::Color& color, int index);
		
			/// <summary>
			/// Resets the vertexes' colour of the tile (gets back to colour 0xFFFFFFFF)
			/// </summary>
			/// <param name="index">Tile index</param>
			void resetColor(int index);
		
			/// <summary>
			/// Returns tile's colour
			/// </summary>
			/// <param name="index">Tile index</param>
			sf::Color getColor(int index) const;
		
			/// <summary>
			/// Returns the subtexture rectangle indicated by the index
			/// </summary>
			/// <param name="index">Subtexture index</param>
			sf::FloatRect getSubTexture(int index) const;
		
			/// <summary>
			/// Returns the subtexture's size indicated by the index
			/// </summary>
			/// <param name="index">Subtexture index</param>
			sf::Vector2f getSubTextureSize(int index) const;
		
			/// <summary>
			/// Returns the number of tiles inside the level
			/// </summary>
			std::size_t size() const;
	};

	template <class T>
	inline T* const opt::Level::derivedPointer(int index)
	{
		return dynamic_cast<T*>(m_tiles[index].get());
	}
}

#endif