#ifndef parser_h
#define parser_h

#include "node.h"
node_t* prog();
node_t* block();
node_t* vars();
node_t* expr();
node_t* A();
node_t* K();
node_t* N();
node_t* B();
node_t* M();
node_t* C();
node_t* R();
node_t* stats();
node_t* mStat();
node_t* stat();
node_t* in();
node_t* out();
node_t* myIf();
node_t* loop();
node_t* assign();
node_t* RO();
node_t* eq();
node_t* x();
node_t* y();

node_t* parser(FILE*);

#endif
