#include "WinFile.h"
#include <Windows.h>

File::File(void) 
	: hFile(nullptr)
{
	DBG_STR("[DEBUG] Constructor initiliazed!\n");
}
File::File(const char* Filepath, DWORD mode) 
	: hFile(nullptr)
{
	DBG_STR("[DEBUG] Constructor initiliazed with parameters!\n");
	hFile = CreateFileA(Filepath, mode, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("Constructor failed!");
	}
	
	DWORD GErr = GetLastError();
	if (GErr == ERROR_FILE_NOT_FOUND)
	{
		hFile = CreateFileA(Filepath, mode, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

void File::open(const char* Filepath, DWORD mode)
{
	if (hFile != nullptr)
	{
		DBG_STR("[DEBUG] Close file before open!\n");
		close();
	}

	hFile = CreateFileA(Filepath, mode, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DBG_STR("[DEBUG] File doesn't exist creating file...\n");
	}
	
	DWORD GErr = GetLastError();
	if (GErr == ERROR_FILE_NOT_FOUND)
	{
		hFile = CreateFileA(Filepath, mode, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

uint64_t File::getsize()
{
	LARGE_INTEGER li;
	GetFileSizeEx(hFile, &li);
	return li.QuadPart;
}
DWORD File::getpos()
{
	DWORD pos = SetFilePointer(hFile, 0, 0, FILE_CURRENT);
	if (pos == INVALID_SET_FILE_POINTER)
	{
		throw std::runtime_error("getpos failed!");
	}
	else
	{
		return pos;
	}
}

void File::read(std::string& Buffer)
{
	DWORD SizeOfFile = getsize() - getpos();	
	Buffer.resize(SizeOfFile);
	
	DWORD ReadBytes;
	BOOL bRead = ReadFile(hFile, (void*)&Buffer[0], SizeOfFile, &ReadBytes, NULL);
	if (bRead == FALSE)
	{
		throw std::runtime_error("Read failed!");
	}
}
void File::read(std::string& Buffer, DWORD NumberOfBytesToRead)
{
	Buffer.resize(NumberOfBytesToRead);
	DWORD ReadBytes;
	BOOL bRead = ReadFile(hFile, (void*)&Buffer[0], NumberOfBytesToRead, &ReadBytes, NULL);
	if (bRead == FALSE)
	{
		throw std::runtime_error("Read failed!");
	}

}



void File::seek(LONG pos, DWORD mode)
{
	DWORD fpseek = SetFilePointer(hFile, pos, NULL, mode);
	if (fpseek == INVALID_SET_FILE_POINTER)
	{
		throw std::runtime_error("seek failed!");
	}
}
void File::seek(LONG pos)
{
	DWORD fpseek = SetFilePointer(hFile, pos, NULL, SeekMode::current);
	if (fpseek == INVALID_SET_FILE_POINTER)
	{
		throw std::runtime_error("seek failed!");
	}
}

void File::write(const char* Buffer) 
{ 
	BOOL bWrite = WriteFile(hFile, (const void*)Buffer, lstrlenA(Buffer), NULL, NULL); 
	if (bWrite == FALSE)
	{
		throw std::runtime_error("write failed!");
	}
}

void File::close()
{
	if (hFile == nullptr) 
	{ 
		DBG_STR("[DEBUG] File already closed!\n"); 
		return; 
	}

	BOOL bClose = CloseHandle(hFile);
	if (bClose == 0) 
	{ 
		throw std::runtime_error("Closing handle failed!"); 
	}
	hFile = nullptr;
	return;
}

File::~File()
{
	DBG_STR("[DEBUG] Deconsturctor initiliazed!\n");
	if (hFile == nullptr) 
	{ 
		return; 
	}

	BOOL bClose = CloseHandle(hFile);
	if (bClose == 0) 
	{ 
		throw std::runtime_error("Closing handle failed!"); 
	}
	hFile = nullptr;
	return;
}