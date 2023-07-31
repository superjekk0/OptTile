#include "pch.h"
#include "LoadException-fr.h"

opt::fr::LoadException::LoadException() : m_error{}
{

}

opt::fr::LoadException::LoadException(const std::string& path) : 
	m_error{ "Une erreur s'est produite a ce chemin : " + path}
{

}