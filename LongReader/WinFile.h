#pragma once
#include <iostream>
#include <Windows.h>

#define DBG_STR(X) 	OutputDebugStringA(X)

namespace FileMode
{
	constexpr DWORD FILE_READ	= GENERIC_READ;
	constexpr DWORD FILE_WRITE	= GENERIC_WRITE;
};

namespace SeekMode 
{
	constexpr DWORD begin	= FILE_BEGIN;
	constexpr DWORD current	= FILE_CURRENT;
	constexpr DWORD end		= FILE_END;
}

class File
{
public:
	// Constructors
	File(void);
	File(const char* Filepath, DWORD mode);

	// Open read close
	void open(const char* Filepath, DWORD mode);
	
	void read(std::string& Buffer);
	void read(std::string& Buffer, DWORD NumberOfBytesToRead);

	
	uint64_t getsize();
	DWORD getpos();

	void seek(LONG pos, DWORD mode);
	void seek(LONG pos);
	
	void write(const char* Buffer);

	void close();
	
	// Deconstructor
	~File();

private:
	HANDLE hFile = nullptr;
};