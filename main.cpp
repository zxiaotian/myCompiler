/******************************
*   Author: zhouxiaotian
*   Time  : 2012.11.9
*   E-mail: zxiaotian.hit@gmail.com
*******************************/
#include "getLR1.h"
#include "getLexan.h"

int main(int argc, char** argv) {

    getLexan token_class;
    token_class.lexan();//对input.txt中源文件分词，将结果保存到tokenList结构中
    token_class.print_token(token_class.tokenList);//打印token串到文件token.file中
    Token token;
    token.name = "#";
    token.type = "#";
    token_class.tokenList.push_back(token);//在tokenList中追加#
    
    getLR1 temp;
    temp.read_p();//读产生式
    temp.read_t();//读终结符
    temp.read_v();//读非终结符
    temp.First();//获取first集
    temp.print_first();//打印first集以检验正确性
    temp.getLR1Table();//获取LR1表
    temp.print_LR_table();//打印LR1表到lt_table中，注：LR1表的数据结构是二维数组，大小定义为250，多余的空间用-1填充
    temp.Reduction(token_class.tokenList);  //将tokenList传入，利用LR1表进行规约，在规约的同时生成汇编代码，在Reduction函数返回之前调用print_assemly()函数将汇编代码打印到code.s文件中
    return 0;
}

