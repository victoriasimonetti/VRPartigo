#include "../include/nodes.h"

Nodes::Nodes(int id, int initialTime, int endTime, int durationTime, int demand, int coordX, int coordY){
	this->id = id;
	this->initialTime = initialTime;
	this->endTime = endTime;
	this->durationTime = durationTime;
	this->demand = demand;
	this->coordX = coordX;
	this->coordY = coordY;
}

int Nodes::getCoordX(){
    return this->coordX;
}

int Nodes::getCoordY(){
    return this->coordY;
}

int Nodes::getId(){
	return this->id;
}

int Nodes::getInitialTime(){
	return this->initialTime;
}

int Nodes::getEndTime(){
	return this->endTime;
}

int Nodes::getDurationTime(){
	return this->durationTime;
}

int Nodes::getDemand(){
    return this->demand;
}

