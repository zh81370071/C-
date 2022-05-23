#include "L.h"

void ObjectCode::ObjCode(){
    int i=0;
    obj=new Object;
    obj_now=obj;
    obj_now->next=NULL;
    obj_now->fron=NULL;
    obj_now->lab=0;
    obj_now->type='0';
    for(;i<30;i++)obj_now->code[i]='\0';
    for(i=0;i<10;i++)obj_now->add[i]='\0';

    UpdateSymbol();
    GetHead();
    blo=optimize_block;
    if(blo!=NULL){
        blo=blo->next;
        for(;blo!=NULL;blo=blo->next){
            qua=blo->qua_block;
            if(qua!=NULL){
                GetObjCode();
            }
        }
    }

    GetTail();
    CoutObj();
}

void ObjectCode::UpdateSymbol(){
    Block* b=optimize_block;
    if(b!=NULL){
        b=b->next;
        for(;b!=NULL;b=b->next){
            Quaternary* q=b->qua_block;
            if(q!=NULL){
                q=q->next;
                for(;q!=NULL;q=q->next){
                    int i=0;
                    for(;i<3;i++){
                        if(q->operand[i]->tpToken!=NULL){
                            if(strcmp(q->operand[i]->tpToken->token,"99")==0){
                                find_num=q->operand[i]->tpToken->line;
                                if(!SystemExist()){
                                    id_now=id;
                                    for(;id_now->next!=NULL;id_now=id_now->next);
                                    Identifier* i_new=new Identifier;
                                    i_new->tp=new SYNBL;
                                    i_new->tp->cat='s';
                                    i_new->next=NULL;
                                    string str;
                                    ConvertItoS(find_num,str);
                                    string ss="t"+str;
                                    ConvertStoC(ss,i_new->tp->name);
                                    id_now->next=i_new;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ObjectCode::CoutObj(){//输出目标代码
    FILE* fp;
    fp=fopen("code.ASM","w");
    Object* o=obj;
    if(o!=NULL){
        for(;o!=NULL;o=o->next){
            if(o->type=='0'){
                fprintf(fp,o->code);
                fprintf(fp,"\n");
            }
        }
    }
    fclose(fp);
}

int ObjectCode::SystemExist(){//查找符号表中是否有必须的中间变量
    string st;
    ConvertItoS(find_num,st);
    string s="t"+st;
    char name[100];
    ConvertStoC(s,name);
    if(id==NULL){return 0;}
    Identifier* id_find=id->next;
    for(;id_find!=NULL;id_find=id_find->next){
        if(strcmp(name,id_find->tp->name)==0){
            return 1;
        }
    }
    return 0;
}

void ObjectCode::GetHead(){//添加头部和数据区
    string s="DSEG SEGMENT";
    ConvertStoC(s,obj_now->code);
    AddObject();
    Identifier* in=id->next;
    for(;in!=NULL;in=in->next){
        string st="";
        st=in->tp->name;
        s=st+" DW 0";
        ConvertStoC(s,obj_now->code);
        AddObject();
    }
    s="DSEG ENDS";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="CSEG SEGMENT";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="ASSUME CS:CSEG,DS:DSEG";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="START:MOV AX,DSEG";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="MOV DS,AX";
    ConvertStoC(s,obj_now->code);
    AddObject();
}

void ObjectCode::GetTail(){//添加尾部结束标志
    lab++;
    string slab="";
    ConvertItoS(lab,slab);//获取地址
    string s1="LAB"+slab+":";
    string s=s1+"MOV AH,4CH";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="MOV AL,0";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="INT 21H";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="CSEG ENDS";
    ConvertStoC(s,obj_now->code);
    AddObject();
    s="END START";
    ConvertStoC(s,obj_now->code);
    AddObject();
}

void ObjectCode::AddObject(){//加入obj结点
    Object* ob=new Object;
    int i=0;
    ob->next=NULL;
    for(;i<30;i++)ob->code[i]='\0';
    for(i=0;i<10;i++)ob->add[i]='\0';
    ob->lab=0;
    ob->type='0';
    obj_now->next=ob;
    ob->fron=obj_now;
    obj_now=ob;
}

void ObjectCode::Judge(string& st){
    if(strcmp(num->tpToken->token,"0")==0){//int
        ConvertItoS(num->tpToken->tpI->numi,st);
    }
    else if(strcmp(num->tpToken->token,"1")==0){//real
        ConvertItoS(num->tpToken->tpR->numf,st);
    }
    else if(strcmp(num->tpToken->token,"2")==0){//char
        st=num->tpToken->tpC->numc;
    }
    else if(strcmp(num->tpToken->token,"4")==0){//identifier
        st=num->tpToken->tpIT->name;
    }
    else if(strcmp(num->tpToken->token,"99")==0){//system
        string s;
        ConvertItoS(num->tpToken->line,s);
        st="t"+s;
    }
}

void ObjectCode::GetObjCode(){//获取目标代码
    qua=qua->next;
    for(;qua!=NULL;qua=qua->next){
        if(strcmp(qua->Operator,"=")==0){
            MOV_BX_A();
            MOV_C_BX();
        }
        else if(strcmp(qua->Operator,"++")==0){
            MOV_BX_A();
            MOV_C_BX();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="INC BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_A_BX();
        }
        else if(strcmp(qua->Operator,"--")==0){
            MOV_BX_A();
            MOV_C_BX();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="DEC BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_A_BX();
        }
        else if(strcmp(qua->Operator,"!")==0){
            MOV_BX_A();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="NEG BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_BX();
        }
        else if(strcmp(qua->Operator,"~")==0){
            MOV_BX_A();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="NOT BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_BX();
        }
        else if(strcmp(qua->Operator,"inc")==0){
            MOV_BX_A();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="INC BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_A_BX();
            MOV_C_BX();
        }
        else if(strcmp(qua->Operator,"dec")==0){
            MOV_BX_A();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="DEC BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_A_BX();
            MOV_C_BX();
        }
        else if(strcmp(qua->Operator,"+")==0){
            MOV_AX_A();
            MOV_BX_B();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="ADD AX,BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"-")==0){
            MOV_AX_A();
            MOV_BX_B();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="SUB AX,BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"*")==0){
            MOV_AX_A();
            MOV_BX_B();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="IMUL BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"/")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_DX_ZERO();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="IDIV BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"%")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_DX_ZERO();
            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="IDIV BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_C_DX();
        }
        else if(strcmp(qua->Operator,">")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_CX_ONE();
            CMP_AX_BX();
            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JG LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,">=")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_CX_ONE();
            CMP_AX_BX();
            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JGE LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"<")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_CX_ONE();
            CMP_AX_BX();
            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JL LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"<=")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_CX_ONE();
            CMP_AX_BX();
            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JLE LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"==")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_CX_ONE();
            CMP_AX_BX();
            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JE LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"!=")==0){
            MOV_AX_A();
            MOV_BX_B();
            MOV_CX_ONE();
            CMP_AX_BX();
            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JNE LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"&&")==0){
            MOV_AX_A();
            MOV_CX_ZERO();
            CMP_AX_ZERO();

            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+5;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JZ LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_BX_B();
            CMP_BX_ZERO();

            lab++;
            slab="";
            slab_jmp="";
            lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            s="";
            s1="LAB"+slab+":";
            s2="JZ LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_CX_ONE();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"||")==0){
            MOV_AX_A();
            MOV_CX_ONE();
            CMP_AX_ZERO();

            lab++;
            string slab;
            string slab_jmp;
            int lab_jmp=lab+5;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JNZ LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_BX_B();
            CMP_BX_ZERO();

            lab++;
            slab="";
            slab_jmp="";
            lab_jmp=lab+2;
            ConvertItoS(lab,slab);//获取地址
            ConvertItoS(lab_jmp,slab_jmp);//获取跳转地址
            s="";
            s1="LAB"+slab+":";
            s2="JNZ LAB"+slab_jmp;
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_CX_ZERO();
            MOV_C_CX();
        }
        else if(strcmp(qua->Operator,"&")==0){
            MOV_AX_A();
            MOV_BX_B();

            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="AND AX,BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"|")==0){
            MOV_AX_A();
            MOV_BX_B();

            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="OR AX,BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"^")==0){
            MOV_AX_A();
            MOV_BX_B();

            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="XOR AX,BX";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,">>")==0){
            MOV_AX_A();
            MOV_CX_B();

            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="SAR AX,CL";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"<<")==0){
            MOV_AX_A();
            MOV_CX_B();

            lab++;
            string slab;
            ConvertItoS(lab,slab);//获取地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="SAL AX,CL";
            s=s1+s2;
            obj_now->lab=lab;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            MOV_C_AX();
        }
        else if(strcmp(qua->Operator,"if")==0){
            MOV_BX_A();
            CMP_BX_ZERO();
            lab++;
            obj_now->lab=lab;
            obj_now->type='i';//if跳转位置待回填
            AddObject();//创建新结点

        }
        else if(strcmp(qua->Operator,"el")==0){
            lab++;
            obj_now->lab=lab;
            obj_now->type='e';//else跳转位置待回填
            AddObject();//创建新结点

            type_find='i';
            int lab_jmp=lab+1;
            string slab_jmp;
            ConvertItoS(lab_jmp,slab_jmp);//获取if要跳转的地址
            Find_obj();//找到if

            string slab;
            ConvertItoS(obj_find->lab,slab);//获取if语句的地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JZ LAB"+slab_jmp;
            s=s1+s2;

            obj_find->type='0';//取消标记
            ConvertStoC(s,obj_find->code);
        }
        else if(strcmp(qua->Operator,"ie")==0){
            type_find='e';
            int lab_jmp=lab+1;
            string slab_jmp;
            ConvertItoS(lab_jmp,slab_jmp);//获取else要跳转的地址
            Find_obj();//找到else

            string slab;
            ConvertItoS(obj_find->lab,slab);//获取else语句的地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JMP LAB"+slab_jmp;
            s=s1+s2;

            obj_find->type='0';//取消标记
            ConvertStoC(s,obj_find->code);
        }
        else if(strcmp(qua->Operator,"wh")==0){
            obj_now->lab=lab+1;
            obj_now->type='w';//while开始位置，记录，但是不产生代码
            AddObject();//创建新结点
        }
        else if(strcmp(qua->Operator,"do")==0){
            MOV_BX_A();
            CMP_BX_ZERO();
            lab++;
            obj_now->lab=lab;
            obj_now->type='d';//do跳转位置待回填
            AddObject();//创建新结点

        }
        else if(strcmp(qua->Operator,"we")==0){
            //回填do语句
            type_find='d';
            int lab_jmp=lab+2;
            string slab_jmp;
            ConvertItoS(lab_jmp,slab_jmp);//获取do要跳转的地址
            Find_obj();//找到do

            string slab;
            ConvertItoS(obj_find->lab,slab);//获取do语句的地址
            string s="";
            string s1="LAB"+slab+":";
            string s2="JZ LAB"+slab_jmp;
            s=s1+s2;

            obj_find->type='0';//取消标记
            ConvertStoC(s,obj_find->code);

            //填写自己的跳转位置
            lab++;
            obj_now->lab=lab;
            obj_now->type='0';
            slab="";
            ConvertItoS(lab,slab);//记录自己的位置
            type_find='w';
            Find_obj();//找到while
            obj_find->type='0';//取消标记
            slab_jmp="";
            ConvertItoS(obj_find->lab,slab_jmp);//获取while语句的地址
            s="";
            s1="LAB"+slab+":";
            s2="JMP LAB"+slab_jmp;
            s=s1+s2;
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点
        }
              else if(strcmp(qua->Operator,"co")==0){
            MOV_AX_A();

            lab++;
            string slab;
            ConvertItoS(lab,slab);
            string s1="LAB"+slab+":";
            string s=s1+"AAA";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"OR AX,3030H";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"MOV BX,AX";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);//获取地址
            s="";
            s1="LAB"+slab+":";
            s=s1+"MOV DL,BH";
            ConvertStoC(s,obj_now->code);
            AddObject();//创建新结点

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"MOV AH,02H";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"INT 21H";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"MOV DL,BL";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"INT 21H";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"MOV DL,10";
            ConvertStoC(s,obj_now->code);
            AddObject();

            lab++;
            ConvertItoS(lab,slab);
            s1="LAB"+slab+":";
            s=s1+"INT 21H";
            ConvertStoC(s,obj_now->code);
            AddObject();
        }
        else if(strcmp(qua->Operator,"arr")==0){;}
        else if(strcmp(qua->Operator,".")==0){;}
    }
}

int ObjectCode::Find_obj(){//从后向前寻找相应类型的首个目标代码语句
    Object* ob=obj_now;
    for(;ob!=NULL;ob=ob->fron){
        if(type_find==ob->type){
            obj_find=ob;
            return 1;
        }
    }
    return 0;
}

int ObjectCode::Find_obj_head(){//从前向后寻找相应类型的首个目标代码语句
    Object* ob=obj;
    for(;ob!=NULL;ob=ob->next){
        if(type_find==ob->type){
            obj_find=ob;
            return 1;
        }
    }
    return 0;
}


void ObjectCode::MOV_BX_A(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV BX,";
    string s3="";
    num=qua->operand[0];
    Judge(s3);
    s=s1+s2+s3;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_BX_B(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV BX,";
    string s3="";
    num=qua->operand[1];
    Judge(s3);
    s=s1+s2+s3;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_CX_B(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV CX,";
    string s3="";
    num=qua->operand[1];
    Judge(s3);
    s=s1+s2+s3;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_C_BX(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="";
    num=qua->operand[2];
    Judge(s2);
    s=s1+"MOV "+s2+",BX";
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_C_CX(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="";
    num=qua->operand[2];
    Judge(s2);
    s=s1+"MOV "+s2+",CX";
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_A_BX(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="";
    num=qua->operand[0];
    Judge(s2);
    s=s1+"MOV "+s2+",BX";
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_AX_A(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV AX,";
    string s3="";
    num=qua->operand[0];
    Judge(s3);
    s=s1+s2+s3;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_C_AX(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="";
    num=qua->operand[2];
    Judge(s2);
    s=s1+"MOV "+s2+",AX";
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_DX_ZERO(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV DX,0";
    s=s1+s2;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_C_DX(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="";
    num=qua->operand[2];
    Judge(s2);
    s=s1+"MOV "+s2+",DX";
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_CX_ONE(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV CX,1";
    s=s1+s2;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::MOV_CX_ZERO(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="MOV CX,0";
    s=s1+s2;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::CMP_AX_BX(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="CMP AX,BX";
    s=s1+s2;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::CMP_AX_ZERO(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="CMP AX,0";
    s=s1+s2;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

void ObjectCode::CMP_BX_ZERO(){
    lab++;
    string slab;
    ConvertItoS(lab,slab);//获取地址
    string s="";
    string s1="LAB"+slab+":";
    string s2="CMP BX,0";
    s=s1+s2;
    obj_now->lab=lab;
    ConvertStoC(s,obj_now->code);
    AddObject();//创建新结点
}

