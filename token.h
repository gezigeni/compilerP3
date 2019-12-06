#ifndef token_h
#define token_h

#include <iostream>

enum tokenID {
    IDENT_tk,
    NUM_tk,

    Begin_tk,
    End_tk,
    Loop_tk,
    Void_tk,
    INT_tk,
    Return_tk,
    Read_tk,
    Output_tk,
    Program_tk,
    IFF_tk,
    Then_tk,
    Let_tk,

    OPDEL_tk,
    EOF_tk,
    ERR_tk
};

struct token {
    tokenID tokenId;
    std::string instance;
    int lineNum;
};

#endif

