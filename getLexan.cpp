/******************************
*   Author: zhouxiaotian
*   Time  : 2012.11.9
*   E-mail: zxiaotian.hit@gmail.com
*******************************/
#include "getLexan.h"

string getLexan::findKeyword(string keyword) {
    string reserverKeyword[] = {
        "int", "char", "void", "main", "if", "else", "do", "while", "scanf", "printf", "return", "struct", "float", "for"
    };
    for (int i = 0; i < 14; i++) {
        if (keyword == reserverKeyword[i])
            return reserverKeyword[i];
    }
    return "id";
}

void getLexan::insert_token(string type, string name) {
    Token tempToken;
    tempToken.type = type;
    tempToken.name = name;
    tokenList.push_back(tempToken);
}

void getLexan::print_token(vector<Token> tokenList) {
    ofstream output;
    output.open("token.file");
    for (int i = 0; i < tokenList.size(); i++) {
        output << tokenList[i].name << "\t" << tokenList[i].type << endl;
    }
    output.close();
}

void getLexan::lexan() {
    char ch;
    string tempString;
    freopen("input.txt", "r", stdin);
    ch = getchar();
    while (ch != EOF) {
        while (ch == ' ' || ch == '\t' || ch == '\n') {
            if (ch == '\n')
                line_record++;
            ch = getchar();
        }
        if (isalpha(ch)) {
            while (isalpha(ch) || isdigit(ch) || ch == '_') {
                tempString.push_back(ch);
                ch = getchar();
            }
            string type_num = findKeyword(tempString);
            insert_token(type_num, tempString);
            tempString.clear();
        } else if (isdigit(ch)) {
            while (isdigit(ch)) {
                tempString.push_back(ch);
                ch = getchar();
            }
            if (ch == '.') {
                tempString.push_back(ch);
                ch = getchar();
                while (isdigit(ch)) {
                    tempString.push_back(ch);
                    ch = getchar();
                }
                if (ch == '.') {
                    cout << "error at:" << line_record << endl;
                    return;
                }
                insert_token("number", tempString);
                tempString.clear();
            } else if (isalpha(ch)) {
                if (isalpha(ch)) {
                    cout << "Error:need a opertor at line:" << line_record;
                    return;
                }
            } else {
                insert_token("number", tempString);
                tempString.clear();
            }

        } else {
            switch (ch) {

                case '*':
                    insert_token("*", "*");
                    ch = getchar();
                    break;
                case '+':
                    ch=getchar();
                    if(ch=='+')
                    {
                        insert_token("++", "++");
                        ch=getchar();
                    }
                    else
                        insert_token("+","+");
                    break;
                case '-':
                    insert_token("-", "-");
                    ch = getchar();
                    break;
                case '/':
                    insert_token("/", "/");
                    ch = getchar();
                    break;
                case '(':
                    insert_token("(", "(");
                    ch = getchar();
                    break;
                case ')':
                    insert_token(")", ")");
                    ch = getchar();
                    break;
                case '[':
                    insert_token("[", "[");
                    ch = getchar();
                    break;
                case ']':
                    insert_token("]", "]");
                    ch = getchar();
                    break;
                case '{':
                    insert_token("{", "{");
                    ch = getchar();
                    break;
                case '}':
                    insert_token("}", "}");
                    ch = getchar();
                    break;
                case ',':
                    insert_token(",", ",");
                    ch = getchar();
                    break;
                case ';':
                    insert_token(";", ";");
                    ch = getchar();
                    break;
                case '<':
                    ch = getchar();
                    if (ch == '=') {
                        insert_token("<=", "<=");
                        ch = getchar();
                    } else
                        insert_token("<", "<");
                    break;
                case '>':
                    ch = getchar();
                    if (ch == '=') {
                        insert_token(">=", ">=");
                        ch = getchar();
                    } else
                        insert_token(">", ">");
                    break;
                case '=':
                    insert_token("=", "=");
                    ch = getchar();
                    break;
                case ':':
                    insert_token(":", ":");
                    ch = getchar();
                    break;
                case '&':
                    insert_token("&", "&");
                    ch = getchar();
                    break;
                case '\"':
                    string print;
                    print.append(1, ch);
                    ch = getchar();
                    while (ch != '\"') {
                        print.append(1, ch);
                        ch = getchar();
                    }
                    print.append(1, ch);
                    insert_token("string", print);
                    ch = getchar();
                    break;
            }
        }
    }
}