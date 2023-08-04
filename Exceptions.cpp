#include "pch.h"
#include "Exceptions.h"

opt::Exception::Exception() : m_error{}
{

}

opt::Exception::Exception(const std::string& message) : m_error{ message }
{

}

opt::Exception::~Exception()
{

}

const std::string_view opt::Exception::showMessage() const
{
	return m_error;
}

opt::LoadException::LoadException() : Exception("An error occured while loading a file")
{

}

opt::LoadException::LoadException(const std::string& path) : Exception("An error occured at this path: " + path)
{

}

opt::ParseException::ParseException() : Exception("A parsing error occured while parsing a string")
{

}

opt::ParseException::ParseException(const std::string& fautiveString) : Exception("A parsing error occured while parsing this string: " + fautiveString)
{
	
}