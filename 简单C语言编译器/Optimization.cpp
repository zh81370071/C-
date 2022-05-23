#include "L.h"
#include "EQU.h"

void Optimization::Optimize(){
    qua=quater->next;
    DivideBlock();//���ֻ�����
    CoutBlock();//�����������
    optimize_block=new Block;
    optimize_block_now=optimize_block;
    optimize_block_now->next=NULL;
    optimize_block_now->qua_block=NULL;

    block_now=block->next;//�����һ��������
    while(block_now!=NULL){
        CreateDAG();//����DAGͼ
        CreateOptimize_Block();//����һ���Ż�������
        CreateOptimize();//����һ���Ż���Ԫʽ����
        GetDAG();//��ȡ��DAG����Ԫʽ
        optimize_block_now->qua_block=optimize;//�����Ż���Ԫʽ���з����Ż���������
        DeleteDAG();//�ͷ�DAG�ռ�
        block_now=block_now->next;//����һ��������
    }
    CoutOptimize();//����Ż������Ԫʽ
}

void Optimization::CreateOptimize_Block(){//����һ���Ż����ͷ��
    Block* b=new Block;
    b->next=NULL;
    b->qua_block=NULL;
    optimize_block_now->next=b;
    optimize_block_now=b;
}

void Optimization::CoutOptimize(){//������������Ż������Ԫʽ
    Block* b=optimize_block;
    cout<<"DAG�ֲ��Ż�����Ԫʽ:"<<endl;
    int j=1;
    while(b->next!=NULL){//�����
        b=b->next;
        Quaternary* p=b->qua_block;
        cout<<"��"<<j<<":"<<endl;
        while(p->next!=NULL){//������е�ȫ����Ԫʽ
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

void Optimization::DeleteDAG(){//ɾ��DAG
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

void Optimization::CreateOptimize(){//����һ���Ż�����Ԫʽͷ��
    optimize=new Quaternary;
    optimize_now=optimize;
    optimize->next=NULL;
    int i=0;
    for(;i<3;i++)optimize->operand[i]=NULL;
    optimize->Operator=NULL;
}

void Optimization::DivideBlock(){//���ֿ�
    block=new Block;//�½�һ����
    block->next=NULL;
    block->qua_block=NULL;
    block_now=block;
    Quaternary* p=qua;
    Quaternary* before=NULL;
    while(p!=NULL){
        if(strcmp(p->Operator,"if")==0||strcmp(p->Operator,"el")==0||strcmp(p->Operator,"wh")==0||
            strcmp(p->Operator,"do")==0||strcmp(p->Operator,"we")==0||strcmp(p->Operator,"gt")==0){
            Block* b=new Block;//�½�һ����
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
            Block* b=new Block;//�½�һ����
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
        Block* b=new Block;//�½�һ����
        b->next=NULL;
        b->qua_block=qua;
        block_now->next=b;
        block_now=b;
    }
}

void Optimization::CoutBlock(){//�����������
    block_now=block->next;
    int i=1;
    while(block_now!=NULL){
        cout<<"������"<<i<<":"<<endl;
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

void Optimization::CreateDAG(){//����һ��DAG
    dag_head=new DAG;//����DAGͷ���
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
            else{//����
                op_now=p->Operator;
                add_now=p->operand[0];
                AddDAG();
            }
        }
        else if(strcmp(p->Operator,"=")==0){//��ֵ��(=,B,_,A)
            find_now_num1=p->operand[0];
            find_begin=dag_tail;
            if(!FindDAG()){
                op_now=NULL;
                add_now=p->operand[0];
                AddDAG();
            }
            Mark* m=pos->mark;//��¼B����λ��
            for(;m->next!=NULL;m=m->next);
            Mark* m_new=new Mark;
            m_new->fron=m;
            m_new->next=NULL;
            m->next=m_new;
            m_new->name=p->operand[2]->tpToken;//��A��Ϣд��
            mark_pos=pos->mark;//B���������λ��
            if(!(strcmp(m_new->name->token,"99")==0)){
                if(MainMark()){
                    temp_dag=pos;
                    temp1=pos->mark;
                    temp2=m_new;
                    SwopMark();//����
                }
            }
            find_now_num1=p->operand[2];//����֮ǰA�Ƿ񱻶���
            find_begin=pos->fron;
            while(find_begin!=NULL){
                if(!FindDAG()){//δ�ҵ�
                    find_begin=NULL;
                }
                else{//�ҵ�
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
                strcmp(p->Operator,"inc")==0||strcmp(p->Operator,"dec")==0){//��Ŀ�����,(w,B,_,A)

            find_now_num1=p->operand[0];//���Ҵ�
            find_now_ope=p->Operator;//���������
            if(!FindDAG_unary()){//δ�ҵ������½�һ�鵥Ŀ������
                find_now_num1=p->operand[0];
                find_begin=dag_tail;//���ҿ�ʼ��
                if(!FindDAG()){//Bû���������
                    op_now=NULL;
                    add_now=p->operand[0];
                    AddDAG();//����B
                }
                DAG* temp=pos;//��¼B��λ��
                op_now=p->Operator;
                add_now=p->operand[2];
                AddDAG();//����A
                pos->lchild=temp;//��B����ΪA������
            }
            else{//�ҵ��ˣ��򷵻���������ڽ���λ��
                Mark* m=pos->mark;
                for(;m->next!=NULL;m=m->next);//�ƶ���a���ڽ�㴦�ĸ��ӱ�ǵ����һ��
                Mark* m_new=new Mark;
                m_new->fron=m;
                m_new->next=NULL;
                m->next=m_new;
                m_new->name=p->operand[2]->tpToken;//��A��Ϣд��
                mark_pos=pos->mark;
                if(!(strcmp(m_new->name->token,"99")==0)){//�ñ�ǲ���ϵͳ����
                    if(MainMark()){//posλ�õ��������ϵͳ���ɱ���
                        temp_dag=pos;
                        temp1=pos->mark;
                        temp2=m_new;
                        SwopMark();
                    }
                }
                find_now_num1=p->operand[2];//����֮ǰA�Ƿ񱻶���
                find_begin=pos->fron;
                while(find_begin!=NULL){
                    if(!FindDAG()){//δ�ҵ�
                        find_begin=NULL;
                    }
                    else{//�ҵ�
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
        else {//˫Ŀ�����,(w,B,C,A)
            find_now_num1=p->operand[0];//���Ҵ�1
            find_now_num2=p->operand[1];//���Ҵ�2
            find_now_ope=p->Operator;//���������
            if((!strcmp(find_now_num1->tpToken->token,"4")==0)&&(!strcmp(find_now_num1->tpToken->token,"99")==0)&&
               (!strcmp(find_now_num2->tpToken->token,"4")==0)&&(!strcmp(find_now_num2->tpToken->token,"99")==0)){//�������������ɼ���
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
                if(grade==1){//����
                    INT* in=new INT;
                    in->numi=(int)sum;
                    p->operand[0]->tpToken->token[0]='0';
                    p->operand[0]->tpToken->token[1]='\0';
                    p->operand[0]->tpToken->tpI=in;
                }
                else if(grade==2){//ʵ��
                    REAL* re=new REAL;
                    re->numf=(float)sum;
                    p->operand[0]->tpToken->token[0]='1';
                    p->operand[0]->tpToken->token[1]='\0';
                    p->operand[0]->tpToken->tpR=re;
                }
                find_now_num1=p->operand[0];//��ǰ������
                find_begin=dag_tail;//���ҿ�ʼ��
                if(!FindDAG()){//δ�鵽
                    op_now=NULL;
                    add_now=p->operand[0];
                    AddDAG();//����B
                }

                Mark* m=pos->mark;//��¼B����λ��
                for(;m->next!=NULL;m=m->next);//�ƶ���a���ڽ�㴦�ĸ��ӱ�ǵ����һ��
                Mark* m_new=new Mark;
                m_new->fron=m;
                m_new->next=NULL;
                m->next=m_new;
                m_new->name=p->operand[2]->tpToken;//��A��Ϣд��
                mark_pos=pos->mark;//B���������λ��
                if(!(strcmp(m_new->name->token,"99")==0)){//�ñ�ǲ���ϵͳ����
                    if(MainMark()){//posλ�õ��������ϵͳ���ɱ���
                        temp_dag=pos;
                        temp1=pos->mark;
                        temp2=m_new;
                        SwopMark();
                    }
                }
                find_now_num1=p->operand[2];//����֮ǰA�Ƿ񱻶���
                find_begin=pos->fron;
                while(find_begin!=NULL){
                    if(!FindDAG()){//δ�ҵ�
                        find_begin=NULL;
                    }
                    else{//�ҵ�
                        find_begin=pos->fron;
                        if(mark_pos->fron==NULL);
                        else{
                            temp1=mark_pos;
                            DeleteMark();
                        }
                    }
                }
            }
            else{//�������к����ɼ�����
                if(!FindDAG_Bi()){//δ�ҵ������½�һ��˫Ŀ������
                    //�ֱ𿴿�B,C��û�У���ֱ���ã�û���ٽ������
                    find_now_num1=p->operand[0];
                    find_begin=dag_tail;//���ҿ�ʼ��
                    if(!FindDAG()){//����B���
                        op_now=NULL;
                        add_now=p->operand[0];
                        AddDAG();//����B
                    }
                    DAG* temp1=pos;//��¼B��λ��
                    find_now_num1=p->operand[1];
                    find_begin=dag_tail;//���ҿ�ʼ��
                    if(!FindDAG()){//����C���
                        op_now=NULL;
                        add_now=p->operand[1];
                        AddDAG();//����C
                    }
                    DAG* temp2=pos;//��¼C��λ��
                    op_now=p->Operator;
                    add_now=p->operand[2];
                    AddDAG();//����A
                    pos->lchild=temp1;//���ӽ��A,B,C
                    pos->rchild=temp2;
                }
                else{//�ҵ��ˣ��򷵻���������ڽ���λ��
                    Mark* m=pos->mark;
                    for(;m->next!=NULL;m=m->next);//�ƶ���a���ڽ�㴦�ĸ��ӱ�ǵ����һ��
                    Mark* m_new=new Mark;
                    m_new->fron=m;
                    m_new->next=NULL;
                    m->next=m_new;
                    m_new->name=p->operand[2]->tpToken;//��A��Ϣд��
                    mark_pos=pos->mark;
                    if(!(strcmp(m_new->name->token,"99")==0)){//�ñ�ǲ���ϵͳ����
                        if(MainMark()){//posλ�õ��������ϵͳ���ɱ���
                            temp_dag=pos;
                            temp1=pos->mark;
                            temp2=m_new;
                            SwopMark();
                        }
                    }
                    find_now_num1=p->operand[2];//����֮ǰA�Ƿ񱻶���
                    find_begin=pos->fron;
                    while(find_begin!=NULL){
                        if(!FindDAG()){//δ�ҵ�
                            find_begin=NULL;
                        }
                        else{//�ҵ�
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

int Optimization::FindDAG_unary(){//��鵥Ŀ�����
    dag_now=dag_tail;//�Ӻ���ǰ��
    if(dag_now==NULL){return 0;}//��Ϊ����δ�ҵ�
    for(;dag_now!=NULL;dag_now=dag_now->fron){//�����������
        if(dag_now->oper!=NULL){//�õ��������������������Ƿ���ͬ
            if(strcmp(dag_now->oper,find_now_ope)==0){//���ҵ��õ�Ŀ�����,����������Ƿ�һ��
                pos=dag_now;//��¼��������Ľ��λ��
                DAG* d=dag_now->lchild;//���������������
                if(d!=NULL&&d->mark!=NULL){
                    Mark* m=d->mark;//��ǰ���
                    for(;m!=NULL;m=m->next){//���ұ���Ƿ�͵�ǰ���Ҵ���ͬ
                        if(strcmp(m->name->token,find_now_num1->tpToken->token)==0){//�ж���������ͬ����ͬ������ֵ
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

int Optimization::FindDAG_Bi(){//���˫Ŀ�����
    dag_now=dag_tail;//�Ӻ���ǰ��
    if(dag_now==NULL){return 0;}//��Ϊ����δ�ҵ�
    for(;dag_now!=NULL;dag_now=dag_now->fron){//�����������
        if(dag_now->oper!=NULL){//�õ��������������������Ƿ���ͬ
            if(strcmp(dag_now->oper,find_now_ope)==0){//���ҵ���˫Ŀ�����,����������Ƿ�һ��
                pos=dag_now;//��¼��������Ľ��λ��
                DAG* d=dag_now->lchild;//���������������
                if(d!=NULL&&d->mark!=NULL){//���Ӵ��ڲ����б��
                    Mark* m=d->mark;//��ǰ���
                    for(;m!=NULL;m=m->next){//���ұ���Ƿ�͵�ǰ���Ҵ���ͬ
                        if(strcmp(m->name->token,find_now_num1->tpToken->token)==0){//�ж���������ͬ����ͬ������ֵ
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
                            if(lab==1){//����ͬ����������Һ���
                                DAG* r=dag_now->rchild;
                                if(r!=NULL&&r->mark!=NULL){
                                    Mark* m=r->mark;//��ǰ���
                                    for(;m!=NULL;m=m->next){//���ұ���Ƿ�͵�ǰ���Ҵ���ͬ
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
                                            if(lab==1){//����ͬ�򷵻�1������¼��������ڽ��λ�ã���¼��pos��
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

void Optimization::AddDAG(){//����һ��DAG���,������λ�õ�pos
    DAG* p=new DAG;
    p->lchild=NULL;
    p->rchild=NULL;
    p->next=NULL;
    p->fron=dag_tail;

    p->mark=new Mark;
    p->mark->fron=NULL;
    p->mark->next=NULL;

    p->oper=op_now;//����������Ϣ
    p->mark->name=add_now->tpToken;//����������Ϣ

    dag_tail->next=p;
    dag_tail=p;

    pos=p;//���ظý��λ��
}

int Optimization::FindDAG(){//���һ������Ƿ����,���鵽�򷵻ظý��λ�úͱ��λ��
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

int Optimization::MainMark(){//���mark_pos����ǲ���ϵͳ������м����
    Mark* m=mark_pos;
    if(strcmp(m->name->token,"99")==0){//�����򷵻�1�����򷵻�0
        return 1;
    }
    else{
        return 0;
    }
}

void Optimization::SwopMark(){//�������
    Mark* t2_f=temp2->fron;
    Mark* t2_n=temp2->next;

    temp_dag->mark=temp2;
    temp2->fron=NULL;

    t2_f->next=t2_n;
    if(t2_n!=NULL){t2_n->fron=t2_f;}
    temp2->next=temp1;
    temp1->fron=temp2;

}

void Optimization::DeleteMark(){//ɾ�����
    Mark* p=temp1->fron;
    Mark* q=temp1->next;
    p->next=q;
    if(q!=NULL){q->fron=p;}
    q=temp1;
    temp1=p;
    delete(q);
}

void Optimization::GetDAG(){//��DAG�л�ȡ�Ż�����Ԫʽ
    dag_now=dag_head->next;
    while(dag_now!=NULL){
        if(dag_now->lchild==NULL&&dag_now->rchild==NULL){//Ҷ�ڵ�
            if(dag_now->oper!=NULL){//�ý���������
                if(strcmp(dag_now->oper,"el")==0||strcmp(dag_now->oper,"wh")==0||strcmp(dag_now->oper,"we")==0||strcmp(dag_now->oper,"ie")==0){//��������������
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
            else{//�ý��û�������
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
        else if(dag_now->lchild!=NULL){//��Ҷ�ڵ�
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
            else if(dag_now->rchild==NULL){//��Ŀ����
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
            else{//˫Ŀ����
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
