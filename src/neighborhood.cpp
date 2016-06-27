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
	int aresta1, aresta2, aresta3, aresta4;
	int aux = 0 ;
	Solution solutionAux(this->instancia);
	solutionAux = s;
	Solution bestSolution(this->instancia);
	bestSolution = s;

	cout << "dentro da inter " << endl;
	for(int rota1=0; rota1 < numRotas; rota1++){ //para cada rota
		for(int rota2=rota1+1; rota2 < numRotas; rota2++){ //para cada rota
			aresta1=0; aresta2=0; aresta3=0; aresta4=0;
			aresta3 = s.getRoute(rota2)->getForward(aresta3); 																												aresta4 = s.getRoute(rota2)->getForward(aresta4);
			cout << "dentro do for 1" << endl;
			if(rota1 != rota2){
				cout << "dentro do for 2" << endl;
				while(aresta1 != numNodos){
						cout << "dentro do while 1" << endl;
						aresta2 = aresta1;
					while(aresta2 != numNodos){	
						cout << "dentro do while 2" << endl;					
						while(aresta3 != numNodos){	
						cout << "dentro do while 3" << endl;
							aresta4 = aresta3;						
							while(aresta4 != numNodos){ //observar para nodos do deposito, ainda n tenho certeza
								cout << "dentro do while 4" << endl;
								//retira trecho de rota da rota 1
								aux = aresta1;
								while(aux !=  s.getRoute(rota1)->getForward(aresta2)){
									cout << "dentro do while 5" << endl;
									solutionAux = retiraNodo(solutionAux, s.getRoute(rota1)->getForward(aux), rota1);
									aux = s.getRoute(rota1)->getForward(aux);
								}

								//retira trecho de rota da rota 2
								aux = aresta3;
								while(aux !=  s.getRoute(rota2)->getForward(aresta4)){
									cout << "dentro do while 6 rota" << endl;
									solutionAux = retiraNodo(solutionAux, s.getRoute(rota2)->getForward(aux), rota2);
									aux = s.getRoute(rota2)->getForward(aux);
								}
																solutionAux.printSolution();
								//add novo trecho na rota 1
								aux = aresta1;
								int nodoSelected = s.getRoute(rota2)->getForward(aresta3);
								while(aux != aresta4){
									cout << "dentro do while 7" << endl;
									solutionAux = addNodo(solutionAux, aux, rota1, nodoSelected);	
									aux = nodoSelected;
									nodoSelected = s.getRoute(rota2)->getForward(aux);
								}
								solutionAux.getRoute(rota1)->setForward(aresta4, s.getRoute(rota1)->getForward(aresta2));


								//add novo trecho na rota 2
								aux = aresta3;
								nodoSelected = s.getRoute(rota1)->getForward(aresta1);
								while(aux != aresta2){
									cout << "dentro do while 8" << endl;
									solutionAux = addNodo(solutionAux, aux, rota2, nodoSelected);	
									aux = nodoSelected;
									nodoSelected = s.getRoute(rota1)->getForward(aux);
								}
								solutionAux.getRoute(rota2)->setForward(aresta2, s.getRoute(rota2)->getForward(aresta4));

								solutionAux.calculaRotaSolution(rota1);
								solutionAux.calculaRotaSolution(rota2);

								//Guarda melhor solução da vizinhança
								if(solutionAux.getTotalCost() < bestSolution.getTotalCost()){
									bestSolution = solutionAux;
									cout << "sol vizinhança " << bestSolution.getTotalCost() << endl;
								}
								solutionAux.printSolution();
								exit(0);
								//restaura
								solutionAux = s;	
								aresta1 = s.getRoute(rota1)->getForward(aresta1); 	
								aresta2 = s.getRoute(rota1)->getForward(aresta2); 	
								aresta3 = s.getRoute(rota2)->getForward(aresta3); 																												aresta4 = s.getRoute(rota2)->getForward(aresta4); 								
																				
							}
						}						
					}				
				}						
			}
		}
	}	
	exit(0);
    return bestSolution;
}










