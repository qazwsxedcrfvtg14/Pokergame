#include "sevens.h"
using namespace std;

using namespace Jtol;
void input(Net net){
    int tp;
    while(1){
        cin.clear();
        cin >> tp;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"請勿輸入非整數"<<endl;
            }
        else{
            break;
            }
        }
    NetSend(net,IntToStr(tp).c_str());
    }
int main(){
    char ip[100]="127.0.0.1";
    scanf("%s",ip);
    Net net=NetCreat(ip,7122);
    //puts("Wait others :P");
    deque<char> qu;
    int typ=0;
    bool a=0,b=0;
    int t1px=0,t1py=0;
    const int t3pxb=55;
    int t3px=t3pxb,t3py=0;
    string cmd;
    while(1){
        const char *buf=NetGet(net);
        while(*buf)qu.push_back(*buf),buf++;
        if(!typ&&qu.size()>=3){
            if(qu[0]=='$'&&qu[1]==':'&&qu[2]=='o')typ=1,gotoxy(t1px,t1py),qu.pop_front(),qu.pop_front(),qu.pop_front(),a=b=0;
            else if(qu[0]=='$'&&qu[1]==':'&&qu[2]=='O')typ=3,qu.pop_front(),qu.pop_front(),qu.pop_front(),a=b=0;
            else if(qu[0]=='$'&&qu[1]==':'&&qu[2]=='P')qu.pop_front(),qu.pop_front(),qu.pop_front(),input(net);
            else if(qu[0]=='$'&&qu[1]==':'&&qu[2]=='x')break;
            else if(qu[0]=='$'&&qu[1]==':'&&qu[2]=='r')typ=2,cmd="",qu.pop_front(),qu.pop_front(),qu.pop_front(),a=b=0;
            else qu.pop_front();
            }
        if(typ==1){
            while(qu.size()){
                if(qu[0]=='$')a=1;
                else if(qu[0]==':'&&a)b=1;
                else if(qu[0]=='e'&&b){
                    qu.pop_front(),typ=0;
                    getxy(t1px,t1py);
                    break;
                    }
                else{
                    if(a)putchar('$');
                    if(b)putchar(':');
                    putchar(qu[0]);
                    a=b=0;
                    }
                qu.pop_front();
                }
            }
        if(typ==3){
            while(qu.size()){
                if(qu[0]=='$')a=1;
                else if(qu[0]==':'&&a)b=1;
                else if(qu[0]=='e'&&b){
                    qu.pop_front(),typ=0;
                    break;
                    }
                else{
                    gotoxy(t3px,t3py);
                    if(a)putchar('$');
                    if(b)putchar(':');
                    if(qu[0]=='\n')
                        gotoxy(t3pxb,t3py+1);
                    else
                        putchar(qu[0]);
                    getxy(t3px,t3py);
                    a=b=0;
                    }
                qu.pop_front();
                }
            }
        if(typ==2){
            while(qu.size()){
                if(qu[0]=='$')a=1;
                else if(qu[0]==':'&&a)b=1;
                else if(qu[0]=='e'&&b){
                    qu.pop_front(),typ=0;
                    if(cmd=="cls"){
                        t1px=t1py=t3py=0;
                        t3px=t3pxb;
                        }
                    system(cmd.c_str());
                    break;
                    }
                else{
                    if(a)cmd+='$';
                    if(b)cmd+=':';
                    cmd+=qu[0];
                    a=b=0;
                    }
                qu.pop_front();
                }
            }
        Sleep(15);
        }
    return 0;
    }
