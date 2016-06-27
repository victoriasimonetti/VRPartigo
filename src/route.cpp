#include "../include/route.h"

#include <stdio.h>

Route::Route(int n) {
	forward.resize(n, NODE_NOT_IN_ROUTE); //Inicializa com nodos -1, não com 0 pq o depósito é 0.
	backward.resize(n, NODE_NOT_IN_ROUTE);
	this->lateCost = 0;
	this->totalTime = 0;
	this->totalCapacitated = 0;
	this->overCapacitated = 0;
	this->totalCostRoute = 0;
}

double Route::getTotalCostRoute(){
    return this->totalCostRoute;
}

void Route::setTotalCostRoute(double time){
    this->totalCostRoute = time;
}

int Route::getOverCapacitated(){
	return this->overCapacitated;
}

void Route::setOverCapacitated(int c){
	this->overCapacitated = c;
}

double Route::getLateCost(){
	return this->lateCost;
}

void Route::setLateCost(double c){
	this->lateCost = c;
}

int Route::getTotalCapacitated(){
	return this->totalCapacitated;
}

void Route::setTotalCapacitated(int c){
	this->totalCapacitated = c;
}

int Route::getForward(int indice){
    return this->forward[indice];
}

int Route::getBackward(int indice){
    return this->backward[indice];
}

void Route::setForward(int indice, int valor){ //sainte
    this->forward[indice] = valor;
}
void Route::setBackward(int indice, int valor){ //entrante
    this->backward[indice] = valor;
}

double Route::getTotalTime(){
	return this->totalTime;
}

void Route::setTotalTime(double time){
	this->totalTime = time;
}




