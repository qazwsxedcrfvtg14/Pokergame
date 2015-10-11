#ifndef POKER_ELEMENT_H_INCLUDED
#define POKER_ELEMENT_H_INCLUDED
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "diagram.h"
#include <ctime>
#include <string>
using namespace std;

class poker_game {
    protected:
        int player[8][18];//玩家手牌
        vector<string> player_name; //玩家名稱
        int card[52];//抽牌堆和棄牌堆
        int card_index;//抽牌堆頂
        int player_card_index[8];//玩家手牌數
        int garbage_index;//棄牌堆頂
        bool used[52];//記錄已出過的牌
        vector<Net> pl_lis;
    public:
        poker_game();//建構式
        void initial();//初始化
        void swap2card(int a, int b); //交換兩張牌
        void shuffle(int n);//洗牌洗n次
        void rshuffle(int n);//將棄牌堆加入抽牌堆並洗牌
        char getsuit(int card_no);//取得花色
        int getpoint(int card_no);//取得數字
        void display_allcard();//顯示棄牌和抽牌堆
        int draw(int player_no, int card_number = 1);//玩家抽牌；預設抽一張
        void display_player_card(int player_no);//顯示玩家手牌
        void ren(int player_no, string name);//重新命名玩家
        void play_card(int player_no, int card_no);//玩家打牌
        void play_card(int player_no, int* card_no, int _size, bool display = 0);//玩家打複數張牌；請在_size指定打的張數；預設不顯示出打的牌
        int get_hand(int player_no, int card_no);//取得玩家某一張手牌
        int* get_allhand(int player_no); //取得玩家全部手牌
        void sort_player_hand(int player_no, int big, bool suit_first); //將玩家手牌排序；指定最大的牌和花色數字優先排列順序
        bool straight(int* a, int _size);//判斷是否為順子
        bool four_of_a_kind(int* a, int _size);//判斷是否為鐵支
        bool full_house(int* a, int _size);//判斷是否為葫蘆
        bool flush(int* a, int _size);//判斷是否為同花
        bool straight_flush(int* a, int _size);//判斷是否為同花順
        bool three_of_a_kind(int* a, int _size);//判斷是否為三條
        bool two_pairs(int* a, int _size);//判斷是否為兩對
        bool one_pair(int* a, int _size);//判斷是否為對子
        bool over_card(int* a, int _size);//判斷是否為高牌
        string get_name(int player_no); //取得玩家名字
        int get_player_index(int player_no);//取得玩家手牌數
        diagram card_picture(int card_no);//取得牌的圖案
    };

poker_game::poker_game() {
    player_name.assign(8,"");
    int i,j;
    player_name[0] = "player 0";
    player_name[1] = "player 1";
    player_name[2] = "player 2";
    player_name[3] = "player 3";
    player_name[4] = "player 4";
    player_name[5] = "player 5";
    player_name[6] = "player 6";
    player_name[7] = "player 7";
    for(i=0; i<=3; i++) {
        for(j=0; j<=12; j++) {
            player[i][j]=-1;
            }
        }
    for(i=0; i<=51; i++) {
        card[i]=i;
        used[i]=0;
        }
    for(i=0; i<=7; i++) {
        player_card_index[i]=0;
        }
    card_index = 0;
    garbage_index = -1;
    srand(time(NULL));
    }

void poker_game::initial() {
    int i,j;
    player_name.assign(8,"");
    player_name[0] = "player 0";
    player_name[1] = "player 1";
    player_name[2] = "player 2";
    player_name[3] = "player 3";
    player_name[4] = "player 4";
    player_name[5] = "player 5";
    player_name[6] = "player 6";
    player_name[7] = "player 7";
    for(i=0; i<=3; i++) {
        for(j=0; j<=12; j++) {
            player[i][j]=-1;
            }
        }
    for(i=0; i<=51; i++) {
        card[i]=i;
        used[i]=0;
        }
    for(i=0; i<=7; i++) {
        player_card_index[i]=0;
        }
    card_index = 0;
    garbage_index = -1;
    srand(time(NULL));
    }

void poker_game::swap2card(int a, int b) {
    int temp;
    temp = card[a];
    card[a] = card[b];
    card[b] = temp;
    }

void poker_game::shuffle(int n) {
    int i;
    for(i=1; i<=(n+(rand()%2)); i++) {
        int sa = (rand() % (52-card_index))+card_index;
        int sb = (rand() % (52-card_index))+card_index;
        swap2card(sa, sb);
        }
    }

void poker_game::rshuffle(int n) {
    int i;
    for(int j = 0; j < 52; j++) {
        used[j] = 0;
        }
    for(i=1; i<=(n+(rand()%2)); i++) {
        int sa = (rand() % (52 - garbage_index - 1))+ garbage_index + 1;
        int sb = (rand() % (52 - garbage_index - 1))+ garbage_index + 1;
        swap2card(sa, sb);
        }
    card_index = garbage_index + 1;
    }

char poker_game::getsuit(int card_no) {
    char sign[4]= {6,3,4,5};
    char suit;
    suit = sign[ card_no/13 ];
    return suit;
    }

int poker_game::getpoint(int card_no) {
    int point;
    point = card_no%13 + 1;
    return point;
    }

void poker_game::display_allcard() {
    int i;
    char suit;
    int point;
    for(i = garbage_index + 1; i<=51; i++) {
        if(i == card_index) {
            cout << "------------------------------------------------"<<endl;
            }
        suit = getsuit(card[i]);
        point = getpoint(card[i]);
        cout << "(" << setw(2) << i << ")card no. " << setw(2) << card[i] << " " << suit << setw(2);
        switch(point) {
            case 1:
                cout << "A" << endl;
                break;
            case 10:
                cout << "T" << endl;
                break;
            case 11:
                cout << "J" << endl;
                break;
            case 12:
                cout << "Q" << endl;
                break;
            case 13:
                cout << "K" << endl;
                break;
            default:
                cout<< point << endl;
            }
        }
    cout << "------------------------------------------------" << endl << endl;
    }

int poker_game::draw(int player_no, int card_number) {
    int card_no=0;
    int index;
    for(int i = 1; i <= card_number; i++) {
        card_no = card[card_index];
        card[card_index] = -1;
        index = player_card_index[player_no];
        player[player_no][index] = card_no;
        player_card_index[player_no] = player_card_index[player_no] + 1;
        for(int j = card_index-1; j >= garbage_index; j--) {
            card[j+1] = card[j];
            }
        card[garbage_index] = -1;
        garbage_index++;
        card_index = card_index + 1;
        }
    return card_no;
    }

void poker_game::display_player_card(int player_no) {
    int i;
    int mv = 0;
    diagram res(1,1);
    int index;
    string op;
    op+=player_name[player_no]+"\n ";
    index = player_card_index[player_no];
    for(int j = 0; j < index - 1; j++) {
        char s[10];
        sprintf(s,"%3d",j);
        op+=s;
        }
    op+=IntToStr(index-1)+"\n";
    for(i=0; i<index; i++) {
        res = diagram_add(res, card_picture(player[player_no][i]), mv, 0);
        mv += 3;
        }
    SendMsg(pl_lis[player_no],op);
    res.printout(pl_lis[player_no]);
    }

void poker_game::ren(int player_no, string name) {
    player_name[player_no] = name;
    }

void poker_game::play_card(int player_no, int card_no) {
    used[player[player_no][card_no]] = 1;
    card[garbage_index] = player[player_no][card_no];
    garbage_index--;
    for(int i = card_no; i <= player_card_index[player_no] -2; i++) {
        player[player_no][i]=player[player_no][i+1];
        }
    player_card_index[player_no]--;
    player[player_no][player_card_index[player_no]] = -1;
    }

int poker_game::get_hand(int player_no, int card_no) {
    return player[player_no][card_no];
    }

int* poker_game::get_allhand(int player_no) {
    return player[player_no];
    }

void poker_game::sort_player_hand(int player_no, int big, bool suit_first) {
    int comp[13];
    if(suit_first) {
        for(int i = 0; i <= player_card_index[player_no] - 1; i++) {
            int n = player[player_no][i];
            comp[i] = (3 - n/13) * 13 + (n - big + 13)%13;
            }
        }
    else {
        for(int i = 0; i <= player_card_index[player_no] - 1; i++) {
            int n = player[player_no][i];
            comp[i] = (3 - n/13) + (n - big + 13)%13 * 4;
            }
        }
    for(int j = 0; j <= player_card_index[player_no] - 2; j++) {
        for(int k = 0; k <= player_card_index[player_no] - j - 2; k++) {
            if(comp[k] > comp[k+1]) {
                int temp = comp[k];
                comp[k] = comp[k+1];
                comp[k+1] = temp;
                int temp1 = player[player_no][k];
                player[player_no][k] = player[player_no][k+1];
                player[player_no][k+1] = temp1;
                }
            }
        }
    }

bool poker_game::straight(int* a, int _size) {
    bool used1[13];
    for(int i = 0; i <= 12; i++) {
        used1[i] = 0;
        }
    for(int i = 0; i <= _size - 1; i++) {
        used1[getpoint(a[i])-1] = 1;
        }
    for(int i = 0; i <= 3; i++) {
        if(used1[0]&& used1[9] && used1[10] && used1[11] && used1[12]) {
            return 1;
            }
        for(int now = 0; now <= 8; now++) {
            if(used1[now] && used1[now+1] && used1[now+2] && used1[now+3] && used1[now+4]) {
                return 1;
                }
            }
        }
    return 0;
    }

bool poker_game::four_of_a_kind(int* a, int _size) {
    int used1[13];
    for(int i = 0; i <= 12; i++) {
        used1[i] = 0;
        }
    for(int i = 0; i <= _size - 1; i++) {
        used1[getpoint(a[i])-1]++;
        }
    for(int i = 0; i <= 12; i++) {
        if(used1[i] == 4) {
            return 1;
            }
        }
    return 0;
    }

bool poker_game::full_house(int* a, int _size) {
    int used1[13];
    for(int i = 0; i <= 12; i++) {
        used1[i] = 0;
        }
    for(int i = 0; i <= _size - 1; i++) {
        used1[getpoint(a[i])-1]++;
        }
    for(int i = 0; i <= 12; i++) {
        if(used1[i] >= 3) {
            for(int j = 0; j <= 12; j++) {
                if((used1[j] >= 2)&&(j != i)) {
                    return 1;
                    }
                }
            return 0;
            }
        }
    return 0;
    }

bool poker_game::flush(int* a, int _size) {
    for(int i = 0; i <= _size - 2; i++) {
        if(getsuit(a[i])!= getsuit(a[i+1])) {
            return 0;
            }
        }
    return 1;
    }

bool poker_game::straight_flush(int* a, int _size) {
    bool used1[13][4];
    for(int i = 0; i <= 12; i++) {
        for(int j = 0; j <= 3; j++) {
            used1[i][j] = 0;
            }
        }
    for(int i = 0; i <= _size - 1; i++) {
        int x = getpoint(a[i])-1;
        int y = (a[i] / 13);
        used1[x][y] = 1;
        }
    for(int i = 0; i <= 3; i++) {
        if(used1[0][i] && used1[9][i] && used1[10][i] && used1[11][i] && used1[12][i]) {
            return 1;
            }
        for(int now = 0; now <= 8; now++) {
            if(used1[now][i] && used1[now+1][i] && used1[now+2][i] && used1[now+3][i] && used1[now+4][i]) {
                return 1;
                }
            }
        }
    return 0;
    }

bool poker_game::three_of_a_kind(int* a, int _size) {
    int used1[13];
    for(int i = 0; i <= 12; i++) {
        used1[i] = 0;
        }
    for(int i = 0; i <= _size - 1; i++) {
        used1[getpoint(a[i])-1]++;
        }
    for(int i = 0; i <= 12; i++) {
        if(used1[i] >= 3) {
            return 1;
            }
        }
    return 0;
    }

bool poker_game::two_pairs(int* a, int _size) {
    int used1[13];
    for(int i = 0; i <= 12; i++) {
        used1[i] = 0;
        }
    for(int i = 0; i <= _size - 1; i++) {
        used1[getpoint(a[i])-1]++;
        }
    for(int i = 0; i <= 12; i++) {
        if(used1[i] >= 2) {
            for(int j = 0; j <= 12; j++) {
                if((used1[j] >= 2)&&(j != i)) {
                    return 1;
                    }
                }
            return 0;
            }
        }
    return 0;
    }

bool poker_game::one_pair(int* a, int _size) {
    int used1[13];
    for(int i = 0; i <= 12; i++) {
        used1[i] = 0;
        }
    for(int i = 0; i <= _size - 1; i++) {
        used1[getpoint(a[i])-1]++;
        }
    for(int i = 0; i <= 12; i++) {
        if(used1[i] >= 2) {
            return 1;
            }
        }
    return 0;
    }

bool poker_game::over_card(int* a, int _size) {
    return !(one_pair(a, _size)||straight(a, _size)||flush(a, _size));
    }

string poker_game::get_name(int player_no) {
    return player_name[player_no];
    }

int poker_game::get_player_index(int player_no) {
    return player_card_index[player_no];
    }

diagram poker_game::card_picture(int card_no) {
    diagram res(7,8);
    char number1[13] = {'A','2','3','4','5','6','7','8','9','1','J','Q','K'};
    char number2[13] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','0','\0','\0','\0'};
    char suit = getsuit(card_no);
    int point = getpoint(card_no);
    char n1 = number1[point - 1];
    char n2 = number2[point - 1];
    res.change_element(9,n1);
    res.change_element(10,n2);
    res.change_element(17,suit);
    res.drawline(0,0,7,0,'-');
    res.drawline(0,6,7,6,'-');
    res.drawline(0,0,0,6,'|');
    res.drawline(7,0,7,6,'|');
    res.change_element(0,'.');
    res.change_element(7,'.');
    res.change_element(48,'.');
    res.change_element(55,'.');
    return res;
    }



#endif // POKER_ELEMENT_H_INCLUDED
