
#ifndef GETLEXAN_H
#define	GETLEXAN_H
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>

using namespace std;

struct Token {
    string type;
    string name;
};

class getLexan {
public:
    vector<Token> tokenList;
    string findKeyword(string keyword);
    void insert_token(string type, string name);
    void print_token(vector<Token> tokenList);
    void lexan();

    getLexan() {
        this->line_record = 0;
    }
private:
    int line_record;

};


#endif	/* LEXAN_H */

