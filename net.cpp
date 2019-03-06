#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include "parser.h"
#include "parser.cpp"
#include <sstream>

using namespace std;

void ISS(int H, int V, int source_x, int source_y, pair<int,pair<int,int> > **vertex, bool **table, pair<int,int> **pre)
{
    for(int i=0;i<H;i++)
    {
        for(int j=0;j<V;j++)
        {
            vertex[i][j].first=2147483646;
            vertex[i][j].second.first=i;
            vertex[i][j].second.second=j;
            table[i][j]=1;
            pre[i][j].first=-1;
            pre[i][j].second=-1;
        }
    }
    vertex[source_x][source_y].first=0;
    table[source_x][source_y]=0;
}

bool relax(int ax,int ay, int bx, int by  ,pair<int,pair<int,int> > **vertex, pair<int, int> **h_edge, pair<int, int> **v_edge, pair<int,int> **pre ,int cap)
{   //a->b
    int ex,ey, edge_cost;
    if(ax==bx)
    {
        ex=ax;
        ey=min(ay,by);
        edge_cost=v_edge[ex][ey].first;
    }
    else if(ay==by)
    {
        ey=ay;
        ex=min(ax,bx);
        edge_cost=h_edge[ex][ey].first;
    }
    if(vertex[bx][by].first > vertex[ax][ay].first + edge_cost)
    {
        vertex[bx][by].first=vertex[ax][ay].first + edge_cost;
        pre[bx][by].first=ax;
        pre[bx][by].second=ay;
        return 1;
    }
    return 0;
}

string to_strin(int a)
{
    ostringstream ama;
    ama<<a;
    return ama.str();
}

int main(int argc, char **argv)
{
    AlgParser parser;
    // read the file in the first argument
    if( ! parser.read( argv[1]) ) { return 1; }
    //construct graph
    int V=parser.gNumVTiles(),H=parser.gNumHTiles();
    pair<int,pair<int,int> > **vertex= new pair<int,pair<int,int> > *[H], tem;
    bool **table = new bool *[H];
    pair<int,int> **pre = new pair<int,int> *[H];
    pair<int, int> **h_edge = new pair<int, int> *[H-1];
    pair<int, int> **v_edge = new pair<int, int> *[H];
    //edge cost initialize
    for(int i=0;i<H;i++)
    {
        v_edge[i] = new pair<int, int> [V-1];
        vertex[i] = new pair<int,pair<int,int> > [V];
        table[i] = new bool [V];
        pre[i] = new pair<int,int> [V];
    }
    for(int i=0;i<H-1;i++)
    {
        h_edge[i] = new pair<int, int> [V];
    }
    for(int i=0;i<H-1;i++)
    {
        for(int j=0;j<V;j++)
        {
            h_edge[i][j].first=1;//cost
            h_edge[i][j].second=0;//demand
        }
    }
    for(int i=0;i<H;i++)
    {
        for(int j=0;j<V-1;j++)
        {
            v_edge[i][j].first=1;//cost
            v_edge[i][j].second=0;//demand
        }
    }
    //sort
    pair <double,int> *order = new pair <double,int> [parser.gNumNets()];
    int frac1,frac2;
    for (int idNet = 0; idNet < parser.gNumNets(); idNet++)
    {
        pair<int, int> posS = parser.gNetStart( idNet );
        pair<int, int> posE = parser.gNetEnd( idNet );
        order[idNet].second=idNet;
        frac1 = max(abs(posS.second-posE.second), abs(posS.first-posE.first)) ;
        frac2 = min(abs(posS.second-posE.second), abs(posS.first-posE.first)) ;
        order[idNet].first=0;
        for(int i=frac1+frac2; i>frac1 ; i--)
        {
            order[idNet].first += log10 (static_cast<double>(i));
        }
        if(frac2 > 1)
            order[idNet].first -= log10 ( frac2 );
    }
    sort(order,order+parser.gNumNets());
    //reverse(order,order+parser.gNumNets());

    //Dijkstra //Near first
    priority_queue< pair<int,pair<int,int> >,vector< pair<int,pair<int,int> > > , greater < pair<int,pair<int,int> > > > pq;
    int cap=parser.gCapacity(), x, y, total_step=0;
    string *final_answer = new string [parser.gNumNets()];
    int overflow=0;
    for (int i = 0; i < parser.gNumNets(); i++){
        int idNet = order[i].second;
        pair<int, int> posS = parser.gNetStart( idNet );
        pair<int, int> posE = parser.gNetEnd( idNet );
        ISS(H, V, posE.first, posE.second, vertex, table, pre);//start from end
        pq.push(vertex[posE.first][posE.second]);
        while(!pq.empty())
        {
            tem=pq.top();
            pq.pop();
            x=tem.second.first; y=tem.second.second;
            if( table[x][y] ==0 && x!=posE.first && y!=posE.second )//re pop?
            {continue;}
            else
            {
                table[x][y]=1;
            }
            if(tem.first > vertex[posS.first][posS.second].first) {break;}//distance optimize
            if(x+1<H && y<V )//->
            {
                if(relax(x,y,x+1,y,vertex,h_edge,v_edge,pre,cap))
                {
                    pq.push(vertex[x+1][y]);
                }
            }
            if(x>0 && y<V )//<-
            {
                if(relax(x,y,x-1,y,vertex,h_edge,v_edge,pre,cap))
                {
                    pq.push(vertex[x-1][y]);
                }
            }
            if(x<H && y+1<V )//^
            {
                if(relax(x,y,x,y+1,vertex,h_edge,v_edge,pre,cap))
                {
                    pq.push(vertex[x][y+1]);
                }
            }
            if(x<H && y>0 )//down
            {
                if(relax(x,y,x,y-1,vertex,h_edge,v_edge,pre,cap))
                {
                    pq.push(vertex[x][y-1]);
                }
            }
        }
        string ans;
        int step=0;
        x=posS.first;y=posS.second;
        //trace back & cost verify
        int ex,ey;
        while(pre[x][y].first != -1 && pre[x][y].second != -1)
        {//ab->xy
            step++;
            int a=pre[x][y].first,b=pre[x][y].second;
            ans+=to_strin(x)+' '+to_strin(y)+' '+to_strin(a)+' '+to_strin(b)+'\n';
            if(a==x)//v
            {
                ex=a;
                ey=min(b,y);
                v_edge[ex][ey].second+=1;
                if(v_edge[ex][ey].second >= cap)
                {
                    v_edge[ex][ey].first=pow(2,20);//*v_edge[ex][ey].second)/cap
                    if(v_edge[ex][ey].second > cap)
                    {
                        overflow++;
                    }     
                }
                else
                {
                    v_edge[ex][ey].first=5;
                }
            }    
            else if(b==y)//h
            {
                ey=b;
                ex=min(a,x);
                h_edge[ex][ey].second+=1;
                if(h_edge[ex][ey].second >= cap)
                {
                    h_edge[ex][ey].first=pow(2,20);
                    if(h_edge[ex][ey].second > cap)
                    {
                        overflow++;
                    } 
                }
                else
                {
                    h_edge[ex][ey].first=3;
                }
            }
            x=a;y=b;
        }
        final_answer[idNet]= to_strin(idNet)+' '+to_strin(step)+'\n'+ans;
        while(!pq.empty())
            pq.pop();
        total_step+=step;
    }
    //cout<<"Total step:"<<total_step<<" Overflow: "<<overflow<<endl;
    //output file
    fstream output;
    output.open( argv[2] , ios::out);
    for(int i=0;i<parser.gNumNets();i++)
    {
        output<<final_answer[i];
    }
    
    output.close();
    for(int i=0;i<H-1;i++)
    {
        delete [] h_edge[i];
    }
    for(int i=0;i<H;i++)
    {
        delete [] vertex[i];
        delete [] v_edge[i];
        delete [] table[i];
        delete [] pre[i];
    }
    delete [] vertex;
    delete [] h_edge;
    delete [] v_edge;
    delete [] pre;
    delete [] table;
    delete [] order;
    delete [] final_answer;
    return 0;
}