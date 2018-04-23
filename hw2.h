#ifndef __HW2__
#define __HW2__

#include "tokens.h"
#include "grammar.h"

#define NUMOFRULES 22

bool aux_nullable(bool *array, std::vector<int> vecG){
    for(int i=0;i<vecG.size(); i++){
        if(vecG[i] < NONTERMINAL_ENUM_SIZE && array[vecG[i]]==false){
            return false;
        }
    }
    return true;
}

/**
 * determines which variables are nullable, i.e. can produce an empty word
 * calls print_nullable when finished
 */
void compute_nullable(){
    std::vector<bool> nullable;
    bool array[NONTERMINAL_ENUM_SIZE]={0};
    bool changed=true;
    while(changed) {
            changed=false;
            for (int i=0;i<NUMOFRULES;i++) {
                if (grammar[i].rhs == std::vector<int>() || aux_nullable(array,grammar[i].rhs)==true ){
                        array[grammar[i].lhs]=true;
                        changed=true;
                }
            }
    }
    for(i=0; i<NONTERMINAL_ENUM_SIZE; i++){
            res.push_back(array[i]);
        }
    print_nullable(nullable);
}

/**
 * computes first for all nonterminal (see nonterminal enum in grammar.h)
 * calls print_first when finished
 */
void compute_first();

/**
 * computes follow for all nonterminal (see nonterminal enum in grammar.h)
 * calls print_follow when finished
 */
void compute_follow();

/**
 * computes select for all grammar rules (see grammar global variable in grammar.h)
 * calls print_select when finished
 */
void compute_select();

/**
 * implements an LL(1) parser for the grammar using yylex()
 */
void parser();
/*
 * implemented in lex.yy.c (generated from lexer.lex)
 */
int yylex();

#endif //__HW2__
