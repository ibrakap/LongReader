#pragma once
#include <iostream>
#include <vector>
#include "WinFile.h"

struct LineInfo
{
	std::string Line;
	int EndPoint;
};

struct PartInfo
{
	uint64_t PartSize;
	uint64_t DividedPart;
	uint64_t RestSize;
};
LineInfo GetALineFromBuffer(int StartPoint, std::string Buffer);
std::string GetLinesFromBuffer(const std::string& Buffer, int DesiredLines);
PartInfo CalculateRestInfo(uint64_t Size, uint64_t PartSize);
void GetAllLineAndPositions(File fi);
