#include "../include/neighborhood.h"

Neighborhood::Neighborhood(Instance* i){
    this->instancia = i;
}


//Retira nodo e religa arestas.
Solution Neighborhood::retiraNodo(Solution s, int nodoSelected, int rota1){

	//apaga arestas adjacentes ao nodo da rota 1 e religa.
	int forwardNodoSelected = s.getRoute(rota1)->getForward(nodoSelected); //3
	int backwardNodoSelected = s.getRoute(rota1)->getBackward(nodoSelected); //0
	s.getRoute(rota1)->setForward(nodoSelected, -1);
	s.getRoute(rota1)->setForward(backwardNodoSelected, forwardNodoSelected);	
	s.getRoute(rota1)->setBackward(nodoSelected, -1);
	s.getRoute(rota1)->setBackward(forwardNodoSelected, backwardNodoSelected);
	return s;
}

//Adiciona nodo selecionado entre uma aresta.
Solution Neighborhood::addNodo(Solution s, int arestaI, int rota2, int nodoSelected){

    //apaga arestas selecionadas da rota 2 e insere nodo selecionado entre elas.
	int forwardNodoI = s.getRoute(rota2)->getForward(arestaI); //6
	s.getRoute(rota2)->setForward(arestaI, nodoSelected);
	s.getRoute(rota2)->setForward(nodoSelected, forwardNodoI);	
	s.getRoute(rota2)->setBackward(nodoSelected, arestaI);
	s.getRoute(rota2)->setBackward(forwardNodoI, nodoSelected);
	return s;
}

Solution Neighborhood::interRoutes(Solution s){

	int numRotas = this->instancia->getNumVehicles();
	int numNodos = this->instancia->getNumNodes();
	

	//cout << "dentro da inter " << endl;
	for(int rota1=0; rota1 < numRotas; rota1++){ //para cada rota
        s.getRoute(rota1)->printCaminho();
    }
    exit(0);
    
    
}










