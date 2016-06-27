#ifndef _NEIGHBORHOOD_H
#define _NEIGHBORHOOD_H	

#include <iostream>
#include "../include/Instance.h"
#include "../include/solution.h"
#include <time.h>   

class Neighborhood {
protected:	
	Instance* instancia;
	
public:
	Neighborhood(Instance* instance);
	Solution interRoutes(Solution s);
	int rotaComCliente(Solution s, int route);
	Solution updateRota1(Solution s, int nodoSelected, int rota1);
	Solution updateRota2(Solution s, int arestaI, int rota2, int nodoSelected);
};


#endif

