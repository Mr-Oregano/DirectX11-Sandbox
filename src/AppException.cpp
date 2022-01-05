
#include "AppException.h"

#include <sstream>

MyAppException::MyAppException(int line, const char *fileName) noexcept
	: line(line), fileName(fileName)
{}

const char *MyAppException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char *MyAppException::GetType() const noexcept
{
	return "App Exception";
}

std::string MyAppException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << fileName << std::endl << "[Line] " << line;
	return oss.str();
}