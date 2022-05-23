#include "L.h"
#include "EQU.h"

void Optimization::Optimize(){
    qua=quater->next;
    DivideBlock();//划分基本块
    CoutBlock();//按基本块输出
    optimize_block=new Block;
    optimize_block_now=optimize_block;
    optimize_block_now->next=NULL;
    optimize_block_now->qua_block=NULL;

    block_now=block->next;//导入第一个基本块
    while(block_now!=NULL){
        CreateDAG();//生成DAG图
        CreateOptimize_Block();//生成一个优化基本块
        CreateOptimize();//生成一个优化四元式队列
        GetDAG();//获取该DAG的四元式
        optimize_block_now->qua_block=optimize;//将该优化四元式队列放入优化基本块内
        DeleteDAG();//释放DAG空间
        block_now=block_now->next;//读下一个基本块
    }
    CoutOptimize();//输出优化后的四元式
}

void Optimization::CreateOptimize_Block(){//创建一个优化后块头部
    Block* b=new Block;
    b->next=NULL;
    b->qua_block=NULL;
    optimize_block_now->next=b;
    optimize_block_now=b;
}

void Optimization::CoutOptimize(){//按基本块输出优化后的四元式
    Block* b=optimize_block;
    cout<<"DAG局部优化后四元式:"<<endl;
    int j=1;
    while(b->next!=NULL){//输出块
        b=b->next;
        Quaternary* p=b->qua_block;
        cout<<"块"<<j<<":"<<endl;
        while(p->next!=NULL){//输出块中的全部四元式
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
        j++;
    }
}

void Optimization::DeleteDAG(){//删除DAG
    dag_now=dag_head;
    while(dag_now!=NULL){
        DAG* d=dag_now;
        for(;dag_now->mark!=NULL;){
            Mark* m=dag_now->mark;
            dag_now->mark=dag_now->mark->next;
            delete(m);
        }
        dag_now=dag_now->next;
        delete(d);
    }
}

void Optimization::CreateOptimize(){//创建一个优化后四元式头部
    optimize=new Quaternary;
    optimize_now=optimize;
    optimize->next=NULL;
    int i=0;
    for(;i<3;i++)optimize->operand[i]=NULL;
    optimize->Operator=NULL;
}

void Optimization::DivideBlock(){//划分块
    block=new Block;//新建一个块
    block->next=NULL;
    block->qua_block=NULL;
    block_now=block;
    Quaternary* p=qua;
    Quaternary* before=NULL;
    while(p!=NULL){
        if(strcmp(p->Operator,"if")==0||strcmp(p->Operator,"el")==0||strcmp(p->Operator,"wh")==0||
            strcmp(p->Operator,"do")==0||strcmp(p->Operator,"we")==0||strcmp(p->Operator,"gt")==0){
            Block* b=new Block;//新建一个块
            b->next=NULL;
            b->qua_block=qua;
            block_now->next=b;
            block_now=b;
            before=p;
            Quaternary* q=p;
            p=p->next;
            q->next=NULL;
            qua=p;
        }
        else if(strcmp(p->Operator,"ie")==0||strcmp(p->Operator,"lb")==0){
            Block* b=new Block;//新建一个块
            b->next=NULL;
            b->qua_block=qua;
            block_now->next=b;
            block_now=b;
            before->next=NULL;
            qua=p;
            before=p;
            p=p->next;
        }
        else{
            before=p;
            p=p->next;
        }
    }
    if(qua!=NULL){
        Block* b=new Block;//新建一个块
        b->next=NULL;
        b->qua_block=qua;
        block_now->next=b;
        block_now=b;
    }
}

void Optimization::CoutBlock(){//按基本块输出
    block_now=block->next;
    int i=1;
    while(block_now!=NULL){
        cout<<"基本块"<<i<<":"<<endl;
        Quaternary* p=block_now->qua_block;
        while(p!=NULL){
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
            p=p->next;
        }
        i++;
        block_now=block_now->next;
    }
}

void Optimization::CreateDAG(){//创建一个DAG
    dag_head=new DAG;//生成DAG头结点
    dag_now=dag_tail=dag_head;
    dag_now->fron=dag_now->next=NULL;
    dag_now->lchild=dag_now->rchild=NULL;
    dag_now->mark=NULL;
    dag_now->oper=NULL;

    for(;block_now->qua_block!=NULL;block_now->qua_block=block_now->qua_block->next){
        Quaternary* p=block_now->qua_block;
        if(strcmp(p->Operator,"gt")==0||strcmp(p->Operator,"lb")==0||
           strcmp(p->Operator,"if")==0||strcmp(p->Operator,"el")==0||
           strcmp(p->Operator,"ie")==0||strcmp(p->Operator,"wh")==0||
           strcmp(p->Operator,"do")==0||strcmp(p->Operator,"we")==0||
           strcmp(p->Operator,"co")==0){
            if(strcmp(p->Operator,"if")==0||strcmp(p->Operator,"do")==0){//if,do
                find_now_num1=p->operand[0];
                find_begin=dag_tail;
                if(!FindDAG()){
                    op_now=NULL;
                    add_now=p->operand[0];
                    AddDAG();
                }
                DAG* temp=pos;
                op_now=p->Operator;
                add_now=p->operand[0];
                AddDAG();
                pos->lchild=temp;

            }
            else if(strcmp(p->Operator,"co")==0){
                op_now=p->Operator;
                add_now=p->operand[0];
                AddDAG();
            }
            else{//其他
                op_now=p->Operator;
                add_now=p->operand[0];
                AddDAG();
            }
        }
        else if(strcmp(p->Operator,"=")==0){//赋值，(=,B,_,A)
            find_now_num1=p->operand[0];
            find_begin=dag_tail;
            if(!FindDAG()){
                op_now=NULL;
                add_now=p->operand[0];
                AddDAG();
            }
            Mark* m=pos->mark;//记录B结点的位置
            for(;m->next!=NULL;m=m->next);
            Mark* m_new=new Mark;
            m_new->fron=m;
            m_new->next=NULL;
            m->next=m_new;
            m_new->name=p->operand[2]->tpToken;//将A信息写入
            mark_pos=pos->mark;//B结点的主标记位置
            if(!(strcmp(m_new->name->token,"99")==0)){
                if(MainMark()){
                    temp_dag=pos;
                    temp1=pos->mark;
                    temp2=m_new;
                    SwopMark();//交换
                }
            }
            find_now_num1=p->operand[2];//查找之前A是否被定义
            find_begin=pos->fron;
            while(find_begin!=NULL){
                if(!FindDAG()){//未找到
                    find_begin=NULL;
                }
                else{//找到
                    find_begin=pos->fron;
                    if(mark_pos->fron==NULL);
                    else{
                        temp1=mark_pos;
                        DeleteMark();
                    }
                }
            }
        }
        else if(strcmp(p->Operator,"++")==0||strcmp(p->Operator,"--")==0||
                strcmp(p->Operator,"!")==0||strcmp(p->Operator,"~")==0||
                strcmp(p->Operator,"inc")==0||strcmp(p->Operator,"dec")==0){//单目运算符,(w,B,_,A)

            find_now_num1=p->operand[0];//查找词
            find_now_ope=p->Operator;//查找运算符
            if(!FindDAG_unary()){//未找到，则新建一组单目运算结点
                find_now_num1=p->operand[0];
                find_begin=dag_tail;//查找开始处
                if(!FindDAG()){//B没被建立结点
                    op_now=NULL;
                    add_now=p->operand[0];
                    AddDAG();//建立B
                }
                DAG* temp=pos;//记录B的位置
                op_now=p->Operator;
                add_now=p->operand[2];
                AddDAG();//建立A
                pos->lchild=temp;//将B连接为A的左孩子
            }
            else{//找到了，则返回运算符所在结点的位置
                Mark* m=pos->mark;
                for(;m->next!=NULL;m=m->next);//移动到a所在结点处的附加标记的最后一个
                Mark* m_new=new Mark;
                m_new->fron=m;
                m_new->next=NULL;
                m->next=m_new;
                m_new->name=p->operand[2]->tpToken;//将A信息写入
                mark_pos=pos->mark;
                if(!(strcmp(m_new->name->token,"99")==0)){//该标记不是系统生成
                    if(MainMark()){//pos位置的主标记是系统生成变量
                        temp_dag=pos;
                        temp1=pos->mark;
                        temp2=m_new;
                        SwopMark();
                    }
                }
                find_now_num1=p->operand[2];//查找之前A是否被定义
                find_begin=pos->fron;
                while(find_begin!=NULL){
                    if(!FindDAG()){//未找到
                        find_begin=NULL;
                    }
                    else{//找到
                        find_begin=pos->fron;
                        if(mark_pos->fron==NULL);
                        else{
                            temp1=mark_pos;
                            DeleteMark();
                        }
                    }
                }
            }
        }
        else {//双目运算符,(w,B,C,A)
            find_now_num1=p->operand[0];//查找词1
            find_now_num2=p->operand[1];//查找词2
            find_now_ope=p->Operator;//查找运算符
            if((!strcmp(find_now_num1->tpToken->token,"4")==0)&&(!strcmp(find_now_num1->tpToken->token,"99")==0)&&
               (!strcmp(find_now_num2->tpToken->token,"4")==0)&&(!strcmp(find_now_num2->tpToken->token,"99")==0)){//两个操作数都可计算
                double n1,n2,sum;
                int grade=0;
                if(strcmp(find_now_num1->tpToken->token,"0")==0){n1=(double)find_now_num1->tpToken->tpI->numi;grade=1;}
                else if(strcmp(find_now_num1->tpToken->token,"1")==0){n1=(double)find_now_num1->tpToken->tpR->numf;grade=2;}
                else if(strcmp(find_now_num1->tpToken->token,"2")==0){n1=(double)find_now_num1->tpToken->tpC->numc;}
                if(strcmp(find_now_num2->tpToken->token,"0")==0){n2=(double)find_now_num2->tpToken->tpI->numi;grade=1;}
                else if(strcmp(find_now_num2->tpToken->token,"1")==0){n2=(double)find_now_num2->tpToken->tpR->numf;grade=2;}
                else if(strcmp(find_now_num2->tpToken->token,"2")==0){n2=(double)find_now_num2->tpToken->tpC->numc;}
                if(strcmp(find_now_ope,"+")==0){sum=n1+n2;}
                else if(strcmp(find_now_ope,"-")==0){sum=n1-n2;}
                else if(strcmp(find_now_ope,"*")==0){sum=n1*n2;}
                else if(strcmp(find_now_ope,"/")==0){sum=n1/n2;}
                else if(strcmp(find_now_ope,">")==0){sum=n1>n2;}
                else if(strcmp(find_now_ope,">=")==0){sum=n1>=n2;}
                else if(strcmp(find_now_ope,"<")==0){sum=n1<n2;}
                else if(strcmp(find_now_ope,"<=")==0){sum=n1<=n2;}
                else if(strcmp(find_now_ope,"==")==0){sum=n1==n2;}
                else if(strcmp(find_now_ope,"!=")==0){sum=n1!=n2;}
                else if(strcmp(find_now_ope,"&&")==0){sum=n1&&n2;}
                else if(strcmp(find_now_ope,"||")==0){sum=n1||n2;}
                if(grade==1){//整型
                    INT* in=new INT;
                    in->numi=(int)sum;
                    p->operand[0]->tpToken->token[0]='0';
                    p->operand[0]->tpToken->token[1]='\0';
                    p->operand[0]->tpToken->tpI=in;
                }
                else if(grade==2){//实型
                    REAL* re=new REAL;
                    re->numf=(float)sum;
                    p->operand[0]->tpToken->token[0]='1';
                    p->operand[0]->tpToken->token[1]='\0';
                    p->operand[0]->tpToken->tpR=re;
                }
                find_now_num1=p->operand[0];//当前查找数
                find_begin=dag_tail;//查找开始处
                if(!FindDAG()){//未查到
                    op_now=NULL;
                    add_now=p->operand[0];
                    AddDAG();//建立B
                }

                Mark* m=pos->mark;//记录B结点的位置
                for(;m->next!=NULL;m=m->next);//移动到a所在结点处的附加标记的最后一个
                Mark* m_new=new Mark;
                m_new->fron=m;
                m_new->next=NULL;
                m->next=m_new;
                m_new->name=p->operand[2]->tpToken;//将A信息写入
                mark_pos=pos->mark;//B结点的主标记位置
                if(!(strcmp(m_new->name->token,"99")==0)){//该标记不是系统生成
                    if(MainMark()){//pos位置的主标记是系统生成变量
                        temp_dag=pos;
                        temp1=pos->mark;
                        temp2=m_new;
                        SwopMark();
                    }
                }
                find_now_num1=p->operand[2];//查找之前A是否被定义
                find_begin=pos->fron;
                while(find_begin!=NULL){
                    if(!FindDAG()){//未找到
                        find_begin=NULL;
                    }
                    else{//找到
                        find_begin=pos->fron;
                        if(mark_pos->fron==NULL);
                        else{
                            temp1=mark_pos;
                            DeleteMark();
                        }
                    }
                }
            }
            else{//操作数中含不可计算量
                if(!FindDAG_Bi()){//未找到，则新建一组双目运算结点
                    //分别看看B,C有没有，有直接用，没有再建立结点
                    find_now_num1=p->operand[0];
                    find_begin=dag_tail;//查找开始处
                    if(!FindDAG()){//查找B结点
                        op_now=NULL;
                        add_now=p->operand[0];
                        AddDAG();//建立B
                    }
                    DAG* temp1=pos;//记录B的位置
                    find_now_num1=p->operand[1];
                    find_begin=dag_tail;//查找开始处
                    if(!FindDAG()){//查找C结点
                        op_now=NULL;
                        add_now=p->operand[1];
                        AddDAG();//建立C
                    }
                    DAG* temp2=pos;//记录C的位置
                    op_now=p->Operator;
                    add_now=p->operand[2];
                    AddDAG();//建立A
                    pos->lchild=temp1;//连接结点A,B,C
                    pos->rchild=temp2;
                }
                else{//找到了，则返回运算符所在结点的位置
                    Mark* m=pos->mark;
                    for(;m->next!=NULL;m=m->next);//移动到a所在结点处的附加标记的最后一个
                    Mark* m_new=new Mark;
                    m_new->fron=m;
                    m_new->next=NULL;
                    m->next=m_new;
                    m_new->name=p->operand[2]->tpToken;//将A信息写入
                    mark_pos=pos->mark;
                    if(!(strcmp(m_new->name->token,"99")==0)){//该标记不是系统生成
                        if(MainMark()){//pos位置的主标记是系统生成变量
                            temp_dag=pos;
                            temp1=pos->mark;
                            temp2=m_new;
                            SwopMark();
                        }
                    }
                    find_now_num1=p->operand[2];//查找之前A是否被定义
                    find_begin=pos->fron;
                    while(find_begin!=NULL){
                        if(!FindDAG()){//未找到
                            find_begin=NULL;
                        }
                        else{//找到
                            find_begin=pos->fron;
                            if(mark_pos->fron==NULL);
                            else{
                                temp1=mark_pos;
                                DeleteMark();
                            }
                        }
                    }
                }
            }
        }
    }
}

int Optimization::FindDAG_unary(){//检查单目运算符
    dag_now=dag_tail;//从后往前找
    if(dag_now==NULL){return 0;}//若为空则未找到
    for(;dag_now!=NULL;dag_now=dag_now->fron){//逆序逐个查找
        if(dag_now->oper!=NULL){//该点有运算符，则检查运算符是否相同
            if(strcmp(dag_now->oper,find_now_ope)==0){//若找到该单目运算符,则检查操作数是否一致
                pos=dag_now;//记录该运算符的结点位置
                DAG* d=dag_now->lchild;//检查该运算符的左孩子
                if(d!=NULL&&d->mark!=NULL){
                    Mark* m=d->mark;//当前标记
                    for(;m!=NULL;m=m->next){//查找标记是否和当前查找词相同
                        if(strcmp(m->name->token,find_now_num1->tpToken->token)==0){//判断其类型相同否，相同则检查数值
                            int lab=0;
                            if(strcmp(find_now_num1->tpToken->token,"0")==0){
                                if(m->name->tpI->numi==find_now_num1->tpToken->tpI->numi){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"1")==0){
                                if(m->name->tpR->numf==find_now_num1->tpToken->tpR->numf){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"2")==0){
                                if(m->name->tpC->numc==find_now_num1->tpToken->tpC->numc){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"4")==0){
                                if(strcmp(m->name->tpIT->name,find_now_num1->tpToken->tpIT->name)==0){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"99")==0){
                                if(m->name->line==find_now_num1->tpToken->line){lab=1;}
                            }
                            if(lab==1){
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int Optimization::FindDAG_Bi(){//检查双目运算符
    dag_now=dag_tail;//从后往前找
    if(dag_now==NULL){return 0;}//若为空则未找到
    for(;dag_now!=NULL;dag_now=dag_now->fron){//逆序逐个查找
        if(dag_now->oper!=NULL){//该点有运算符，则检查运算符是否相同
            if(strcmp(dag_now->oper,find_now_ope)==0){//若找到该双目运算符,则检查操作数是否一致
                pos=dag_now;//记录该运算符的结点位置
                DAG* d=dag_now->lchild;//检查该运算符的左孩子
                if(d!=NULL&&d->mark!=NULL){//左孩子存在并且有标记
                    Mark* m=d->mark;//当前标记
                    for(;m!=NULL;m=m->next){//查找标记是否和当前查找词相同
                        if(strcmp(m->name->token,find_now_num1->tpToken->token)==0){//判断其类型相同否，相同则检查数值
                            int lab=0;
                            if(strcmp(find_now_num1->tpToken->token,"0")==0){
                                if(m->name->tpI->numi==find_now_num1->tpToken->tpI->numi){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"1")==0){
                                if(m->name->tpR->numf==find_now_num1->tpToken->tpR->numf){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"2")==0){
                                if(m->name->tpC->numc==find_now_num1->tpToken->tpC->numc){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"4")==0){
                                if(strcmp(m->name->tpIT->name,find_now_num1->tpToken->tpIT->name)==0){lab=1;}
                            }
                            else if(strcmp(find_now_num1->tpToken->token,"99")==0){
                                if(m->name->line==find_now_num1->tpToken->line){lab=1;}
                            }
                            if(lab==1){//若相同则检查运算符右孩子
                                DAG* r=dag_now->rchild;
                                if(r!=NULL&&r->mark!=NULL){
                                    Mark* m=r->mark;//当前标记
                                    for(;m!=NULL;m=m->next){//查找标记是否和当前查找词相同
                                        if(strcmp(m->name->token,find_now_num2->tpToken->token)==0){
                                            int lab=0;
                                            if(strcmp(find_now_num2->tpToken->token,"0")==0){
                                                if(m->name->tpI->numi==find_now_num2->tpToken->tpI->numi){lab=1;}
                                            }
                                            else if(strcmp(find_now_num2->tpToken->token,"1")==0){
                                                if(m->name->tpR->numf==find_now_num2->tpToken->tpR->numf){lab=1;}
                                            }
                                            else if(strcmp(find_now_num2->tpToken->token,"2")==0){
                                                if(m->name->tpC->numc==find_now_num2->tpToken->tpC->numc){lab=1;}
                                            }
                                            else if(strcmp(find_now_num2->tpToken->token,"4")==0){
                                                if(strcmp(m->name->tpIT->name,find_now_num2->tpToken->tpIT->name)==0){lab=1;}
                                            }
                                            else if(strcmp(find_now_num2->tpToken->token,"99")==0){
                                                if(m->name->line==find_now_num2->tpToken->line){lab=1;}
                                            }
                                            if(lab==1){//若相同则返回1，并记录运算符所在结点位置，记录在pos中
                                                return 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    return 0;
}

void Optimization::AddDAG(){//生成一个DAG结点,并返回位置到pos
    DAG* p=new DAG;
    p->lchild=NULL;
    p->rchild=NULL;
    p->next=NULL;
    p->fron=dag_tail;

    p->mark=new Mark;
    p->mark->fron=NULL;
    p->mark->next=NULL;

    p->oper=op_now;//添加运算符信息
    p->mark->name=add_now->tpToken;//添加主标记信息

    dag_tail->next=p;
    dag_tail=p;

    pos=p;//返回该结点位置
}

int Optimization::FindDAG(){//检查一个结点是否存在,若查到则返回该结点位置和标记位置
    dag_now=find_begin;
    if(dag_now==NULL){return 0;}
    for(;dag_now!=NULL;dag_now=dag_now->fron){
        if(dag_now->mark==NULL){return 0;}
        Mark* m=dag_now->mark;
        if(strcmp(find_now_num1->tpToken->token,"0")==0){
            while(m!=NULL){
                if(m->name!=NULL){
                    if(strcmp(m->name->token,"0")==0){
                        if(m->name->tpI->numi==find_now_num1->tpToken->tpI->numi){
                            mark_pos=m;
                            pos=dag_now;
                            return 1;
                        }
                    }
                }
                m=m->next;
            }
        }
        else if(strcmp(find_now_num1->tpToken->token,"1")==0){
            while(m!=NULL){
                if(strcmp(m->name->token,"1")==0){
                    if(m->name->tpR->numf==find_now_num1->tpToken->tpR->numf){
                        mark_pos=m;
                        pos=dag_now;
                        return 1;
                    }
                }
                m=m->next;
            }
        }
        else if(strcmp(find_now_num1->tpToken->token,"2")==0){
            while(m!=NULL){
                if(strcmp(m->name->token,"2")==0){
                    if(m->name->tpC->numc==find_now_num1->tpToken->tpC->numc){
                        mark_pos=m;
                        pos=dag_now;
                        return 1;
                    }
                }
                m=m->next;
            }
        }
        else if(strcmp(find_now_num1->tpToken->token,"4")==0){
            while(m!=NULL){
                if(m->name!=NULL){
                    if(strcmp(m->name->token,"4")==0){
                        if(strcmp(m->name->tpIT->name,find_now_num1->tpToken->tpIT->name)==0){
                            mark_pos=m;
                            pos=dag_now;
                            return 1;
                        }
                    }
                }
                m=m->next;
            }
        }
        else if(strcmp(find_now_num1->tpToken->token,"99")==0){
            while(m!=NULL){
                if(strcmp(m->name->token,"99")==0){
                    if(m->name->line==find_now_num1->tpToken->line){
                        mark_pos=m;
                        pos=dag_now;
                        return 1;
                    }
                }
                m=m->next;
            }
        }
    }
    return 0;
}

int Optimization::MainMark(){//检查mark_pos标记是不是系统分配的中间变量
    Mark* m=mark_pos;
    if(strcmp(m->name->token,"99")==0){//若是则返回1，否则返回0
        return 1;
    }
    else{
        return 0;
    }
}

void Optimization::SwopMark(){//交换标记
    Mark* t2_f=temp2->fron;
    Mark* t2_n=temp2->next;

    temp_dag->mark=temp2;
    temp2->fron=NULL;

    t2_f->next=t2_n;
    if(t2_n!=NULL){t2_n->fron=t2_f;}
    temp2->next=temp1;
    temp1->fron=temp2;

}

void Optimization::DeleteMark(){//删除标记
    Mark* p=temp1->fron;
    Mark* q=temp1->next;
    p->next=q;
    if(q!=NULL){q->fron=p;}
    q=temp1;
    temp1=p;
    delete(q);
}

void Optimization::GetDAG(){//从DAG中获取优化后四元式
    dag_now=dag_head->next;
    while(dag_now!=NULL){
        if(dag_now->lchild==NULL&&dag_now->rchild==NULL){//叶节点
            if(dag_now->oper!=NULL){//该结点有运算符
                if(strcmp(dag_now->oper,"el")==0||strcmp(dag_now->oper,"wh")==0||strcmp(dag_now->oper,"we")==0||strcmp(dag_now->oper,"ie")==0){//是特殊语句运算符
                    Operand* op1=new Operand;
                    Operand* op2=new Operand;
                    Operand* op3=new Operand;
                    op1->tpToken=NULL;
                    op2->tpToken=NULL;
                    op3->tpToken=NULL;
                    optimize_now->next=new Quaternary;
                    optimize_now=optimize_now->next;
                    optimize_now->next=NULL;
                    optimize_now->operand[0]=op1;
                    optimize_now->operand[1]=op2;
                    optimize_now->operand[2]=op3;
                    optimize_now->Operator=dag_now->oper;
                }
                else if(strcmp(dag_now->oper,"co")==0){
                    Operand* op1=new Operand;
                    Operand* op2=new Operand;
                    Operand* op3=new Operand;
                    op1->tpToken=dag_now->mark->name;
                    op2->tpToken=NULL;
                    op3->tpToken=NULL;
                    optimize_now->next=new Quaternary;
                    optimize_now=optimize_now->next;
                    optimize_now->next=NULL;
                    optimize_now->operand[0]=op1;
                    optimize_now->operand[1]=op2;
                    optimize_now->operand[2]=op3;
                    optimize_now->Operator=dag_now->oper;
                }
            }
            else{//该结点没有运算符
                Mark* m=dag_now->mark;
                if(m!=NULL){
                    m=m->next;
                    for(;m!=NULL;m=m->next){
                        mark_pos=m;
                        if(!MainMark()){
                            Operand* op1=new Operand;
                            Operand* op2=new Operand;
                            Operand* op3=new Operand;
                            op1->tpToken=dag_now->mark->name;
                            op2->tpToken=NULL;
                            op3->tpToken=m->name;
                            optimize_now->next=new Quaternary;
                            optimize_now=optimize_now->next;
                            optimize_now->next=NULL;
                            optimize_now->operand[0]=op1;
                            optimize_now->operand[1]=op2;
                            optimize_now->operand[2]=op3;
                            optimize_now->Operator=equ;
                        }
                    }
                }
            }
        }
        else if(dag_now->lchild!=NULL){//非叶节点
            if(strcmp(dag_now->oper,"if")==0||strcmp(dag_now->oper,"do")==0){
                DAG* m=dag_now->lchild;
                if(m!=NULL&&m->lchild==NULL){
                    Operand* op1=new Operand;
                    Operand* op2=new Operand;
                    Operand* op3=new Operand;
                    op1->tpToken=m->mark->name;
                    op2->tpToken=NULL;
                    op3->tpToken=m->mark->next->name;
                    optimize_now->next=new Quaternary;
                    optimize_now=optimize_now->next;
                    optimize_now->next=NULL;
                    optimize_now->operand[0]=op1;
                    optimize_now->operand[1]=op2;
                    optimize_now->operand[2]=op3;
                    optimize_now->Operator=equ;
                }
                Operand* op1=new Operand;
                Operand* op2=new Operand;
                Operand* op3=new Operand;
                op1->tpToken=dag_now->mark->name;
                op2->tpToken=NULL;
                op3->tpToken=NULL;
                optimize_now->next=new Quaternary;
                optimize_now=optimize_now->next;
                optimize_now->next=NULL;
                optimize_now->operand[0]=op1;
                optimize_now->operand[1]=op2;
                optimize_now->operand[2]=op3;
                optimize_now->Operator=dag_now->oper;
            }
            else if(dag_now->rchild==NULL){//单目运算
                Mark* m=dag_now->mark;
                Mark* lm=dag_now->lchild->mark;
                if(m!=NULL){
                    Operand* op1=new Operand;
                    Operand* op2=new Operand;
                    Operand* op3=new Operand;
                    if(strcmp(dag_now->oper,"++")==0||strcmp(dag_now->oper,"--")==0||strcmp(dag_now->oper,"inc")==0
                       ||strcmp(dag_now->oper,"dec")==0||strcmp(dag_now->oper,"!")==0||strcmp(dag_now->oper,"~")==0){
                        while(!strcmp(lm->name->token,"4")==0){
                            lm=lm->next;
                        }
                    }
                    op1->tpToken=lm->name;
                    op2->tpToken=NULL;
                    op3->tpToken=m->name;
                    optimize_now->next=new Quaternary;
                    optimize_now=optimize_now->next;
                    optimize_now->next=NULL;
                    optimize_now->operand[0]=op1;
                    optimize_now->operand[1]=op2;
                    optimize_now->operand[2]=op3;
                    optimize_now->Operator=dag_now->oper;
                }
                m=m->next;
                for(;m!=NULL;m=m->next){
                    mark_pos=m;
                    if(!MainMark()){
                        Operand* op1=new Operand;
                        Operand* op2=new Operand;
                        Operand* op3=new Operand;
                        if(strcmp(dag_now->oper,"++")==0||strcmp(dag_now->oper,"--")==0||strcmp(dag_now->oper,"inc")==0
                            ||strcmp(dag_now->oper,"dec")==0||strcmp(dag_now->oper,"!")==0||strcmp(dag_now->oper,"~")==0){
                            while(!strcmp(lm->name->token,"4")==0){
                                lm=lm->next;
                            }
                        }
                        op1->tpToken=dag_now->mark->name;
                        op2->tpToken=NULL;
                        op3->tpToken=m->name;
                        optimize_now->next=new Quaternary;
                        optimize_now=optimize_now->next;
                        optimize_now->next=NULL;
                        optimize_now->operand[0]=op1;
                        optimize_now->operand[1]=op2;
                        optimize_now->operand[2]=op3;
                        optimize_now->Operator=equ;
                    }
                }
            }
            else{//双目运算
                Mark* m=dag_now->mark;
                Mark* lm=dag_now->lchild->mark;
                Mark* rm=dag_now->rchild->mark;
                if(m!=NULL){
                    Operand* op1=new Operand;
                    Operand* op2=new Operand;
                    Operand* op3=new Operand;
                    op1->tpToken=lm->name;
                    op2->tpToken=rm->name;
                    op3->tpToken=m->name;
                    optimize_now->next=new Quaternary;
                    optimize_now=optimize_now->next;
                    optimize_now->next=NULL;
                    optimize_now->operand[0]=op1;
                    optimize_now->operand[1]=op2;
                    optimize_now->operand[2]=op3;
                    optimize_now->Operator=dag_now->oper;
                }
                m=m->next;
                for(;m!=NULL;m=m->next){
                    mark_pos=m;
                    if(!MainMark()){
                        Operand* op1=new Operand;
                        Operand* op2=new Operand;
                        Operand* op3=new Operand;
                        op1->tpToken=dag_now->mark->name;
                        op2->tpToken=NULL;
                        op3->tpToken=m->name;
                        optimize_now->next=new Quaternary;
                        optimize_now=optimize_now->next;
                        optimize_now->next=NULL;
                        optimize_now->operand[0]=op1;
                        optimize_now->operand[1]=op2;
                        optimize_now->operand[2]=op3;
                        optimize_now->Operator=equ;
                    }
                }
            }
        }
        dag_now=dag_now->next;
    }
}
