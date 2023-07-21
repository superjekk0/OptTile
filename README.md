Important: the most accurate version of the Readme.md is the French version, wich is lower in the document.

================================================================================================================================================================================

Welcome in the project OptTile! 

This project has the goal to wrap a few SFML elements under a unique class, but also by making the tiles as convenient to use as the Sprite class from SFML 2.5.1
To make sure that the library works, it is necessary to have the graphic libraries used by SFML 2.5.1. For convenience, the library is contained inside the "opt" namespace.
The main classes inside the library are Tile and Level. The Level class contains a few datas, such as a generic list of Tile, a texture and sub-texture rectangles.
It can also be drawn directly by a RenderTarget from SFML 2.5.1 without having to do a loop.
The Tile class is an element wich can be drawn when contained inside a Level object. It mainly contains a reference from the Level's texture and coordinates of the Tile and of the sub-texture.

Feel free to contribute if you wish to. Note however that it may take time to see your contributions to the main branch.

================================================================================================================================================================================

Bienvenue dans le projet OptTile!

Ce projet a pour objectif d'encapsuler certains éléments SFML qui sont pénibles individuellement sous quelques classes simplifiées, comme s'il s'agissait de la classe Sprite de SFML 2.5.1
Pour faire fonctionner la bibliothèque de contenu, il est obligatoire d'avoir les bibliothèques graphiques utilisées par SFML 2.5.1. Pour la simplicité d'utilisation, la bibliothèque utilise l'espace de nom "opt".
Les classes principales de la bibliothèque sont Tile et Level. La classe Level contient plusieurs données, telles qu'une liste générique de Tile, une texture sf::Texture et des rectangles de sous-texture.
Elle peut être dessinée directement par un élément RenderTarget compatible de SFML 2.5.1 sans l'aide d'une boucle.
La classe Tile est un élément pouvant être dessiné lorsque contenu à l'intérieur d'un objet Level. Les membres principaux sont une référence de la texture de l'objet Level et les coordonnées de la tuile (Tile) et de sa sous-texture.

Sentez-vous libre de contribuer si vous le désirez. Sachez cependant qu'il pourrait se passer un long moment entre votre contribution et l'apparition sur la branche principale.
