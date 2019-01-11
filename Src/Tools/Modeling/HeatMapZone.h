class HeatMapZone
{ 
public: 
    int initialX;
	int finalX;
	int initialY;
	int finalY;
	int timeInside;
	HeatMapZone(int x1_, int x2_, int y1_, int y2_);
	void updateTime(float x, float y);
};
