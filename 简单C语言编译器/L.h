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

typedef struct SYNBL{//��ʶ����
    char name[100];//��ʶ�����֣�����η�ֹ��ʶ�����ֹ������ֻ�����������⣬�������
    struct TYPEL* type;//���ͱ�ָ��
    char cat;//���࣬Ŀǰ�У�f(����),a(����),d(�ṹ),v(����),n(�����β�),g(��ֵ�β�)
    char L;//��Ծ��Ϣ
    struct PFINFL* pfinfl;//������ָ��
    struct LENL* lenl;//���ȱ�ָ��
    struct VALL* vall;//���¼��ָ��
    struct SYNBL* next;
}SYNBL;

typedef struct TYPEL{//���ͱ�
    char tval;//���룬���ʹ��룬��������ָ��ѡ����������Ϊ��i(����),r(ʵ��),c(�ַ���),a(����),d(�ṹ��)
    struct AINFL* ainfl;//�����ָ��
    struct RINFL* rinfl;//�ṹ���ָ��
}TYPEL;

typedef struct AINFL{//�����
    int low;//�����½�
    int up;//�����Ͻ�
    struct TYPEL* ctp;//�ɷ�����ָ�룬ָ���ά����ɷֵ����͵�ָ��
    int clen;//�ɷ����͵ĳ��ȣ��ɷ�����������ռֵ��Ԫ�ĸ���
}AINFL;

typedef struct RINFL{//�ṹ��
    char ID[10];//�ṹ����
    int OFF;//(����)����idk��ֵ��Ԫ��ַ��������ڼ�¼ֵ����ͷλ��
    struct TYPEL* TP;//ָ�룬ָ��idk��ɷ�����(�����ͱ��е���Ϣ)
}RINFL;

typedef struct PFINFL{//������
    char LEVEL[3];//��κţ��ù�����̬���Ƕ�׺�
    int OFF;//�ù���������������ʼ��Ԫ��Ըù���ֵ����ͷλ��
    int FN;//�ù�������ʽ�����ĸ���
    char ENTRY[5];//�ú���Ŀ������׵�ַ(����ʱ��д)
    struct SYNBL* PARAM;//ָ�룬ָ���βα�
}PFINFL;

typedef struct INT{//�����ͱ�
    int numi;
}INT;

typedef struct REAL{//ʵ���ͱ�
    float numf;
}REAL;

typedef struct CHAR{//�ַ��ͱ�
    char numc;
}CHAR;


typedef struct LENL{//���ȱ�
    int length;//������Ϣ
}LENL;

typedef struct VALL{//���¼��

}VALL;

typedef struct ERRORL{//�����¼��
    int line;//��������
    char type;//�������ͣ�
    char name[100];//��������
    struct ERRORL* next;
}ERRORL;


typedef struct TOKEN{//TOKEN��¼
    char token[3];//token��
    int line;//������¼��ϵͳ������
    struct INT* tpI;//����ָ��
    struct REAL* tpR;//ʵ��ָ��
    struct CHAR* tpC;//�ַ�ָ��
    struct SYNBL* tpIT;//��ʶ����ָ��
    char* tpKT;//�ؼ���ָ��
    char* tpPT;//���ָ��
    char* tpOT;//�����ָ��
    struct TOKEN* next;
}TOKEN;

typedef struct Identifier{//��ʶ����
    struct SYNBL* tp;
    struct Identifier* next;
}Identifier;

typedef struct SEM{//����ջ
    struct TOKEN* tpToken;
    struct SEM* fron;
    struct SEM* next;
}SEM;

typedef struct Operand{//������
    struct TOKEN* tpToken;
}Operand;


typedef struct Quaternary{//��Ԫʽ
    char* Operator;//��������
    struct Operand* operand[3];//3��������
    struct Quaternary* next;
}Quaternary;

class Scanner{
private:
    char ch,ch_before;//��ǰ�ʣ�ǰһ����
    int state,state_before;//״̬��ǰ״̬
    int line,warn;//����������
    void CreatNewToken(TOKEN*& t);
    void reset(int& state,char* code,int& i,int& warn);
    int state_change(int st,char ch,int& warn);
    void state_to_code(TOKEN* t, int state_before, char code[100], int line, int warn);
public:
    struct TOKEN* token;
    struct ERRORL* error_head;//�����ͷָ��
    struct ERRORL* error_now;//�����ǰλ��
    Scanner(){token=NULL;error_head=NULL;error_now=NULL;}
    void Scan();
    int Findexist(char* code, TOKEN*& p);
    void ConvertItoS(int i, string& s);
    void ConvertFtoS(float f,string& st);
    void ConvertStoC(string st,char* c);
    void ConvertStoI(string st,int& i);
    void ConvertStoF(string st,float& f);
    void CoutErrorL();//��������
};

class GrammaticalAnalysis:public Scanner{
private:
    //�﷨����
    struct TOKEN* ch;//��ǰ��
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
    //���岿��
    int d;//�ṹ����
    struct TOKEN* type;//���ͱ��
    struct TOKEN* ch_sem;//������ջ�Ĵ�
    struct TOKEN* operand_now;//��ǰ������
    struct SEM* top;//ջ��
    struct SEM* base;//ջ��
    struct Quaternary* quater_now;//��ǰ��Ԫʽָ��
    void CreateSEM();//����һ������ջ
    void PushStack();//��ջ
    void DeStack();//��ջ
    int mark;//���������λ��
    int counter;//ϵͳ����������
    void CreateQuaternary();//����һ����Ԫʽ
    void PushQuaternary();//����Ԫʽ����
    void Bi_oper_qua();//˫Ŀ����Ĳ���������
    void Unary_oper_qua();//��Ŀ����Ĳ���������
    void Assign_oper_qua();//��ֵ����������
    void If_While_qua();//IF����,WHILEѭ������������
    void GetMark(TOKEN* ch_ope);//��ȡ�����
    void GetMark_front(TOKEN* ch_ope);//��ȡǰ�������
    int FindSynbl();//����
    void PushSynbl();//��ӽ����ű�
    int CoutSynbl();//������ű�
    void CheckSynbl();//����ʶ���Ƿ񱻶��壬δ�������¼���������

public:
    void Grammar();
    //�﷨����
    int result;
    GrammaticalAnalysis(){token=NULL;counter=1;d=0;error_head=NULL;error_now=NULL;}
    //���岿��
    struct Operand* operand[3];//���������
    struct Quaternary* quater;//��Ԫʽ
    struct Identifier* id;//�û������ʶ��
    struct Identifier* id_now;//��ǰ��ʶ��ָ��
    void CoutQuaternary();//�����Ԫʽ
};

typedef struct Block{
    struct Quaternary* qua_block;//�ֿ�����Ԫʽ�鼯��
    struct Block* next;//��һ����
}Block;

typedef struct DAG{//����ͼ
    int num;
    char* oper;
    struct Mark* mark;
    struct DAG* lchild;
    struct DAG* rchild;
    struct DAG* next;
    struct DAG* fron;
}DAG;

typedef struct Mark{//���
    struct TOKEN* name;
    struct Mark* fron;
    struct Mark* next;
}Mark;

class Optimization:public GrammaticalAnalysis{
private:
    struct Quaternary* optimize_now;//��ǰ�Ż���Ԫʽ
    struct Block* optimize_block_now;//�Ż���ĵ�ǰ��
    struct Block* block_now;//��ǰ��ָ��
    struct Quaternary* qua;//��Ԫʽ
    struct DAG* dag_head;//DAGͷָ��
    struct DAG* dag_tail;//DAGβָ��
    struct DAG* dag_now;//DAG��ǰָ��
    struct Operand* find_now_num1;//��ǰ���Ҵ�
    struct Operand* find_now_num2;//��ǰ���Ҵ�
    char* find_now_ope;//��ǰ���������
    struct Operand* add_now;//��ǰ��Ҫ����Ľ��
    char* op_now;//��ǰ�����
    struct DAG* find_begin;//���ҿ�ʼ��
    struct DAG* pos;//�ҵ���λ��
    struct Mark* mark_pos;//�ҵ��ı��λ��
    struct DAG* temp_dag;//������������ڵ�dag���
    struct Mark* temp1;//���������1
    struct Mark* temp2;//���������2
    void CreateOptimize();//����һ���Ż�����Ԫʽͷ��
    void CreateOptimize_Block();//����һ���Ż����ͷ��
    void DivideBlock();//�黮�ֺ���
    void CoutBlock();//�������������Ԫʽ
    void CreateDAG();//����һ��DAG
    void DeleteDAG();//ɾ��DAG
    void AddDAG();//����һ��DAG��
    int FindDAG();//����ظ�DAG���
    int FindDAG_unary();//��鵥Ŀ������ظ�DAG���
    int FindDAG_Bi();//���˫Ŀ������ظ�DAG���
    int MainMark();//���ñ���ǲ��������
    void SwopMark();//�����������
    void DeleteMark();//ɾ��һ�����
    void GetDAG();//�õ�DAG���ɽ��
    void CoutOptimize();//����Ż������Ԫʽ

public:
    struct Quaternary* optimize;//�Ż���Ԫʽ
    struct Block* optimize_block;//�Ż���Ŀ�
    struct Block* block;//��ָ��
    void Optimize();
    Optimization(){dag_head=NULL;dag_tail=NULL;dag_now=NULL;}


};

class ObjectCode:public Optimization{
private:
    struct Object* obj_now;//��ǰĿ�����
    struct Object* obj_find;//�����ҵ���Ŀ�����λ��
    struct Block* blo;
    struct Quaternary* qua;
    struct Operand* num;//��ǰ������
    char numType;//������������
    int find_num;//��Ҫ���ҵ�ϵͳ��������
    int lab;//���
    char type_find;//��Ҫ���ҵ�Ŀ��������ͣ�iΪif��eΪelse��dΪdo�����ڻ���������תλ��
    int SystemExist();//���ҷ��ű����Ƿ��б�����м����
    void UpdateSymbol();//���·��ű���ϵͳ�����м��������
    void GetObjCode();//����Ŀ�����
    void AddObject();//����obj���
    void CoutObj();//���Ŀ�����
    void Judge(string& st);//��ȡ���������ͣ��浽numType
    int Find_obj();//�Ӻ���ǰѰ����Ӧ���͵��׸�Ŀ��������
    int Find_obj_head();//��ǰ���Ѱ����Ӧ���͵��׸�Ŀ��������
    //Ŀ��������ɺ���������ξ�����䣩
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
    void GetHead();//Ŀ�����ͷ������(�����ݶ�)
    void GetTail();//Ŀ�����β������
public:
    struct Object* obj;
    void ObjCode();
    ObjectCode(){lab=0;}
};

typedef struct Object{//Ŀ�����
    char code[30];//�洢��Ŀ�����
    char add[10];//����ĵ�ַ���
    int lab;//���
    char type;//��Ҫ���������,0Ϊ����Ҫ���iΪif�Ļ��eΪelse�Ļ��dΪdo�Ļ���
    struct Object* fron;
    struct Object* next;
}Object;

class Compiler:public ObjectCode{
private:

public:

};

#endif // L_H_INCLUDED
