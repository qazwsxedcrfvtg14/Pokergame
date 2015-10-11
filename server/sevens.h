#include "poker_element.h"
#include <windows.h>
using namespace std;
using namespace Jtol;

class sevens_element:public poker_game {
        int st[4],ed[4];
        vector<int> covered[4];
        int cst[4][4], ced[4][4];
        int score[4];
        int thisscore[4];
        int nowplay;
        bool first_play;
        bool ai[4]= {0,0,0,0};
    public:
        sevens_element();
        void initial();
        void display_now_state();
        void display_now_player();
        bool play(int ind);
        bool can_play();
        bool cover(int ind);
        bool able_to_play[4][13];
        int get_nowplay() {
            return nowplay;
            }
        void start_a_game(vector<Net>pl_lis,int ais);
        void clear_score();
    };

sevens_element::sevens_element() {
    shuffle(10000);
    first_play = 1;
    for(int i = 0; i < 4; i++) {
        st[i] = ed[i] = -10;
        draw(i,13);
        sort_player_hand(i,13,1);
        for(int j = 0; j < player_card_index[i]; j++) {
            if(player[i][j] == 6) {
                nowplay = i;
                }
            }
        covered[i].clear();
        thisscore[i] = 0;
        score[i] = 0;
        for(int j = 0; j < 4; j++) {
            cst[i][j] = 0;
            ced[i][j] = 14;
            }
        }
    }

void sevens_element::initial() {
    poker_game::initial();
    first_play = 1;
    shuffle(10000);
    for(int i = 0; i < 4; i++) {
        st[i] = ed[i] = -10;
        draw(i,13);
        sort_player_hand(i,13,1);
        for(int j = 0; j < player_card_index[i]; j++) {
            if(player[i][j] == 6) {
                nowplay = i;
                }
            }
        covered[i].clear();
        thisscore[i] = 0;
        for(int j = 0; j < 4; j++) {
            cst[i][j] = 0;
            ced[i][j] = 14;
            }
        }
    }

void sevens_element::display_now_state() {
    SendMsg(pl_lis[nowplay],"============================================\n");
    for(int i = 0; i < 4; i++) {
        if(st[i]<0) {
            continue;
            }
        diagram temp(1,1);
        int movex = 0;
        for(int j = st[i]; j <= ed[i]; j++, movex += 3) {
            temp = diagram_add(temp, card_picture(j), movex, 0);
            }
        temp.printout(pl_lis[nowplay]);
        }
    SendMsg(pl_lis[nowplay],"============================================\n");
    }

bool sevens_element::play(int ind) {
    if((first_play)&&(player[nowplay][ind] != 6)){
        SendMsg(pl_lis[nowplay],"非法地出牌。請重新出牌\n");
        return 0;
        }
    for(int i = 0; i < 4; i++) {
        if((st[i] - player[nowplay][ind] == 1)&&(st[i]%13)) {
            st[i] = player[nowplay][ind];
            SendCmd(pl_lis[nowplay],"cls");
            SendMsg(pl_lis,player_name[nowplay]+"出牌\n");
            card_picture(player[nowplay][ind]).printout(pl_lis);
            play_card(nowplay, ind);
            nowplay++;
            nowplay%=4;
            first_play = 0;Sleep(500);
            return 1;
            }
        if((ed[i] - player[nowplay][ind] == -1)&&(player[nowplay][ind]%13)) {
            ed[i] = player[nowplay][ind];
            SendCmd(pl_lis[nowplay],"cls");
            SendMsg(pl_lis,player_name[nowplay]+"出牌\n");
            card_picture(player[nowplay][ind]).printout(pl_lis);
            play_card(nowplay, ind);
            nowplay++;
            nowplay%=4;
            first_play = 0;
            Sleep(500);
            return 1;
            }
        if((player[nowplay][ind]/13 == i)&&(getpoint(player[nowplay][ind]) == 7)) {
            st[i] = ed[i] = player[nowplay][ind];
            SendCmd(pl_lis[nowplay],"cls");
            SendMsg(pl_lis,player_name[nowplay]+"出牌\n");
            card_picture(player[nowplay][ind]).printout(pl_lis);
            play_card(nowplay, ind);
            nowplay++;
            nowplay%=4;
            first_play = 0;
            Sleep(500);
            return 1;
            }
        }
    SendMsg(pl_lis[nowplay],"非法地出牌。請重新出牌\n");
    return 0;
    }

bool sevens_element::can_play() {
    bool success = 0;
    if(first_play) {
        for(int i = 0; i < 13; i++) {
            able_to_play[nowplay][i] = (player[nowplay][i] == 6);
            }
        return 1;
        }
    for(int ind = 0; ind < player_card_index[nowplay]; ind++) {
        able_to_play[nowplay][ind] = 0;
        for(int i = 0; i < 4; i++) {
            if((st[i] - player[nowplay][ind] == 1)&&(st[i]%13)) {
                success = 1;
                able_to_play[nowplay][ind] = 1;
                }
            if((ed[i] - player[nowplay][ind] == -1)&&(player[nowplay][ind]%13)) {
                success = 1;
                able_to_play[nowplay][ind] = 1;
                }
            if((player[nowplay][ind]/13 == i)&&(getpoint(player[nowplay][ind]) == 7)) {
                success = 1;
                able_to_play[nowplay][ind] = 1;
                }
            }
        }
    return success;
    }

bool sevens_element::cover(int ind) {
    if(can_play()) {
        return 0;
        }
    thisscore[nowplay] -= getpoint(player[nowplay][ind]);
    covered[nowplay].push_back(player[nowplay][ind]);
    SendMsg(pl_lis,player_name[nowplay]+"蓋\牌\n");
    play_card(nowplay, ind);
    nowplay++;
    nowplay%=4;
    Sleep(500);
    return 1;
    }

void sevens_element::start_a_game(vector<Net>pl_lis_fk,int ais){
    for(int i=0;i<4-ais;i++)
        ai[i]=0;
    for(int i=4-ais;i<4;i++)
        ai[i]=1;
    pl_lis=pl_lis_fk;
    initial();
    while(player_card_index[nowplay]) {
        if(ai[nowplay]) {
            if(can_play()) {
                int tmax = -2147483647;
                int ti = 0;
                for(int i = 0; i < player_card_index[nowplay]; i++) {
                    if(!able_to_play[nowplay][i]) {
                        continue;
                        }
                    int sum1 = 0;
                    int sum2 = 0;
                    bool cons1 = 0, cons2 = 0;
                    if(getpoint(player[nowplay][i]) <= 7) {
                        int ind = 1;
                        int rr = cst[nowplay][player[nowplay][i]/13];
                        sum1 -= (getpoint(player[nowplay][i])*(getpoint(player[nowplay][i]) - 1)/2 - rr*(rr+1)/2);
                        while((ind <= i)&&(player[nowplay][i-ind]/13 == player[nowplay][i]/13)&&(getpoint(player[nowplay][i-ind]) > rr)) {
                            if(getpoint(player[nowplay][i]) - getpoint(player[nowplay][i-ind]) > ind) {
                                sum1 += 4*getpoint(player[nowplay][i-ind]);
                                }
                            else {
                                sum1 += getpoint(player[nowplay][i-ind]);
                                cons1 = 1;
                                }
                            ind++;
                            }
                        }
                    if(getpoint(player[nowplay][i]) >= 7) {
                        int ind = 1;
                        int rr = ced[nowplay][player[nowplay][i]/13];
                        sum2 -= rr*(rr-1)/2 - getpoint(player[nowplay][i])*(getpoint(player[nowplay][i]) + 1)/2;
                        while((ind < player_card_index[nowplay]-i)&&(player[nowplay][ind+i]/13 == player[nowplay][i]/13)&&(getpoint(player[nowplay][i-ind]) < rr)) {
                            if(getpoint(player[nowplay][ind+i]) - getpoint(player[nowplay][i]) > ind) {
                                sum2 += 4*getpoint(player[nowplay][ind+i]);
                                }
                            else {
                                sum2 += getpoint(player[nowplay][ind+i]);
                                cons2 = 1;
                                }
                            ind++;
                            }
                        }
                    if(cons1) {
                        sum1 = max(0, sum1);
                        }
                    if(cons2) {
                        sum2 = max(0,sum2);
                        }
                    int sum = sum1+sum2;
                    if(sum > tmax) {
                        tmax = sum;
                        ti = i;
                        }
                    }
                //Working

                play(ti);
                }
            else {
                int tmax = -2147483647;
                int ti = 0;
                for(int i = 0; i < player_card_index[nowplay]; i++) {
                    int sum = 0;
                    if(getpoint(player[nowplay][i]) <= 7) {
                        int ind = 1;
                        int rr = cst[nowplay][player[nowplay][i]/13];
                        sum += (getpoint(player[nowplay][i])*(getpoint(player[nowplay][i]) - 1)/2 - rr*(rr+1)/2);
                        while((ind <= i)&&(player[nowplay][i-ind]/13 == player[nowplay][i]/13)&&(getpoint(player[nowplay][i-ind]) > rr)) {
                            sum -= 4*getpoint(player[nowplay][i-ind]);
                            ind++;
                            }
                        }
                    if(getpoint(player[nowplay][i]) >= 7) {
                        int ind = 1;
                        int rr = ced[nowplay][player[nowplay][i]/13];
                        sum += rr*(rr-1)/2 - getpoint(player[nowplay][i])*(getpoint(player[nowplay][i]) + 1)/2;
                        while((ind < player_card_index[nowplay]-i)&&(player[nowplay][ind+i]/13 == player[nowplay][i]/13)&&(getpoint(player[nowplay][i-ind]) < rr)) {
                            sum -= 4*getpoint(player[nowplay][ind+i]);
                            ind++;
                            }
                        }
                    if((getpoint(player[nowplay][i]) > cst[nowplay][player[nowplay][i]/13])&&(getpoint(player[nowplay][i]) < ced[nowplay][player[nowplay][i]/13])) {
                        sum -= 3*getpoint(player[nowplay][i]);
                        }
                    if(sum > tmax) {
                        tmax = sum;
                        ti = i;
                        }
                    }
                if(getpoint(player[nowplay][ti]) < 7) {
                    cst[nowplay][player[nowplay][ti]/13] = getpoint(player[nowplay][ti]);
                    }
                else {
                    ced[nowplay][player[nowplay][ti]/13] = getpoint(player[nowplay][ti]);
                    }
                cover(ti);
                }
            //Sleep(500);
            continue;
            }
        display_now_state();
        display_player_card(nowplay);
        bool tpp = can_play();
        if(!tpp) {
            SendMsg(pl_lis[nowplay],"請蓋\牌\n");
            }
        else {
            string op;
            for(int i = 0; i < player_card_index[nowplay]; i++) {
                if(able_to_play[nowplay][i]) {
                    op+=" ^ ";
                    }
                else {
                    op+="   ";
                    }
                }
            op+="\n請出牌\n";
            SendMsg(pl_lis[nowplay],op);
            }
        while(1) {
            int tp;
            while(1) {
                tp=GetNum(pl_lis[nowplay]);
                if((tp < 0)||(tp >= player_card_index[nowplay])) {
                    SendMsg(pl_lis[nowplay],"請勿輸入超出範圍的整數\n");
                    }
                else{
                    break;
                    }
                }
            if(!tpp) {
                SendCmd(pl_lis[nowplay],"cls");
                cover(tp);
                break;
                }
            else {
                if(play(tp))break;
                }
            }
        }
    pair<int,int> temp(-2147483648,0);
    int tempi = 0;
    int sum = 0;
    for(int i = 0; i < 4; i++) {
        diagram res(1,1);
        int movex = 0;
        for(int j = 0; j < (int)covered[i].size(); j++, movex += 3) {
            res = diagram_add(res, card_picture(covered[i][j]), movex, 0);
            }
        for(Net x:pl_lis)
            SendMsg(x,player_name[i]+"蓋\的牌\n"),res.printout(x);
        }
    for(int i = 0; i < 4; i++) {
        sum += thisscore[i];
        if(pair<int,int>(thisscore[i],covered[i].size()) > temp) {
            temp = pair<int,int>(thisscore[i],covered[i].size());
            tempi = i;
            }
        score[i] += thisscore[i];
        if(covered[i].size() == 13) {
            tempi = i;
            temp = pair<int,int>(2147483647,0);
            }
        }
    score[tempi] -= sum;
    for(int i = 0; i < 4; i++) {
        for(Net x:pl_lis)
            SendMsg(x,player_name[i]+"的分數為"+IntToStr(score[i])+"\n");
        }
    }

void sevens_element::clear_score() {
    for(int i = 0; i < 4; i++) {
        score[i] = 0;
        }
    }
