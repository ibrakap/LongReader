#include "Parser.h"

LineInfo GetALineFromBuffer(int StartPoint, std::string Buffer)
{
	std::string TempBuffer;

	int i = StartPoint;
	bool Found = false;
	do
	{
		TempBuffer = TempBuffer + Buffer[i];
		if (Buffer[i] == '\n')
		{
			Found = true;
		}
		i++;
	} while (!Found);
	return { TempBuffer, i };
}
std::string GetLinesFromBuffer(const std::string& Buffer, int DesiredLines)
{
	std::string TempBuffer;
	LineInfo li;
	li.EndPoint = 0;

	for (int i = 0; i < DesiredLines; i++)
	{
		li = GetALineFromBuffer(li.EndPoint, Buffer);
		TempBuffer = TempBuffer + li.Line;
	}
	return TempBuffer;
}
PartInfo CalculateRestInfo(uint64_t Size, uint64_t PartSize)
{
	uint64_t Rest = Size % PartSize;
	uint64_t Divided = Size / PartSize;
	return { PartSize, Divided, Rest };
}
void GetAllLineAndPositions(File fi)
{
	std::string Temp;
	std::vector<uint64_t> Pos;
	uint64_t size = fi.getsize();
	PartInfo pi = CalculateRestInfo(size, 4096);
	LineInfo li;
	li.EndPoint = 0;

	bool finished = false;
	if (pi.DividedPart == 0)
	{
		fi.read(Temp, pi.RestSize);
		LineInfo li;
		li.EndPoint = 0;
		std::string Atmp;
		for (uint64_t i = 0; i < pi.RestSize; i++)
		{
			if (Temp[i] == '\n')
			{
				Pos.push_back(i);
			}
		}
		return;
	}
	else
	{
		

	 for (uint64_t i = 0; i < pi.DividedPart; i += pi.PartSize)
	 {
	 	fi.read(Temp, pi.PartSize);
	 	for (uint64_t ii = 0; ii < pi.PartSize; ii++)
	 	{
	 		if (Temp[ii] == '\n')
	 		{
	 			if (Pos.size() == 0)
	 				Pos.push_back(ii);
	 			else
	 				Pos.push_back(Pos.back() + ii);
	 		}
	 	}
	 }
	 
	 for (uint64_t ai = 0; ai < pi.RestSize; ai++)
	 {
	 	if (Temp[ai] == '\n')
	 	{
	 		if (Pos.size() == 0)
	 			Pos.push_back(ai);
	 		else
	 			Pos.push_back(Pos.back() + ai);
	 	}
	 }
	}

	return;
}