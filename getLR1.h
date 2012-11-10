
#ifndef GETLR1_H
#define	GETLR1_H
#include <map>
#include <cstdlib>
#include <string>
#include <set>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <stack>
#include "getLexan.h"
using namespace std;

#define LR_MOST 250
class State {
public:
    string left;
    int position;
    vector<string> right;
    string flag;

    State(string left, vector<string> right, int position, string flag) {
        this->left = left;
        this->right = right;
        this->position = position;
        this->flag = flag;
    };

    bool operator<(const State &x) const {
        if (left != x.left) return left < x.left;
        if (position != x.position) return position < x.position;
        if (flag != x.flag) return flag < x.flag;
        return right < x.right;
    }
};
class symbol{
public:
    vector<string> name;
    string type;
    string code;
    string type1;
    string type2;
    bool isArray;
    int int_value;
    float float_value;
    symbol(){
        this->name.clear();
        this->type="";
        this->code.clear();
        this->isArray=false;
    }    
};
struct exp {
    string left;
    vector<string> right;
};
struct Table{
    string state;
    int number;
};

class getLR1 {
public:
    set<string> first;
    map<string, set<string> > first_all;
    set<string> V;
    map<string,int> vt_loc;
    map<set<State>,int> state_loc;
    set<string> T;
    vector<exp> P;
    set<string> nullable;
    set<string> S;
    Table LR_table[LR_MOST][LR_MOST];
    int str_num;
    int lable_num;
    string dataSection;
    string textSection;
    string bssSection;
    map<string,string> id_type;

    bool fun_nullable(string str);
    map<string, set<string> > First();
    set<string> getFirst(vector<string> temp);
    set<State> getClosure(set<State> &I);
    set<State> Goto(set<State> I, string x);
    bool belongTo(set<State> one, set<State> two);
    set<set<State> > getStandSet();
    Table** getLR1Table();
    int getPLoc(exp p);
    void read_v();
    void read_p();
    void read_t();
    void print_first();
    void print_StandSet(set<set<State> > hello);
    bool canNullable(string str);
    void print_LR_table();
    void Reduction(vector<Token> tokenList);
    void Gencode(int exp_num,stack<symbol> &trans);
    void print_assemly();
    
    getLR1(){
        this->str_num=0;
        this->lable_num=0;
    }
};
#endif	/* GETLR1_H */

