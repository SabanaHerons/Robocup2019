#include "HeatMapRepresentation.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Debugging/DebugDrawings3D.h"

void HeatMapRepresentation::verify() const
{
  
}

void HeatMapRepresentation::draw() const
{
	DEBUG_DRAWING("representation:HeatMapRepresentation", "drawingOnField")
	{
		/*RECTANGLE2("representation:HeatMapRepresentation:rectangle", Vector2f(0,0), 1000, 
		1000, 0, 16, 
		Drawings::PenStyle::solidPen, ColorRGBA::black, 
		Drawings::solidBrush, ColorRGBA(0,0,0,0));*/
		for(int i = 0; i < numberOfZones; i++)
		{
			/*RECTANGLE2("representation:HeatMapRepresentation:rectangle", Vector2f(initialXs[i],initialYs[i]), stepSizeX, 
			stepSizeY, 0, 16, 
			Drawings::PenStyle::dashedPen, ColorRGBA(0,0,0,50), 
			Drawings::noBrush, ColorRGBA(0,0,0,0));*/
			FILLED_RECTANGLE("representation:HeatMapRepresentation", initialXs[i], initialYs[i], finalXs[i], finalYs[i], 
			0, Drawings::PenStyle::dashedPen, ColorRGBA(0,0,0,50), 
			Drawings::solidBrush, ColorRGBA(percentage[i]*255,0,(1-percentage[i])*255,40+100*percentage[i]));
			
		}
	}
}

