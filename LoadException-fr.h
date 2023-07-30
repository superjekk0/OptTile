#pragma once

#ifndef LOADEXCEPTION_FR_H
#define LOADEXCEPTION_FR_H

#include <string>
#include "OptTile.h"

namespace opt
{
	namespace fr
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
			/// Indique une exception au niveau du chargement d'un objet au chemin indiqu�
			/// </summary>
			/// <param name="path">Chemin qui a d�clench� l'exception</param>
			LoadException(const std::string& path);
		};
	}
}

#endif


