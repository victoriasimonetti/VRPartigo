#ifndef _NODES_H
#define _NODES_H

using namespace std;

class Nodes {
private:
	int id;
	int initialTime;
	int endTime;
	int durationTime;
	int demand;
	int coordX;
	int coordY;

public:
	Nodes(int id, int initialTime, int endTime, int durationTime, int demand, int coordX, int coordY);
	int getId();
	int getDemand();
	int getCoordX();
	int getCoordY();
	int getInitialTime();
	int getEndTime();
	int getDurationTime(); 

};

#endif
