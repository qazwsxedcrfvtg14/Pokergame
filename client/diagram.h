#include"Jtol.h"
#ifndef DIAGRAM_H_INCLUDED
#define DIAGRAM_H_INCLUDED

#include <iostream>
#include <cmath>
#include <queue>
using namespace std;
using namespace Jtol;
void SendMsg(Net id,string mg149) {
    NetSend(id,mg149.c_str());
    }
void SendMsg(vector<Net> ids,string mg149) {
    for(Net nt:ids)
        NetSend(nt,mg149.c_str());
    }
int GetNum(Net id) {
    Time tim=GetTime();
    string get;
    while(1) {
        const char *buf=NetGet(id);
        if(strlen(buf)) {
            get+=buf;
            tim=GetTime();
            }
        Sleep(15);
        }
    tim=GetTime();
    while(GetTime()-tim<100) {
        const char *buf=NetGet(id);
        if(strlen(buf)) {
            get+=buf;
            tim=GetTime();
            }
        Sleep(15);
        }
    return StrToInt(get);
    }

class diagram {
        int height;//高度
        int width;//寬度
        char element[1000];//內容
    public:
        diagram(int h, int w);;//建構式，請給定高度和寬度
        int element_no(int h_no, int w_no) {
            return (h_no*width + w_no);
            };//將高度座標和寬度座標轉換成元素位置
        int get_x(int no) {
            return (no % width);
            };//取得元素位置的x座標
        int get_y(int no) {
            return (no / width);
            };//取得元素位置的y座標
        int get_height() {
            return height;
            };//取得高度
        int get_width() {
            return width;
            };//取得寬度
        char get_element(int no) {
            return element[no];
            };//取得元素
        void change_element(int no, char c) {
            element[no] = c;
            };//置換元素
        template<typename T>
        void printout(T id);//印出圖像
        void drawline(int x1, int y1, int x2, int y2,char c = '.');//畫直線
        void operator=(diagram d);//設值運算子
        void color(int x, int y, char c);//塗滿
        void invisible(int x, int y) {
            color(x, y, 1);
            };//透明化
    };

diagram::diagram(int h, int w) {
    height = h;
    width = w;
    int k = height * width;
    if(height < 0) {
        height = -height;
        }
    if(width < 0) {
        width = -width;
        }
    if(k > 1000000) {
        height = 1000000/width;
        }
    for(int i = 0; i <= height - 1; i++) {
        for(int j = 0; j <= width - 1; j++) {
            element[element_no(i,j)] = '\0';
            }
        }
    }
template<typename T>
void diagram::printout(T id){
    string output;
    for(int i = 0; i <= height - 1; i++) {
        for(int j = 0; j <= width - 1; j++) {
            if(element[element_no(i,j)] == 1) {
                output+=' ';
                //cout<<' ';
                }
            else {
                output+=element[element_no(i,j)];
                //cout<<element[element_no(i,j)];
                }
            }
        output+="\n";
        //cout<<endl;
        }
    SendMsg(id,output);
    }

void diagram::drawline(int x1, int y1, int x2, int y2, char c) {
    if(x1 == x2) {
        if((0 <= x1)&&(x1 < width)) {
            if(y1 > y2) {
                int temp = y2;
                y2 = y1;
                y1 = temp;
                }
            for(int i = y1; i <= y2; i++) {
                if((0 <= i) && (i < height)) {
                    element[element_no(i,x1)] = c;
                    }
                }
            }
        return;
        }
    if(y1 == y2) {
        if((0 <= y1)&&(y1 < height)) {
            if(x1 > x2) {
                int temp = x2;
                x2 = x1;
                x1 = temp;
                }
            for(int i = x1; i <= x2; i++) {
                if((0 <= i) && (i < width)) {
                    element[element_no(y1,i)] = c;
                    }
                }
            }
        return;
        }
    int x = x1;
    int y = y1;
    element[element_no(y,x)] = '.';
    while((x != x2)||(y != y2)) {
        int mx = (y-y1)*(x2-x1)-((x+1)-x1)*(y2-y1);
        int my = ((y+1)-y1)*(x2-x1)-(x-x1)*(y2-y1);
        int ax = abs(mx);
        int ay = abs(my);
        if(ax < ay) {
            x++;
            }
        else {
            y++;
            }
        element[element_no(y,x)] = '.';
        }
    }

void diagram::color(int x, int y, char c) {
    int now = element_no(y,x);
    if(element[now] == c) {
        return;
        }
    queue<int> bfs;
    char init = element[now];
    bfs.push(now);
    element[now] = c;
    while(!bfs.empty()) {
        now = bfs.front();
        if(get_y(now) >= 1) {
            if(element[now-width] == init) {
                bfs.push(now-width);
                element[now-width] = c;
                }
            }
        if(get_y(now) <= height - 2) {
            if(element[now+width] == init) {
                bfs.push(now+width);
                element[now+width] = c;
                }
            }
        if(get_x(now) >= 1) {
            if(element[now-1] == init) {
                bfs.push(now-1);
                element[now-1] = c;
                }
            }
        if(get_x(now) <= width - 2) {
            if(element[now+1] == init) {
                bfs.push(now+1);
                element[now+1] = c;
                }
            }
        bfs.pop();
        }
    }

void diagram::operator=(diagram d) {
    height = d.get_height();
    width = d.get_width();
    for(int i = 0; i <= (height*width - 1); i++) {
        change_element(i, d.get_element(i));
        }
    }

diagram diagram_add(diagram d1, diagram d2, int move_x, int move_y, bool d2_up = 1) {
    int h1 = d1.get_height(), h2 = d2.get_height(), w1 = d1.get_width(), w2 = d2.get_width();
    int h = max(h1,(h2 + move_y));
    int w = max(w1,(w2 + move_x));
    diagram res(h,w);
    if(d2_up) {
        for(int i = 0; i <= (h1*w1 - 1); i++) {
            res.change_element(res.element_no(d1.get_y(i),d1.get_x(i)),d1.get_element(i));
            }
        for(int i = 0; i <= (h2*w2 - 1); i++) {
            if(d2.get_element(i) != 1) {
                res.change_element(res.element_no((d2.get_y(i)+move_y),(d2.get_x(i)+move_x)),d2.get_element(i));
                }
            }
        return res;
        }
    for(int i = 0; i <= (h2*w2 - 1); i++) {
        res.change_element(res.element_no((d2.get_y(i)+move_y),(d2.get_x(i)+move_x)),d2.get_element(i));
        }
    for(int i = 0; i <= (h1*w1 - 1); i++) {
        if(d1.get_element(i) != 1) {
            res.change_element(res.element_no(d1.get_y(i),d1.get_x(i)),d1.get_element(i));
            }
        }
    return res;
    }

#endif // DIAGRAM_H_INCLUDED
