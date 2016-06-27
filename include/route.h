#ifndef _ROUTE_H
#define _ROUTE_H	

#include <vector>
#include <iostream>
#define NODE_NOT_IN_ROUTE -1
using namespace std;

class Route {
private:	
	vector<int> forward;
	vector<int> backward;
	double lateCost; //custo total de atraso, incluindo o de chegar no depósito.
	double totalTime; //tempo total da rota.
	int totalCapacitated; //capacidade da rota.
	int overCapacitated; //capacidade excedida da rota.
	double totalCostRoute;

public:
	Route(int n);
	int getForward(int indice);
	int getBackward(int indice);
	int getTotalCapacitated();
	void setTotalCapacitated(int c);
	int getOverCapacitated();
	void setOverCapacitated(int c);
	double getLateCost();
	void setLateCost(double c);
	void setForward(int indice, int valor);
	void setBackward(int indice, int valor);
	double getTotalTime();
	void setTotalTime(double time);	
	double getTotalCostRoute();
	void setTotalCostRoute(double time);
};


#endif
