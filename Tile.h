///
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

//namespace TestsOptTile
//{
//	class TestsOptTile;
//}

namespace opt
{
	//class OPTTILE_API Level;

	enum class OPTTILE_API TextureRule {
		// La texture sera répétée dans la tuile. Augmenter l'échelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
		// The texture is repeated inside the tile. Increasing the scale only increased the size of the tile, without inflating the texture zoom
		repeat_texture,	
		// La texture sera transformée en mettant la hauteur en priorité. Ex: l'échelle de la tuile au début est de 1:1, puis devient 2:1, ça va être 1:1, puisque la hauteur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répétait 2 fois, elle restera répétée 2 fois)
		// The texture is transformed by taking first the height. Eg: a tile begins with the scale 1:1 (100x100), then becomes 2:1, it will be 1:1, because height has the priority. The texture rectangle is not affected (if a texture is repeated twice, it will stay repeated twice)
		keep_height,
		// La texture sera transformée en mettant la largeur en priorité. Ex: l'échelle de la tuile au début est de 1:1 (100x100), puis devient 2:1. La tuile aura l'échelle 2:2 (200x200), puisque la largeur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répète 2 fois, elle restera répétée 2 fois)
		// The texture is transformed by taking first the width. Eg: a tile begins with the scale 1:1 (100x100), then becomes 2:1. The tile will have the scale 2:2 (200x200), as width has the priority. The texture rectangle is not affected (if a texture is repeated twice, it will stay repeated twice)
		keep_width,	
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

		// À enlever une fois les tests terminés
		//friend class TestsOptTile::TestsOptTile;

	protected:
		// Donne le rectangle de taille et de position de la tuile ///
		// Give the tile's rectangle for its size and its position											
		sf::FloatRect m_tileRect;
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
		int m_subTextureIndex;												
		// Indique les rectangles de sous-textures (peuvent être asymétriques ///
		// Indicates subtexture's rectangles (they may be asymetrical)
		std::shared_ptr<const std::vector<sf::FloatRect>> m_subTextures;
		// Indique la couleur appliquée aux sommets ///
		// Indicates the vertexes colour
		sf::Color m_colour;
		// Indique l'échelle de position par rapport au coin supérieur gauche de la tuile ///
		// Indicates the position scale center from in the tile
		sf::Vector2f m_centerPositionScale{0.f, 0.f};

		void intializeVertexes();
	public:

		/// <summary>
		/// Ce constructeur ne devrait jamais être utilisé autrement que pour changer la taille du std::vector ///
		/// This constructor should not be used for other reasons than changing the tiles' vector size
		/// </summary>
		Tile();

		/// <summary>
		/// Constructeur n'apportant que le minimum pour fonctionner, soit les références à absolument avoir ///
		/// Constructor bringing only the minimum to make the Tile work, which are the absolutely necessary references to have
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
		Tile(int noTuileDebutTexture, const sf::FloatRect& tileRect, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures,
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
		/// Retourne la hauteur de la tuile /// Returns the tile's height
		/// </summary>
		float height() const;

		/// <summary>
		/// Retourne la largeur de la tuile /// Returns the tile's width
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
		void changeTextureRect(int numberSubTexture);

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
		/// Met le coin supérieur gauche à la position entrée en paramètre ///
		/// Puts the top left corner at the position entered in parameter
		/// </summary>
		/// <param name="position">Nouvelle position /// New position</param>
		void setPosition(const sf::Vector2f& position);

		/// <summary>
		/// Met le coin supérieur gauche à la position entrée en paramètre ///
		/// Puts the top left corner at the position entered in parameter
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
		/// Permet d'accéder à l'adresse de l'objet ///
		/// Allows to get the address of the object
		/// </summary>
		virtual Tile* getThis();

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
		void changeColour(const sf::Color& colour);

		/// <summary>
		/// Réinitialise la couleur de la tuile (met la couleur à 0xFFFFFFFF) ///
		/// Resets the tile's colour (puts the colour 0xFFFFFFFF)
		/// </summary>
		void resetColour();

		/// <summary>
		/// Retourne la couleur de la tuile ///
		/// Returns the tile's colour
		/// </summary>
		sf::Color getColour() const;

		/// <summary>
		/// Retourne la taille de la tuile ///
		/// Returns the tile's size
		/// </summary>
		sf::Vector2f getSize() const;

		/// <summary>
		/// Retourne l'index de la sous-texture utilisée par la tuile ///
		/// Returns the index used for subtexture by the tile
		/// </summary>
		int subTextureIndex() const;

		/// <summary>
		/// Retoune le nombre de sommets composant la tuile ///
		/// Returns the number of vertexes composing the Tile
		/// </summary>
		std::size_t vertexCount() const;

		/// \brief Change le centre utilisé pour la méthode getPosition. Par exemple, mettre (0.f, 0.f) revient à mettre au coin supérieur gauche
		/// \param scaleX Proportion par rapport à la largeur
		/// \param scaleY Proportion par rapport à la hauteur
		void changePositionCenter(float scaleX, float scaleY);
	};
}
#endif 


