#include "RS.h"

RS::RS(){
	h = 1;
	N = 9;
	
}
void RS::Scatter(int round, int step, float* alleic_array){

}
void RS::Gather(int round, int step, float* alleic_array){

}
RS::RS(int rank){
	h = 1;
	N = 9;
	this->rank = rank;
	
}
void RS::Sync(){
	int k = 0;
	for( k = 0; k<= h ; k++){

	}
}

RS::~RS(){

}