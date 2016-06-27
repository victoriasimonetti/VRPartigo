#include "../include/Instance.h"
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <math.h>

Instance::Instance(const char* filename){
	fileRead = fopen(filename, "rt");
	this->filename = std::string(filename);
	if(!fileRead){/*Erro*/
		printf("File %s not found.\n", filename);
		exit(-1);
	}
}

Nodes* Instance::getNode(int n){
    Nodes* auxNodo = &this->nodes[n];
	return auxNodo;
}

int Instance::getNumVehicles(){
	return this->numVehicles;
}

int Instance::getCapacitated(){
    return this->capacitated;
}

void Instance::setCapacitated(int n){
    this->capacitated = n;
}

double Instance::getDurationTimeNode(int n){
	return this->nodes[n].getDurationTime();
}

double Instance::getInitialTimeNode(int n){
	return this->nodes[n].getInitialTime();
}
	
double Instance::getEndTimeNode(int n){
	return this->nodes[n].getEndTime();
}

double Instance::getCost(int i, int j){
	return this->cost[i][j];
}

int Instance::getNumNodes(){
	return this->numNodes;
}

void Instance::setNumNodes(int n){
	this->numNodes = n;
}

void Instance::readFile(){

    char string[100];
	int numVeiculos=0, capacidade=0;
	//Eliminar as 4 primeiras linhas.
	for(int i=0; i<4; i++){
	    fscanf(fileRead, "%s", string);
	}
    fscanf(fileRead, "%i %i\n", &numVeiculos, &capacidade);
    this->numVehicles = numVeiculos;
    this->capacitated = capacidade;
    //cout << "veiculos " << numVeiculos << "   capacidade " << capacidade << endl; 
    //Eliminar as 4 seguintes linhas, pega por string ¬¬.
	for(int i=0; i<12; i++){
	    fscanf(fileRead, "%s \n", string);
	}
    
    int idNode=0, coorx=0, coory=0, demanda=0, ti=0, tf=0, ts=0;   
    
    while(fscanf(fileRead, "%i %i %i %i %i %i %i" , &idNode, &coorx, &coory, &demanda, &ti, &tf, &ts) != EOF){			
		Nodes n(idNode, ti, tf, ts, demanda, coorx, coory);
	    //cout << idNode << " " << coorx << " " << coory << " " << demanda << " " << ti << " " << tf << " " << ts << endl;
		this->nodes.push_back(n);
	}
	this->numNodes = idNode+1; //Número de clientes mais o depósito e o depósito virtual.
	//Add depósito virtual, igual ao primeiro nodo. (depósito 0)
	Nodes n(this->numNodes, nodes[0].getInitialTime(), nodes[0].getEndTime(), nodes[0].getDurationTime(), nodes[0].getDemand(), nodes[0].getCoordX(), nodes[0].getCoordY());
	this->nodes.push_back(n);
    //cout << idNode+1 << " " <<  nodes[0].getCoordX() << " " << nodes[0].getCoordY() << " " << nodes[0].getDemand() << " " << nodes.at(0).getInitialTime() << " " << nodes[0].getEndTime() << " " <<  nodes[0].getDurationTime() << " " <<endl;
    fclose(this->fileRead);
	this->calculateCost();
	this->calculateT();
}

void Instance::calculateCost(){

	cost.resize(numNodes+1, vector<double>(numNodes+1,0)); 
	
	for (int i=0; i<this->numNodes+1; i++){
		//cout << i << " " ;
		for (int j=0; j<this->numNodes+1; j++){
			if(i==j){
				this->cost[i][j] = 0.0;
				//cout << "0" << "  ";
			}else{
				//Equação de cálculo de distância (hipotenusa) como apresentado no artigo.
				double eq1 = pow( (this->nodes[i].getCoordX() - this->nodes[j].getCoordX()), 2 );
				double eq2 = pow( (this->nodes[i].getCoordY() - this->nodes[j].getCoordY()), 2 );
				double result = (floor( 10 * (sqrt(eq1 + eq2))))/10.0;
				this->cost[i][j] = result;
				this->cost[j][i] = result;
				//cout << result << "  ";
			}
		}
		//cout << endl;
	}
}

void Instance::calculateT(){
	t.resize(numNodes+2, vector<double>(numNodes+2,0)); 
	
	for (int i=0; i<this->numNodes+1; i++){
		for (int j=0; j<this->numNodes+1; j++){
			t[i][j] = this->cost[i][j] + this->nodes[i].getDurationTime();
		}
	}
}

void Instance::createGLPKDataFile(){

	cout << "data;" << endl;
	cout << "set N:= ";
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " ";
	}
	cout << ";" << endl;

	cout << "set C:= ";
	for(int i=1; i<this->numNodes; i++){
		cout << i << " ";
	}
	cout << ";" << endl;

	cout << "set V:= ";
	for(int i=1; i<this->numVehicles+1; i++){
		cout << i << " ";
	}
	cout << ";" << endl;

	cout << "param G:= 100000;" << endl; //Podemos calcular isso!!!

	cout << "param a:= " << endl;
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " " << this->nodes[i].getInitialTime() << endl;
	}
	cout << ";" << endl;

	cout << "param b:= " << endl;
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " " << this->nodes[i].getEndTime() << endl;
	}
	cout << ";" << endl;

	cout << "param c: " << endl;
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " ";
	}
	cout << ":=" << endl;
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " ";
		for(int j=0; j<this->numNodes+1; j++){
			cout << this->cost[i][j] << " ";
		}
		cout << endl;
	}
	cout << ";" << endl;

	cout << "param t: " << endl;
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " ";
	}
	cout << ":=" << endl;
	for(int i=0; i<this->numNodes+1; i++){
		cout << i << " ";
		for(int j=0; j<this->numNodes+1; j++){
			cout << this->t[i][j] << " ";
		}
		cout << endl;
	}
	cout << ";" << endl;
	cout << "param d:= " << endl;
	for(int i=1; i<this->numNodes; i++){
		cout << i << " " << this->nodes[i].getDemand() << endl;
	}
	cout << ";" << endl;

	cout << "param q:= " << this->capacitated << ";" << endl;
	cout << "param m:= " << this->numVehicles << ";" << endl;
	cout << "end;" << endl;
	

}















