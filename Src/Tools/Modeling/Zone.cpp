#include "Zone.h"

Zone::Zone(int x1_, int x2_, int y1_, int y2_)
{
	timeInside = 0;
	initialX = x1_;
	finalX = x2_;
	initialY = y1_;
	finalY = y2_;
}

void Zone::updateTime(float x, float y)
{
	if(x >= initialX && x < finalX && y >= initialY && y < finalY)
	{
		this->timeInside += 1;
	}
}

