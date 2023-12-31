﻿///
/// Version de la bibliothèque datant de 2023 - Library writen in 2023
/// 
/// ========================================================================================
/// ========================================================================================
/// 
///	Fait par Charles Mandziuk. Aucune mention n'est nécessaire, mais est toujours appréciée
/// 
/// Writen by Charles Mandziuk. No any mention is mendatory, but is always appreciated
///

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
	/// <summary>
	/// Retourne une liste de chaînes de caractères après une filtration
	/// </summary>
	/// <param name="str">Chaîne de caractères de base</param>
	/// <param name="separator">Caractère servant à séparer</param>
	std::vector<std::string> splitString(std::string str, const char separator);

	template <typename T>
	T parse(const std::string& line);

	/// <summary>
	/// Permet de référencer un objet d'un type générique. Fonctionne même dans des vectors (normalement interdit par le compilateur) ///
	/// Allows to reference an object of a generic type. Works even in vectors, which normally forbid reference-type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class OPTTILE_API Reference {
	private:
		T* m_reference;

	public:

		/// <summary>
		/// Crée une référence vide ///
		/// Creates a null reference, that needs to be initialized
		/// </summary>
		Reference() : m_reference{ nullptr }
		{}

		/// <summary>
		/// Crée une référence à un objet déjà existant ///
		/// Create a reference to an already existing object
		/// </summary>
		/// <param name="reference">Variable à référencer /// Variable to reference</param>
		Reference(T& reference) : m_reference{ &reference }
		{}

		/// <summary>
		/// Assigne la référence à l'adresse de l'autre référence ou assigne la valeur de la référence à partir de l'autre référence ///
		/// Assigns the reference to the other's reference or assigns the other reference's value to the reference
		/// </summary>
		/// <param name="other">Autre référence utilisée /// Other utilised reference</param>
		Reference& operator= (const Reference& other)
		{
			m_reference = other.m_reference;
			return *this;
		}

		/// <summary>
		/// Assigne la référence à l'adresse d'une variable référée ou assigne la valeur de la variable référée dans la référence ///
		/// Assign the address' reference from a referenced variable or assigns its value inside the class
		/// </summary>
		/// <param name="reference">Variable référencée /// Referenced variable</param>
		Reference& operator=(T& reference)
		{
			if (!m_reference)
				m_reference = &reference;
			else
				*m_reference = reference;
			return *this;
		}

		/// <summary>
		/// Assigne une valeur à la référence si une adresse existe ///
		/// Assigns a value if an address exists for the reference
		/// </summary>
		/// <param name="value">Valeur à assigner à la référence /// Value to assign to the reference</param>
		Reference& operator=(const T&& value)
		{
			if (m_reference)
				*m_reference = value;
			return *this;
		}

		/// <summary>
		/// Indique si la valeur de la référence correspond à la valeur d'une autre référence ///
		/// Indicates if the reference's value is equal to another reference's value
		/// </summary>
		/// <param name="other">Autre référence /// The other reference</param>
		bool operator==(const Reference& other)
		{
			return (*m_reference == *other.m_reference);
		}

		/// <summary>
		/// Retourne une référence constante de la variable référencée ///
		/// Returns a const reference of the referenced variable
		/// </summary>
		const T& get() const
		{
			return *m_reference;
		}

		/// <summary>
		/// Retourne une référence de la variable référencée ///
		/// Returns a reference of the referenced variable
		/// </summary>
		T& get()
		{
			return *m_reference;
		}

		/// <summary>
		/// Lorsqu'une méthode ou une fonction demande un argument de type T, la référence fera la conversion automatiquement ///
		/// When a method or a function asks for typed T argument, the reference will automatically make the conversion
		/// </summary>
		operator T& () const
		{
			if (!m_reference)
				throw "up";
			return *m_reference;
		}

	};

	class OPTTILE_API Level : public sf::Drawable {
	private:
		std::vector<std::unique_ptr<Tile>> m_tiles;
		// Texture utilisée pour toutes les cases ///
		// Texture used by all tiles
		sf::Texture m_texture;
		// Indique l'index de commencement des sommets de chaque tuile ///
		// Indicates the beginning index of vertexes of each tile
		std::vector<std::size_t> m_beginTileIndex;
		// Ensemble des sommets copiés par valeur des tuiles. À n'utiliser que pour la méthode draw et ce qui aide à faire le rendu ///
		// Vector of the vertexes (by value copy) of the tiles. Should be used only for draw method and anything else that helps the rendering
		std::vector<sf::Vertex> m_vertexes;
		// Indique les sous-textures possibles ///
		// Indicates every possible subtextures
		std::vector<sf::FloatRect> m_subTextures;
		// Sert à faire le rendu des sommets ///
		// Allows to make the render
		sf::VertexBuffer m_renderVertexes;
		// Indique si à chaque changement de tuile, il faut mettre à jour le buffer ///
		// Indicates if at each Tile change, it has to be updated.
		bool m_autoUpdate;

		/// <summary>
		/// Indique si on continue de mettre à jour les sommets ///
		/// Indicates if we continue to update vertexes
		/// </summary>
		/// <param name="index">Index de la tuile /// Tile index</param>
		/// <param name="itterator">Index dans la liste générique de sommets /// Index inside the vector of vertexes</param>
		bool continueUpdate(std::size_t index, std::size_t itterator);

		/// <summary>
		/// Indique l'emplacement de l'objet Tile ayant un changement dans ses sommets s'il y en a un
		/// </summary>
		//int vertexesChanges();
	public:

		/// <summary>
		/// Recharge la liste générique de sommets et l'index de départ des tuiles par rapport aux sommets. Utile lorsque plusieurs tuiles ont été modifiées directement ///
		/// Reloads the vertexes vector and the beginning index of the tiles for their vertexes. Usefull when many tiles have been modified without being updated by the level
		/// </summary>
		void reloadBuffer();

		/// <summary>
		/// Charge en mémoire la texture désirée et met le compteur de cases à 0 ///
		/// Loads inside memory the desired texture et initialize the tile vector at 0 tile
		/// </summary>
		/// <param name="pPathTexture">Chemin utilisé pour le chargement de la texture /// Path used for the texture loading</param>
		/// <param name="NbTextures">Diviser la texture en sous-textures de même hauteur et largeur /// Divides the texture in subtextures of same width and height</param>
		Level(const std::string& pPathTexture, std::size_t pNbTextures);

		/// <summary>
		/// Constructeur par défaut. Utile lors de la création d'une structure contenant un niveau ///
		/// Default constructor. Usefull when creating a structure that contains a Level
		/// </summary>
		Level();

		/// <summary>
		/// Détruit le niveau. Empêche la corruption du heap ///
		/// Classe's destructor. Usefull to prevent heap corruption
		/// </summary>
		~Level();

		/// <summary>
		/// Retourne une référence de la case à l'index spécifié ///
		/// Returns a reference of the tile at the specificated index
		/// </summary>
		/// <param name="index">Index de la tuile /// Tile index</param>
		Tile& operator[](int index);

		/// <summary>
		/// Retourne une référence de la case à l'index spécifié ///
		/// Returns a reference of the tile at the specificated index
		/// </summary>
		/// <param name="index">Index de la tuile /// Tile index</param>
		const Tile& operator[](int index) const;

		/// <summary>
		/// Modifie la classe Level pour la transformer en l'autre objet Level. Copie tout et bouge tous les unique_ptr /// 
		/// Modifies the Level to make it become the other Level. Copies everything and move every unique_ptr
		/// </summary>
		/// <param name="other">L'autre objet Level /// The other Level</param>
		Level& operator=(Level&& other) noexcept;

		/// <summary>
		/// Dessine le niveau sur l'élément SFML cible ///
		/// Draws the level on the SFML target element
		/// </summary>
		/// <param name="target">Élément SFML cible du rendu /// SFML target element of the render</param>
		/// <param name="states">États à ajouter aux vecteurs /// States to add to the vertexes</param>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		/// <summary>
		/// Bouge la tuile spécifiée à l'index au mouvement spécifié puis met à jour les sommets ///
		/// Moves the specified tile by the index to the specified movement indicated in parameter, then updates vertexes 
		/// </summary>
		/// <param name="x">Mouvement horizontal /// Horizontal movement</param>
		/// <param name="y">Mouvement vertical /// Vertical movement</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void move(float offsetX, float offsetY, std::size_t index);

		/// <summary>
		/// Bouge la tuile spécifiée par l'index au mouvement indiqué en paramètre puis met à jour les sommets /// 
		/// Moves the specified tile by the index to the specified movement indicated in parameter, then updates vertexes
		/// </summary>
		/// <param name="offset">Mouvement à effecture /// Movement to do</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void move(const sf::Vector2f& offset, std::size_t index);

		/// <summary>
		/// Bouge la tuile spécifiée par l'index à la position indiquée en paramètre, puis met à jour les sommets ///
		/// Moves the specified tile by the index to the position indicated in parameter, then updates vertexes
		/// </summary>
		/// <param name="x">Nouvelle position horizontale /// New horizontal position</param>
		/// <param name="y">Nouvelle position verticale /// New vertical position</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setPosition(float x, float y, std::size_t index);

		/// <summary>
		/// Bouge la tuile spécifiée par l'index à la position indiquée en paramètre, puis met à jour les sommets ///
		/// Moves the specified tile by the index to the position indicated in parameter, then updates vertexes
		/// </summary>
		/// <param name="position">Nouvelle position de la tuile /// New tile position</param>
		/// <param name="">Index de la tuile /// Tile index</param>
		void setPosition(const sf::Vector2f& position, std::size_t index);

		/// <summary>
		/// Permet de redimensionner la tuile et met à jour les sommets pour faire le rendu ///
		/// Resizes the tile, then updates vertexes for rendering
		/// </summary>
		/// <param name="x">Nouvelle taille horizontale /// New horizontal size</param>
		/// <param name="y">Nouvelle taille verticale /// New vertical size</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void resize(float x, float y, std::size_t index);

		/// <summary>
		/// Permet de redimensionner la tuile et met à jour les sommets pour faire le rendu ///
		/// Resizes the tile, then updates vertexes for rendering
		/// </summary>
		/// <param name="size">Nouvelle taille de la tuile /// New tile size</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void resize(const sf::Vector2f& size, std::size_t index);

		/// <summary>
		/// Permet de redimensionner la tuile, de changer la règle de texture et met à jour les sommets pour faire le rendu /// 
		/// Resizes the tile, changes the texture rule, then updates vertexes for rendering
		/// </summary>
		/// <param name="x">Nouvelle taille horizontale /// New horizontal size</param>
		/// <param name="y">Nouvelle taille verticale /// New vertical size</param>
		/// <param name="textureRule">Nouvelle règle de texture de la tuile /// New tile's texture rule</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void resize(float x, float y, TextureRule textureRule, std::size_t index);

		/// <summary>
		/// Permet de redimensionner la tuile, de changer la règle de texture et met à jour les sommets pour faire le rendu ///
		/// Resizes the tile, changes the texture rule, then updates vertexes for rendering
		/// </summary>
		/// <param name="size">Nouvelle taille /// New size</param>
		/// <param name="textureRule">Nouvelle règle de texture de la tuile /// New tile's texture rule</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void resize(const sf::Vector2f& size, TextureRule textureRule, std::size_t index);

		/// <summary>
		/// Recharge la texture globale au chemin indiqué et redéfinit les rectangles de sous-textures en rectangles de même largeur et hauteur ///
		/// Reloads the global texture at the indicated path and redefines the number of subtextures in equal tiles of same height and width 
		/// </summary>
		/// <param name="path">Chemin de la texture /// Texture's path</param>
		/// <param name="subTextureCount">Nombre de sous-textures dans la texture /// Number of subtextures inside the texture</param>
		void loadTexture(const std::string& path, int subTextureCount);

		/// <summary>
		/// Recharge la texture globale au chemin indiqué, redéfinit les rectangles de sous-textures ///
		/// Reloads the global texture at the indicated path and the subtexture's rectangles
		/// </summary>
		/// <param name="path">Chemin de la texture /// Texture path</param>
		/// <param name="subTextures">Rectangles de sous-textures /// Subtexture rectangles</param>
		void loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures);

		/// <summary>
		/// Recharge la texture globale au chemin indiqué ainsi que les sous-textures au chemin indiqué ///
		/// Reloads the global texture at the indicated path as well as the subtexture's rectangles
		/// </summary>
		/// <param name="path">Chemin de texture /// Texture path</param>
		/// <param name="subTexturePath">Chemin de la sous-texture. Le formattage est ainsi : "left,top,width,height" /// Subtexture path. The subtexture rectangle must be formatted this way: "left,top,width,height"</param>
		void loadTexture(const std::string& path, const std::string& subTexturePath);

		/// <summary>
		/// Recharge la texture gloable à la texture indiquée et redéfinis le nombre de sous-textures en rectangles égaux ///
		/// Reloads the global texture at the indicated path and redefines the number of subtextures in equals subtexture's rectangles
		/// </summary>
		/// <param name="texture">Nouvelle texture globale /// New global texture</param>
		/// <param name="subTextureCount">Nombre de sous-textures dans la nouvelle texture globale /// Number of subtextures inside the new global texture</param>
		void loadTexture(const sf::Texture& texture, int subTextureCount);

		/// <summary>
		/// Recharge la texture globale à la texture indiquée, redéfinis le nombre de sous-textures et les rectangles de sous-textures ///
		/// Reloads the global texture from the indicated texture and reloads subtexture's rectangles from an existing vector
		/// </summary>
		/// <param name="texture">Nouvelle texture globale /// New global texture</param>
		/// <param name="subTextures">Rectangles définissant les sous-textures /// Rectangles defining the subtextures</param>
		void loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures);

		/// <summary>
		/// Recharge la texture depuis une texture déjà en mémoire et lit les dimensions de chaque sous-texture ///
		/// Reloads the global from an already present texture insie memory and reads dimensions of subtexture's rectangles from a file
		/// </summary>
		/// <param name="texture">Référence de texture utilisée /// Texture reference used to copy</param>
		/// <param name="subTexturePath">Chemin du fichier de sous-textures. Le formattage doit se faire ainsi: "left,top,width,height" /// Subtexture file path. The file must be formatted that way: "left,top,width,height"</param>
		void loadTexture(const sf::Texture& texture, const std::string& subTexturePath);

		/// <summary>
		/// Change le facteur d'agrandissement de la tuile indiquée en paramètre ///
		/// Changes the zoom of the Tile indicated in parameter
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement/// Zoom factor</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setScale(const sf::Vector2f& scale, std::size_t index);

		/// <summary>
		/// Change le facteur d'agrandissement de la tuile indiquée en paramètre ///
		/// Changes the zoom of the Tile indicated in parameter
		/// </summary>
		/// <param name="x">Facteur d'agrandissement horizontal /// Horizontal zoom factor</param>
		/// <param name="y">Facteur d'agrandissement vertical /// Vertical zoom factor</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setScale(float x, float y, std::size_t index);

		/// <summary>
		/// Change le facteur d'agrandissement de la tuile indiquée en paramètre après avoir changé la règle de texture ///
		/// Changes the zoom of the Tile indicated in parameter after having changed the texture rule
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement /// Zoom factor</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setScale(const sf::Vector2f& scale, TextureRule textureRule, std::size_t index);

		/// <summary>
		/// Change le facteur d'agrandissement de la tuile indiquée en paramètre après avoir changé la règle de texture ///
		/// Changes the zoom of the Tile indicated in parameter after having changed the texture rule
		/// </summary>
		/// <param name="x">Facteur d'agrandissement horizontal /// Horizontal zoom factor</param>
		/// <param name="y">Facteur d'agrandissement vertical /// Vertical zoom factor</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setScale(float x, float y, TextureRule textureRule, std::size_t index);

		/// <summary>
		/// Change le facteur d'agrandissement de la tuile indiquée en paramètre après avoir changé la règle de texture ///
		/// Changes the zoom of the Tile indicated in parameter after having changed the texture rule
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement vertical et horizontal /// Horizontal and vertical zoom factor</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setScale(float scale, TextureRule textureRule, std::size_t index);

		/// <summary>
		/// Change le facteur d'agrandissement de la tuile indiquée en paramètre après avoir changé la règle de texture ///
		/// Changes the zoom of the Tile indicated in parameter after having changed the texture rule
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement vertical et horizontal /// Horizontal and vertical zoom factor</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void setScale(float scale, std::size_t index);

		/// <summary>
		/// Réinitialise la liste générique de sommets (pour le rendu) et la liste générique de tuiles /// 
		/// Resets the vertexes' vector (for rendering) and the tile vector
		/// </summary>
		void resetTiles();

		/// <summary>
		/// Rajoute un objet Tile au vector de Tile. Permet de rajouter dans le vector une classe dérivée de Tile ///
		/// Adds a tile to the tiles' vector. It allows to add derived objects inside the vector
		/// </summary>
		/// <param name="tile">Objet Tile à rajouter /// Tile to add</param>
		void add(const Tile& tile);

		/// <summary>
		/// Construit un nouvel objet Tile et le rajoute au vector de Tile ///
		/// Constructs and add a new Tile inside the tiles' vector
		/// </summary>
		/// <param name="size">Taille du nouvel objet Tile /// Size of the new tile</param>
		/// <param name="position">Position du nouvel objet Tile /// Position of the new tile</param>
		/// <param name="numberSubTexture">Nombre de sous-textures /// Number of subtexture</param>
		/// <param name="textureRule">Règle de texture appliquée à l'objet Tile. Vérifier la documentation pour plus de détails /// Texture rule applied to the tile. Check documentation for more details</param>
		void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule);

		/// <summary>
		/// Construit un nouvel objet Tile et le rajoute au vector de Tile ///
		/// Constructs and add a new tile inside the tiles' vector
		/// </summary>
		/// <param name="size">Taille du nouvel objet Tile /// Size of the new tile</param>
		/// <param name="position">Position du nouvel objet Tile /// Position of the new tile</param>
		/// <param name="numberSubTexture">Numéro de sous-texture /// Number of subtexture</param>
		/// <param name="textureRule">Règle de texture appliquée à l'objet. Vérifier la documentation pour plus d'informations /// Texture rule applied to the tile. Check documentation for more details</param>
		/// <param name="scale">Zoom appliqué à la texture /// Zoom applied to the texture</param>
		void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, TextureRule textureRule, const sf::Vector2f& scale);

		/// <summary>
		/// Retourne un pointeur d'un type dérivé ///
		/// Returns a pointer of a derived object
		/// </summary>
		/// <typeparam name="T">Type de l'objet dérivé /// Type of the derived object</typeparam>
		/// <param name="index">Index de l'objet /// Tile index</param>
		/// <returns>Si null, l'objet n'est pas de ce type /// If null, the object is not this type</returns>
		template <class T>
		T* const derivedPointer(int index);

		/// <summary>
		/// Retourne une référence constante de la texture utilisée dans le niveau ///
		/// Returns a constant reference of the utilised texture for the level
		/// </summary>
		const sf::Texture& getTexture() const;

		/// <summary>
		/// Retourne une référence de la liste générique. Ne devrait être utilisé que pour faire des objets dérivés de Tile ///
		/// Returns a reference of the subtextures' vector. Should only be used to make derived objects
		/// </summary>
		const std::vector<sf::FloatRect>& getSubTextures() const;

		/// <summary>
		/// Change le rectangle de texture (par exemple, pour faire de l'animation) et met à jour les sommets. Il est possible que la méthode ne soit pas optimisée ///
		/// Changes subtexture rectangle (for example, animate) and updates vertexes. It might be possible that the method is not optimised
		/// </summary>
		/// <param name="numberTexture">Numéro de la nouvelle sous-texture utilisée /// Number of the new subtexture</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void changeTextureRect(int numberTexture, int index);

		/// <summary>
		/// Change la couleur des sommets d'un objet Tile ///
		/// Changes the colour of vertexes of a tile
		/// </summary>
		/// <param name="color">Nouvelle couleur de la tuile (pour appliquer un effet de couleur) /// New tile color (to apply a colour effect)</param>
		/// <param name="index">Index de la tuile /// Tile index</param>
		void changeColour(const sf::Color& color, int index);

		/// <summary>
		/// Réinitialise la couleur des points d'une tuile (remet à la couleur 0xFFFFFFFF)///
		/// Resets the vertexes' colour of the tile (gets back to colour 0xFFFFFFFF)
		/// </summary>
		/// <param name="index">index de la tuile /// Tile index</param>
		void resetColour(int index);

		/// <summary>
		/// Retourne la couleur d'une tuile /// Returns tile's colour
		/// </summary>
		/// <param name="index">Index de la tuile /// Tile index</param>
		sf::Color getColour(int index) const;

		/// <summary>
		/// Retourne le rectangle de sous-texture indiqué par l'index ///
		/// Returns the subtexture rectangle indicated by the index
		/// </summary>
		/// <param name="index">Index de la tuile /// Subtexture index</param>
		sf::FloatRect getSubTexture(int index) const;

		/// <summary>
		/// Retourne la taille de la sous-texture à l'index indiqué ///
		/// Returns the subtexture's size indicated by the index
		/// </summary>
		/// <param name="index">Index de sous-texture /// Subtexture index</param>
		sf::Vector2f getSubTextureSize(int index) const;

		/// <summary>
		/// Retourne le nombre de tuiles dans le niveau ///
		/// Returns the number of tiles inside the level
		/// </summary>
		std::size_t size() const;

		/// <summary>
		/// Retourne une référence des sommets de l'ensemble des tuiles ///
		/// Returns all vertexes of every Tile
		/// </summary>
		std::vector<sf::Vertex>& vertices();

		/// <summary>
		/// Retourne une référence des index de début des tuiles ///
		/// Returns a reference 
		/// </summary>
		std::vector<std::size_t>& beginTileIndexes();

		/// <summary>
		/// Indique si le tampon doit se mettre à la bonne taille à chaque modification ///
		/// Indicates if the buffer has to be resized at each modification
		/// </summary>
		/// <param name="autoUpdate">Doit être mis à jour à chaque changement /// Must update at each change</param>
		void bufferAutoUpdate(bool autoUpdate);
	};

	template <class T>
	inline T* const opt::Level::derivedPointer(int index)
	{
		return dynamic_cast<T*>(m_tiles[index].get());
	}
}

#endif