#ifndef NODE_H_
#define NODE_H_
#include <string>
#include "token.h"
#include "scanner.h"
using namespace std;

struct node_t {
        int key;
        int level;
        string nodeStr;
        node_t *child1;
        node_t *child2;
        token myToken, myToken2;
        string label;
};

#endif
