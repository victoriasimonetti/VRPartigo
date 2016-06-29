#include "../include/Instance.h"
#include "../include/nodes.h"
#include "../include/route.h"
#include "../include/solution.h"
#include "../include/neighborhood.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>   
#include <math.h> 
using namespace std;
const int T= 100000;


/* Garantimos em uma solução que todos os clientes são atendidos e que não se pode "voltar no tempo". As restrições de janela de tempo e capacidade podem ser desrespeitadas e são contabilizadas na função objetivo com peso 1000. */

Instance *instance = NULL;

//Busca Tabu
Solution buscaTabu(Solution s){

	Solution bestSolution(instance);
	bestSolution = s;
	Solution currentSolution(instance);
	currentSolution = s;
	Neighborhood* nb = new Neighborhood(instance);
	int iter = 0;
	int melhorIter = 0;
	int BTmax = 100; //numero max de iterações sem melhora << ajustar parametro
	double aspiracao;
	vector<int> listaTabu(T, 0);

	while(iter - melhorIter <= BTmax){
		iter++;

		currentSolution  = nb->interRoutes(s, iter, listaTabu); 
		if(currentSolution.getTotalCost() != s.getTotalCost()){//se teve uma sol
		    int tenure = floor(iter/2);
		    int t = ((long long)(currentSolution.getTotalCost()*1000)) % T;
		    //t = ((long long)(currentSolution.getTotalCost())) % T;
            listaTabu[t] = iter + tenure; //atualiza lista tabu.
            s = currentSolution;       
        }
        cout << "solucao atual: " << currentSolution.getTotalCost() << endl;
		if(currentSolution.getTotalCost() - bestSolution.getTotalCost() <= -0.000001){
			bestSolution = currentSolution;
			melhorIter = iter;
			cout << "melhor solução: " << bestSolution.getTotalCost() << endl;
		}
		//atualiza funcao de aspiracao

	}
	return bestSolution;
}

//construtivo guloso
Solution constructionGuloso(){

    Solution initialSolution(instance);
	int veiculos = instance->getNumVehicles();
	int endNode = 0;
	int forwardNodoI = 0;
	int bestRoute = 0;
	unsigned int costBestRoute = instance->getNumNodes()*10000000;
	int forwardNodoSelected = 0;
	int backwardNodoSelected = 0;
	
	//Inicializa todas as rotas.
	for(int v=0; v<veiculos; v++){
	    initialSolution.getRoute(v)->setForward(0, instance->getNumNodes());
	    initialSolution.getRoute(v)->setBackward(instance->getNumNodes(), 0);
	}
    initialSolution.forcaBrutaRecalculaSolution();
	
	//Para cada cliente, é escolhido a rota onde ele causa menos custo.
	for(int c=1;  c<instance->getNumNodes(); c++){
	    costBestRoute = instance->getNumNodes()*10000000;
	    //Avalia todas as rotas inserindo o cliente, escolhe a que causa menor impacto.
	    for(int v=0; v<veiculos; v++){
	    
	        //Último nodo inserido
	        endNode = initialSolution.getRoute(v)->getBackward(instance->getNumNodes());
	        
	        //insere cliente na rota.    
	        forwardNodoI = initialSolution.getRoute(v)->getForward(endNode);
	        initialSolution.getRoute(v)->setForward(endNode, c);
	        initialSolution.getRoute(v)->setForward(c, forwardNodoI);	
	        initialSolution.getRoute(v)->setBackward(c, endNode);
	        initialSolution.getRoute(v)->setBackward(forwardNodoI, c);
	        
	        //calcula custo.
	        initialSolution.recalculateSolutionOnlyRoute(v);

	        if(initialSolution.getTotalCost() < costBestRoute){
	            bestRoute = v;
	            costBestRoute = initialSolution.getTotalCost();
	        }   
	        
	        //retira nodo da rota
	        forwardNodoSelected = initialSolution.getRoute(v)->getForward(c);
	        backwardNodoSelected = initialSolution.getRoute(v)->getBackward(c);
	        initialSolution.getRoute(v)->setForward(c, -1);
	        initialSolution.getRoute(v)->setForward(backwardNodoSelected, forwardNodoSelected);	
	        initialSolution.getRoute(v)->setBackward(c, -1);
	        initialSolution.getRoute(v)->setBackward(forwardNodoSelected, backwardNodoSelected);
	        initialSolution.recalculateSolutionOnlyRoute(v);
	    }
	    
	    //insere cliente na melhor rota
	    endNode = initialSolution.getRoute(bestRoute)->getBackward(instance->getNumNodes());
        forwardNodoI = initialSolution.getRoute(bestRoute)->getForward(endNode);
        initialSolution.getRoute(bestRoute)->setForward(endNode, c);
        initialSolution.getRoute(bestRoute)->setForward(c, forwardNodoI);	
        initialSolution.getRoute(bestRoute)->setBackward(c, endNode);
        initialSolution.getRoute(bestRoute)->setBackward(forwardNodoI, c);
        
        //calcula custo.
        initialSolution.recalculateSolutionOnlyRoute(bestRoute);
	    
	}
    cout << endl << "----------------- SOLUCAO INICIAL ---------------------------" << endl << endl;
	initialSolution.printSolution();  
	return initialSolution; 
}

//Construtivo Aleatório
Solution construction(){
    
    Solution initialSolution(instance);
	int veiculos = instance->getNumVehicles();
	
	for(int c=1; c<instance->getNumNodes(); c++){
	    int v = rand() % veiculos;
	    int indice =0;
	    while(initialSolution.getRoute(v)->getForward(indice) != -1){ //Busca último elemento adicionado, então força sair do depósito.
	        indice = initialSolution.getRoute(v)->getForward(indice);
	    }  
	
	    initialSolution.getRoute(v)->setForward(indice, c); 
	    initialSolution.getRoute(v)->setBackward(c, indice); 
	}
	
	//força voltar para o depósito.
	for(int r=0; r<instance->getNumVehicles(); r++){
	    int indice =0;
	    while(initialSolution.getRoute(r)->getForward(indice) != -1){ //Busca último elemento adicionado
	        indice = initialSolution.getRoute(r)->getForward(indice);
	    }  
		initialSolution.getRoute(r)->setForward(indice, instance->getNumNodes()); 
	    initialSolution.getRoute(r)->setBackward(instance->getNumNodes(), indice); 
	    initialSolution.calculateTimeServiceAndFaults(indice, instance->getNumNodes(), r);
	}

	initialSolution.forcaBrutaRecalculaSolution();
	//Adcionar a função objetivo os custo das infactibilidades de capacidade janela de tempo.
    cout << endl << "----------------- SOLUCAO INICIAL ---------------------------" << endl << endl;
	initialSolution.printSolution();   
	return initialSolution;    	
}


int main(int argc, char** argv){

    time_t seconds;
	time(&seconds);
    //srand((unsigned int) seconds);
    srand((unsigned int) 10);
    instance = new Instance(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.

    instance->readFile();
	//Solution s = construction();
	Solution s = constructionGuloso();

	s = buscaTabu(s); //double alfa, Solution s, double t0, int maxIterTemp
    //s.forcaBrutaRecalculaSolution();
	cout << endl << "----------------- SOLUCAO FINAL ---------------------------" << endl << endl;

	s.printSolution();

	return 0;
}

