#pragma once

#ifndef LEVEL_FR_H
#define LEVEL_FR_H

#include "OptTile.h"
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile-fr.h"

namespace opt
{
	namespace fr
	{
		class OPTTILE_API Level : public sf::Drawable {
		private:
			std::vector<std::unique_ptr<opt::fr::Tile>> m_tiles;
			sf::Texture m_texture;						// Texture utilisée pour toutes les cases
			//std::size_t m_nbTexture;					// Indique le nombre de sous-textures dans le fichier
			std::vector<std::size_t> m_beginTileIndex;	// Indique l'index de commencement des sommets de chaque tuile
			std::vector<sf::Vertex> m_vertexes;			// Ensemble des sommets copiés par valeur des tuiles. À n'utiliser que pour la méthode draw et ce qui aide à faire le rendu
			std::vector<sf::FloatRect> m_subTextures;	// Indique les sous-textures possibles

			/// <summary>
			/// Indique si on continue de mettre à jour les sommets
			/// </summary>
			/// <param name="index">Index de la tuile</param>
			/// <param name="itterator">Index dans la liste générique de sommets</param>
			bool continueUpdate(std::size_t index, std::size_t itterator);

		public:

			/// <summary>
			/// Recharge la liste générique de sommets et l'index de départ des tuiles par rapport aux sommets. Utile lorsque plusieurs tuiles ont été modifiées directement
			/// </summary>
			void reloadVertexes();

			/// <summary>
			/// Charge en mémoire la texture désirée et met le compteur de cases à 0
			/// </summary>
			/// <param name="pPathTexture"></param>
			Level(const std::string& pPathTexture, std::size_t pNbTextures);

			/// <summary>
			/// Constructeur par défaut. Utile lors de la création d'une structure contenant un niveau
			/// </summary>
			Level();

			/// <summary>
			/// Retourne une référence de la case à l'index spécifié
			/// </summary>
			/// <param name="index">Index de la tuile</param>
			Tile* const operator[](int index);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other"></param>
			Level& operator=(Level&& other) noexcept;

			/// <summary>
			/// Dessine le niveau sur l'élément SFML cible
			/// </summary>
			/// <param name="target">Élément SFML cible du rendu</param>
			/// <param name="states">États à ajouter aux vecteurs</param>
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			/// <summary>
			/// Bouge la tuile spécifiée à l'index au mouvement spécifié puis met à jour les sommets
			/// </summary>
			/// <param name="x">Mouvement horizontal</param>
			/// <param name="y">Mouvement vertical</param>
			/// <param name="index">Index de la tuile</param>
			void move(float offsetX, float offsetY, std::size_t index);

			/// <summary>
			/// Bouge la tuile spécifiée par l'index au mouvement indiqué en paramètre puis met à jour les sommets
			/// </summary>
			/// <param name="offset">Mouvement à effectuer</param>
			/// <param name="index">Index de la tuile</param>
			void move(const sf::Vector2f& offset, std::size_t index);

			/// <summary>
			/// Bouge la tuile spécifiée par l'index à la position indiquée en paramètre, puis met à jour les sommets
			/// </summary>
			/// <param name="x">Nouvelle position horizontale</param>
			/// <param name="y">Nouvelle position verticale</param>
			/// <param name="index">Index de la tuile</param>
			void setPosition(float x, float y, std::size_t index);

			/// <summary>
			/// Bouge la tuile spécifiée par l'index à la position indiquée en paramètre, puis met à jour les sommets
			/// </summary>
			/// <param name="position">Nouvelle position de la tuile</param>
			/// <param name="">Index de la tuile</param>
			void setPosition(const sf::Vector2f& position, std::size_t index);

			/// <summary>
			/// Permet de redimensionner la tuile et met à jour les sommets pour faire le rendu
			/// </summary>
			/// <param name="x">Nouvelle taille horizontale</param>
			/// <param name="y">Nouvelle taille verticale</param>
			/// <param name="index">Index de la tuile</param>
			void resize(float x, float y, std::size_t index);

			/// <summary>
			/// Permet de redimensionner la tuile et met à jour les sommets pour faire le rendu
			/// </summary>
			/// <param name="size">Nouvelles dimensions de la tuile</param>
			/// <param name="index">Index de la tuile</param>
			void resize(const sf::Vector2f& size, std::size_t index);

			/// <summary>
			/// Permet de redimensionner la tuile, de changer la règle de texture et met à jour les sommets pour faire le rendu
			/// </summary>
			/// <param name="x">Nouvelle taille horizontale</param>
			/// <param name="y">Nouvelle taille verticale</param>
			/// <param name="textureRule">Nouvelle règle de texture de la tuile</param>
			/// <param name="index">Index de la tuile</param>
			void resize(float x, float y, opt::fr::TextureRule textureRule, std::size_t index);

			/// <summary>
			/// Permet de redimensionner la tuile, de changer la r�gle de texture et met à jour les sommets pour faire le rendu
			/// </summary>
			/// <param name="size">Nouvelles dimensions de la tuile</param>
			/// <param name="textureRule">Nouvelle règle de texture de la tuile</param>
			/// <param name="index">Index de la tuile</param>
			void resize(const sf::Vector2f& size, opt::fr::TextureRule textureRule, std::size_t index);

			/// <summary>
			/// Recharge la texture globale au chemin indiqué et redéfinis le nombre de sous-textures
			/// </summary>
			/// <param name="path">Chemin de la texture</param>
			/// <param name="subTextureCount">Nombre de sous-textures dans la texture globale</param>
			/// <returns>Indique si la texture a pu être chargée</returns>
			bool loadTexture(const std::string& path, int subTextureCount);

			/// <summary>
			/// Recharge la texture globale au chemin indiqué, redéfinis le nombre de textures et les rectangles de sous-textures
			/// </summary>
			/// <param name="path">Chemin de la texture</param>
			/// <param name="subTextureCount">Nombre de sous-textures</param>
			/// <param name="subTextures">Rectangles définissant les sous-textures</param>
			bool loadTexture(const std::string& path, std::vector<sf::FloatRect>& subTextures);

			/// <summary>
			/// Recharge la texture globale au chemin indiqué
			/// </summary>
			/// <param name="path">Chemin de la texture</param>
			/// <param name="subTexturePath">Chemin de la règle régissant les sous-textures. Le fichier doit être ordonné de même : left, top, width, height</param>
			bool loadTexture(const std::string& path, const std::string& subTexturePath);

			/// <summary>
			/// Recharge la texture gloable à la texture indiquée et redéfinis le nombre de sous-textures
			/// </summary>
			/// <param name="texture">Nouvelle texture globale</param>
			/// <param name="subTextureCount">Nombre de sous-textures dans la nouvelle texture globale</param>
			/// <returns>Indique si la texture a pu être chargée</returns>
			void loadTexture(const sf::Texture& texture, int subTextureCount);

			/// <summary>
			/// Recharge la texture globale à la texture indiquée, redéfinis le nombre de sous-textures et les rectangles de sous-textures
			/// </summary>
			/// <param name="texture">Nouvelle texture globale</param>
			/// <param name="subTextureCount">Nombre de sous textures dans la nouvelle texture globale</param>
			/// <param name="subTextures">Rectangles définissant les sous-textures</param>
			void loadTexture(const sf::Texture& texture, std::vector<sf::FloatRect>& subTextures);

			/// <summary>
			/// Recharge la texture depuis une texture déjà en mémoire et lit les dimensions de chaque sous-texture
			/// </summary>
			/// <param name="texture">Référence de texture utilisée</param>
			/// <param name="subTexturePath">Chemin du fichier régissant les sous-textures</param>
			void loadTexture(const sf::Texture& texture, const std::string& subTexturePath);

			/// <summary>
			/// Réinitialise la liste générique de sommets (pour le rendu) et la liste générique de tuiles
			/// </summary>
			void resetTiles();

			/// <summary>
			/// Rajoute une tuile à la liste générique de tuiles. Permet de rajouter dans la liste générique une classe dérivée de Tile
			/// </summary>
			/// <param name="tile">Tuile à rajouter</param>
			void add(const opt::fr::Tile& tile);

			/// <summary>
			/// Construit une nouvelle tuile à la liste générique
			/// </summary>
			/// <param name="size">Taille de la nouvelle tuile</param>
			/// <param name="position">Position de la nouvelle tuile</param>
			/// <param name="numberSubTexture">Numéro de sous-texture</param>
			/// <param name="textureRule">Règle appliquée à la sous-texture. Voir la documentation pour plus de détails</param>
			void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, opt::fr::TextureRule textureRule);

			/// <summary>
			/// Construit une nouvelle tuile à la liste générique
			/// </summary>
			/// <param name="size">Taille de la nouvelle tuile</param>
			/// <param name="position">Position de la nouvelle tuile</param>
			/// <param name="numberSubTexture">Numéro de sous-texture</param>
			/// <param name="textureRule">Règle appliquée à la sous-texture. Voir la documentation pour plus de détails</param>
			/// <param name="scale">Échelle appliquée à la texture</param>
			void add(const sf::Vector2f& size, const sf::Vector2f& position, int numberSubTexture, opt::fr::TextureRule textureRule, const sf::Vector2f& scale);

			/// <summary>
			/// Retourne un pointeur d'un objet d'une classe spécifiée
			/// </summary>
			/// <typeparam name="T">Type de l'objet dérivé</typeparam>
			/// <param name="index">Index de l'objet à vérifier</param>
			/// <returns>Si null, l'objet à l'index spécifié n'est pas de ce type</returns>
			template <class T>
			T* const derivedPointer(int index);

			/// <summary>
			/// Retourne une référence constante de la texture utilisée dans le niveau
			/// </summary>
			const sf::Texture& getTexture() const;

			/// <summary>
			/// Retourne une référence de la liste générique. Ne devrait être utilisé que pour faire des objets dérivés de Tile
			/// </summary>
			/// <returns></returns>
			const std::vector<sf::FloatRect>& getSubTextures() const;

			/// <summary>
			/// Change le rectangle de texture (par exemple, pour faire de l'animation) et met à jour les sommets. Il est possible que la méthode ne soit pas optimisée
			/// </summary>
			/// <param name="numberTexture">Numéro de la nouvelle sous-texture</param>
			/// <param name="index">Index de la tuile</param>
			void changeTextureRect(int numberTexture, int index);

			/// <summary>
			/// Change la couleur des points d'une tuile
			/// </summary>
			/// <param name="color">Nouvelle couleur de la tuile (pour appliquer un effet de couleur)</param>
			/// <param name="index">Index de la tuile</param>
			void changeColor(const sf::Color& color, int index);

			/// <summary>
			/// Réinitialise la couleur des points d'une tuile
			/// </summary>
			/// <param name="index">Index de la tuile</param>
			void resetColor(int index);

			/// <summary>
			/// Retourne la couleur d'une tuile
			/// </summary>
			/// <param name="index">Index de la tuile</param>
			sf::Color getColor(int index) const;

			/// <summary>
			/// Retourne le rectangle de sous-texture à l'index indiqué
			/// </summary>
			/// <param name="index">Index de la sous-texture</param>
			sf::FloatRect getSubTexture(int index) const;

			/// <summary>
			/// Retourne la taille de la sous-texture à l'index indiqué
			/// </summary>
			/// <param name="index">Index de la sous-texture</param>
			sf::Vector2f getSubTextureSize(int index) const;

			/// <summary>
			/// Retourne le nombre de tuiles dans le niveau
			/// </summary>
			/// <returns></returns>
			std::size_t size() const;
		};
	}

	template <class T>
	inline T* const opt::fr::Level::derivedPointer(int index)
	{
		return dynamic_cast<T*>(m_tiles[index].get());
	}
}

#endif