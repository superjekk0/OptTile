#include "pch.h"
#include "Exceptions.h"

opt::LoadException::LoadException() : m_error{}
{

}

opt::LoadException::LoadException(const std::string& path) : 
	m_error{ "An error occured at this path: " + path}
{

}