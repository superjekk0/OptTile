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
		repeat_texture,		// La texture sera répétée dans la tuile. Augmenter l'échelle augmentera simplement la taille de la tuile, sans augmenter la taille des textures
		keep_height,		// La texture sera transformée en mettant la hauteur en priorité. Ex: l'échelle de la tuile au début est de 1:1, puis devient 2:1, ça va être 1:1, puisque la hauteur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répétait 2 fois, elle restera répétée 2 fois)
		keep_width,			// La texture sera transformée en mettant la largeur en priorité. Ex: l'échelle de la tuile au début est de 1:1 (100x100), puis devient 2:1. La tuile aura l'échelle 2:2 (200x200), puisque la largeur a priorité. Le rectangle de texture n'est pas affecté (si une texture se répète 2 fois, elle restera répétée 2 fois)
		keep_size,			// La texture sera plus grosse, mais l'objet gardera la même taille
		adjustable_size,	// Aucune tentative n'est faite pour garder la même taille que celle passée auparavant, tout sera transformée selon ce qui est donné, sans garder les proportions
		fill_space			// La texture prend l'entièreté de l'espace sans se répéter. La taille de la case a donc priorité sur son échelle
	};

	class OPTTILE_API Tile {
	protected:
		std::shared_ptr<const sf::Texture> m_texture;						// La texture héritée de la classe contenant la tuile
		//sf::Vector2f m_textureSize;							// Indique la taille de la sous-texture utilisée
		//sf::Vector2f m_texturePosition;						// Indique la position de départ de la sous-texture
		sf::Vector2f m_position;								// Donne la position de la case au coin supérieur gauche
		sf::Vector2f m_tileSize;								// Donne la taille de la case
		std::vector<sf::Vertex> m_vertexes;						// L'ensemble des points qui composent l'objet dessinable
		TextureRule m_textureRule;								// Règle régissant le comportement d'une texture lorsque la taille ou l'échelle est changée
		sf::Vector2f m_scale{ 1.f, 1.f };						// Indique le rapport entre la texture et taille demandée (lorsque l'objet est instancié, il équivaut à 1:1 par défaut)
		//const int* const m_textureCount;						// Indique le nombre de textures dans la texture globale
		int m_numberSubTexture;									// Indique le numéro de sous-texture (utile lorsque la texture est changée)
		std::shared_ptr<const std::vector<sf::FloatRect>> m_subTextures;	// Indique les rectangles de sous-textures (peuvent être asymétriques
		sf::Color m_color;										// Indique la couleur appliquée aux sommets

		void intializeVertexes();
	public:

		/// <summary>
		/// Ce constructeur ne devrait jamais être utilisé autrement que pour changer la taille du std::vector
		/// </summary>
		Tile();

		/// <summary>
		/// Constructeur d'une tuile mettant par défaut le ratio de la texture à 1:1 (même si la sous-texture n'est pas carrée)
		/// </summary>
		/// <param name="texture">Référence de la texture utilisée (provenant souvent du niveau)</param>
		/// <param name="noTuileDebutTexture">Numéro de sous-texture utilisé</param>
		/// <param name="desiredSize">Taille de la tuile</param>
		/// <param name="position">Position au coin supérieur gauche de la texture</param>
		/// <param name="subTextureCount">Nombre de sous-textures</param>
		/// <param name="textureRule">Règle appliquée à la texture</param>
		/// <param name="subTextures">Les positions et tailles des sous-textures</param>
		Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, TextureRule textureRule, const std::vector<sf::FloatRect>& subTextures);

		/// <summary>
		/// Constructeur d'une tuile mettant le ratio de la texture par rapport à la texture à l'échelle demandée
		/// </summary>
		/// <param name="texture">Référence de la texture utilisée (provenant probablement du niveau)</param>
		/// <param name="noTuileDebutTexture">Numéro de sous-texture utilisé</param>
		/// <param name="desiredSize">Taille de la tuile</param>
		/// <param name="position">Position au coin supérieur gauche de la texture</param>
		/// <param name="subTextureCount">Nombre de sous-textures</param>
		/// <param name="textureRule">Règle appliquée à la sous-texture</param>
		/// <param name="scale">Échelle de la sous-texture</param>
		/// <param name="subTextures">Les positions et tailles des sous-textures</param>
		Tile(const sf::Texture& texture, int noTuileDebutTexture, const sf::Vector2f& desiredSize, const sf::Vector2f& position, TextureRule textureRule, const sf::Vector2f& scale, const std::vector<sf::FloatRect>& subTextures);

		/// <summary>
		/// Retourne une référence de la liste générique de sommets (pour pouvoir tout dessiner en un appel de la méthode draw)
		/// </summary>
		const std::vector<sf::Vertex>& vertexes() const;

		/// <summary>
		/// Retourne la hauteur de la tuile
		/// </summary>
		float height() const;

		/// <summary>
		/// Retourne la largeur de la tuile
		/// </summary>
		float width() const;

		/// <summary>
		/// Retourne les coordonnées du coin supérieur gauche de la tuile
		/// </summary>
		sf::Vector2f topLeftCorner() const;

		/// <summary>
		/// Retourne les coordonnées du coin supérieur droit de la tuile
		/// </summary>
		sf::Vector2f topRightCorner() const;

		/// <summary>
		/// Retourne les coordonnées du coin inférieur gauche de la tuile
		/// </summary>
		sf::Vector2f bottomLeftCorner() const;

		/// <summary>
		/// Retourne les coordonnées du coin inférieur droit de la tuile
		/// </summary>
		sf::Vector2f bottomRightCorner() const;

		/// <summary>
		/// Retourne les coordonnées du coin supérieur gauche de la tuile
		/// </summary>
		sf::Vector2f getPosition() const;

		/// <summary>
		/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). Attention! Pour que le changement soit perceptible par l'utilisateur, l'ensemble des sommets doit être recopié dans une liste générique.
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement en deux dimensions</param>
		void setScale(const sf::Vector2f& scale);

		/// <summary>
		/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). Attention! Pour que le changement soit perceptible par l'utilisateur, il faut recopier l'entièreté des sommets dans une liste générique.
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement appliqué en x et en y</param>
		void setScale(float scale);

		/// <summary>
		/// Mets la tuile à l'échelle spécifiée selon la règle de textures (voir la documentation pour plus de détails). Attention! Pour que le changement soit perceptible par l'utilisateur, il faut recopier l'entièreté des sommets dans une liste générique.
		/// </summary>
		/// <param name="x">Facteur d'agrandissement horizontal</param>
		/// <param name="y">Facteur d'agrandissement vertical</param>
		void setScale(float x, float y);

		/// <summary>
		/// Met l'échelle de la texture à l'échelle spécifiée selon la règle de texture ou change la taille autrement (voir la documentation pour plus de détails)
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement de la texture</param>
		/// <param name="textureRule">Nouvelle règle de texture</param>
		void setScale(const sf::Vector2f& scale, TextureRule textureRule);

		/// <summary>
		/// Met l'échelle de la texture à l'échelle spécifiée dans les deux axes selon la règle de texture (ou change la taille autrement). Voir la documentation pour plus de détails.
		/// </summary>
		/// <param name="scale">Facteur d'agrandissement de la texture è l'horizontal et à la verticale</param>
		/// <param name="textureRule">Nouvelle règle de texture</param>
		void setScale(float scale, TextureRule textureRule);

		/// <summary>
		/// Met l'échelle de la texture à l'échelle spécifié à l'axe spécifié en paramètre selon la règle de texture (ou change la taille, autrement). Voir la documentation pour plus de détails.
		/// </summary>
		/// <param name="x">Facteur d'agrandissement horizontal</param>
		/// <param name="y">Facteur d'agrandissement vertical</param>
		/// <param name="textureRule">Nouvelle règle de texture</param>
		void setScale(float x, float y, TextureRule textureRule);

		/// <summary>
		/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails.
		/// </summary>
		/// <param name="size">Nouvelle taille demandée</param>
		void resize(const sf::Vector2f& size);

		/// <summary>
		/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails.
		/// </summary>
		/// <param name="x">Nouvelle taille horizontale demandée</param>
		/// <param name="y">Nouvelle taille verticale demandée</param>
		void resize(float x, float y);

		/// <summary>
		/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation des règles de textures pour plus de détails.
		/// </summary>
		/// <param name="size">Nouvelle taille demandée</param>
		/// <param name="textureRule">Nouvelle règle de texture</param>
		void resize(const sf::Vector2f& size, TextureRule textureRule);

		/// <summary>
		/// Si la règle le permet, la taille de la tuile sera mise à jour selon la nouvelle taille. Voir la documentation pour plus de détails.
		/// </summary>
		/// <param name="x">Nouvelle taille horizontale demandée</param>
		/// <param name="y">Nouvelle taille verticale demandée</param>
		/// <param name="textureRule">Nouvelle règle de texture</param>
		void resize(float x, float y, TextureRule textureRule);

		/// <summary>
		/// Change la sous-texture de la tuile
		/// </summary>
		/// <param name="numberSubTexture">Correspond au numéro de la texture. Le premier numéro possible est 0 et va jusqu'au nombre de texture - 1. Si le numéro est non valide, le rectangle de texture reste inchangé.</param>
		void changeTextureRect(int numberSubTexture);

		/// <summary>
		/// Sert à uniquement changer la règle de texture
		/// </summary>
		/// <param name="textureRule">Nouvelle règle de texture</param>
		void setTextureRule(TextureRule textureRule);

		/// <summary>
		/// Sert à obtenir la règle de texture depuis l'extérieur de la tuile
		/// </summary>
		TextureRule getTextureRule();

		/// <summary>
		/// Bouge la tuile du vecteur rentré en paramètre de manière plus efficace que si rentré séparément
		/// </summary>
		/// <param name="offset">Déplacement à effectuer</param>
		void move(const sf::Vector2f& offset);

		/// <summary>
		/// Bouge la tuile du vecteur horizontal et vertical rentrés en paramètres de manière plus efficace que si rentré séparément
		/// </summary>
		/// <param name="offsetX">D�placement horizontal à effectuer</param>
		/// <param name="offsetY">D�placement vertical à effectuer</param>
		void move(float offsetX, float offsetY);

		/// <summary>
		/// Met la tuile au coin supérieur gauche depuis la position rentrée en paramètre
		/// </summary>
		/// <param name="position">Nouvelle position de la tuile</param>
		void setPosition(const sf::Vector2f& position);

		/// <summary>
		/// Met la tuile au coin supérieur gauche depuis la position rentrée en paramètre
		/// </summary>
		/// <param name="x">Nouvelle position horizontale</param>
		/// <param name="y">Nouvelle position verticale</param>
		void setPosition(float x, float y);

		/// <summary>
		/// Recharge les sommets pour que les tuiles suivent bien la nouvelle texture. Si le numéro de texture dépasse la nouvelle valeur maximale (contenue dans la classe Niveau), la sous-texture correspondra à la dernière disponible
		/// </summary>
		void reloadTexture();

		/// <summary>
		/// Permet d'obtenir l'adresse de l'objet
		/// </summary>
		virtual Tile* getThis();

		/// <summary>
		/// Crée un clone de l'objet pouvant contenir tous les champs de classe dérivée quelconque. Pour pouvoir permettre le clonage de classe dérivée, il faut surcharger cette méthode
		/// </summary>
		virtual std::unique_ptr<Tile> clone() const;

		/// <summary>
		/// Donne la hauteur de la sous-texture (utile si on veut limiter une tuile à une fois sa hauteur)
		/// </summary>
		float subTextureHeight() const;

		/// <summary>
		/// Donne la largeur de la sous-texture (utile si on veut répéter une texture un certain nombre de fois)
		/// </summary>
		float subTextureWidth() const;

		/// <summary>
		/// Donne les dimensions de la sous-texture
		/// </summary>
		sf::Vector2f subTextureSize() const;

		/// <summary>
		/// Change la couleur de la tuile
		/// </summary>
		/// <param name="color">Nouvelle couleur à appliquer</param>
		void changeColor(const sf::Color& color);

		/// <summary>
		/// Réinitialise la couleur de la tuile (met la couleur à blanc)
		/// </summary>
		void resetColor();

		/// <summary>
		/// Retourne la couleur de la tuile
		/// </summary>
		sf::Color getColor() const;

		/// <summary>
		/// Retourne la taille de la tuile
		/// </summary>
		sf::Vector2f getSize() const;

		/// <summary>
		/// Retourne l'index de la sous-texture
		/// </summary>
		int subTextureIndex() const;
	};
}


#endif 


