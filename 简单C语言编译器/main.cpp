#include "L.h"


int main(){
    Scanner scan;//����һ��ɨ����
    GrammaticalAnalysis grammar;//����һ���﷨������
    Optimization opti;//����һ���Ż���
    ObjectCode obj;//����һ��Ŀ�����������
    Compiler compiler;//����һ��������

    scan.Scan();//�ʷ�����
    if(scan.error_head->next!=NULL){
        cout<<"�ʷ�����!"<<endl;
        scan.CoutErrorL();
    }
    else{
        grammar.token=scan.token;
        grammar.Grammar();//�﷨����&���嶯��
        if(grammar.result==0){cout<<"�����﷨����"<<endl;}
        else{
            if(grammar.error_head->next!=NULL){
            cout<<"�������!"<<endl;
            grammar.CoutErrorL();
            }
            else{
                cout<<"��Ԫʽ��"<<endl;
                grammar.CoutQuaternary();//����м���롪����Ԫʽ
                opti.quater=grammar.quater;
                opti.Optimize();//�м�����Ż�
                obj.optimize_block=opti.optimize_block;
                obj.id=grammar.id;
                obj.ObjCode();//Ŀ���������
                cout<<"����ɹ�!"<<endl;
                system("DOSBox.exe");
            }
        }
    }
    return 0;
}
