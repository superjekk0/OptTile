///
/// Version de la biblioth�que datant de 2023 - Library writen in 2023
/// 
/// ========================================================================================
/// ========================================================================================
/// 
///	Fait par Charles Mandziuk. Aucune mention n'est n�cessaire, mais est toujours appr�ci�e
/// 
/// Writen by Charles Mandziuk. No any mention is mendatory, but is always appreciated
///

#ifndef LOADEXCEPTION_H
#define LOADEXCEPTION_H

#include <string>
#include <string_view>
#include "OptTile.h"

namespace opt 
{
	class OPTTILE_API Exception {
	protected:
		std::string m_error;
	
	public:

		Exception();

		Exception(const std::string& message);

		const std::string_view showMessage() const;

		virtual ~Exception() = 0;
	};

	class OPTTILE_API LoadException : Exception
	{
	public:
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

	class OPTTILE_API ParseException : Exception {
	public:

		ParseException();

		ParseException(const std::string& fautiveString);
	};
}

#endif // !LOADEXCEPTION_H


