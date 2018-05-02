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

// bool joinSets(std::set<tokens>* sourceSet,std::set<tokens>* destSet){
// 	int oldSize=destSet->size();
// 	for(std::set<tokens>::iterator it=sourceSet->begin(); it!=sourceSet->end();it++){//add the first to the left val first
// 		destSet->insert(tokens(*it));
// 	}
// 	int newSize=destSet->size();
// 	if(oldSize != newSize){
// 		return true;
// 	}
// 	return false;
// }

//fills the AFIS array
void fillNullableArray(bool *array){
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

//fills the FIRST array
void fillFirstArray(std::set<tokens>* firstArray,bool *nullableArray){
	bool changed=true;
	for(int k=STARTSTRUCT; k<NUMOFVALS ; k++){
		firstArray[k].insert(tokens(k));
	}

	for (int i=0;i<NUMOFRULES;i++) {	//marks who is the val that are first gen of first
		if(grammar[i].rhs != std::vector<int>() && grammar[i].rhs[0] > NONTERMINAL_ENUM_SIZE){ //first on the right side is a TERMINAL (and not empty)
			firstArray[grammar[i].lhs].insert(tokens(grammar[i].rhs[0]));
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
					//changed=joinSets(&firstArray[grammar[i].rhs[j]],&firstArray[grammar[i].lhs]);
					std::set<tokens>* set=&firstArray[grammar[i].rhs[j]];
					std::set<tokens>* destSet=&firstArray[grammar[i].lhs];
					int oldSize=destSet->size();
					for(std::set<tokens>::iterator it=set->begin(); it!=set->end();it++){//add the first to the left val first
						destSet->insert(tokens(*it));
					}
					int newSize=destSet->size();
					if(oldSize != newSize){
						changed=true;
					}
					if(nullableArray[grammar[i].rhs[j]]!=true){//if its  not nullable - finish / else continue to add firsts
						break;
					}
				}
		}
	}
}

//fills the FOLLOW array
void fillFollowArray(std::set<tokens>* followArray,std::set<tokens>* firstArray,bool *nullable){
	followArray[grammar[0].lhs].insert(EF); //inserting $ to the first rule
	bool changed=true;
	while(changed){
		changed=false;
		for (int i=0;i<NUMOFRULES;i++) { //check all the rules
				int ruleSize=grammar[i].rhs.size(); //size of the right side of the rule
				for(int j=0;j<ruleSize;j++){ //for each (VAR/terminal) in the right side of the rule
					if(grammar[i].rhs[j] < NONTERMINAL_ENUM_SIZE){ // is a VAR
						int k=j+1; //index to the next obj of the current VAR
						while(k<ruleSize){
							std::set<tokens>* set=&firstArray[grammar[i].rhs[k]];
							std::set<tokens>* destSet=&followArray[grammar[i].rhs[j]];
							int oldSize=destSet->size();
							for(std::set<tokens>::iterator it=set->begin(); it!=set->end();it++){//add the first to the left val first
								destSet->insert(tokens(*it));	//adding next VAL FIRST to the current FOLLOW
							}
							int newSize=destSet->size();
							if(oldSize != newSize){
								changed=true;
							}
							if(nullable[grammar[i].rhs[k]]==false){ //if not nullable
								break;
							}else{	//is nullable
								k++;
							}
						}
						if(k==ruleSize){ //all VARs to the right of the VAR were nullable OR end of right side
							std::set<tokens>* set=&followArray[grammar[i].lhs];
							std::set<tokens>* destSet=&followArray[grammar[i].rhs[j]];
							int oldSize=destSet->size();
							for(std::set<tokens>::iterator it=set->begin(); it!=set->end();it++){//add the first to the left val first
								destSet->insert(tokens(*it));
							}
							int newSize=destSet->size();
							if(oldSize != newSize){
								changed=true;
							}
						}
					}
				}
		}
	}
}

//----------------------------------------------------
void compute_nullable(){
	
    std::vector<bool> nullable;
    bool array[NONTERMINAL_ENUM_SIZE]={false};
    fillNullableArray(array);		//fills the AFIS array
    for(int i=0; i<NONTERMINAL_ENUM_SIZE; i++){
            nullable.push_back(array[i]);
    }
    print_nullable(nullable);
}

//----------------------------------------------------
void compute_first(){ 

	std::vector<std::set<tokens> > vec;	//vec to return
	std::set<tokens>* array= new std::set<tokens>[NUMOFVALS]; //array of sets to copy
	bool nullable[NUMOFVALS]={false};//check all nullable before 
	fillNullableArray(nullable);		//**fill the AFIS array**
	fillFirstArray(array,nullable);

	for (int i=0;i<NONTERMINAL_ENUM_SIZE;i++) {		//for all nonterminal
			vec.push_back(array[i]);
	}
	delete[] array;
	print_first(vec);
}

void compute_follow(){
	std::vector<std::set<tokens> > vec;	//vec to return
	std::set<tokens>* firstArray= new std::set<tokens>[NUMOFVALS]; //array of firsts
	bool nullable[NUMOFVALS]={false};//check all nullable before 
	bool changed=true;
	fillNullableArray(nullable);		//**fill the AFIS array**
	fillFirstArray(firstArray,nullable);
	std::set<tokens>* followArray= new std::set<tokens>[NONTERMINAL_ENUM_SIZE]; //array of sets FOLLOW to copy from
	fillFollowArray(followArray,firstArray,nullable);
	for (int i=0;i<NONTERMINAL_ENUM_SIZE;i++) {		//for all nonterminal
			vec.push_back(followArray[i]);
	}
	delete[] followArray;
	delete[] firstArray;
	print_follow(vec);
}

void compute_select(){
	/*std::vector<std::set<tokens> > vec;	//vec to return
	bool nullable[NUMOFVALS]={false};//check all nullable before 
	std::set<tokens>* firstArray= new std::set<tokens>[NUMOFVALS]; //array of sets to copy
	std::set<tokens> followArray[NUMOFVALS]; //array of sets of FIRST
	std::set<tokens> selectArray[NUMOFRULES]; //array of sets of SELECT to copy (for each *rule*)
	bool changed=true;

	fillNullableArray(nullable);		//**fill the AFIS array**
	fillFirstArray(firstArray,nullable);	//**fill the FIRST array**
	fillFollowArray(followArray,firstArray,nullable);	//**fill the FOLLOW array**
	while(changed){
		changed=false;
		for (int i=0;i<NUMOFRULES;i++) { //check all the rules
				std::set<tokens> destSet=selectArray[i];			//the DEST set to add to
				int rightSideAfis=0;
				for(int j=0;j<grammar[i].rhs.size();j++){ //for each (VAR/terminal) in the right side of the rule
					if(nullable[grammar[i].rhs[j]] == true ){	//checks if all the right side is AFIS
							rightSideAfis++;
					}
				if(grammar[i].rhs.size() == rightSideAfis || grammar[i].rhs == std::vector<int>()){	//the right side is AFIS
					std::set<tokens> followSet=followArray[grammar[i].lhs] //we take the follow of the LEFT SIDE
					int oldSize=destSet->size(); //check size before adding
					for(std::set<tokens>::iterator it=followSet->begin(); it!=followSet->end();it++){//add the first to the left val first
						destSet->insert(tokens(*it));
					}
					int newSize=destSet->size();
					if(oldSize != newSize){
						changed=true;		//there are new members in the select - change have been made
					}
				}// insert the FIRST of the right rule

				std::set<tokens> firstSet=firstArray[grammar[i].rhs[0]] //we take the FIRST of the RIGHT SIDE
				int oldSize=destSet->size(); //check size before adding
				for(std::set<tokens>::iterator it=firstSet->begin(); it!=firstSet->end();it++){//add the first to the left val first
					destSet->insert(tokens(*it));
				}
				int newSize=destSet->size();
				if(oldSize != newSize){
					changed=true;		//there are new members in the select - change have been made
				}
		}
	}

	for (int i=0;i<NUMOFRULES;i++) {		//for all nonterminal
			vec.push_back(selectArray[i]);
	}
	delete[] firstArray;
	//do not forget to delete
	print_select(vec);
	*/
}

void parser(){}
