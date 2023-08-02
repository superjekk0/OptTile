#pragma once

#ifndef LOADEXCEPTION_H
#define LOADEXCEPTION_H

#include <string>
#include "OptTile.h"

namespace opt 
{
	class OPTTILE_API LoadException
	{
	public:
		std::string m_error;

		/// <summary>
		/// Indique une erreur lors du chargement d'un objet � un chemin ind�termin� ///
		/// Indicates an error that occured during the loading of an object at an undefined path
		/// </summary>
		LoadException();

		/// <summary>
		/// Indique une exception au niveau du chargement d'un objet au chemin indiqu� ///
		/// Indicates an error that occured during the loading of an object at a defined path
		/// </summary>
		/// <param name="path">Chemin qui a d�clench� l'exception /// Path that has thrown the exception</param>
		LoadException(const std::string& path);
	};
}

#endif // !LOADEXCEPTION_H


