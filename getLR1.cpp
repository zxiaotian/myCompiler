/******************************
*   Author: zhouxiaotian
*   Time  : 2012.11.9
*   E-mail: zxiaotian.hit@gmail.com
*******************************/
#include "getLR1.h"
map<string, set<string> > getLR1::First() {
    first.clear();
    map<string, bool>::iterator iter;
    nullable.clear();
    set<string>::iterator iter_t;
    for (iter_t = T.begin(); iter_t != T.end(); iter_t++) {
        first.insert(*iter_t);
        first_all.insert(pair<string, set<string> >(*iter_t, first));
        first.clear();
    }
    bool changed;
    do {
        changed = false;
        vector<exp>::iterator iter_exp;
        for (iter_exp = P.begin(); iter_exp != P.end(); iter_exp++) {
            bool all_null = true;
            for (int i = 0; i < iter_exp->right.size(); i++) {
                if (all_null == true) {
                    for (set<string>::iterator y_first = first_all[iter_exp->right[i]].begin(); y_first != first_all[iter_exp->right[i]].end(); y_first++) {
                        if (first_all[iter_exp->left].find(*y_first) == first_all[iter_exp->left].end()) {
                            changed = true;
                            first_all[iter_exp->left].insert(*y_first);
                        }
                    }
                }
                if (nullable.find(iter_exp->right[i]) == nullable.end()) {
                    all_null = false;
                    break;
                }
            }
            if (all_null == true) {
                if (nullable.find(iter_exp->left) == nullable.end()) {
                    changed = true;
                    nullable.insert(iter_exp->left);
                }
            }
        }
    } while (changed);
    return first_all;
}

bool getLR1::canNullable(string str) {
    if (nullable.find(str) == nullable.end())
        return false;
    return true;
}

set<string> getLR1::getFirst(vector<string> temp) {
    bool canBeNull = true;
    set<string> new_first;
    for (int i = 0; i < temp.size(); i++) {
        for (set<string>::iterator iter = first_all[temp[i]].begin(); iter != first_all[temp[i]].end(); iter++)
            new_first.insert(*iter);
        if (!canNullable(temp[i]))
            break;
    }
    return new_first;
}

set<State> getLR1::getClosure(set<State> &I) {
    bool changed = false;
    do {
        changed = false;
        for (set<State>::iterator iter = I.begin(); iter != I.end(); iter++) {
            State stateTemp = *iter;
            if (stateTemp.position < stateTemp.right.size()) {
                string x = stateTemp.right[stateTemp.position];
                vector<string> last;
                for (int i = stateTemp.position + 1; i < stateTemp.right.size(); i++) {
                    last.push_back(stateTemp.right[i]);
                }
                last.push_back(stateTemp.flag);
                set<string> temp_first = getFirst(last);
                for (vector<exp>::iterator iter_exp = P.begin(); iter_exp != P.end(); iter_exp++) {
                    if (iter_exp->left == x) {
                        for (set<string>::iterator iter_first = temp_first.begin(); iter_first != temp_first.end(); iter_first++) {
                            State temp = State(x, iter_exp->right, 0, *iter_first);
                            if (I.find(temp) == I.end()) {
                                I.insert(temp);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
    return I;
}

set<State> getLR1::Goto(set<State> I, string x) {
    set<State> J;
    J.clear();
    for (set<State>::iterator iter = I.begin(); iter != I.end(); iter++) {
        if (iter->position < iter->right.size()) {
            if (iter->right[iter->position] == x) {
                State temp = State(iter->left, iter->right, iter->position + 1, iter->flag);
                if (J.find(temp) == J.end())
                    J.insert(temp);
            }
        }
    }
    return J;
}

set<set<State> > getLR1::getStandSet() {
    vector<string> vec;
    vec.push_back("S");
    State temp = State("S'", vec, 0, "#");
    set<State> start;
    start.insert(temp);
    set<set<State> > C;
    C.insert(getClosure(start));
    bool changed;
    do {
        changed = false;
        for (set<set<State> >::iterator iter = C.begin(); iter != C.end(); iter++) {
            for (set<string>::iterator iter_v = V.begin(); iter_v != V.end(); iter_v++) {
                set<State> tempGoto = Goto(*iter, *iter_v);
                if ((!tempGoto.empty()) && C.find(tempGoto) == C.end()) {
                    set<State> tempClosure = getClosure(tempGoto);
                    if (C.find(tempClosure) == C.end()) {
                        C.insert(tempClosure);
                        changed = true;
                    }
                }
            }
            for (set<string>::iterator iter_t = T.begin(); iter_t != T.end(); iter_t++) {
                set<State> tempGoto_t = Goto(*iter, *iter_t);
                if ((!tempGoto_t.empty()) && C.find(tempGoto_t) == C.end()) {
                    set<State> tempClosure = getClosure(tempGoto_t);
                    if (C.find(tempClosure) == C.end()) {
                        C.insert(tempClosure);
                        changed = true;
                    }
                }
            }
        }
    } while (changed);
    return C;
}

void getLR1::print_StandSet(set<set<State> > const hello) {
    int i = 0;
    for (set<set<State> >::iterator iter = hello.begin(); iter != hello.end(); iter++) {
        cout << "I" << i++ << endl;
        for (set<State>::iterator it = iter->begin(); it != iter->end(); it++) {
            cout << it->left << "->";
            int j;
            for (j = 0; j < it->right.size(); j++) {
                if (j == it->position)
                    cout << "." << " ";
                cout << it->right[j] << " ";
            }
            if (j == it->position)
                cout << "." << " ";
            cout << it->flag << endl;
        }
    }
}

int getLR1::getPLoc(exp p) {
    for (int i = 0; i < P.size(); i++) {
        bool flag = false;
        if (P[i].left == p.left) {
            flag = true;
            if (P[i].right.size() != p.right.size()) {
                flag = false;
                continue;
            }
            for (int j = 0; j < P[i].right.size(); j++)
                if (P[i].right[j] != p.right[j])
                    flag = false;
        }
        if (flag)
            return i;
    }
    cout << "Error!" << endl;
    return -1;
}

Table** getLR1::getLR1Table() {
    set<set<State> > C;
    C = getStandSet();

    for (int i = 0; i < LR_MOST; i++) {
        for (int j = 0; j < LR_MOST; j++) {
            LR_table[i][j].state = "";
            LR_table[i][j].number = -1;
        }
    }
    int i = 0;
    for (set<string>::iterator iter_t = T.begin(); iter_t != T.end(); iter_t++) {
        vt_loc.insert(make_pair(*iter_t, i));
        //cout << *iter_t << "\t" << i << endl;
        i++;
    }
    for (set<string>::iterator iter_v = V.begin(); iter_v != V.end(); iter_v++) {
        vt_loc.insert(make_pair(*iter_v, i));
        //cout << *iter_v << "\t" << i << endl;
        i++;
    }
    i = 0;
    for (set<set<State> >::iterator iter = C.begin(); iter != C.end(); iter++) {
        state_loc.insert(make_pair(*iter, i));
        i++;
    }
    int k = 0;
    for (set<set<State> >::iterator iter_c = C.begin(); iter_c != C.end(); iter_c++) {
        for (set<State>::iterator iter_i = iter_c->begin(); iter_i != iter_c->end(); iter_i++) {
            //第一个if语句
            if ((iter_i->position < iter_i->right.size()) && T.find(iter_i->right[iter_i->position]) != T.end()) {
                set<State> J = Goto(*iter_c, iter_i->right[iter_i->position]);
                getClosure(J);
                if (C.find(J) != C.end()) {
                    int loc_j = state_loc[J];
                    int loc_t = vt_loc[iter_i->right[iter_i->position ]];
                    LR_table[k][loc_t].state = "S";
                    LR_table[k][loc_t].number = loc_j;
                }
            }
            //第二个if
            for (set<string>::iterator iter_v = V.begin(); iter_v != V.end(); iter_v++) {
                set<State> J = Goto(*iter_c, *iter_v);
                getClosure(J);
                if (C.find(J) != C.end()) {
                    int loc_v = vt_loc[*iter_v];
                    int loc_j = state_loc[J];
                    LR_table[k][loc_v].state = "";
                    LR_table[k][loc_v].number = loc_j;
                }
            }
            //第三个if
            if (iter_i->position == iter_i->right.size()) {
                exp temp;
                temp.left = iter_i->left;
                temp.right = iter_i->right;
                int loc_exp = getPLoc(temp);
                int loc_t = vt_loc[iter_i->flag];
                if (loc_exp != -1) {
                    LR_table[k][loc_t].state = "R";
                    LR_table[k][loc_t].number = loc_exp;
                } else
                    cout << "Get exp_loc Error!" << endl;
            }
            //第四个if
            if (iter_i->left == "S'" && iter_i->position == 1 && iter_i->right.size() == 1 && iter_i->right[0] == "S") {
                int loc_t = vt_loc["#"];
                LR_table[k][loc_t].state = "ACC";
                LR_table[k][loc_t].number = 0;
            }
        }
        k++;
    }
}

void getLR1::print_LR_table() {
    ofstream output;
    output.open("lr_table");
    output << "\t";
    for (set<string>::iterator iter_t = T.begin(); iter_t != T.end(); iter_t++) {
        output << *iter_t << "\t";
    }
    for (set<string>::iterator iter_v = V.begin(); iter_v != V.end(); iter_v++) {
        output << *iter_v << "\t";
    }
    output << endl;
    int count = V.size() + T.size();
    for (int i = 0; i < LR_MOST; i++) {
        output << i << "\t";
        for (int j = 0; j < count; j++)
            output << LR_table[i][j].state << LR_table[i][j].number << "\t";
        output << endl;
    }
    output.close();
    cout << "Print LR_table End!" << endl;
}

void getLR1::Reduction(vector<Token> tokenList) {
    vector<string> vec;
    vec.push_back("S");
    State temp = State("S'", vec, 0, "#");
    set<State> start;
    start.insert(temp);
    getClosure(start);
    int start_state = state_loc[start];

    stack<string> op;
    stack<int> state;

    stack<symbol> trans;

    op.push("#");
    state.push(start_state);
    int ip = 0;
    bool beContinue = true;
    do {
        int S = state.top();
        string a = tokenList[ip].type;
        int a_loc = vt_loc[a];
        if (LR_table[S][a_loc].state == "S") {

            symbol token = symbol();
            token.name.push_back(tokenList[ip].name);
            // token.type=tokenList[ip].type;
            trans.push(token);


            state.push(LR_table[S][a_loc].number);
            op.push(a);
            ip++;
        } else if (LR_table[S][a_loc].state == "R") {
            int exp_num = LR_table[S][a_loc].number;
            int count = P[exp_num].right.size();
            while (count--) {
                op.pop();
                state.pop();
            }
            op.push(P[exp_num].left);
            int new_state = LR_table[state.top()][vt_loc[P[exp_num].left]].number;
            state.push(new_state);


            Gencode(exp_num, trans);

            cout << P[exp_num].left << " -> ";
            for (int i = 0; i < P[exp_num].right.size(); i++) {
                cout << P[exp_num].right[i] << "   ";
            }
            cout << endl;
        } else if (LR_table[S][a_loc].state == "ACC") {
            beContinue = false;
        } else {
            cout << "There has some errors!" << endl;
            break;
        }

    } while (beContinue);
    //symbol temp_trans = trans.top();
    // freopen("code.s", "w", stdout);
    //cout << "Datasection:  " << endl << dataSection << endl << "Code:\n" << temp_trans.code << endl;
    print_assemly();
}

void getLR1::read_v() {
    string v = "S' S declarations statements declaration number_type ids numbers statement expr asgn iterator if_statement E T F left relation inc op";
    // string v = "S' S L R ";
    string str;
    istringstream input(v);
    while (input >> str) {
        V.insert(str);
    }
}

void getLR1::read_t() {
    string t = "int main ( ) { } float id [ ] number , return ; + - * / = for < >= ++ if else  & printf scanf string #";
    // string t = "* i = #";
    string str;
    istringstream input(t);
    while (input >> str) {
        T.insert(str);
    }
}

void getLR1::read_p() {
    freopen("grammer.txt", "r", stdin);
    //freopen("grammer_test.txt", "r", stdin);
    char buffer[100];
    while (gets(buffer) != NULL) {
        string x = string(buffer);
        exp temp;
        string str;
        istringstream input(x);
        input >> str;
        temp.left = str;
        while (input >> str) {
            temp.right.push_back(str);
        }
        P.push_back(temp);
    }
}

void getLR1::print_first() {
    for (map<string, set<string> >::iterator iter = first_all.begin(); iter != first_all.end(); iter++) {
        cout << iter->first << "\t";
        for (set<string>::iterator iter_se = iter->second.begin(); iter_se != iter->second.end(); iter_se++) {
            cout << *iter_se << "\t";
        }
        cout << endl;
    }
}

void copyCode(stack<symbol> trans, symbol &A) {
    symbol temp = trans.top();
    A.code.append(temp.code);
}

void copyVector(vector<string> from, vector<string> &to) {
    for (vector<string>::iterator iter = from.begin(); iter != from.end(); iter++) {
        to.push_back(*iter);
    }
}

void getLR1::Gencode(int exp_num, stack<symbol> &trans) {
    string code;
    string var_name;
    string op;
    string seg;
    symbol A = symbol();
    char tmp_str[30];
    symbol temp;
    stack<symbol> trans_temp;
    int count = P[exp_num].right.size();
    while (count--) {
        trans_temp.push(trans.top());
        trans.pop();
    }
    switch (exp_num) {
        case 0://S'->S
            copyCode(trans_temp, A);
            break;
        case 1://S->int main(){declarations statements}
            for (int i = 0; i < 6; i++) {
                trans_temp.pop();
            }
            A.code.append("\tfinit\n");
            copyCode(trans_temp, A);
            trans_temp.pop();
            textSection.append(A.code);
            bssSection.append(".comm itof, 4\n");
            break;
        case 2://declarations declarations declaration
            copyCode(trans_temp, A);
            trans_temp.pop();
            break;
        case 3://declarations declaration
            copyCode(trans_temp, A);
            trans_temp.pop();
            break;
        case 4://declaration number_type ids ;
            temp = trans_temp.top();
            var_name = temp.type;
            A.type = temp.type;
            trans_temp.pop();
            temp = trans_temp.top();
            if (temp.isArray) {
                for (int i = 0; i < temp.name.size(); i++) {
                    id_type.insert(make_pair(temp.name[i], var_name));
                    A.code.append(temp.name[i]);
                    A.code.append(":\n.");
                    A.code.append(var_name);
                    A.code.append("  ");
                    A.code.append(temp.code);
                    A.code.append("\n");

                    dataSection.append(temp.name[i]);
                    dataSection.append(":\n.");
                    dataSection.append(var_name);
                    dataSection.append("  ");
                    dataSection.append(temp.code);
                    dataSection.append("\n");
                }
            } else {
                for (int i = 0; i < temp.name.size(); i++) {
                    id_type.insert(make_pair(temp.name[i], var_name));
                    A.code.append(".comm ");
                    A.code.append(temp.name[i]);
                    A.code.append(",4\n");

                    bssSection.append(".comm ");
                    bssSection.append(temp.name[i]);
                    bssSection.append(",4\n");
                }
            }
            break;
        case 5://number_type int
            A.type = "int";
            break;
        case 6://number_type float
            A.type = "float";
            break;
        case 7://ids ids , id
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            trans_temp.pop();
            break;
        case 8://ids id
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            break;
        case 9://ids id [ number ] = { numbers }
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            A.isArray = true;
            trans_temp.pop();
            trans_temp.pop();
            trans_temp.pop();
            trans_temp.pop();
            trans_temp.pop();
            trans_temp.pop();
            copyCode(trans_temp, A);
            trans_temp.pop();
            break;
        case 10://numbers numbers , number
            copyCode(trans_temp, A);
            trans_temp.pop();
            A.code.append(",");
            trans_temp.pop();
            temp = trans_temp.top();
            A.code.append(temp.name[0]);
            break;
        case 11://numbers number
            temp = trans_temp.top();
            A.code.append(temp.name[0]);
            break;
        case 12://statements statements statement
            copyCode(trans_temp, A);
            trans_temp.pop();
            copyCode(trans_temp, A);
            trans_temp.pop();
            break;
        case 13://statements statement
            copyCode(trans_temp, A);
            trans_temp.pop();
            break;
        case 14://statement return expr ;
            A.code.append("\tpushl $0\n");
            A.code.append("\tcall exit\n");
            break;
        case 15://statement asgn ;
            copyCode(trans_temp, A);
            break;
        case 16://statement iterator
            copyCode(trans_temp, A);
            break;
        case 17://statement if_statement
            copyCode(trans_temp, A);
            break;
        case 18://statement { statements }
            trans_temp.pop();
            copyCode(trans_temp, A);
            break;
        case 19://statement { }
            trans_temp.pop();
            trans_temp.pop();
            break;
        case 20://expr E
            temp = trans_temp.top();
            A.type = temp.type;
            A.int_value = temp.int_value;
            A.float_value = temp.float_value;
            copyCode(trans_temp, A);
            break;
        case 21://E E + T
            temp = trans_temp.top();
            A.type1 = temp.type;
            copyCode(trans_temp, A);
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            A.type2 = temp.type;
            copyCode(trans_temp, A);
            if (A.type1 == "int" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tpopl %ebx\n\taddl %eax, %ebx\n\tpushl %ebx\n");
                A.type = "int";
            } else if (A.type1 == "float" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tmovl %eax, itof\n\tfiadd itof\n");
                A.type = "float";
            }
            break;
        case 22://E E - T
            temp = trans_temp.top();
            A.type1 = temp.type;
            copyCode(trans_temp, A);
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            A.type2 = temp.type;
            copyCode(trans_temp, A);
            if (A.type == "int" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tpopl %ebx\n\tsubl %eax, %ebx\n\tpushl %ebx\n");
                A.type = "int";
            } else if (A.type1 == "float" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tmovl %eax, itof\n\tfisub itof\n");
                A.type = "float";
            } else if (A.type1 == "int" && A.type2 == "float") {
                A.code.append("\tpopl %eax\n\tmovl %eax, itof\n\tfisubr itof\n");
                A.type = "float";
            }
            break;
        case 23://E T
            temp = trans_temp.top();
            A.type = temp.type;
            A.int_value = temp.int_value;
            A.float_value = temp.float_value;
            copyCode(trans_temp, A);
            break;
        case 24://T T * F
            temp = trans_temp.top();
            A.type1 = temp.type;
            copyCode(trans_temp, A);
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            A.type2 = temp.type;
            copyCode(trans_temp, A);
            if (A.type1 == "int" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tpopl %ebx\n\tmull %ebx\n\tpushl %eax\n");
                A.type = "int";
            } else if (A.type1 == "float" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tmovl %eax, itof\n\tfimul itof\n");
                A.type = "float";
            }
            break;
        case 25://T T / F
            temp = trans_temp.top();
            A.type1 = temp.type;
            copyCode(trans_temp, A);
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            A.type2 = temp.type;
            copyCode(trans_temp, A);
            if (A.type1 == "int" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tpopl %ebx\n\tdivl %eax, %ebx\n\tpushl %ebx\n");
                A.type = "int";
            } else if (A.type1 == "float" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tmovl %eax, itof\n\tfidiv itof\n");
                A.type = "float";
            }
            break;
        case 26://T F
            temp = trans_temp.top();
            A.type = temp.type;
            A.int_value = temp.int_value;
            A.float_value = temp.float_value;
            copyCode(trans_temp, A);
            break;
        case 27://F ( E )
            trans_temp.pop();
            temp = trans_temp.top();
            A.int_value = temp.int_value;
            A.float_value = temp.float_value;
            copyCode(trans_temp, A);
            break;
        case 28://F number
            temp = trans_temp.top();
            A.type = "int";
            A.code.append("\tpushl $");
            A.code.append(temp.name[0]);
            A.code.append("\n");
            break;
        case 29://F id
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            A.type = id_type[A.name[0]];
            if (A.type == "int") {
                A.code.append("\tpushl  ");
                A.code.append(temp.name[0]);
                A.code.append("\n");
            } else {
                A.code.append("\tflds  ");
                A.code.append(temp.name[0]);
                A.code.append("\n");
            }
            break;
        case 30://F id [ expr ]
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            A.type = id_type[A.name[0]];
            A.isArray = true;
            trans_temp.pop();
            trans_temp.pop();
            copyCode(trans_temp, A);
            // temp = trans_temp.top();
            A.code.append("\tpopl %eax\n\tmovl ");
            A.code.append(A.name[0]);
            A.code.append("(, %eax, 4) ,%ebx\n\tpushl %ebx\n");
            break;
        case 31://asgn left = expr
            temp = trans_temp.top();
            if (temp.isArray) {

            } else {
                trans_temp.pop();
                trans_temp.pop();
                var_name = temp.name[0];
                temp = trans_temp.top();
                if (temp.type == "int") {
                    copyCode(trans_temp, A);
                    A.code.append("\tpopl  %eax\n\tmovl %eax,  ");
                    A.code.append(var_name);
                    A.code.append("\n");
                } else {
                    copyCode(trans_temp, A);
                    A.code.append("\tfstps ");
                    A.code.append(var_name);
                    A.code.append("\n");
                }
            }
            break;
        case 32://left id
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            trans_temp.pop();
            break;
        case 33://left id [ expr ]
            break;
        case 34://iterator for ( asgn ; relation ; inc ) statement
            trans_temp.pop(); //for
            trans_temp.pop(); //(
            copyCode(trans_temp, A);
            trans_temp.pop(); //asgn
            trans_temp.pop(); //;
            copyCode(trans_temp, A);
            trans_temp.pop(); //relation
            trans_temp.pop(); //;
            temp = trans_temp.top();
            code.clear();
            code = temp.code;
            trans_temp.pop(); //inc
            trans_temp.pop(); //)             
            copyCode(trans_temp, A);
            A.code.append(code);
            A.code.append("\tjmp .code_seg");
            sprintf(tmp_str, "%d", lable_num - 1);
            A.code.append(string(tmp_str));
            A.code.append("\n");

            sprintf(tmp_str, "%d", lable_num++);
            seg = ".code_seg";
            seg.append(string(tmp_str));
            seg.append(":\n");
            A.code.append(seg);

            break;
        case 35://relation expr op expr
            temp = trans_temp.top();
            A.type1 = temp.type;


            sprintf(tmp_str, "%d", lable_num++);
            seg = ".code_seg";
            seg.append(string(tmp_str));
            seg.append(":\n");
            A.code.append(seg);

            copyCode(trans_temp, A);
            trans_temp.pop();
            temp = trans_temp.top();
            op = temp.name[0];
            trans_temp.pop();
            temp = trans_temp.top();
            A.type2 = temp.type;
            copyCode(trans_temp, A);
            
            if (A.type1 == "int" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tpopl %ebx\n\tcmpl %eax, %ebx\n");
            } else if (A.type1 == "float" && A.type2 == "int") {
                A.code.append("\tpopl %eax\n\tmovl %eax, itof\n\tficomp itof\n\tfstsw\n\tsahf\n");
            }
            if (op == "<") {
                A.code.append("\tjge .code_seg");
                sprintf(tmp_str, "%d", lable_num);
                A.code.append(string(tmp_str));
                A.code.append("\n");
            } else if (op == ">=") {
                A.code.append("\tjl .code_seg");
                sprintf(tmp_str, "%d", lable_num);
                A.code.append(string(tmp_str));
                A.code.append("\n");
            } else {
                cout << "There are some errors!" << endl;
            }

            break;
        case 36://op <
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            trans_temp.pop();
            break;
        case 37://op >=
            temp = trans_temp.top();
            copyVector(temp.name, A.name);
            trans_temp.pop();
            break;
        case 38://inc left ++
            A.code.append("\tincl i\n");
            break;
        case 39://if_statement if ( relation ) statement else statement
            trans_temp.pop();
            trans_temp.pop();
            copyCode(trans_temp, A);
            trans_temp.pop();
            trans_temp.pop();
            copyCode(trans_temp, A);
            A.code.append("\tjmp  .code_seg");
            sprintf(tmp_str, "%d", lable_num + 1);
            A.code.append(string(tmp_str));
            A.code.append("\n");
            trans_temp.pop();
            trans_temp.pop();

            sprintf(tmp_str, "%d", lable_num++);
            seg = ".code_seg";
            seg.append(string(tmp_str));
            seg.append(":\n");
            A.code.append(seg);
            copyCode(trans_temp, A);

            sprintf(tmp_str, "%d", lable_num++);
            seg = ".code_seg";
            seg.append(string(tmp_str));
            seg.append(":\n");
            A.code.append(seg);
            break;
        case 40://statement printf ( string ) ;
            trans_temp.pop();
            trans_temp.pop();
            dataSection.append("string");
            sprintf(tmp_str, "%d", str_num);
            dataSection.append(string(tmp_str));
            dataSection.append(":\n.asciz ");
            temp = trans_temp.top();
            var_name = temp.name[0];
            dataSection.append(var_name);
            dataSection.append("\n");

            A.code.append("\tpushl  $string");
            sprintf(tmp_str, "%d", str_num++);
            A.code.append(string(tmp_str));
            A.code.append("\n\tcall printf\n\taddl $4, %esp\n");
            break;
        case 41://statement printf ( string , ids ) ;
            trans_temp.pop();
            trans_temp.pop();
            dataSection.append("string");
            sprintf(tmp_str, "%d", str_num);
            dataSection.append(string(tmp_str));
            dataSection.append(":\n.asciz ");
            temp = trans_temp.top();
            var_name = temp.name[0];
            dataSection.append(var_name);
            dataSection.append("\n");
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            for (int i = temp.name.size() - 1; i >= 0; i--) {
                if (id_type[temp.name[i]] == "int") {
                    code.clear();
                    code.append("\tpushl ");
                    code.append(temp.name[i]);
                    code.append("\n");
                    A.code.append(code);
                } else {
                    code.clear();
                    code.append("\tflds ");
                    code.append(temp.name[i]);
                    code.append("\n\tfstpl (%esp)\n");
                    A.code.append(code);
                }
            }
            A.code.append("\tpushl  $string");
            sprintf(tmp_str, "%d", str_num++);
            A.code.append(string(tmp_str));
            A.code.append("\n\tcall   printf  \n");
            A.code.append("\taddl $");
            sprintf(tmp_str, "%d", (temp.name.size() + 1)*4);
            A.code.append(string(tmp_str));
            A.code.append(", %esp\n");
            break;
        case 42://statement scanf ( string , & id ) ;
            trans_temp.pop();
            trans_temp.pop();
            dataSection.append("string");
            sprintf(tmp_str, "%d", str_num);
            dataSection.append(string(tmp_str));
            dataSection.append(":\n.asciz ");
            temp = trans_temp.top();
            var_name = temp.name[0];
            dataSection.append(var_name);
            dataSection.append("\n");
            trans_temp.pop();
            trans_temp.pop();
            trans_temp.pop();
            temp = trans_temp.top();
            A.code.append("\tpushl $");
            A.code.append(temp.name[0]);
            code = "\n\tpushl $string";
            sprintf(tmp_str, "%d", str_num++);
            code.append(string(tmp_str));
            code.append("\n");
            A.code.append(code);
            A.code.append("\n\tcall scanf \n");
            A.code.append("\taddl $8,%esp\n");
            break;
    };
    trans.push(A);
}

void getLR1::print_assemly() {
    string code;
    code.append(".section  .data\n");
    code.append(dataSection);
    code.append("\n.section .bss\n");
    code.append(bssSection);
    code.append("\n.section .text\n.globl main\nmain:\n");
    code.append(textSection);
    code.append("\n");
    ofstream output;
    output.open("code.s");
    output << code;
    output.close();
}




