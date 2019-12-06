#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "node.h"

token tk;
FILE *fp = NULL;
int lineNum = 1;



node_t* parser(FILE * input_file){

    fp = input_file;
    node_t *root_node;
    tk = scanner(fp, lineNum);
    root_node = prog();

    printf("Parse Tree is Created Successfully\n\n");
    printf("Please check out.preorder file for output\n\n");
    return root_node;

}
node_t* create_node(std::string s){
    node_t* node = new node_t;
    node->label = s;
    return node;
}

node_t* prog() {
    node_t *myNode = create_node("<program>");
     myNode->child1 = vars();
    myNode->child2 = block();
    return myNode;
}

/*  <block>       ->      Begin <vars> <stats> End     */
node_t* block(){
    node_t* myNode = create_node("<block>");
    if(tk.tokenId == Begin_tk){
        tk = scanner(fp, lineNum);
        myNode->child1 = vars();
        myNode->child2 = stats();
        if(tk.tokenId == End_tk){
            tk = scanner(fp, lineNum);
            return myNode;
        }
        else{
            printf("Incorrect token:\"%s\" at line %d, expected \"End\" token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else {
        printf("Incorrect token: \"%s\" at line %d, expected \"Begin\" token to begin <block>.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/*  <vars>         ->      empty | INT Identifier Integer <vars> */
node_t* vars(){
    node_t *myNode = create_node("<vars>");
   if(tk.tokenId == Begin_tk || tk.tokenId == Read_tk || tk.tokenId == Output_tk || tk.tokenId == IFF_tk  || tk.tokenId == Loop_tk || tk.tokenId == IDENT_tk){
        return myNode;
    }
    else if(tk.tokenId == INT_tk){
        tk = scanner(fp, lineNum);
        if(tk.tokenId == IDENT_tk){
            myNode->myToken = tk;
            tk = scanner(fp, lineNum);
            if(tk.tokenId == NUM_tk){
                myNode->myToken2 = tk;
                tk = scanner(fp, lineNum);
                myNode->child1 = vars();
                return myNode;
            }
            else{
                printf("Incorrect token:\"%s\" at line %d, expected \"Number\" token.\n", tk.instance.c_str(), lineNum);
            }
        }
        else {
            printf("Incorrect token:\"%s\" at line %d, expected \"Identifier\" token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else{
        printf("Incorrect token: %s at line %d, expected either empty case for <var> or \"int\" keyword to signal Identifier declaration.", tk.instance.c_str(), lineNum);

	exit(1);
    }
    return myNode;
}

/*  <expr>         ->      <A> <K>*/
node_t* expr(){
    node_t* myNode = create_node("<expr>");
    myNode->child1 = A();
    myNode->child2 = K();
    return myNode;
}

/*  <K>            ->      + <expr> | - <expr> | ε  */
node_t* K(){
    node_t* myNode = create_node("<K>");
    if (tk.instance == "]" || tk.instance == ":" || tk.instance == "<" || tk.instance == ">" || tk.instance == "=") {
        return myNode;
    }else{
        if(tk.tokenId == OPDEL_tk && tk.instance == "+"){
            myNode->myToken = tk;
            tk = scanner(fp, lineNum);
            myNode->child1 = expr();
            return myNode;
        }
        else if(tk.tokenId == OPDEL_tk && tk.instance == "-"){
            myNode->myToken = tk;
            tk = scanner(fp, lineNum);
            myNode->child1 = expr();
            return myNode;
        }
        else {
		printf("Incorrect token:\"%s\" at line %d, expected either \"+\" or \"-\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
}

/*      <A>            ->      <N> <B>      */
node_t* A(){
    node_t* myNode = create_node("<A>");
    myNode->child1 = N();
    myNode->child2 = B();
    return myNode;
}

/*      <B>            ->      * <A> | ε        */
node_t* B(){
    node_t* myNode = create_node("<B>");
    if(tk.tokenId == OPDEL_tk && tk.instance == "*"){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        myNode->child1 = A();
        return myNode;
    }
    else if(tk.instance == "+" || tk.instance == ":" || tk.instance == "-" || tk.instance == "]" || tk.instance == "=" || tk.instance == "<" || tk.instance == ">") {
        return myNode;
    }else {
        printf("Incorrect token:\"%s\" at line %d, should be +,-,:,],=,<,>,*\n", tk.instance.c_str(), lineNum);
            exit(1);
    }
}

/*       <N>            ->      <M> <C>     */
node_t* N(){
    node_t* myNode = create_node("<N>");
    myNode->child1 = M();
    myNode->child2 = C();
    return myNode;
}

/*      <C>            ->      / <N> | ε        */
node_t* C(){
    node_t* myNode = create_node("<C>");
    if(tk.tokenId == OPDEL_tk && tk.instance == "/"){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        myNode->child1 = N();
        return myNode;
    }
    else if(tk.instance == "+" || tk.instance == ":" || tk.instance == "-" || tk.instance == "]" || tk.instance == "=" || tk.instance == "<" || tk.instance == ">" || tk.instance == "*") {
        return myNode;
    }
    else {
        printf("Incorrect token:\"%s\" at line %d, should be /,+,-,:,],=,<,>,*\n", tk.instance.c_str(), lineNum);
            exit(1);
    }
}

/*      <M>            ->      - <M> |  <R>     */
node_t* M(){
    node_t* myNode = create_node("<M>");
    if(tk.tokenId == OPDEL_tk && tk.instance == "-"){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        myNode->child1 = M();
        return myNode;
    } else if(tk.tokenId == NUM_tk || tk.tokenId == IDENT_tk || tk.instance == "["){
        myNode->child1 = R();
        return myNode;
    }
    else {
        printf("Incorrect token:\"%s\" at line %d, expected either NUM_tk or \"-\" or \"[\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/*      <R>            ->      [ <expr> ] | Identifier | Integer        */
node_t* R() {
    node_t* myNode = create_node("<R>");
    if (tk.tokenId == OPDEL_tk && tk.instance == "[") {
        tk = scanner(fp, lineNum);
        myNode->child1 = expr();
        if (tk.tokenId == OPDEL_tk && tk.instance == "]") { // )
            tk = scanner(fp, lineNum);
            return myNode;
        }
        else{
            printf("Incorrect token: \"%s\" at line %d, expected \"]\" token.\n", tk.instance.c_str(), lineNum);
            exit(1);
	 }
    } else if (tk.tokenId == IDENT_tk) {
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    } else if (tk.tokenId == NUM_tk) {
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    } else {
        printf("Incorrect token: \"%s\" at line %d, expected should have been either Identifier, Number, or \"[\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/*      <stats>        ->      <stat> : <mStat>     */
node_t* stats() {
    node_t* myNode = create_node("<stats>");
    myNode->child1 = stat();
    if(tk.instance == ":"){
        tk = scanner(fp, lineNum);
        myNode->child2 = mStat();
        return myNode;
    }
    else{
        printf("Incorrect token:\"%s\" at line %d, expected \":\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }

}
/*      <mStat>        ->      empty |  <stat>  :  <mStat>      */
node_t* mStat() {
    node_t *myNode = create_node("<mStat>");
    if (tk.tokenId == End_tk) {
        return myNode;
    } else if(tk.tokenId == Begin_tk || tk.tokenId == IFF_tk || tk.tokenId == Loop_tk || tk.tokenId == Read_tk || tk.tokenId == Output_tk || tk.tokenId == IDENT_tk) {
        myNode->child1 = stat();
        if(tk.tokenId == OPDEL_tk && tk.instance == ":"){
            tk = scanner(fp, lineNum);
            myNode->child2 = mStat();
            return myNode;
        }
        else{
            printf("Incorrect token: token \"%s\" at line %d, expected \":\" delimiter token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else{
            printf("Incorrect token: token \"%s\" at line %d, expected either empty case for <mstat> or \":\" delimiter token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
}

/*      <stat>  ->  <in> | <out> | <block> | <if> | <loop> | <assign> */
node_t* stat() {
    node_t *myNode = create_node("<stat>");

    if (tk.tokenId == Read_tk) {
        tk = scanner(fp, lineNum);
        myNode->child1 = in();
        return myNode;
    } else if (tk.tokenId == Output_tk) {
        tk = scanner(fp, lineNum);
        myNode->child1 = out();
        return myNode;
    } else if (tk.tokenId == IFF_tk) {
        tk = scanner(fp, lineNum);
        myNode->child1 = myIf();
        return myNode;
    } else if (tk.tokenId == Loop_tk) {
        tk = scanner(fp, lineNum);
        myNode->child1 = loop();
        return myNode;
    } else if (tk.tokenId == IDENT_tk) {
        myNode->child1 = assign();
        return myNode;
    } else if (tk.tokenId == Begin_tk) {
        myNode->child1 = block();
        return myNode;
    } else {
        printf("Incorrect token: \"%s\" at line %d does not apply to expected <stat> instance.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/*      <in>           ->      Read [ Identifier ]  */
node_t* in() {
    node_t *myNode = create_node("<in>");
    if (tk.tokenId == OPDEL_tk && tk.instance == "[") {
            tk = scanner(fp, lineNum);
            if (tk.tokenId == IDENT_tk) {
                myNode->myToken = tk;
                tk = scanner(fp, lineNum);
                if (tk.tokenId == OPDEL_tk && tk.instance == "]") {
                    tk = scanner(fp, lineNum);
                    return myNode;
                }
                else{
                    printf("Incorrect token:\"%s\" at line %d, expected \"]\".\n", tk.instance.c_str(), lineNum);
                    exit(1);
                }

            }
            else{
                printf("Incorrect token:\"%s\" at line %d, expected \"Identifier Token\".\n", tk.instance.c_str(), lineNum);
                exit(1);
            }
        }
        else{
            printf("Incorrect token:\"%s\" at line %d, expected \"[\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }

    }

/*  <out>          ->      Output [ <expr> ]    */
node_t* out() {
    node_t *myNode = create_node("<out>");
           if (tk.tokenId == OPDEL_tk && tk.instance == "[") {
            tk = scanner(fp, lineNum);
            myNode->child1 = expr();
            if (tk.tokenId == OPDEL_tk && tk.instance == "]") {
                tk = scanner(fp, lineNum);
                return myNode;
            }
            else{
                printf("Incorrect token:\"%s\" at line %d, expected \"]\".\n", tk.instance.c_str(), lineNum);
                exit(1);
            }
        }
        else{
            printf("Incorrect token:\"%s\" at line %d, expected \"[\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }


/*  <if>           ->      IFF [ <expr> <X>    */
node_t* myIf(){
    node_t* myNode = create_node("<if>");
        if(tk.instance == "["){
            tk = scanner(fp, lineNum);
            myNode->child1 = expr();
            myNode->child2 = x();
            return myNode;
        }
        else{
		printf("Incorrect token: \"%s\" at line %d, expected \"[\" .\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }

/*      <loop>         ->      Loop [ <expr> <X>   */
node_t* loop(){
    node_t* myNode = create_node("<loop>");
          if(tk.instance == "["){
            tk = scanner(fp, lineNum);
            myNode->child1 = expr();
            myNode->child2 = x();
            return myNode;
        }
        else{
            printf("Incorrect token: \"%s\" at line %d, expected \"[\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }

/*  <assign>       ->      Identifier  = <expr> */
node_t* assign() {
    node_t* myNode = create_node("<assign>");
        if(tk.tokenId == IDENT_tk){
                myNode->myToken = tk;
                tk = scanner(fp,lineNum);
        if (tk.instance == "=") {
            myNode->myToken2 = tk;
            tk = scanner(fp, lineNum);
            myNode->child1 = expr();
            return myNode;
 }
        else {
            printf("Incorrect token: \"%s\" at line %d, expected \"=\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
        }
        else{
                printf("Incorrect token: \"%s\" at line %d, expected Identifier.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }

/*      <RO>           ->      < | = <eq> | >   */
node_t* RO(){
    node_t* myNode = create_node("<RO>");
    if (tk.tokenId == OPDEL_tk && tk.instance == "<") {
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    }
    else if(tk.tokenId == OPDEL_tk && tk.instance == "="){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        myNode->child1 = eq();
        return myNode;
    }
    else if (tk.tokenId == OPDEL_tk && tk.instance == ">"){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    }
    else{
        printf("Incorrect token: \"%s\" at line %d does not apply to expected <RO> instance(<,=,>).\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/*      <eq>           ->      < | > | = | ε    */
node_t* eq(){
    node_t* myNode = create_node("<eq>");
    if (tk.tokenId == OPDEL_tk && tk.instance == "<") {
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    }
    else if (tk.tokenId == OPDEL_tk && tk.instance == ">"){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    }
    else if(tk.tokenId == OPDEL_tk && tk.instance == "="){
        myNode->myToken = tk;
        tk = scanner(fp, lineNum);
        return myNode;
    }
    else if(tk.instance == "-" || tk.instance == "[" || tk.tokenId == IDENT_tk || tk.tokenId == NUM_tk){
        return myNode;
    }
    else{
        printf("Incorrect token: \"%s\" at line %d does not apply to expected <eq> instance(<,=,>).\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/*      <X>            ->      <RO> <Y>     */
node_t* x(){
    node_t* myNode = create_node("<X>");
    myNode->child1 = RO();
    myNode->child2 = y();
    return myNode;
}

/*      <Y>            ->      <expr> ] <stat> */
node_t* y(){
    node_t* myNode = create_node("<Y>");
    myNode->child1 = expr();
    if (tk.tokenId == OPDEL_tk && tk.instance == "]") {
        tk = scanner(fp, lineNum);
        myNode->child2 = stat();
        return myNode;
    }
    else{
        printf("Incorrect token:\"%s\" at line %d, expected \"]\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}
