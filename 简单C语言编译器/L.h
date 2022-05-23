#ifndef L_H_INCLUDED
#define L_H_INCLUDED

#include <iostream>
#include <string.h>
#include <string>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>

#define OK 1
#define ERROR 0

typedef struct SYNBL{//标识符表
    char name[100];//标识符名字（？如何防止标识符名字过长出现缓冲区溢出问题，待解决）
    struct TYPEL* type;//类型表指针
    char cat;//种类，目前有：f(函数),a(数组),d(结构),v(变量),n(换名形参),g(赋值形参)
    char L;//活跃信息
    struct PFINFL* pfinfl;//函数表指针
    struct LENL* lenl;//长度表指针
    struct VALL* vall;//活动记录表指针
    struct SYNBL* next;
}SYNBL;

typedef struct TYPEL{//类型表
    char tval;//类码，类型代码，决定下列指针选择，现有类码为：i(整型),r(实型),c(字符型),a(数组),d(结构体)
    struct AINFL* ainfl;//数组表指针
    struct RINFL* rinfl;//结构体表指针
}TYPEL;

typedef struct AINFL{//数组表
    int low;//数组下界
    int up;//数组上界
    struct TYPEL* ctp;//成分类型指针，指向该维数组成分的类型的指针
    int clen;//成分类型的长度，成分类型数据所占值单元的个数
}AINFL;

typedef struct RINFL{//结构表
    char ID[10];//结构域名
    int OFF;//(区距)—是idk的值单元首址相对于所在记录值区区头位置
    struct TYPEL* TP;//指针，指向idk域成分类型(在类型表中的信息)
}RINFL;

typedef struct PFINFL{//函数表
    char LEVEL[3];//层次号，该过函静态层次嵌套号
    int OFF;//该过函自身数据区起始单元相对该过函值区区头位置
    int FN;//该过函的形式参数的个数
    char ENTRY[5];//该函数目标程序首地址(运行时填写)
    struct SYNBL* PARAM;//指针，指向形参表
}PFINFL;

typedef struct INT{//整数型表
    int numi;
}INT;

typedef struct REAL{//实数型表
    float numf;
}REAL;

typedef struct CHAR{//字符型表
    char numc;
}CHAR;


typedef struct LENL{//长度表
    int length;//长度信息
}LENL;

typedef struct VALL{//活动记录表

}VALL;

typedef struct ERRORL{//错误记录表
    int line;//错误行数
    char type;//错误类型，
    char name[100];//错误内容
    struct ERRORL* next;
}ERRORL;


typedef struct TOKEN{//TOKEN记录
    char token[3];//token码
    int line;//行数记录，系统编码标记
    struct INT* tpI;//整型指针
    struct REAL* tpR;//实型指针
    struct CHAR* tpC;//字符指针
    struct SYNBL* tpIT;//标识符表指针
    char* tpKT;//关键字指针
    char* tpPT;//界符指针
    char* tpOT;//运算符指针
    struct TOKEN* next;
}TOKEN;

typedef struct Identifier{//标识符表
    struct SYNBL* tp;
    struct Identifier* next;
}Identifier;

typedef struct SEM{//语义栈
    struct TOKEN* tpToken;
    struct SEM* fron;
    struct SEM* next;
}SEM;

typedef struct Operand{//操作数
    struct TOKEN* tpToken;
}Operand;


typedef struct Quaternary{//四元式
    char* Operator;//操作符表
    struct Operand* operand[3];//3个操作数
    struct Quaternary* next;
}Quaternary;

class Scanner{
private:
    char ch,ch_before;//当前词，前一个词
    int state,state_before;//状态，前状态
    int line,warn;//行数，警告
    void CreatNewToken(TOKEN*& t);
    void reset(int& state,char* code,int& i,int& warn);
    int state_change(int st,char ch,int& warn);
    void state_to_code(TOKEN* t, int state_before, char code[100], int line, int warn);
public:
    struct TOKEN* token;
    struct ERRORL* error_head;//错误表头指针
    struct ERRORL* error_now;//错误表当前位置
    Scanner(){token=NULL;error_head=NULL;error_now=NULL;}
    void Scan();
    int Findexist(char* code, TOKEN*& p);
    void ConvertItoS(int i, string& s);
    void ConvertFtoS(float f,string& st);
    void ConvertStoC(string st,char* c);
    void ConvertStoI(string st,int& i);
    void ConvertStoF(string st,float& f);
    void CoutErrorL();//输出错误表
};

class GrammaticalAnalysis:public Scanner{
private:
    //语法部分
    struct TOKEN* ch;//当前词
    int Start();
    int Function();
    int Parameter();
    int FType();
    int Type();
    int Declaration();
    int Base_declaration();
    int Id();
    int Id_Expression();
    int Content();
    int Structure();
    int Expression();
    int Cout();
    int Assignment();
    int Logical_or();
    int Logical_and();
    int Inclusive_or();
    int Exclusive_or();
    int And();
    int Equality();
    int Relational();
    int Shift();
    int Additive();
    int Multiplicative();
    int Unary();
    int Postfix();
    int Primary();
    int IF();
    int While();
    int Fun();
    //语义部分
    int d;//结构体标记
    struct TOKEN* type;//类型标记
    struct TOKEN* ch_sem;//入语义栈的词
    struct TOKEN* operand_now;//当前操作数
    struct SEM* top;//栈顶
    struct SEM* base;//栈底
    struct Quaternary* quater_now;//当前四元式指针
    void CreateSEM();//生成一个语义栈
    void PushStack();//入栈
    void DeStack();//出栈
    int mark;//标明运算符位置
    int counter;//系统变量计数器
    void CreateQuaternary();//生成一个四元式
    void PushQuaternary();//入四元式队列
    void Bi_oper_qua();//双目算符的操作数生成
    void Unary_oper_qua();//单目算符的操作数生成
    void Assign_oper_qua();//赋值操作数生成
    void If_While_qua();//IF条件,WHILE循环操作数生成
    void GetMark(TOKEN* ch_ope);//获取运算符
    void GetMark_front(TOKEN* ch_ope);//获取前置运算符
    int FindSynbl();//查重
    void PushSynbl();//添加进符号表
    int CoutSynbl();//输出符号表
    void CheckSynbl();//检查标识符是否被定义，未定义则记录到错误表中

public:
    void Grammar();
    //语法部分
    int result;
    GrammaticalAnalysis(){token=NULL;counter=1;d=0;error_head=NULL;error_now=NULL;}
    //语义部分
    struct Operand* operand[3];//语义操作数
    struct Quaternary* quater;//四元式
    struct Identifier* id;//用户定义标识符
    struct Identifier* id_now;//当前标识符指针
    void CoutQuaternary();//输出四元式
};

typedef struct Block{
    struct Quaternary* qua_block;//分块后的四元式块集合
    struct Block* next;//下一个块
}Block;

typedef struct DAG{//无向图
    int num;
    char* oper;
    struct Mark* mark;
    struct DAG* lchild;
    struct DAG* rchild;
    struct DAG* next;
    struct DAG* fron;
}DAG;

typedef struct Mark{//标记
    struct TOKEN* name;
    struct Mark* fron;
    struct Mark* next;
}Mark;

class Optimization:public GrammaticalAnalysis{
private:
    struct Quaternary* optimize_now;//当前优化四元式
    struct Block* optimize_block_now;//优化后的当前块
    struct Block* block_now;//当前块指针
    struct Quaternary* qua;//四元式
    struct DAG* dag_head;//DAG头指针
    struct DAG* dag_tail;//DAG尾指针
    struct DAG* dag_now;//DAG当前指针
    struct Operand* find_now_num1;//当前查找词
    struct Operand* find_now_num2;//当前查找词
    char* find_now_ope;//当前查找运算符
    struct Operand* add_now;//当前需要加入的结点
    char* op_now;//当前运算符
    struct DAG* find_begin;//查找开始处
    struct DAG* pos;//找到的位置
    struct Mark* mark_pos;//找到的标记位置
    struct DAG* temp_dag;//待交换标记所在的dag结点
    struct Mark* temp1;//待交换标记1
    struct Mark* temp2;//待交换标记2
    void CreateOptimize();//创建一个优化后四元式头部
    void CreateOptimize_Block();//创建一个优化后块头部
    void DivideBlock();//块划分函数
    void CoutBlock();//按基本块输出四元式
    void CreateDAG();//创建一个DAG
    void DeleteDAG();//删除DAG
    void AddDAG();//加入一个DAG点
    int FindDAG();//检查重复DAG结点
    int FindDAG_unary();//检查单目运算符重复DAG结点
    int FindDAG_Bi();//检查双目运算符重复DAG结点
    int MainMark();//检查该标记是不是主标记
    void SwopMark();//交换两个标记
    void DeleteMark();//删除一个标记
    void GetDAG();//得到DAG生成结果
    void CoutOptimize();//输出优化后的四元式

public:
    struct Quaternary* optimize;//优化四元式
    struct Block* optimize_block;//优化后的块
    struct Block* block;//块指针
    void Optimize();
    Optimization(){dag_head=NULL;dag_tail=NULL;dag_now=NULL;}


};

class ObjectCode:public Optimization{
private:
    struct Object* obj_now;//当前目标代码
    struct Object* obj_find;//回溯找到的目标代码位置
    struct Block* blo;
    struct Quaternary* qua;
    struct Operand* num;//当前操作数
    char numType;//操作数的类型
    int find_num;//需要查找的系统变量名称
    int lab;//标号
    char type_find;//需要查找的目标代码类型，i为if，e为else，d为do，用于回溯重填跳转位置
    int SystemExist();//查找符号表中是否有必须的中间变量
    void UpdateSymbol();//更新符号表，将系统生成中间变量放入
    void GetObjCode();//生成目标代码
    void AddObject();//加入obj结点
    void CoutObj();//输出目标代码
    void Judge(string& st);//获取操作数类型，存到numType
    int Find_obj();//从后向前寻找相应类型的首个目标代码语句
    int Find_obj_head();//从前向后寻找相应类型的首个目标代码语句
    //目标代码生成函数（代码段具体语句）
    void MOV_BX_A();
    void MOV_BX_B();
    void MOV_CX_B();
    void MOV_C_BX();
    void MOV_C_CX();
    void MOV_A_BX();
    void MOV_AX_A();
    void MOV_C_AX();
    void MOV_DX_ZERO();
    void MOV_C_DX();
    void MOV_CX_ONE();
    void MOV_CX_ZERO();
    void CMP_AX_BX();
    void CMP_AX_ZERO();
    void CMP_BX_ZERO();
    void GetHead();//目标代码头部生成(含数据段)
    void GetTail();//目标代码尾部生成
public:
    struct Object* obj;
    void ObjCode();
    ObjectCode(){lab=0;}
};

typedef struct Object{//目标语句
    char code[30];//存储的目标代码
    char add[10];//回填的地址标号
    int lab;//标号
    char type;//需要回填的类型,0为不需要回填，i为if的回填，e为else的回填，d为do的回填
    struct Object* fron;
    struct Object* next;
}Object;

class Compiler:public ObjectCode{
private:

public:

};

#endif // L_H_INCLUDED
