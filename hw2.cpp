#include "hw2.h"

/**
 * determines which variables are nullable, i.e. can produce an empty word
 * calls print_nullable when finished
 */
bool aux_nullable(bool *array, std::vector<int>& vecG){
	
	//std::cout << "nullable" << std::endl ;
	
    for(int i=0;i<vecG.size(); i++){
		if(vecG[i] > NONTERMINAL_ENUM_SIZE)
			return false;
		
		//std::cout << "vecg[i] = " << vecG[i] << ", i=" << i << std::endl;
        if(vecG[i] < NONTERMINAL_ENUM_SIZE && array[vecG[i]]==false){
            return false;
        }
    }
    return true;
}

void compute_nullable(){
	
	std::cout << "point1" << std::endl ;
    std::vector<bool> nullable;
    bool array[NONTERMINAL_ENUM_SIZE]={false};
    bool changed=true;
	std::cout << "point2" << std::endl ;
	
	for (int i=0;i<NUMOFRULES;i++) {
		if ( grammar[i].rhs == std::vector<int>() ){
				std::cout << "true111111111111111111" << std::endl ;
				array[grammar[i].lhs]=true;
		}
	}
	
	while(changed) {
		changed=false;
		for (int i=0;i<NUMOFRULES;i++) {
			if (  array[grammar[i].lhs]==false && aux_nullable(array,grammar[i].rhs)==true  ){
					std::cout << "true2222222222222222222 i= "<< i << std::endl ;
					array[grammar[i].lhs]=true;
					changed=true;
			}
		}
    }
	
	std::cout << "point3" << std::endl ;
	
    for(int i=0; i<NONTERMINAL_ENUM_SIZE; i++){
            nullable.push_back(array[i]);
        }
		
	std::cout << "point4" << std::endl ;	
    print_nullable(nullable);
}

void compute_first(){}

void compute_follow(){}

void compute_select(){}

void parser(){}
