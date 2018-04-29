#include "hw2.h"


#define NUMOFVALS (EF+1)
/**
 * determines which variables are nullable, i.e. can produce an empty word
 * calls print_nullable when finished
 */
bool aux_nullable(bool *array, std::vector<int>& vecG){	
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
//fills the AFIS array
void fillArray(bool *array){
	bool changed=true;
	for (int i=0;i<NUMOFRULES;i++) {
		if ( grammar[i].rhs == std::vector<int>() ){
				array[grammar[i].lhs]=true;
		}
	}
	while(changed) {
		changed=false;
		for (int i=0;i<NUMOFRULES;i++) {
			if (  array[grammar[i].lhs]==false && aux_nullable(array,grammar[i].rhs)==true  ){
					array[grammar[i].lhs]=true;
					changed=true;
			}
		}
	}
}

void compute_nullable(){
	
    std::vector<bool> nullable;
    bool array[NONTERMINAL_ENUM_SIZE]={false};
    fillArray(array);		//fills the AFIS array
    for(int i=0; i<NONTERMINAL_ENUM_SIZE; i++){
            nullable.push_back(array[i]);
    }
    print_nullable(nullable);
}

//----------------------------------------------------
void compute_first(){

	std::vector<std::set<tokens> > vec;	//vec to return
	std::set<tokens>* array= new std::set<tokens>[NUMOFVALS]; //array of sets to copy
	//check if ok
	bool arrayFirst[NUMOFVALS]={false};	//array of all firsts
	bool nullable[NUMOFVALS]={false};//check all nullable before 
	bool changed=true;
	fillArray(nullable);		//fills the AFIS array

	for(int k=STARTSTRUCT; k<NUMOFVALS ; k++){
		arrayFirst[k]=true; 		//all the tokens are first by definition
		array[k].insert(tokens(k));
	}

	for (int i=0;i<NUMOFRULES;i++) {	//marks who is the val that are first gen of first
		if(grammar[i].rhs != std::vector<int>() && grammar[i].rhs[0] > NONTERMINAL_ENUM_SIZE){ //first on the right side is a TERMINAL (and not empty)
			array[grammar[i].lhs].insert(tokens(grammar[i].rhs[0]));
			arrayFirst[grammar[i].lhs]=true; 
		}	
	}
	while(changed){
		changed=false;
		for (int i=0;i<NUMOFRULES;i++) { //check all the rules
				 if(grammar[i].rhs == std::vector<int>()){//empty derive
				 	continue;
				 }
				int j=0;
				for(;j<grammar[i].rhs.size();j++){ // for each val on the right side
					if(nullable[grammar[i].rhs[j]]!=true){//if its  nullable - check to the right
						break;
					}
				}
				//arrayFirst[grammar[i].lhs] == false &&
				//if( arrayFirst[grammar[i].rhs[j]] == true){ //there is a first rule to our first right
					//array[grammar[i].lhs].insert(array[grammar[i].rhs[j]]);
				std::set<tokens>* set=&array[grammar[i].rhs[j]];
				std::set<tokens>* destSet=&array[grammar[i].lhs];
				//std::cout << "grammar[" << i << "].lhs =" << grammar[i].lhs << std::endl;
				int oldSize=destSet->size();
				for(std::set<tokens>::iterator it=set->begin(); it!=set->end();it++){//add the first to the left val first
					destSet->insert(tokens(*it));
				}
			//	arrayFirst[grammar[i].lhs]=true; //mark who is the val that are first gen of first
				int newSize=destSet->size();
				if(oldSize != newSize){
					changed=true;
				}
				//-------------make a pointer to the set and not local!!!!!!!!!!!!!!!!!!!!


				//}	
		}
	}
	for (int i=0;i<NONTERMINAL_ENUM_SIZE;i++) {		//for all nonterminal
		//if(arrayFirst[i]==true){
			vec.push_back(array[i]);
		//}
	}
	delete[] array;
	print_first(vec);
}

void compute_follow(){}

void compute_select(){}

void parser(){}
