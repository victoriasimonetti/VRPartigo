#include "../include/solution.h"
#include <cstdlib>

Solution::Solution(Instance* i){
    this->instancia = i;
    this->delayedArrivalCost = 0.0;
    this->totalCost = 0.0;
	this->initialServiceTime.resize(this->instancia->getNumNodes()+1, 0.0);
	this->overCapacitated = 0;
	
	int veiculos = this->instancia->getNumVehicles();

	for(int k=0; k<veiculos; k++){ //abrir rota para todos os veículos 
		Route route(this->instancia->getNumNodes()+1);	
		routes.push_back(route);
    }	
}

int Solution::getOverCapacitated(){
	return this->overCapacitated;
}

void Solution::setOverCapacitated(int c){
	this->overCapacitated = c;
}

double Solution::getDelayedArrivalCost(){
    return this->delayedArrivalCost;
}

void Solution::setDelayedArrivalCost(double t){
    this->delayedArrivalCost = t;
}

double Solution::getTotalCost(){
    return this->totalCost;
}

Route* Solution::getRoute(int i){
    return &this->routes[i];
}

void Solution::setTotalCost(double t){
    this->totalCost = t;
}

//atualizar tempo de atendimento do nodo, tempo total da rota e infactibilidades (cap. e janela)
void Solution::calculateTimeServiceAndFaults(int i, int j, int v){
	
	cout  << this->instancia->getCost(i,j) << endl;
    this->getRoute(v)->setTotalCostRoute( this->getRoute(v)->getTotalCostRoute() + this->instancia->getCost(i,j));

	//tempo que o nodo j começa a ser atendido.
	this->initialServiceTime[j] = this->initialServiceTime[i] + this->instancia->getDurationTimeNode(i) + this->instancia->getCost(i,j);

	//se o veiculo chega antes do tempo, o tempo inicial é o mesmo de janela, se chega depois do fim da janela, acrescenta penalizações.
	if(this->initialServiceTime[j] <= this->instancia->getNode(j)->getInitialTime()){
		this->initialServiceTime[j] = this->instancia->getNode(j)->getInitialTime();
	}else if(this->initialServiceTime[j] > this->instancia->getNode(j)->getEndTime()){
		double diffService = this->initialServiceTime[j] - this->instancia->getNode(j)->getEndTime();
		this->routes[v].setLateCost( routes[v].getLateCost() + diffService); //add penalizaçao na rota
		this->delayedArrivalCost += diffService; //add penalização de atraso na solução, soma das penalizações de todas as rotas.
	}
	this->routes[v].setTotalTime(this->initialServiceTime[j]); //atualiza tempo total da rota

	this->routes[v].setTotalCapacitated( this->routes[v].getTotalCapacitated() +  this->instancia->getNode(j)->getDemand()); //atualiza capacidade da rota
	if(this->routes[v].getTotalCapacitated() > this->instancia->getCapacitated()){ //se ultrapassou a capacidade permitida
		int diffCapacitated = this->routes[v].getTotalCapacitated() - this->instancia->getCapacitated();
		this->routes[v].setOverCapacitated(diffCapacitated );//add penalização  de capacidade na rota
		if(diffCapacitated > this->instancia->getNode(j)->getDemand()){
		    this->overCapacitated += this->instancia->getNode(j)->getDemand(); //add penalização de excesso de carga na solução,soma das penalizações de todas as rotas.
		}else{
		    this->overCapacitated += diffCapacitated;
		}
	}
}

//Recalcula a solução a partir E apenas da rota que foi modificada.
void Solution::recalculateSolutionOnlyRoute(int v){

	//this->totalCost = 0.0;
	//cout << "custo da solucao antes de remover rota: " << this->getTotalCost() << endl;
	this->setTotalCost( this->getTotalCost() - (this->getRoute(v)->getTotalCostRoute() + 1000*this->getRoute(v)->getLateCost() + 1000*this->getRoute(v)->getOverCapacitated()));
		//cout <<  1000*this->getRoute(v)->getLateCost() << " " << 1000*this->getRoute(v)->getOverCapacitated();
		//cout << "custo da solucao depois de remover rota: " << this->getTotalCost() << endl;
	this->delayedArrivalCost -= this->getRoute(v)->getLateCost();
	this->overCapacitated -= this->getRoute(v)->getOverCapacitated();
	this->calculaRotaSolution(v);
	this->setTotalCost(this->getTotalCost() + this->getRoute(v)->getTotalCostRoute());
	this->setTotalCost(this->getTotalCost() + 1000 * this->getDelayedArrivalCost() + 1000 * this->getOverCapacitated());
   // this->setTotalCost(this->getTotalCost() + this->getRoute(v)->getTotalCostRoute() + 1000*this->getRoute(v)->getLateCost() - 1000*this->getRoute(v)->getOverCapacitated());
	cout << this->getTotalCost() << endl;
	
}

void Solution::calculaRotaSolution(int v){
	this->getRoute(v)->setLateCost(0.0); //custo total de atraso, incluindo o de chegar no depósito.
    this->getRoute(v)->setTotalTime(0.0); //tempo total da rota.
    this->getRoute(v)->setTotalCapacitated(0); //capacidade da rota.
    this->getRoute(v)->setOverCapacitated(0); //capacidade excedida da rota.
    this->getRoute(v)->setTotalCostRoute(0.0);

    int i =0;
    int j= this->getRoute(v)->getForward(i);
    while(j != this->instancia->getNumNodes()){
        this->calculateTimeServiceAndFaults(i, j, v);
        i = j;
        j = this->getRoute(v)->getForward(i); 
    }
	this->calculateTimeServiceAndFaults(i, j, v); 
}

void Solution::forcaBrutaRecalculaSolution(){

    this->delayedArrivalCost = 0.0;
    this->totalCost = 0.0;
	this->initialServiceTime.resize(this->instancia->getNumNodes()+1, 0.0);
	this->overCapacitated = 0;

    for(int v=0; v<this->instancia->getNumVehicles(); v++){
    	this->calculaRotaSolution(v);
		this->setTotalCost(this->getTotalCost() + this->getRoute(v)->getTotalCostRoute());
		cout <<  this->getRoute(v)->getTotalCostRoute() << endl;
    } 
    
    this->setTotalCost(this->getTotalCost() + 1000 * this->getDelayedArrivalCost() + 1000 * this->getOverCapacitated());
}

void Solution::printSolution(){

	for(int r=0; r<this->instancia->getNumVehicles(); r++){
		cout << "Total Time da rota: " << this->routes[r].getTotalTime() << endl;
		cout << "Atrasos da rota: " << this->routes[r].getLateCost() << endl;
		cout << "excesso de capacidade da rota: " << this->routes[r].getOverCapacitated() << endl;
		for(int c=0; c<this->instancia->getNumNodes()+1; c++){
			cout << this->routes[r].getForward(c) << " " ;
		}
		cout << endl;
		for(int c=0; c<this->instancia->getNumNodes()+1; c++){
			cout << this->routes[r].getBackward(c) << " " ;
		}
		cout << endl;
		cout << endl;

	}
	cout << "*******************************************************" << endl << endl;
	cout << "Custo da solução: " << this->getTotalCost() << endl;
	cout << "Penalidade atraso: " << this->getDelayedArrivalCost() << endl;
	cout << "Penalidade de excesso de carga: " << this->getOverCapacitated() << endl;
	cout << endl << "*******************************************************" << endl << endl;

}










