#include "../include/neighborhood.h"

Neighborhood::Neighborhood(Instance* i){
    this->instancia = i;
}

int Neighborhood::rotaComCliente(Solution s, int route){
	for(int i=1; i<this->instancia->getNumNodes(); i++){
		if(s.getRoute(route)->getForward(i) != -1){
			return 1; //pelo menos um cliente está nessa rota.
		}
	}
	return 0;
}

Solution Neighborhood::updateRota1(Solution s, int nodoSelected, int rota1){

	//apaga arestas adjacentes ao nodo da rota 1 e religa.
	int forwardNodoSelected = s.getRoute(rota1)->getForward(nodoSelected); //3
	int backwardNodoSelected = s.getRoute(rota1)->getBackward(nodoSelected); //0
	s.getRoute(rota1)->setForward(nodoSelected, -1);
	s.getRoute(rota1)->setForward(backwardNodoSelected, forwardNodoSelected);	
	s.getRoute(rota1)->setBackward(nodoSelected, -1);
	s.getRoute(rota1)->setBackward(forwardNodoSelected, backwardNodoSelected);
	return s;
}

Solution Neighborhood::updateRota2(Solution s, int arestaI, int rota2, int nodoSelected){

    //apaga arestas selecionadas da rota 2 e insere nodo selecionado entre elas.
	int forwardNodoI = s.getRoute(rota2)->getForward(arestaI); //6
	s.getRoute(rota2)->setForward(arestaI, nodoSelected);
	s.getRoute(rota2)->setForward(nodoSelected, forwardNodoI);	
	s.getRoute(rota2)->setBackward(nodoSelected, arestaI);
	s.getRoute(rota2)->setBackward(forwardNodoI, nodoSelected);
	return s;
}

Solution Neighborhood::interRoutes(Solution s){

	//seleciona duas rotas aleatoriamente, rota 1 com pelo menos um cliente. ** rota 2 != rota 1.
	int rota1 = rand() % this->instancia->getNumVehicles(); //0 a veiculos-1
	while (rotaComCliente(s, rota1) == 0){
		rota1 = rand() % this->instancia->getNumVehicles();	
	}

	int rota2 = rand() % this->instancia->getNumVehicles();
	while (rota2 == rota1){
		rota2 = rand() % this->instancia->getNumVehicles();	
	}
	
	//rota 1 : seleciona 1 nodo aleatoriamente, não pode ser depósito e tem q ser cliente da rota.
	int nodoSelected = s.getRoute(rota1)->getForward(rand() % this->instancia->getNumNodes());
	while (nodoSelected == -1 || nodoSelected == this->instancia->getNumNodes()){
		nodoSelected = s.getRoute(rota1)->getForward(rand() % this->instancia->getNumNodes());
	}

	// rota 2 seleciona uma aresta.
	int arestaI = rand() % this->instancia->getNumNodes();
	int arestaJ = s.getRoute(rota2)->getForward(arestaI);

	while (arestaJ == -1){
        arestaI = rand() % this->instancia->getNumNodes();
	    arestaJ = s.getRoute(rota2)->getForward(arestaI);
	}

	s = updateRota1(s, nodoSelected, rota1);
	s = updateRota2(s, arestaI, rota2, nodoSelected);

	cout << "porrota 1" << endl;
	s.recalculateSolutionOnlyRoute(rota2);
	cout << "porrota 2" << endl;
	s.recalculateSolutionOnlyRoute(rota1);
	s.printSolution();
	double srota = s.getTotalCost();

	cout << "porforcabruta" << endl;
	s.forcaBrutaRecalculaSolution();
	s.printSolution();
	double sbruta = s.getTotalCost();
	

	if(srota != sbruta){
		cout << "bruta " << sbruta << "  " << "srota  " << srota << endl;
		exit(0);
	}
	
    return s;
}










