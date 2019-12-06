#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "staticSem.h"
#include <map>
#include <string>

std::vector<std::string> scopeStack;
std::vector<std::string> temp;
int scope_index = 0;

enum StringValue {
    evStringValue1,
    evStringValue2,
    evStringValue3,
    evStringValue4,
    evStringValue5,
    evStringValue6,
    evStringValue7,
    evStringValue8,
    evStringValue9,
    evStringValue10,
    evStringValue11,
    evStringValue12,
    evStringValue13,
    evStringValue14,
    evStringValue15,
    evStringValue16,
    evStringValue17,
    evStringValue18,
    evStringValue19,
    evStringValue20,
    evStringValue21,
    evStringValue22,
    evStringValue23,
    evStringValue24,
    evStringValue25,
 
};

static std::map<std::string, StringValue> s_mapStringValues;

void Initialize()
{
    s_mapStringValues["<program>"] = evStringValue2;
    s_mapStringValues["<block>"] = evStringValue3;
    s_mapStringValues["<vars>"] = evStringValue4;
    s_mapStringValues["<expr>"] = evStringValue5;
    s_mapStringValues["<M>"] = evStringValue6;
    s_mapStringValues["<A>"] = evStringValue7;
    s_mapStringValues["<K>"] = evStringValue8;
    s_mapStringValues["<N>"] = evStringValue9;
    s_mapStringValues["<B>"] = evStringValue10;
    s_mapStringValues["<C>"] = evStringValue12;
    s_mapStringValues["<R>"] = evStringValue13;
    s_mapStringValues["<stats>"] = evStringValue14;
    s_mapStringValues["<mStat>"] = evStringValue15;
    s_mapStringValues["<stat>"] = evStringValue16;
    s_mapStringValues["<in>"] = evStringValue17;
    s_mapStringValues["<out>"] = evStringValue18;
    s_mapStringValues["<if>"] = evStringValue19;
    s_mapStringValues["<loop>"] = evStringValue20;
    s_mapStringValues["<assign>"] = evStringValue21;
    s_mapStringValues["<RO>"] = evStringValue22;
    s_mapStringValues["<eq>"] = evStringValue23;
    s_mapStringValues["<x>"] = evStringValue24;
    s_mapStringValues["<y>"] = evStringValue25;
}

void checkRedef(token tk){
    for (int i = 0; i < scopeStack.size(); i++) {
        if (scopeStack[i] == tk.instance) {
            printf("Scoping Error: Line %d: token instance \"%s\" already defined.\n", tk.lineNum, tk.instance.c_str());
            exit(0);
        }
    }
    scopeStack.push_back(tk.instance);
}
int find(std::string ident) {
    for (int i = (int)scopeStack.size() - 1; i > -1; i--) {
        if (scopeStack[i] == ident) {
            return (int)scopeStack.size() - i - 1;
        }
    }
    return -1;
}
void printScope() {
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--) {
        std::cout << scopeStack[i] << std::endl;
    }
}
void checkSem(node_t* node) {
	
	
	if (node == NULL) {
        	return;
	} else {
        std::string label = node->label;

		switch(s_mapStringValues[label])
        	{
		case evStringValue2://<program>
		{
			
			checkSem(node->child1);
			checkSem(node->child2);
                	break; 
		}
		case evStringValue3://<block>
		{
			
			checkSem(node->child1);
                	checkSem(node->child2);
			break;
		}
		case evStringValue4://<vars>
            	{
			
			if(!node->myToken.instance.empty()){
				checkRedef(node->myToken);
			}
			if (node->child1 != NULL){
				checkSem(node->child1);
			}
			break;
		}
		case evStringValue5: // <expr>
            	{
			
			checkSem(node->child1);
                        checkSem(node->child2);
                        break;
		}
		case evStringValue6: //<M>
                {
			
                        checkSem(node->child1);
                        break;
                }
		case evStringValue7: //<A>
                {
                        checkSem(node->child1);
                        checkSem(node->child2);
                        break;
                } 
		case evStringValue8: //<K>
                { 
                        if (node->child1 != NULL){
                                checkSem(node->child1);
                        }
                        break;
                }
		case evStringValue9: //<N>
                {
                        checkSem(node->child1);
                        checkSem(node->child2);
                        break;
                }
		case evStringValue10: //<B>
                {       
                        if (node->child1 != NULL){
                                checkSem(node->child1);
                        }
                        break;
                }
		case evStringValue12: //<C>
                {
                        if (node->child1 != NULL){
                                checkSem(node->child1);
                        }
                        break;
                }
		case evStringValue13://<R>
                {
		
		

                        if(!(node->myToken.instance.empty())){
				
                                if(node->myToken.tokenId == IDENT_tk){
					int x = find(node->myToken.instance);
				
                        		if(x == -1){
						printf("Scoping Error: Line %d: token instance \"%s\" are not defined.\n",node->myToken.lineNum, node->myToken.instance.c_str());
            					exit(0);
					}
				}
			}
                        if (node->child1 != NULL){
                                checkSem(node->child1);
                        }
                        break;
                }
		case evStringValue14: //<stats>
          	{
			
                        checkSem(node->child1);
			checkSem(node->child2);
                        break;
                }
		case evStringValue15: //<mStat>
                {
			
                        if (node->child1 != NULL){
                                checkSem(node->child1);
				checkSem(node->child2);
                        }
                        break;
                }
		case evStringValue16: //<stat>
                {
			
                        checkSem(node->child1);
                        break;
                }
		case evStringValue17://<in>
                {
                        if(!node->myToken.instance.empty()){
                                if(node->myToken.tokenId == IDENT_tk){
                                        int x = find(node->myToken.instance);
                                        if(x == -1){
                                                printf("Scoping Error: Line %d: token instance \"%s\" is not  defined.\n", node->myToken.lineNum, node->myToken.instance.c_str());
                                                exit(0);
                                        }
                                }
                        }
			break;
		}
		case evStringValue18: //<out>
                {
		
                        checkSem(node->child1);
                        break;
                }
		case evStringValue19: //<if>
                {
                        checkSem(node->child1);
                        checkSem(node->child2);
                        break;
                }
		case evStringValue20: //<Loop>
                {
                        checkSem(node->child1);
                        checkSem(node->child2);
                        break;
                }	
		case evStringValue21://<assign>
                {
			
                        if(!node->myToken.instance.empty()){
                                if(node->myToken.tokenId == IDENT_tk){
                                        int x = find(node->myToken.instance);
                                        if(x == -1){
                                                printf("Scoping Error: Line %d: token instance \"%s\" is not defined.\n", node->myToken.lineNum, node->myToken.instance.c_str());
                                                exit(0);
                                        }
                                }
                        }
			checkSem(node->child1);
                        break;
                }
		case evStringValue22: //<RO>
                {
                        if (node->child1 != NULL){
                                checkSem(node->child1);
                        }
                        break;
                }
		case evStringValue23: //<eq>
                {
                        break;
                }
		case evStringValue24: //<x>
                {
                        checkSem(node->child1);
                        checkSem(node->child2);
                        break;
                }
		case evStringValue25: //<y>
                {
                        checkSem(node->child1);
                        checkSem(node->child2);
                        break;
                }
		default:
            	{
                	checkSem(node->child1);
                	checkSem(node->child2);
                	break;
            	}
	}
   }	
}
