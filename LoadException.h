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
		/// Indique une exception au niveau du chargement d'un objet quelconque
		/// </summary>
		LoadException();

		/// <summary>
		/// Indique une exception au niveau du chargement d'un objet au chemin indiqué
		/// </summary>
		/// <param name="path">Chemin qui a déclenché l'exception</param>
		LoadException(const std::string& path);
	};
}

#endif // !LOADEXCEPTION_H


