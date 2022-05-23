#include "L.h"
#include "Operator.h"

void GrammaticalAnalysis::Grammar(){
    error_head=new ERRORL;
    error_now=error_head;
    error_now->next=NULL;
    id=new Identifier;//����һ�����ű�
    id->next=NULL;
    id_now=id;
    CreateSEM();//����һ������ջ
    CreateQuaternary();//����һ����Ԫʽ
    ch=token->next;
    result=0;
    result=Start();//ִ���﷨�����嶯��
    CheckSynbl();//�����Ԫʽ���û�����ı�ʶ���Ƿ��ڷ��ű��д���
    CoutSynbl();//������ű���Ϣ
}

void GrammaticalAnalysis::CheckSynbl(){//����ʶ���Ƿ񱻶���
    int m;
    Quaternary* q=quater;
    if(q!=NULL){
        q=q->next;
        for(;q!=NULL;q=q->next){
            int i=0;
            for(;i<3;i++){
                if(q->operand[i]->tpToken!=NULL){
                    m=0;
                    if(strcmp(q->operand[i]->tpToken->token,"4")==0){
                        Identifier* p=id;
                        if(p!=NULL){
                            p=p->next;
                            for(;p!=NULL;p=p->next){
                                if(strcmp(p->tp->name,q->operand[i]->tpToken->tpIT->name)==0){
                                    m=1;
                                }
                            }
                        }
                        if(m==0){
                            ERRORL* e=new ERRORL;
                            e->next=NULL;
                            strcpy(e->name,q->operand[i]->tpToken->tpIT->name);
                            e->line=q->operand[i]->tpToken->line;
                            e->type='d';
                            error_now->next=e;
                            error_now=e;
                        }
                    }
                }
            }
        }
    }



}

int GrammaticalAnalysis::CoutSynbl(){//������ű�
    Identifier* p=id;
    if(p->next==NULL){return 0;}
    p=p->next;
    for(;p!=NULL;p=p->next){
        cout<<"������:"<<p->tp->name<<"  ����:"<<p->tp->cat<<"  ����:"<<p->tp->type->tval<<"  ����:"<<p->tp->lenl->length<<endl;
    }
    return 0;
}

void GrammaticalAnalysis::CreateSEM(){//����һ������ջ
    base=new SEM;
    top=base;
    base->fron=NULL;
    base->next=NULL;
}

void GrammaticalAnalysis::PushStack(){//������ջ
    SEM* p;
    p=new SEM;
    p->tpToken=ch_sem;
    p->fron=top;
    p->next=NULL;
    top->next=p;
    top=p;
    /*if(strcmp(top->tpToken->token,"4")==0){cout<<"ruzhan:"<<top->tpToken->tpIT->name<<endl;}
    else if(strcmp(top->tpToken->token,"0")==0){cout<<"ruzhan:"<<top->tpToken->tpI->numi<<endl;}
    else if(strcmp(top->tpToken->token,"99")==0){cout<<"ruzhan:t"<<top->tpToken->line<<endl;}*/
    /*����*/
}

void GrammaticalAnalysis::DeStack(){//������ջ
    SEM* p;
    p=top;
    operand_now=top->tpToken;
    if(top->fron!=NULL){
        top=top->fron;
        top->next=NULL;
        delete p;
    }
}

void GrammaticalAnalysis::CreateQuaternary(){//����һ����Ԫʽ
    int i=0;
    quater=new Quaternary;
    quater_now=quater;
    quater->next=NULL;
    for(;i<3;i++)quater->operand[i]=NULL;//����Ԫʽ��3����������ʼ��
    quater->Operator=NULL;//����Ԫʽ���������ʼ��
}

void GrammaticalAnalysis::PushQuaternary(){//����Ԫʽ����
    int i=0;
    Quaternary* p=new Quaternary;
    p->next=NULL;
    for(;i<3;i++)p->operand[i]=operand[i];//��������������Ԫʽ����
    //cout<<"!"<<OperatorL[mark]<<"!"<<endl;/*����*/
    p->Operator=OperatorL[mark];//ָ����Ӧ�����
    quater_now->next=p;
    quater_now=p;
}

void GrammaticalAnalysis::CoutQuaternary(){//�����Ԫʽ
    Quaternary* p=quater;
    while(p->next!=NULL){//��������е�ȫ����Ԫʽ
        p=p->next;
        cout<<"("<<p->Operator;
        int i=0;
        for(;i<3;i++){
            if(p->operand[i]->tpToken==NULL){cout<<",_";}
            else{
                if(strcmp(p->operand[i]->tpToken->token,"0")==0){cout<<","<<p->operand[i]->tpToken->tpI->numi;}
                else if(strcmp(p->operand[i]->tpToken->token,"1")==0){cout<<","<<p->operand[i]->tpToken->tpR->numf;}
                else if(strcmp(p->operand[i]->tpToken->token,"2")==0){cout<<","<<p->operand[i]->tpToken->tpC->numc;}
                else if(strcmp(p->operand[i]->tpToken->token,"4")==0){cout<<","<<p->operand[i]->tpToken->tpIT->name;}
                else if(strcmp(p->operand[i]->tpToken->token,"99")==0){cout<<",t"<<p->operand[i]->tpToken->line;}
            }
        }
        cout<<")"<<endl;
    }
}

void GrammaticalAnalysis::Unary_oper_qua(){//��Ŀ�������������
    int i=0;
    for(;i<3;i++)operand[i]=new Operand;//�½�3���������ռ�
    DeStack();//��ȡ��һ��������
    operand[0]->tpToken=operand_now;//���ӵ�һ��������
    operand[1]->tpToken=NULL;//�ڶ���������Ϊ��
    operand[2]->tpToken=new TOKEN;//������������Ϊϵͳ���ɵ��м����
    strcpy(operand[2]->tpToken->token,"99");//��ϵͳ���ı�ʶ��Ϊ99
    operand[2]->tpToken->line=counter;//ʹ��line��¼�����
    counter++;//ϵͳ���ɵ��м��������+1
    PushQuaternary();//����Ԫʽ����
    ch_sem=operand[2]->tpToken;
    /*cout<<"danmu"<<endl;*/
    PushStack();//��ϵͳ���ɵ��м����������ջ
}

void GrammaticalAnalysis::Assign_oper_qua(){//��ֵ����������
    int i=0;
    for(;i<3;i++)operand[i]=new Operand;
    DeStack();
    operand[0]->tpToken=operand_now;
    operand[1]->tpToken=NULL;
    DeStack();
    operand[2]->tpToken=operand_now;
    PushQuaternary();
    ch_sem=operand[2]->tpToken;
    /*cout<<"fuzhi"<<endl;*/
    PushStack();
}

void GrammaticalAnalysis::Bi_oper_qua(){//˫Ŀ�������������
    int i=0;
    for(;i<3;i++)operand[i]=new Operand;
    DeStack();//������һ��������
    operand[1]->tpToken=operand_now;
    DeStack();//�����ڶ���������
    operand[0]->tpToken=operand_now;
    operand[2]->tpToken=new TOKEN;
    strcpy(operand[2]->tpToken->token,"99");//������������Ϊϵͳ�����м����
    operand[2]->tpToken->line=counter;
    counter++;
    PushQuaternary();
    ch_sem=operand[2]->tpToken;
    /*cout<<"shuangmu"<<endl;*/
    PushStack();
}

void GrammaticalAnalysis::If_While_qua(){//IF,WHILE,COUT�ṹ����������
    int i=0;
    for(;i<3;i++)operand[i]=new Operand;
    if(mark==25||mark==29||mark==36){
        DeStack();
        operand[0]->tpToken=operand_now;
        operand[1]->tpToken=NULL;
        operand[2]->tpToken=NULL;
    }
    else{
         for(i=0;i<3;i++)operand[i]->tpToken=NULL;
    }
    PushQuaternary();
    /*cout<<"If"<<endl;*/
}

void GrammaticalAnalysis::GetMark(TOKEN* ch_ope){//��ȡ�����
    string st;
    st=ch_ope->token;
    ConvertStoI(st,mark);
    mark=mark-70;
    /*cout<<"��������:"<<mark<<endl;*/
}

void GrammaticalAnalysis::GetMark_front(TOKEN* ch_ope){
    if(strcmp(ch_ope->token,"86")==0||strcmp(ch_ope->token,"90")==0){//!,~
        GetMark(ch_ope);
    }
    else if(strcmp(ch_ope->token,"75")==0){//++
        mark=31;
    }
    else if(strcmp(ch_ope->token,"76")==0){//--
        mark=32;
    }
    else if(strcmp(ch_ope->token,"40")==0){//sizeof
        mark=34;
    }
}

int GrammaticalAnalysis::Start(){//��ʼ
    while(ch!=NULL){//Function()
        if(!Function()){return 0;}
    }
    return 1;
}


int GrammaticalAnalysis::Function(){//����
    if(!FType()){
        return 0;
    }
    if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
        ch=ch->next;
    }
    else{return 0;}
    if(ch!=NULL&&strcmp(ch->token,"52")==0){//(
        ch=ch->next;
    }
    else{return 0;}
    if(!Parameter()){//Parameter()
        return 0;
    }
    if(ch!=NULL&&strcmp(ch->token,"53")==0){//)
        ch=ch->next;
    }
    else{return 0;}
    if(ch!=NULL&&strcmp(ch->token,"50")==0){//{
        ch=ch->next;
    }
    else{return 0;}
    if(!Declaration()){return 0;}//Declaration()
    if(!Content()){return 0;}//Content()
    if(ch!=NULL&&strcmp(ch->token,"51")==0){//}
        ch=ch->next;
    }
    else{return 0;}

    return 1;
}

int GrammaticalAnalysis::Parameter(){//��������
    if(!Type()){//Type()
        if(ch!=NULL&&strcmp(ch->token,"28")==0){//void
            ch=ch->next;
            return 1;
        }
        else {return 1;}
    }
    else{
        if(!Id()){return 0;}//Id()
        else {
            while(ch!=NULL&&strcmp(ch->token,"59")==0){//,
                ch=ch->next;
                if(!Type()){return 0;}//Type()
                else{
                    if(!Id()){return 0;}//Id()
                }
            }
        }
    }
    return 1;
}

int GrammaticalAnalysis::FType(){//��������
    if(ch!=NULL&&strcmp(ch->token,"28")==0){//void
        ch=ch->next;
        return 1;
    }
    if(!Type()){return 0;}//Type()
    return 1;
}

int GrammaticalAnalysis::Type(){//������������
    if(ch!=NULL&&strcmp(ch->token,"12")==0){//int
        type=ch;
        ch=ch->next;
        return 1;
    }
    else if(ch!=NULL&&strcmp(ch->token,"14")==0){//real
        type=ch;
        ch=ch->next;
        return 1;
    }
    else if(ch!=NULL&&strcmp(ch->token,"16")==0){//char
        type=ch;
        ch=ch->next;
        return 1;
    }
    return 0;
}

int GrammaticalAnalysis::Declaration(){//����
    while(ch!=NULL&&(strcmp(ch->token,"12")==0||strcmp(ch->token,"14")==0||strcmp(ch->token,"16")==0||strcmp(ch->token,"17")==0)){
        if(ch!=NULL&&strcmp(ch->token,"17")==0){//struct
            ch=ch->next;
            if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
                ch=ch->next;
                d=1;//�ṹ��
                if(ch!=NULL&&strcmp(ch->token,"50")==0){//{
                    ch=ch->next;
                    if(!Base_declaration()){return 0;}//Base_declaration()
                    else {
                        if(ch!=NULL&&strcmp(ch->token,"51")==0){//}
                            ch=ch->next;
                            if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
                                ch=ch->next;
                                d=0;
                            }
                            else {return 0;}
                        }
                        else {return 0;}
                    }
                }
                else{//�ṹ�嶨��
                    if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
                        ch=ch->next;
                        if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
                            ch=ch->next;
                        }
                        else {return 0;}
                    }
                    else {return 0;}
                }
            }
            else {return 0;}
        }
        if(!Base_declaration()){return 1;}//Base_declaration()
    }
    return 1;
}

int GrammaticalAnalysis::Base_declaration(){//������������
    while(Type()){//Type()
        if(!Id()){return 0;}//Id()
        else{
            while(ch!=NULL&&strcmp(ch->token,"59")==0){//,
                ch=ch->next;
                if(!Id()){return 0;}//Id()
            }
        }
        if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
            ch=ch->next;
        }
        else {return 0;}
    }
    return 1;
}

int GrammaticalAnalysis::Id(){//Id
    if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
        ch_sem=ch;
        ch=ch->next;
        if(ch!=NULL&&strcmp(ch->token,"54")==0){//[
            ch=ch->next;
            if(ch!=NULL&&strcmp(ch->token,"0")==0){//Integer
                TOKEN* num_temp=ch;
                ch=ch->next;
                if(ch!=NULL&&strcmp(ch->token,"55")==0){//]
                    if(!FindSynbl()){
                        PushSynbl();
                        TYPEL* p=new TYPEL;
                        p->ainfl=NULL;
                        p->rinfl=NULL;
                        if(d){p->tval='d';}
                        else {p->tval='a';}
                        p->ainfl=new AINFL;
                        p->ainfl->low=0;
                        p->ainfl->up=num_temp->tpI->numi-1;
                        p->ainfl->ctp=new TYPEL;
                        if(strcmp(type->token,"12")==0){id_now->tp->cat='i';}
                        else if(strcmp(type->token,"14")==0){id_now->tp->cat='r';}
                        else if(strcmp(type->token,"16")==0){id_now->tp->cat='c';}
                        p->ainfl->clen=(p->ainfl->up+1)*2;
                        id_now->tp->lenl=new LENL;
                        id_now->tp->lenl->length=p->ainfl->clen;
                        id_now->tp->type=p;
                    }
                    else{
                        ERRORL* e=new ERRORL;
                        e->next=NULL;
                        strcpy(e->name,ch_sem->tpIT->name);
                        e->line=ch_sem->line;
                        e->type='m';
                        error_now->next=e;
                        error_now=e;
                    }
                    ch=ch->next;
                    return 1;//��������
                }
                else {return 0;}
            }
            else {return 0;}
        }
        else {
            if(!FindSynbl()){
                PushSynbl();
                TYPEL* p=new TYPEL;
                p->ainfl=NULL;
                p->rinfl=NULL;
                id_now->tp->type=p;
                if(d){id_now->tp->type->tval='d';}
                else {id_now->tp->type->tval='v';}
                if(strcmp(type->token,"12")==0){id_now->tp->cat='i';}
                else if(strcmp(type->token,"14")==0){id_now->tp->cat='r';}
                else if(strcmp(type->token,"16")==0){id_now->tp->cat='c';}
                id_now->tp->lenl=new LENL;
                id_now->tp->lenl->length=2;
            }
            else{
                ERRORL* e=new ERRORL;
                e->next=NULL;
                strcpy(e->name,ch_sem->tpIT->name);
                e->line=ch_sem->line;
                e->type='m';
                error_now->next=e;
                error_now=e;
            }
            return 1;
        }//�������ͱ�ʶ��
    }
    return 0;
}

int GrammaticalAnalysis::FindSynbl(){
    Identifier* p=id;
    if(p->next==NULL){return 0;}
    p=p->next;
    for(;p!=NULL;p=p->next){
        if(strcmp(p->tp->name,ch_sem->tpIT->name)==0){return 1;}
    }
    return 0;
}

void GrammaticalAnalysis::PushSynbl(){
    Identifier* p=new Identifier;
    p->next=NULL;
    p->tp=ch_sem->tpIT;
    id_now->next=p;
    id_now=p;
}

int GrammaticalAnalysis::Id_Expression(){//Id_Expression
    if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
        ch_sem=ch;
        ch=ch->next;
        if(ch!=NULL&&strcmp(ch->token,"54")==0){//[
            PushStack();//�������ͱ�ʶ����ջ
            ch=ch->next;
            if(ch!=NULL&&strcmp(ch->token,"0")==0){//Integer
                ch_sem=ch;
                ch=ch->next;
                if(ch!=NULL&&strcmp(ch->token,"55")==0){//]
                    PushStack();
                    mark=33;
                    Bi_oper_qua();
                    ch=ch->next;
                    return 1;//��������
                }
                else {return 0;}
            }
            else {return 0;}
        }
        else{
            if(ch!=NULL&&strcmp(ch->token,"77")==0){
                PushStack();//�������ͱ�ʶ����ջ
            }
            return 1;
        }//�������ͱ�ʶ��
    }
    return 0;
}

int GrammaticalAnalysis::Content(){//����
    while(ch!=NULL){
        if(!Structure()){//Structure()
            return 0;
        }
        else {
            if(ch!=NULL&&strcmp(ch->token,"51")==0){return 1;}//����һ����Ϊ'}',��˵������������
        }
    }
    return 1;
}

int GrammaticalAnalysis::Structure(){//���ݽṹѡ��
    if(!Fun()){//Fun()
        if(!Expression()){//Expression()
            if(!IF()){//If()
                if(!While()){//While()
                    if(!Cout()){//Cout()
                        if(ch!=NULL&&strcmp(ch->token,"51")==0){return 1;}//����һ����Ϊ'}',��˵������������,�˴�������
                        else {return 0;}
                    }
                    else {return 1;}
                }
                else {return 1;}
            }
            else {return 1;}
        }
        if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
            ch=ch->next;
            return 1;
        }
        else {return 0;}
    }
    else {return 1;}
}

int GrammaticalAnalysis::Cout(){//�������
    if(ch!=NULL&&strcmp(ch->token,"43")==0){//cout
        ch=ch->next;
        if(!Logical_or()){return 0;}
        else{//Logical_or()
            if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
                mark=36;
                If_While_qua();
                ch=ch->next;
                return 1;
            }
            else{return 0;}
        }
    }
    else {return 0;}
}

int GrammaticalAnalysis::Fun(){//�������ʽ
    TOKEN* ch_temp=ch;
    if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
        ch=ch->next;
        if(ch!=NULL&&strcmp(ch->token,"52")==0){//(
            ch=ch->next;
            if(!Assignment()){//Assignment(),�޲���
                if(ch!=NULL&&strcmp(ch->token,"53")==0){//)
                    ch=ch->next;
                    if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
                        ch=ch->next;
                        return 1;
                    }
                    else {return 0;}
                }
                else {return 0;}
            }
            else{//�в���
                while(ch!=NULL&&strcmp(ch->token,"59")==0){//,
                    ch=ch->next;
                    if(!Assignment()){return 0;}//Assignment()
                }
                if(ch!=NULL&&strcmp(ch->token,"53")==0){//)
                    ch=ch->next;
                    if(ch!=NULL&&strcmp(ch->token,"58")==0){//;
                        ch=ch->next;
                        return 1;
                    }
                    else {return 0;}
                }
                else {return 0;}
            }
        }
        else{//�����Ǻ������ʽ���򷵻ؼ�¼λ��
            ch=ch_temp;
            return 0;
        }
    }
    else {return 0;}
}

int GrammaticalAnalysis::Expression(){//���ʽ
    if(!Assignment()){return 0;}//Assignment()
    while(ch!=NULL&&strcmp(ch->token,"59")==0){//,
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Assignment()){return 0;}//Assignment()
            GetMark(ch_ope);
            Unary_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Assignment(){//��ֵ
    TOKEN* ch_ope;
    TOKEN* ch_temp=ch;
    if(ch!=NULL&&strcmp(ch->token,"4")==0){
        ch_sem=ch;
        ch=ch->next;
        if(ch!=NULL&&strcmp(ch->token,"60")==0){
            PushStack();//��Ϊ.(�ṹ���Ա),��.ǰ��ʶ����ջ
            ch_sem=ch;
            ch=ch->next;
            if(!Id_Expression()){return 0;}
            else{
                mark=35;
                Bi_oper_qua();
                if(ch!=NULL&&strcmp(ch->token,"77")==0){
                    ch_ope=ch;
                    ch=ch->next;
                    if(!Logical_or()){return 0;}
                    else {
                        GetMark(ch_ope);
                        Assign_oper_qua();
                        return 1;
                    }
                }
                else {return 0;}
            }
        }
        else{
            ch=ch_temp;
            if(!Id_Expression()){
                if(!Logical_or()){return 0;}
                else {return 1;}
            }
            else{
                if(ch!=NULL&&strcmp(ch->token,"77")==0){
                   // PushStack();//��Ϊ��ֵ����=֮ǰ�ı�ʶ����ջ��������
                    ch_ope=ch;
                    ch=ch->next;
                    if(!Logical_or()){return 0;}
                    else {
                        GetMark(ch_ope);
                        Assign_oper_qua();
                        return 1;
                    }
                }
                else{
                    ch=ch_temp;
                    if(!Logical_or()){return 0;}
                    else {return 1;}
                }
            }
        }
    }
    else {
        if(!Logical_or()){return 0;}
        else {return 1;}
    }
    return 0;
}

int GrammaticalAnalysis::Logical_or(){//�߼���
    if(!Logical_and()){return 0;}
    while(ch!=NULL&&strcmp(ch->token,"85")==0){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Logical_and()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Logical_and(){//�߼���
    if(!Inclusive_or()){return 0;}
    while(ch!=NULL&&strcmp(ch->token,"84")==0){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Inclusive_or()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Inclusive_or(){//������
    if(!Exclusive_or()){return 0;}
    while(ch!=NULL&&strcmp(ch->token,"88")==0){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Exclusive_or()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Exclusive_or(){//�������
    if(!And()){return 0;}
    while(ch!=NULL&&strcmp(ch->token,"89")==0){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!And()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::And(){//������
    if(!Equality()){return 0;}
    while(ch!=NULL&&strcmp(ch->token,"87")==0){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Equality()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Equality(){//��ֵ�жϣ�==����!=
    if(!Relational()){return 0;}
    while(ch!=NULL&&(strcmp(ch->token,"82")==0||strcmp(ch->token,"83")==0)){//==,!=
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Relational()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Relational(){//��ϵ���㣬>,>=,<,<=
    if(!Shift()){return 0;}
    while(ch!=NULL&&(strcmp(ch->token,"78")==0||strcmp(ch->token,"79")==0||strcmp(ch->token,"80")==0||strcmp(ch->token,"81")==0)){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Shift()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Shift(){//��λ���㣬>>,<<
    if(!Additive()){return 0;}
    while(ch!=NULL&&(strcmp(ch->token,"91")==0||strcmp(ch->token,"92")==0)){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Additive()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Additive(){//�ӷ���������+,-
    if(!Multiplicative()){return 0;}
    while(ch!=NULL&&(strcmp(ch->token,"70")==0||strcmp(ch->token,"71")==0)){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Multiplicative()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Multiplicative(){//�˷���������ȡģ��*,/,%
    if(!Unary()){return 0;}
    while(ch!=NULL&&(strcmp(ch->token,"72")==0||strcmp(ch->token,"73")==0||strcmp(ch->token,"74")==0)){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Unary()){return 0;}
            GetMark(ch_ope);
            Bi_oper_qua();
    }
    return 1;
}

int GrammaticalAnalysis::Unary(){//ǰ��������ǣ�ȡ�����������Լ������С,!,~,INC,DEC,sizeof
    if(ch!=NULL&&(strcmp(ch->token,"86")==0||strcmp(ch->token,"90")==0||strcmp(ch->token,"75")==0||strcmp(ch->token,"76")==0||strcmp(ch->token,"40")==0)){
        TOKEN* ch_ope=ch;
        ch=ch->next;
        if(!Postfix()){return 0;}
            GetMark_front(ch_ope);
            Unary_oper_qua();
    }
    else{
        if(!Postfix()){return 0;}
    }
    return 1;
}

int GrammaticalAnalysis::Postfix(){//���������[Expression],++,--,.
    if(!Primary()){return 0;}
    while(ch!=NULL&&(strcmp(ch->token,"54")==0||strcmp(ch->token,"75")==0||strcmp(ch->token,"76")==0||strcmp(ch->token,"60")==0)){
        if(strcmp(ch->token,"54")==0){//[
            ch=ch->next;
            if(!Expression()){return 0;}//Expression()
            else{
                if(ch!=NULL&&strcmp(ch->token,"55")==0){//]
                    ch=ch->next;
                    mark=33;
                    Bi_oper_qua();
                }
                else {return 0;}
            }
        }
        else if(strcmp(ch->token,"60")==0){//.
            ch=ch->next;
            if(strcmp(ch->token,"4")==0){//Identifier
                ch_sem=ch;
                PushStack();
                mark=35;
                Bi_oper_qua();
                ch=ch->next;
            }
            else {return 0;}
        }
        else{//��Ϊ++��--��ֱ�Ӷ�ȡ��һ���ַ�
            GetMark(ch);
            Unary_oper_qua();
            ch=ch->next;
        }
    }
    return 1;
}

int GrammaticalAnalysis::Primary(){//�����ʽ,��ʶ��������������ѭ����i,constant,(Expression)
    if(ch!=NULL&&strcmp(ch->token,"4")==0){//Identifier
        ch_sem=ch;
        PushStack();
        ch=ch->next;
        return 1;
    }
    if(ch!=NULL&&strcmp(ch->token,"0")==0){//Integer
        ch_sem=ch;
        PushStack();
        ch=ch->next;
        return 1;
    }
    if(ch!=NULL&&strcmp(ch->token,"1")==0){//Real
        ch_sem=ch;
        PushStack();
        ch=ch->next;
        return 1;
    }
    if(ch!=NULL&&strcmp(ch->token,"2")==0){//Char
        ch_sem=ch;
        PushStack();
        ch=ch->next;
        return 1;
    }
    if(ch!=NULL&&strcmp(ch->token,"52")==0){//(
        ch=ch->next;
        if(!Expression()){return 0;}//Expression()
        else{
            if(ch!=NULL&&strcmp(ch->token,"53")==0){//)
                ch=ch->next;
                return 1;
            }
            else {return 0;}
        }
    }
    else {return 0;}
}


int GrammaticalAnalysis::IF(){//IF
    if(ch!=NULL&&strcmp(ch->token,"29")==0){//if
        ch=ch->next;
        if(ch!=NULL&&strcmp(ch->token,"52")==0){//(
            ch=ch->next;
            if(!Expression()){return 0;}//Expression()
            else{
                if(ch!=NULL&&strcmp(ch->token,"53")==0){//)
                    ch=ch->next;
                    mark=25;
                    If_While_qua();
                    if(ch!=NULL&&strcmp(ch->token,"50")==0){//{
                        ch=ch->next;
                        if(!Content()){return 0;}//Content()
                        else{
                            if(ch!=NULL&&strcmp(ch->token,"51")==0){//}
                                ch=ch->next;
                                if(ch!=NULL&&strcmp(ch->token,"30")==0){//else
                                    ch=ch->next;
                                    mark=26;
                                    If_While_qua();
                                    if(ch!=NULL&&strcmp(ch->token,"50")==0){//{
                                        ch=ch->next;
                                        if(!Content()){return 0;}//Content()
                                        else {
                                            if(ch!=NULL&&strcmp(ch->token,"51")==0){//}
                                                ch=ch->next;
                                                mark=27;
                                                If_While_qua();
                                                return 1;
                                            }
                                            else {return 0;}
                                        }
                                    }
                                    else {return 0;}
                                }
                                else {return 1;}//��else
                            }
                            else {return 0;}
                        }
                    }
                    else {return 0;}
                }
                else {return 0;}
            }
        }
        else {return 0;}
    }
    else {return 0;}
}

int GrammaticalAnalysis::While(){//WHILE
    if(ch!=NULL&&strcmp(ch->token,"35")==0){//while
        ch=ch->next;
        mark=28;
        If_While_qua();
        if(ch!=NULL&&strcmp(ch->token,"52")==0){//(
            ch=ch->next;
            if(!Expression()){return 0;}//Expression()
            else{
                if(ch!=NULL&&strcmp(ch->token,"53")==0){//)
                    ch=ch->next;
                    mark=29;
                    If_While_qua();
                    if(ch!=NULL&&strcmp(ch->token,"50")==0){//{
                        ch=ch->next;
                        if(!Content()){return 0;}//Content()
                        else{
                            if(ch!=NULL&&strcmp(ch->token,"51")==0){//}
                                ch=ch->next;
                                mark=30;
                                If_While_qua();
                                return 1;
                            }
                            else {return 0;}
                        }
                    }
                    else {return 0;}
                }
                else {return 0;}
            }
        }
        else {return 0;}
    }
    else {return 0;}
}

