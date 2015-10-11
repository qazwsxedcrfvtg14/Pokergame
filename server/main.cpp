#include "sevens.h"
using namespace std;

using namespace Jtol;
void Play_Main(vector<Net> player,int ais){
    sevens_element a;
    begin:;
    puts("New Game Start!");
    SendCmd(player,"cls");
    SendMsg(player,"New Game Start!!!\n");
    a.start_a_game(player,ais);
    //a.clear_score();
    SendMsg(player[0],"還要再玩一局嗎？是請輸入1，否請輸入0，重設分數請輸入2\n");
    int k;
    while(1){
        k=GetNum(player[0]);
        if(k==2){
            a.clear_score();
            goto begin;
            }
        else if(k==1){
            goto begin;
            }
        else if(k==0){
            break;
            }
        }
    for(Net x:player)
        NetSend(x,"$:x"),NetClose(x);
    return;
    }
void Server_Main(){
    set<Net> *snet=SNetCreat(7122,1);
    if(snet==NULL)return;
    puts("Sever Start!!!");
    while(1){
        set<Net> snet_c=*snet;
        if(snet_c.size()){
            int pls=0;
            SendMsg(*snet_c.begin(),"How Many Real Players?\n");
            while(1){
                pls=GetNum(*snet_c.begin());
                if(pls>=1&&pls<=4)break;
                else SendMsg(*snet_c.begin(),"Range Error\n");
                }
            vector<Net>pl;
            SendMsg(*snet_c.begin(),"Please Wait :P\n");
            pl.push_back(*snet_c.begin());
            snet->erase(*snet_c.begin());
            int cnt=pls-1;
            while(cnt){
                snet_c=*snet;
                if(snet_c.size()){
                    SendMsg(*snet_c.begin(),"Please Wait :P\n");
                    pl.push_back(*snet_c.begin());
                    snet->erase(*snet_c.begin());
                    cnt--;
                    }
                Sleep(20);
                }
            Thread setr=ThreadCreat(Play_Main,pl,4-pls);
            }
        Sleep(20);
        }
    }
int main(){
    Server_Main();
    sevens_element a;
    return 0;
}
