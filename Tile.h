﻿///
/// Version de la bibliothèque datant de 2023 - Library written in 2023
/// 
/// ========================================================================================
/// ========================================================================================
/// 
///	Fait par Charles Mandziuk. Aucune mention n'est nécessaire, mais est toujours appréciée
/// 
/// Written by Charles Mandziuk. No any mention is mandatory, but is always appreciated
///

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

namespace ModTile
{
	class Position;
	class Zoom;
	class Couleur;
	class Taille;
	class Centre;
	class Angle;
}

namespace ModLevel 
{
	class AjoutTile;
}

namespace opt
{
	/// <summary>
	/// Sert à définir la manière dont la texture sera transformée lorsque la taille ou l'échelle de la tuile sera changée. ///
	/// Used to define how the texture will be transformed when the tile's size or scale will be changed.
	/// </summary>
	enum class OPTTILE_API TextureRule {
		// La texture sera répétée dans la tuile. Augmenter l'échelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
		// The texture is repeated inside the tile. Increasing the scale only increased the size of the tile, without inflating the texture zoom
		repeat_texture,
		// La texture sera transformée en mettant la hauteur en priorité. Ex: l'échelle de la tuile au début est de 1:1, puis devient 2:1, ça va être 1:1, puisque la hauteur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répétait 2 fois, elle restera répétée 2 fois)
		// The texture is transformed by taking first the height. Eg: a tile begins with the scale 1:1 (100x100), then becomes 2:1, it will be 1:1, because height has the priority. The texture rectangle is not affected (if a texture is repeated twice, it will stay repeated twice)
		height_first,
		// La texture sera transformée en mettant la largeur en priorité. Ex: l'échelle de la tuile au début est de 1:1 (100x100), puis devient 2:1. La tuile aura l'échelle 2:2 (200x200), puisque la largeur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répète 2 fois, elle restera répétée 2 fois)
		// The texture is transformed by taking first the width. Eg: a tile begins with the scale 1:1 (100x100), then becomes 2:1. The tile will have the scale 2:2 (200x200), as width has the priority. The texture rectangle is not affected (if a texture is repeated twice, it will stay repeated twice)
		width_first,	
		// La texture sera plus grosse, mais l'objet gardera la même taille
		// The texture will be more zoomed, but the object will keep the same size
		keep_size,			
		// Aucune tentative n'est faite pour garder la même taille que celle passée auparavant, tout sera transformée selon ce qui est donné, sans garder les proportions
		// No attempt is made to keep the same size. Everything will be transformed; the texture is zoomed, the tile is bigger.
		adjustable_size,	
		// La texture prend l'entièreté de l'espace sans se répéter. La taille de la case a donc priorité sur son échelle
		// The texture takes the entirety of the tile, without repeat. The size has priority over its scale
		fill_space			
	};

	/// <summary>
	/// Permet de faire une représentation d'une fonction affine à l'aide de la formule y = ax + b ///
	/// Allows to make a representation of a linear function using the formula y = ax + b
	/// </summary>
	struct OPTTILE_API LinearFunc {
		// Pente de la fonction
		// Slope of the function
		float a;
		// Ordonnée à l'origine
		// Y-intercept
		float b;
		// Indique si la fonction est constante en x. Dans ce cas, a devient la valeur en X
		// Indicates if the function is constant in x. In this case, a becomes the value in X
		bool constantX;

		/// <summary>
		/// Crée une fonction affine de base de la forme y = x ///
		/// Creates a basic linear function of the form y = x
		/// </summary>
		LinearFunc();

		/// <summary>
		/// Crée une fonction affine avec les paramètres ///
		/// Creates a linear function with the parameters
		/// </summary>
		/// <param name="a">Pente de la fonction /// Slope of the function</param>
		/// <param name="b">Ordonnée à l'origine /// Y-intercept value</param>
		/// <param name="constantX">Indique s'il s'agit d'une fonction du genre x = a</param>
		LinearFunc(float a, float b, bool constantX);

		/// <summary>
		/// Crée une fonction affine à partir de deux points ///
		/// Creates a linear function from two points
		/// </summary>
		/// <param name="point1">Premier point. Contient x1 et y1 /// First point. Contains x1 and y1</param>
		/// <param name="point2">Deuxième point. Contient x2 et y2 /// Second point. Contains x2 and y2</param>
		LinearFunc(const sf::Vector2f& point1, const sf::Vector2f& point2);

		/// <summary>
		/// Change l'ordonnée à l'origine de la fonction en prenant comme référence le point en paramètre. Si la fonction est une constante en X, l'abscice est modifiée ///
		/// Changes the y-intercept of the function using the point in parameter as reference. If the function is constant in X, the abscissa is modified
		/// </summary>
		/// <param name="point">Point de référence /// Reference point</param>
		void changeYIntercept(const sf::Vector2f& point);

		/// <summary>
		/// Change l'ordonnée à l'orgine de la fonction ///
		/// Changes the y-intercept of the function
		/// </summary>
		/// <param name="yIntercept">Nouvelle ordonnée à l'origine /// New Y-intercept</param>
		void changeYIntercept(float yIntercept);

		/// <summary>
		/// Change la pente de la fonction. Si la fonction est constante en X, l'abscice est modifiée ///
		/// Changes the slope of the function. If the function is constant in X, the abscissa is modified
		/// </summary>
		/// <param name="point1">Nouveau point de référence contenant les valeurs x1 et y1 /// New reference point containing values x1 and y1</param>
		/// <param name="point2">Nouveau point de référence contenant les valeurs x2 et y2 /// New reference point containing values x2 and y2</param>
		void changeSlope(const sf::Vector2f& point1, const sf::Vector2f& point2);

		/// <summary>
		/// Change la pente de la fonction. Si la fonction est constante en X, l'abscice est modifiée ///
		/// Changes the slope of the function. If the function is constant in X, the abscissa is modified
		/// </summary>
		/// <param name="newA">Nouveau paramètre a /// New parameter a</param>
		void changeSlope(float newA);

		/// <summary>
		/// Indique si le point se trouve exactement dans la fonction. Correspond y = a * x + b ///
		/// Indicates if the point is exactly inside the function. Corresponds to y = a * x + b
		/// </summary>
		/// <param name="point">Point à vérifier /// Point to verify</param>
		bool operator==(const sf::Vector2f& point) const;

		/// <summary>
		/// Vérifie si les deux fonctions sont égales ///
		/// Checks if the two functions are equal
		/// </summary>
		/// <param name="func">Autre fonction à vérifier</param>
		bool operator==(const LinearFunc& func) const;

		/// <summary>
		/// Vérifie l'inéquation de la fonction correspondant à y < a * x + b ///
		/// Checks the inequation of the function corresponding to y < a * x + b
		/// </summary>
		/// <param name="point">Point de référence de l'inéquation</param>
		bool operator<(const sf::Vector2f& point) const;

		/// <summary>
		/// Vérifie l'inéquation de la fonction correspondant à y <= a * x + b ///
		/// Checks the inequation of the function corresponding to y <= a * x + b
		/// </summary>
		/// <param name="point">Point de référence de l'inéquation</param>
		bool operator<=(const sf::Vector2f& point) const;

		/// <summary>
		/// Vérifie l'inéquation de la fonction correspondant à y > a * x + b ///
		/// Checks the inequation of the function corresponding to y > a * x + b
		/// </summary>
		/// <param name="point">Point de référence de l'inéquation</param>
		bool operator>(const sf::Vector2f& point) const;

		/// <summary>
		/// Vérifie l'inéquation de la fonction correspondant à y >= a * x + b ///
		/// Checks the inequation of the function corresponding to y >= a * x + b
		/// </summary>
		/// <param name="point">Point de référence de l'inéquation</param>
		bool operator>=(const sf::Vector2f& point) const;
	};

	/// <summary>
	/// Objet permettant d'encapsuler certains éléments SFML. Nécessite cependant un objet autre pour pouvoir être dessiné, tel que opt::Level ///
	/// Object allowing to wrap a few SFML-linked inside this class. Needs however another object to be drawn, such as opt::Level
	/// </summary>
	class OPTTILE_API Tile {
	private:
		/// <summary>
		/// Regarde le nouveau nombre de sommets pour la tuile et bouge dans le vector les sommets au besoin ///
		/// Looks the new vertexes number for the tile and moves vertexes inside the vector if needed
		/// </summary>
		/// <param name="nbVertexes">Nombre de sommets dans la tuile /// Number of vertexes inside Tile</param>
		void moveVertexes(int nbVertexes);

		/// <summary>
		/// Applique la rotation à l'angle indiqué à un des sommets de la tuile ///
		/// Applies the rotation to the angle indicated to one of the tile's vertexes
		/// </summary>
		/// <param name="vertex">Sommet à tourner /// Vertex to rotate</param>
		sf::Vector2f rotateVertex(const sf::Vector2f& vertex, bool invertY);

		constexpr static float pi {3.14159265f};
		// Transforme les degrés en radians /// Transforms degrees in radians
		constexpr static float rad {pi / 180.f};

		// À enlever une fois les tests terminés
		friend class ModTile::Centre;
		friend class ModTile::Couleur;
		friend class ModTile::Position;
		friend class ModTile::Taille;
		friend class ModTile::Zoom;
		friend class ModTile::Angle;
		friend class ModLevel::AjoutTile;

		// Donne le rectangle de taille et de position de la tuile ///
		// Give the tile's rectangle for its size and its position											
		sf::FloatRect m_tileRect;
		// Donne la position au niveau du centre de la tuile ///
		// Give the position at the center of the tile
		sf::Vector2f m_position;
		// L'ensemble des points du niveau ///
		// Entirety of vertexes creating the Level
		std::shared_ptr<std::vector<sf::Vertex>> m_vertexes;
		// Désigne combien il y a de sommets pour la tuile ///
		// Designates how many vertexes there are for the Tile
		std::size_t m_tileVertexesCount;
		// Indique le commencement de chaque tuile ///
		// Indicates the beginning index of each Tile
		std::shared_ptr<std::vector<std::size_t>> m_beginTiles;
		// Indique l'index de la tuile (this) ///
		// Indicate the tile index (this)
		std::size_t m_tileIndex;
		// Règle régissant le comportement d'une texture lorsque la taille ou l'échelle est changée ///
		// Texture rule dictating the texture's behaviour when resized or rescaled
		TextureRule m_textureRule;										
		// Indique le rapport entre la texture et taille demandée (lorsque l'objet est instancié, il équivaut à 1:1 par défaut) ///
		// Indicates the zoom of the subtexture (1:1 by default)
		sf::Vector2f m_scale{ 1.f, 1.f };									
		// Indique le numéro de sous-texture (utile lorsque la texture est changée) ///
		// Indicates the subtexture number (usefull when texture is changed)
		std::size_t m_subTextureIndex;												
		// Indique les rectangles de sous-textures (peuvent être asymétriques) ///
		// Indicates subtexture's rectangles (they may be asymetrical)
		std::shared_ptr<const std::vector<sf::FloatRect>> m_subTextures;
		// Indique la couleur appliquée aux sommets ///
		// Indicates the vertexes colour
		sf::Color m_colour;
		// Indique l'échelle de position par rapport au coin supérieur gauche de la tuile ///
		// Indicates the position scale center from in the tile
		sf::Vector2f m_centerPositionScale{0.f, 0.f};
		// Indique l'orientation de la tuile ///
		// Indicates the tile's orientation
		float m_angle{ 0.f };

		sf::Vector2f m_topRight;
		sf::Vector2f m_bottomLeft;
		sf::Vector2f m_bottomRight;

		constexpr static float m_cercleComplet{360.f};	// Indique la valeur d'un cercle complet en degrés
	protected:

		/// <summary>
		/// S'occupe de mettre à jour les sommets de la tuile ///
		/// Updates the tile's vertexes
		/// </summary>
		void intializeVertexes();

		/// <summary>
		/// Met à jour les sommets de la tuile selon les nouveaux paramètres ///
		/// Updates the tile's vertexes according to the new parameters
		/// </summary>
		void updateSummits();
	public:

		/// <summary>
		/// Ce constructeur ne devrait jamais être utilisé autrement que pour changer la taille du std::vector ///
		/// This constructor should not be used for other reasons than changing the tiles' vector size
		/// </summary>
		Tile();

		/// <summary>
		/// Constructeur d'une tuile ne demandant que le minimum, soit les références pour manipuler les collections. Ce constructeur est utile pour les tuiles qui n'ont pas à afficher de texture, mais seulement une couleur ///
		/// Constructor initializing the minimum, which are the references to manipulate the collections. This constructor is usefull for tiles that don't have to display a texture, but only a color
		/// </summary>
		/// <param name="beginTiles">Référence des index de chaque tuile /// Reference of indexes of each tile</param>
		/// <param name="vertices">Références de tous les sommets /// References of all vertexes</param>
		Tile(std::vector<std::size_t>& beginTiles, std::vector<sf::Vertex>& vertices);

		/// <summary>
		/// Constructeur d'une tuile mettant le ratio de la texture par rapport à la texture à l'échelle demandée ///
		/// Constructor intializing the zoom from the scale asked
		/// </summary>
		/// <param name="noTuileDebutTexture">Numéro de sous-texture utilisé /// Number of the used texture</param>
		/// <param name="tileRect">Rectangle de la tuile /// Tile rectangle</param>
		/// <param name="textureRule">Règle appliquée à la sous-texture /// Rule applied to the subtexture</param>
		/// <param name="scale">Zoom appliqué à la texture /// Zoom applied to the texture</param>
		/// <param name="subTextures">Positions et tailles des sous-textures /// Positions and sizes of the subtextures</param>
		Tile(std::size_t noTuileDebutTexture, const sf::FloatRect& tileRect, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures,
			std::vector<std::size_t>& beginTiles, std::vector<sf::Vertex>& vertices, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));

		/// <summary>
		/// Retourne une référence de la liste générique de sommets (pour pouvoir tout dessiner en un appel de la méthode draw) ///
		/// Returns a reference of the vertexes' vector (usefull to draw)
		/// </summary>
		const std::vector<sf::Vertex>& vertexes() const;

		/// <summary>
		/// Retourne une référence de la liste générique de sommets (pour pouvoir tout dessiner en un appel de la méthode draw) ///
		/// Returns a reference of the vertexes' vector (usefull to draw)
		/// </summary>
		std::vector<sf::Vertex>& vertexes();

		/// <summary>
		/// Retourne la hauteur de la tuile ///
		/// Returns the tile's height
		/// </summary>
		float height() const;

		/// <summary>
		/// Retourne la largeur de la tuile ///
		/// Returns the tile's width
		/// </summary>
		float width() const;

		/// <summary>
		/// Retourne les coordonnées au coin supérieur gauche ///
		/// Returns the coordinates of the top left corner
		/// </summary>
		sf::Vector2f topLeftCorner() const;

		/// <summary>
		/// Retourne les coordonnées au coin supérieur droit ///
		/// Returns the coordinates of the top right corner
		/// </summary>
		sf::Vector2f topRightCorner() const;

		/// <summary>
		/// Retourne les coordonnées au coin inférieur gauche ///
		/// Returns the coordinates of the bottom left corner
		/// </summary>
		sf::Vector2f bottomLeftCorner() const;

		/// <summary>
		/// Retourne les coordonnées au coin inférieur droit ///
		/// Returns the coordinates of the bottom right corner
		/// </summary>
		sf::Vector2f bottomRightCorner() const;

		/// <summary>
		/// Retourne les coordonnées au coin supérieur gauche ///
		/// Returns the coordinates of the top left corner
		/// </summary>
		sf::Vector2f getPosition() const;

		/// <summary>
		/// Met la tuile au zoom spécifié selon la règle de texture (consulter la documentation pour plus de détails) ///
		/// Puts the tile at the specified zoom according to the texture rule (consult documentation for more details)
		/// </summary>
		/// <param name="scale">Facteur de zoom /// Zoom factor</param>
		void setScale(const sf::Vector2f& scale);

		/// <summary>
		/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). ///
		/// Puts the tile at the specified zoom according to the texture rule (consult documentation for more details).
		/// </summary>
		/// <param name="scale">Facteur de zoom appliqué horizontalement et verticalement /// Zoom factor applied vertically and horizontally</param>
		void setScale(float scale);

		/// <summary>
		/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails) ///
		/// Puts the tile at the specified zoom according to the texture rule (consult documentation for more details)
		/// </summary>
		/// <param name="x">Facteur de zoom horizontal /// Horizontal zoom factor</param>
		/// <param name="y">Facteur de zoom vertical /// Vertical zoom factor</param>
		void setScale(float x, float y);

		/// <summary>
		/// Met l'échelle de la texture à l'échelle spécifiée selon la règle de texture ou change la taille autrement (voir la documentation pour plus de détails) ///
		/// Changes the texture rule, then puts the tile at the specified zoom according the new texture rule (consult documentation for more details).
		/// </summary>
		/// <param name="scale">Facteur de zoom /// Zoom factor</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		void setScale(const sf::Vector2f& scale, TextureRule textureRule);

		/// <summary>
		/// Met l'échelle de la texture à l'échelle spécifiée selon la règle de texture ou change la taille autrement (voir la documentation pour plus de détails) ///
		/// Changes the texture rule, then puts the tile at the specified zoom according the new texture rule (consult documentation for more details).
		/// </summary>
		/// <param name="scale">Facteur de zoom appliqué horizontalement et verticalement /// Zoom factor applied vertically and horizontally</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		void setScale(float scale, TextureRule textureRule);

		/// <summary>
		/// Met l'échelle de la texture à l'échelle spécifié à l'axe spécifié en paramètre selon la règle de texture (ou change la taille, autrement). Voir la documentation pour plus de détails. ///
		/// Changes the texture rule, then puts the tile at the specified zoom according the new texture rule (consult documentation for more details). Warning! To make the changes visible, all vertexes must copied inside a vector.
		/// </summary>
		/// <param name="x">Facteur de zoom horizontal /// Horizontal zoom factor</param>
		/// <param name="y">Facteur de zoom vertical ///Vertical zoom factor</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		void setScale(float x, float y, TextureRule textureRule);

		/// <summary>
		/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails. ///
		/// If the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="size">Nouvelle taille demandée /// New required size</param>
		void resize(const sf::Vector2f& size);

		/// <summary>
		/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails. ///
		/// If the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="x">Nouvelle taille horizontale demandée /// New required horizontal size</param>
		/// <param name="y">Nouvelle taille verticale demandée /// New required vertical size</param>
		void resize(float x, float y);

		/// <summary>
		/// La règle est mise à jour, puis si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails. ///
		/// The texture rule is updated, then if the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="size">Nouvelle taille demandée /// New required size</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		void resize(const sf::Vector2f& size, TextureRule textureRule);

		/// <summary>
		/// La règle est mise à jour, puis si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails. ///
		/// The texture rule is updated, then if the texture rule allows, the tile's size will be updated according to what is allowed. Seek documentation for more details.
		/// </summary>
		/// <param name="x">Nouvelle taille horizontale demandée /// New required horizontal size</param>
		/// <param name="y">Nouvelle taille verticale demandée /// New required vertical size</param>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		void resize(float x, float y, TextureRule textureRule);

		/// <summary>
		/// Change la sous-texture de la tuile ///
		/// Changes tile's subtexture
		/// </summary>
		/// <param name="numberSubTexture">Index de la sous-texture /// Index of the subtexture</param>
		/// <returns>Retourne true si l'index de sous-texture est valide, false sinon /// Returns true if the index is valid, false otherwise</returns>
		bool changeTextureRect(std::size_t subTextureIndex);

		/// <summary>
		/// Change la règle de texture ///
		/// Changes the texture rule
		/// </summary>
		/// <param name="textureRule">Nouvelle règle de texture /// New texture rule</param>
		void setTextureRule(TextureRule textureRule);

		/// <summary>
		/// Obtient la règle de texture de la tuile ///
		/// Obtains the texture rule of the tile
		/// </summary>
		TextureRule getTextureRule();

		/// <summary>
		/// Déplace la tuile de l'équivalent du vecteur ///
		/// Moves the tile of the equivalent of the offset
		/// </summary>
		/// <param name="offset">Mouvement vectoriel en 2 dimensions à faire /// Move to do in 2 dimensions</param>
		void move(const sf::Vector2f& offset);

		/// <summary>
		/// Déplace la tuile de l'équivalent du vecteur ///
		/// Moves the tile of the equivalent of the offset
		/// </summary>
		/// <param name="offsetX">Mouvement vectoriel horizontal /// Horizontal movement</param>
		/// <param name="offsetY">Mouvement vectoriel vertical /// Vertical movement</param>
		void move(float offsetX, float offsetY);

		/// <summary>
		/// Met le centre de position à la position entrée en paramètre ///
		/// Puts the position center at the position entered in parameter
		/// </summary>
		/// <param name="position">Nouvelle position /// New position</param>
		void setPosition(const sf::Vector2f& position);

		/// <summary>
		/// Met le centre de position à la position entrée en paramètre ///
		/// Puts the position center at the position entered in parameter
		/// </summary>
		/// <param name="x">Nouvelle position horizontale /// New horizontal position</param>
		/// <param name="y">Nouvelle position verticale /// New vertical position</param>
		void setPosition(float x, float y);

		/// <summary>
		/// Recharge les sommets pour que les tuiles suivent bien la nouvelle texture. Si le numéro de texture dépasse la nouvelle valeur maximale (contenue dans la classe Niveau), la sous-texture correspondra à la dernière disponible ///
		/// Reloads vertexes to make sure it follows a subtexture that exists. If the texture number overwhelm the maximum value, the subtexture will be the subtexture at the last index.
		/// </summary>
		void reloadTexture();

		/// <summary>
		/// Crée un clone de l'objet qui peut contenir des méthodes dérivées tout comme des membres dérivés. Pour clôner un objet dérivé, surcharger cette méthode ///
		/// Creates a clone of the object wich can contain the derived methods as well as their unique members. To clone a derived object, overload this method.
		/// </summary>
		virtual std::unique_ptr<Tile> clone() const;

		/// <summary>
		/// Donne la hauteur de la sous-texture (utile lorsqu'on veut quelques fois la hauteur de la sous-texture) ///
		/// Gives the subtexture's height (usefull when want only a few times its height)
		/// </summary>
		float subTextureHeight() const;

		/// <summary>
		/// Donne la largeur de la sous-texture (utile lorqu'on veut quelques fois la largeur de la sous-texture) ///
		/// Gives the subtexture's width (usefull when want only a few times its width)
		/// </summary>
		float subTextureWidth() const;

		/// <summary>
		/// Donne la taille de la sous-texture ///
		/// Gives the size of the subtexture
		/// </summary>
		sf::Vector2f subTextureSize() const;

		/// <summary>
		/// Change la couleur de la tuile ///
		/// Changes the tile's colour
		/// </summary>
		/// <param name="color">Nouvelle couleur à appliquer /// New colour to apply</param>
		void changeColor(const sf::Color& colour);

		/// <summary>
		/// Réinitialise la couleur de la tuile (met la couleur à 0xFFFFFFFF) ///
		/// Resets the tile's colour (puts the colour 0xFFFFFFFF)
		/// </summary>
		void resetColor();

		/// <summary>
		/// Retourne la couleur de la tuile ///
		/// Returns the tile's colour
		/// </summary>
		sf::Color getColor() const;

		/// <summary>
		/// Retourne la taille de la tuile ///
		/// Returns the tile's size
		/// </summary>
		sf::Vector2f getSize() const;

		/// <summary>
		/// Retourne l'index de la sous-texture utilisée par la tuile ///
		/// Returns the index used for subtexture by the tile
		/// </summary>
		std::size_t subTextureIndex() const;

		/// <summary>
		/// Retoune le nombre de sommets composant la tuile ///
		/// Returns the number of vertexes composing the Tile
		/// </summary>
		std::size_t vertexCount() const;

		/// <summary>
		/// Change la position servant de centre pour la tuile en ratio par rapport à sa taille, où 0 correspond à 0% de la valeur et 1 correspond à 100% de la valeur ///
		/// Changes the position used as center for the tile in ratio from its size, where 0 corresponds to 0% of the value and 1 corresponds to 100% of the value
		/// </summary>
		/// <param name="scaleX">Ratio en pourcentage de la largeur de la tuile /// Proportion in percentage of the Tile's width</param>
		/// <param name="scaleY">Ratio en pourcentage de la hauteur de la tuile /// Proportion in percentage of the Tile's height</param>
		void setPositionCenter(float scaleX, float scaleY);

		/// <summary>
		/// Change la position servant de centre pour la tuile en ratio par rapport à sa taille, où 0 correspond à 0% de la valeur et 1 correspond à 100% de la valeur ///
		/// Changes the position used as center for the tile in ratio from its size, where 0 corresponds to 0% of the value and 1 corresponds to 100% of the value
		/// </summary>
		/// <param name="scale">Ratio en pourcentage vertical et horizontal de la tuile /// Vertical and horizontal proportions in percentage of the Tile</param>
		void setPositionCenter(const sf::Vector2f& scale);

		/// <summary>
		/// Fait une rotation de la tuile ///
		/// Makes a rotation of the tile
		/// </summary>
		/// <param name="angle">Valeur en degrés à additionner à l'angle de la tuile /// Value in degrees to add to the Tile's angle</param>
		void rotate(float angle);

		/// <summary>
		/// Met la tuile à l'orientation en degrés à la valeur demandée ///
		/// Puts the tile at the specified angle in degrees
		/// </summary>
		/// <param name="angle">Valeur d'orientation en degrés /// Orientation value in degrees</param>
		void setOrientation(float angle);

		/// <summary>
		/// Indique si la surface de la tuile contient la position entrée en paramètre ///
		/// Indicates if the tile's area contains the position entered in parameter
		/// </summary>
		/// <param name="position">Position à vérifier /// Position to check</param>
		/// <returns>Si vrai, la position se trouve dans la tuile /// If true, the position is inside the Tile</returns>
		bool intersects(const sf::Vector2f& position) const;
	};
}
#endif 