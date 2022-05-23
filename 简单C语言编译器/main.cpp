#include "L.h"


int main(){
    Scanner scan;//创建一个扫描器
    GrammaticalAnalysis grammar;//创建一个语法分析器
    Optimization opti;//创建一个优化器
    ObjectCode obj;//创建一个目标代码生成器
    Compiler compiler;//创建一个编译器

    scan.Scan();//词法分析
    if(scan.error_head->next!=NULL){
        cout<<"词法错误!"<<endl;
        scan.CoutErrorL();
    }
    else{
        grammar.token=scan.token;
        grammar.Grammar();//语法分析&语义动作
        if(grammar.result==0){cout<<"存在语法错误！"<<endl;}
        else{
            if(grammar.error_head->next!=NULL){
            cout<<"定义错误!"<<endl;
            grammar.CoutErrorL();
            }
            else{
                cout<<"四元式："<<endl;
                grammar.CoutQuaternary();//输出中间代码——四元式
                opti.quater=grammar.quater;
                opti.Optimize();//中间代码优化
                obj.optimize_block=opti.optimize_block;
                obj.id=grammar.id;
                obj.ObjCode();//目标代码生成
                cout<<"编译成功!"<<endl;
                system("DOSBox.exe");
            }
        }
    }
    return 0;
}
