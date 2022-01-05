#pragma once

#include <exception>
#include <string>

class MyAppException : public std::exception
{
public:
	MyAppException(int line, const char *fileName) noexcept;

	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	inline int GetLine() const noexcept { return line; }
	inline const std::string& GetFile() const noexcept { return fileName; }
	std::string GetOriginString() const noexcept;

private:
	int line;
	std::string fileName;

protected:
	mutable std::string whatBuffer;
};