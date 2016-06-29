#include "../include/neighborhood.h"
const int T = 100000;

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

Solution Neighborhood::interRoutes(Solution s, int iter, vector<int>& listaTabu){

	Solution auxSolution(instancia);
	Solution bestSolution(instancia);
	auxSolution = s;
	bestSolution = s;
	int numRotas = this->instancia->getNumVehicles();
	vector<int> caminhoRota1;
	vector<int> caminhoRota2;
	int aresta1, aresta2, aresta3, aresta4;
	int i, auxAresta;
	int iterSemMelhora = 0, iterSemMelhora2 = 0;
	double ultimoCusto = 0, melhorCustoLocal = 0, melhorCustoGlobal = 0;

	for(int rota1=0; rota1 < numRotas; rota1++){
		caminhoRota1 = s.getRoute(rota1)->getCaminho();
		for(int rota2 = rota1+1; rota2 < numRotas; rota2++){
			caminhoRota2 = s.getRoute(rota2)->getCaminho();

			for(aresta1=0; aresta1<caminhoRota1.size()-1; aresta1++){
				iterSemMelhora2 = 0;
				melhorCustoGlobal = auxSolution.getTotalCost();

				for(aresta3=0; aresta3<caminhoRota2.size()-1; aresta3++){
					melhorCustoLocal = 99999999;
					
					for(aresta2=aresta1; aresta2<caminhoRota1.size()-1; aresta2++){
						iterSemMelhora = 0;
						ultimoCusto = auxSolution.getTotalCost();
					    
						for(aresta4=aresta3; aresta4<caminhoRota2.size()-1; aresta4++){
					
						    if(aresta4 - aresta3 <= 7 && aresta2 - aresta1 <= 7){
							    for(i=aresta1+1; i<=aresta2; i++){
								    auxSolution = retiraNodo(auxSolution, caminhoRota1[i], rota1);
							    }
							    for(i=aresta3+1; i<=aresta4; i++){
								    auxSolution = retiraNodo(auxSolution, caminhoRota2[i], rota2);
							    }
							    //add nodo na rota 2.
							    auxAresta = caminhoRota2[aresta3];
							    for(i=aresta1+1; i<=aresta2; i++){
								    auxSolution = addNodo(auxSolution, auxAresta, rota2, caminhoRota1[i]);
								    auxAresta = caminhoRota1[i];
							    }
							    //add nodo na rota 1.
							    auxAresta = caminhoRota1[aresta1];
							    for(i=aresta3+1; i<=aresta4; i++){
								    auxSolution = addNodo(auxSolution, auxAresta, rota1, caminhoRota2[i]);
								    auxAresta = caminhoRota2[i];
							    }

							    //recalcula
							    auxSolution.recalculateSolutionOnlyRoute(rota1);
							    auxSolution.recalculateSolutionOnlyRoute(rota2);
				                			                
							    //verifica se eh a melhor e n esta na lista tabu
							    int t = ((long long)(auxSolution.getTotalCost()*1000)) % T;
							    if(listaTabu[t] - iter <= 0){

									//Add em iter sem melhora -> monitonicamente	        
									if(auxSolution.getTotalCost() > ultimoCusto){
										iterSemMelhora++;
										ultimoCusto = auxSolution.getTotalCost();
									}else{
										iterSemMelhora = 0;
									}

									//guardar a melhor solução iter y1 e y2
									if(auxSolution.getTotalCost() < melhorCustoLocal){
										melhorCustoLocal = auxSolution.getTotalCost();
									}

									//atualiza para melhor solução
							        if(auxSolution.getTotalCost() - bestSolution.getTotalCost() <= -0.000001){
							            bestSolution = auxSolution;														   
							        }
							    }
					
							    auxSolution = s;

								//se tiver 3 iterações sem melhora, cai fora.
								if(iterSemMelhora == 3){
									break;
								}
						    }
					    }
				    }
					if(melhorCustoLocal > melhorCustoGlobal){
						iterSemMelhora2++;
						melhorCustoGlobal = melhorCustoLocal;
					}else{
						iterSemMelhora2 = 0;
						melhorCustoGlobal = auxSolution.getTotalCost();
					}

					if(iterSemMelhora2 == 3){
						break;
					}	
				}	
			}	
		}
	}	
	
    return bestSolution;
}










