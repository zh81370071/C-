#include "L.h"
#include "KEY.h"


void Scanner::Scan(){
    error_head=new ERRORL;
    error_now=error_head;
    error_now->next=NULL;
    int i;//当前词数
	char code[100];
	for(i=0;i<100;i++)code[i]='\0';
	FILE* fp=NULL;
	i=warn=0;
	state=line=1;//状态初始化为1
	fp=fopen("code.txt","r");
	ch=getc(fp);
	TOKEN* t;
	token=new TOKEN;
	token->next=NULL;
	t=token;
	while(ch!=EOF){
        if(ch=='$'){
            for(;ch!='\n';){
                ch=getc(fp);
            }
            line++;
        }
        else{
            if((ch!=' '&&ch!='\n'&&ch!='\t')||state!=1){//过滤空格、回车和Tab
                state_before=state;//保存当前状态
                state=state_change(state,ch,warn);//进行自动机状态转换
                if(state){
                    if(ch!='\n'){//输入字符非换行时，将其存入token
                        code[i++]=ch;
                        ch_before=ch;
                        ch=getc(fp);
                    }
                }
                else{
                    TOKEN* p=new TOKEN;
                    CreatNewToken(p);
                    state_to_code(p,state_before,code,line,warn);//生成Token类别码
                    t->next=p;
                    t=p;
                    reset(state,code,i,warn);//重置
                }
            }
            else{
                if(ch=='\n'){line++;}//记录行数
                ch_before=ch;
                ch=getc(fp);
            }
        }
	}
	fclose(fp);
	if(ch_before!='\n'&&ch_before!=' '&&ch_before!='\t'){
        TOKEN* p=new TOKEN;
        CreatNewToken(p);
        state_to_code(p,state,code,line,warn);//生成Token类别码
        t->next=p;
	}

	                /**************************测试用************************/
                t=token;
                while(t->next!=NULL){
                t=t->next;
                cout<<t->line<<"    ";
                cout<<t->token<<"    ";
                if(t->token[0]=='0'&&t->token[1]=='\0')cout<<t->tpI->numi<<"  ";
                if(t->token[0]=='1'&&t->token[1]=='\0')cout<<t->tpR->numf<<"  ";
                if(t->token[0]=='2'&&t->token[1]=='\0')cout<<t->tpC->numc<<"  ";
                if(t->token[0]=='4'&&t->token[1]=='\0')cout<<t->tpIT->name<<"  ";
                if(t->token[0]=='1'&&t->token[1]!='\0')cout<<t->tpKT<<"  ";
                if(t->token[0]=='2'&&t->token[1]!='\0')cout<<t->tpKT<<"  ";
                if(t->token[0]=='3'&&t->token[1]!='\0')cout<<t->tpKT<<"  ";
                if(t->token[0]=='4'&&t->token[1]!='\0')cout<<t->tpKT<<"  ";
                if(t->token[0]=='5'&&t->token[1]!='\0')cout<<t->tpPT<<"  ";
                if(t->token[0]=='6'&&t->token[1]!='\0')cout<<t->tpPT<<"  ";
                if(t->token[0]=='7'&&t->token[1]!='\0')cout<<t->tpOT<<"  ";
                if(t->token[0]=='8'&&t->token[1]!='\0')cout<<t->tpOT<<"  ";
                if(t->token[0]=='9'&&t->token[1]!='\0')cout<<t->tpOT<<"  ";
                if(t->token[0]=='-')cout<<t->tpIT->name<<"  ";
                cout<<endl;
                }
                /*************************************************************/
}

void Scanner::CreatNewToken(TOKEN*& p){
    p->line=0;
    p->next=NULL;
    p->tpC=NULL;
    p->tpI=NULL;
    p->tpIT=NULL;
    p->tpKT=NULL;
    p->tpOT=NULL;
    p->tpPT=NULL;
    p->tpR=NULL;
}

void Scanner::reset(int& state,char* code,int& i,int& warn){
	state=1;
	warn=0;
	for(;i>=0;i--)code[i]='\0';
	i=0;
}

//make PDA's current state jump to the next state

int Scanner::state_change(int st,char ch,int& warn){
	int ch_asc;
	ch_asc=(int)ch;
	if(st==1){
	    if(ch_asc>=48&&ch_asc<=57){
			st=3;
		}
		else if((ch_asc>=97&&ch_asc<=122)||(ch_asc>=65&&ch_asc<=90)||(ch=='_')){
			st=2;
		}
		else{
            switch(ch){
            case '{':st=50;break;
            case '}':st=51;break;
	        case '(':st=52;break;
	        case ')':st=53;break;
	        case '[':st=54;break;
	        case ']':st=55;break;
	        case '\"':st=56;break;
	        case '\'':st=57;break;
	        case ';':st=58;break;
	        case ',':st=59;break;
	        case '.':st=60;break;
            case '?':st=62;break;
	        case '#':st=63;break;
	        case '+':st=70;break;
	        case '-':st=71;break;
	        case '*':st=72;break;
	        case '/':st=73;break;
	        case '%':st=74;break;
	        case '=':st=77;break;
	        case '>':st=78;break;
	        case '<':st=80;break;
	        case '!':st=86;break;
            case '&':st=87;break;
	        case '|':st=88;break;
	        case '^':st=89;break;
	        case '~':st=90;break;
	        }
		}
	}
	else if(st==3){
		if(ch_asc>=48&&ch_asc<=57){
			st=3;
		}
		else if(ch=='.'){
			st=8;
		}
		else st=0;
	}
	else if(st==57){
		st=10;
	}
	else if(st==56){
		st=12;
	}
	else if(st==70){//++,+
		if(ch=='+'){
			st=75;
		}
		else st=0;
	}
    else if(st==71){//--,-
		if(ch=='-'){
			st=76;
		}
		else st=0;
	}
    else if(st==78){//>>,>=,>
		if(ch=='>'){
			st=91;
		}
		else if(ch=='='){
            st=79;
		}
		else st=0;
	}
    else if(st==80){//<<,<=,<
		if(ch=='<'){
			st=92;
		}
		else if(ch=='='){
            st=81;
		}
		else st=0;
	}
	else if(st==77){//=,==
		if(ch=='='){
			st=82;
		}
		else st=0;
	}
	else if(st==86){/*!,!=*/
		if(ch=='='){
			st=83;
		}
		else st=0;
	}
    else if(st==87){//&,&&
		if(ch=='&'){
			st=84;
		}
		else st=0;
	}
    else if(st==88){//|,||
		if(ch=='|'){
			st=85;
		}
		else st=0;
	}
	else if(st==8){
		if(ch_asc>=48&&ch_asc<=57){
			st=7;
		}
		else st=-2;//错误2
	}
	else if(st==10){
		if(ch=='\''){
			st=16;
		}
		else if(ch!='\n'){
            st=10;
            warn=1;//报警1
		}
		else st=-4;//错误4
	}
	else if(st==12){
		if(ch=='\"'){
			st=17;
		}
		else if(ch!='\n'){
            st=12;
		}
		else st=-3;//错误3
	}
	else if(st==7){
		if(ch_asc>=48&&ch_asc<=57){
			st=7;
		}
		else st=0;
	}
	else if(st==2){
		 if((ch_asc>=97&&ch_asc<=122)||(ch_asc>=65&&ch_asc<=90)||(ch_asc>=48&&ch_asc<=57)||(ch=='_')){
			st=2;
		}
		 else st=0;
	}
	else st=0;
	if(st==1&&ch!=' '&&ch!='\n'){//can't tell the first character except ' 'and'\n',so the character hasn't been defined in compiler.
        st=-1;//The character is undefined in compiler
	}
 return  st;
}

//get code according to state

void Scanner::state_to_code(TOKEN* t, int state_before, char code[100], int line, int warn){
    t->line=line;
	if(state_before>=0){//状态正确
	    if(state_before>=70){//运算符
	        string s;
	        ConvertItoS(state_before,s);
	        ConvertStoC(s,t->token);
	        t->tpOT=OT[state_before-70];
	    }
        else if(state_before>=50){//界符
	        string s;
	        ConvertItoS(state_before,s);
	        ConvertStoC(s,t->token);
	        t->tpPT=PT[state_before-50];
	    }
	    else if(state_before==3){//整型
	        int n;
	        t->token[0]='0';
	        t->token[1]='\0';
	        ConvertStoI(code,n);
            t->tpI=new INT;
            t->tpI->numi=n;
	    }
        else if(state_before==7){//实型
            float r;
            t->token[0]='1';
            t->token[1]='\0';
            ConvertStoF(code,r);
            t->tpR=new REAL;
            t->tpR->numf=r;
	    }
        else if(state_before==16){//字符型
            t->token[0]='2';
            t->token[1]='\0';
            t->tpC=new CHAR;
            t->tpC->numc=code[1];
	    }
        else if(state_before==17){//字符串
            t->token[0]='3';
            t->token[1]='\0';
	    }
        else if(state_before==2){//关键字或者标识符
            int i;
            for(i=0;i<34;i++){
                if(strcmp(KT[i],code)==0){
                    t->tpKT=KT[i];
                    i=i+10;
                    string s;
                    ConvertItoS(i,s);
                    ConvertStoC(s,t->token);

                    i=50;
                }
            }
            if(i!=51){//为标识符
                t->token[0]='4';
                t->token[1]='\0';
                TOKEN* p=NULL;
                if(!Findexist(code,p)){
                    t->tpIT=new SYNBL;
                    strcpy(t->tpIT->name,code);
                }
                else{
                    t->tpIT=p->tpIT;
                }
            }
	    }
	}
	else{//状态错误,词法错误
        string s;
        ConvertItoS(state_before,s);
        ConvertStoC(s,t->token);
        t->tpIT=new SYNBL;
        strcpy(t->tpIT->name,code);

        ERRORL* e=new ERRORL;
        e->next=NULL;
        e->line=t->line;
        strcpy(e->name,code);
        if(state_before==-2){e->type='r';}
        else if(state_before==-4){e->type='c';}
        else if(state_before==-3){e->type='s';}
        else if(state_before==-1){e->type='u';}
        error_now->next=e;
        error_now=e;
	}

}

int Scanner::Findexist(char* code, TOKEN*& p){
    p=token;
    if(p->next==NULL){return 0;}
    p=p->next;
    while(p!=NULL){
        char c[2]={"4"};
        if(strcmp(p->token,c)==0){
            if(strcmp(p->tpIT->name,code)==0){
                return 1;
            }
        }
        p=p->next;
    }
    return 0;
}

void Scanner::ConvertItoS(int i,string& st){
	stringstream s;
	s<<i;
	s>>st;
}

void Scanner::ConvertFtoS(float f,string& st){
	stringstream s;
	s<<f;
	s>>st;
}

void Scanner::ConvertStoI(string st,int& i){
	stringstream s;
	s<<st;
	s>>i;
}

void Scanner::ConvertStoF(string st, float& f){
	stringstream s;
	s<<st;
	s>>f;
}

void Scanner::ConvertStoC(string st,char* c){
    strcpy(c,st.c_str());
}

void Scanner::CoutErrorL(){
    ERRORL* e=error_head->next;
    for(;e!=NULL;e=e->next){
        cout<<"第"<<e->line<<"行,";
        if(e->type=='r'){cout<<"实数错误,"<<e->name<<endl;}
        else if(e->type=='c'){cout<<"字符无法识别,"<<e->name<<endl;}
        else if(e->type=='s'){cout<<"字符串无法识别,"<<e->name<<endl;}
        else if(e->type=='u'){cout<<"符号无法识别,"<<e->name<<endl;}
        else if(e->type=='d'){cout<<"标识符未定义,"<<e->name<<endl;}
        else if(e->type=='m'){cout<<"标识符重复定义,"<<e->name<<endl;}
    }
}
