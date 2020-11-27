#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <queue>

using namespace std;

template <typename WT>
class Graph
{
public:
    int nV, nE;

    vector<pair<int, WT>> *edges;

    Graph() {}
    Graph(const char *_filename)
    {
        FILE *input = fopen(_filename, "r");
        fscanf(input, "%d", &nV);
        fscanf(input, "%d", &nE);
        edges = new vector<pair<int, WT>>[nV];

        for (int i = 0; i < nE; i++)
        {
            int vA, vB;
            double weight;
            fscanf(input, "%d %d %lf", &vA, &vB, &weight);
            vA--;
            vB--;
            edges[vA].push_back(make_pair(vB, (WT)weight));
            edges[vB].push_back(make_pair(vA, (WT)weight));
        }
        fclose(input);
    }
};

/*
프림스 알고리즘은 시작점을 정하고
시작점 외의 거리는 무한대로 초기 설정을 한다.
그 다음 우선순위 큐를 이용해
weight가 작은 순서대로 방문 체크를 해가며 거리를 업데이트하는 것이 핵심이다.

필요조건
priority_queue<pair<WT, int>, vector<pair<WT, int>>, greater<pair<WT, int>>>
visit[v]
parent[v]
dist[v]

내가 볼땐 다익스트라와 매우 유사하지만
거리 계산을 좀 더 쉽게 한다는 점에서 
유용한 알고리즘이라고 생각한다.
*/
template <typename WT>
WT Prim(Graph<WT> &_g)
{
    bool *visit = new bool[_g.nV];
    int *parent = new int[_g.nV];
    WT *dist = new WT[_g.nV];

    for (int i = 0; i < _g.nV; i++)
    {
        visit[i] = false;
        parent[i] = -1;
        dist[i] = (WT)9999999999;
    }

    WT ret = (WT)0;

    int rootIdx = 0;
    dist[rootIdx] = (WT)0;
    parent[rootIdx] = 0;

    priority_queue<pair<WT, int>, vector<pair<WT, int>>, greater<pair<WT, int>>> pq;
    pq.push(make_pair((WT)0, rootIdx));

    for (;;)
    {
        if (pq.empty())
            break;

        pair<WT, int> x = pq.top();
        pq.pop();

        if (visit[x.second])
            continue;
        visit[x.second] = true;
        ret += x.first;

        for (int i = 0; i < _g.edges[x.second].size(); i++)
        {
            int idx = _g.edges[x.second][i].first;
            WT w = _g.edges[x.second][i].second;
            if (!visit[idx] && w < dist[idx])
            {
                pq.push(make_pair(w, idx));
                parent[idx] = x.second;
                dist[idx] = w;
            }
        }
    }

    printf("Idx\tV\tDist\tParent\n");
    for (int i = 0; i < _g.nV; i++)
    {
        printf("%2d\t%d\t%.2lf\t%2d\n", i + 1, (int)visit[i], (double)dist[i], parent[i] + 1);
    }

    delete[] visit;
    delete[] parent;
    delete[] dist;

    return ret;
}

/*
여기서 솔직히 height는 왜 두는지 잘 모르겠다.
*/
class Disjoint_set
{
public:
    int n;
    int *parent;
    int *height;

    Disjoint_set() {}
    Disjoint_set(int _n)
    {
        this->n = _n;
        parent = new int[_n];
        height = new int[_n];
        for (int i = 0; i < _n; i++)
        {
            parent[i] = i;
            height[i] = 0;
        }
    }
    ~Disjoint_set()
    {
        delete[] parent;
        delete[] height;
    }

    int Find_Set(int _x)
    {
        while (parent[_x] != _x)
        {
            _x = parent[_x];
        }
        return _x;
    }

    bool Union_Set(int _x, int _y)
    {
        _x = Find_Set(_x);
        _y = Find_Set(_y);

        if (_x == _y)
            return false;

        if (height[_x] == height[_y])
        {
            parent[_y] = _x;
            height[_x]++;
        }
        else if (height[_x] > height[_y])
        {
            parent[_y] = _x;
        }
        else
        {
            parent[_x] = _y;
        }
    }
};

/*
크루스칼은 모든 간선에 대한 weight 중
작은 weight부터 하나씩 연결 시켜가면서
cycle이 있는지 판단 후 없으면 연결시키는 알고리즘이다.

필요조건
vector<pair<WT, pair<int, int>>> 
disjoint_set
*/
template <typename WT>
WT Kruskal(Graph<WT> &_g)
{
    vector<pair<WT, pair<int, int>>> d;

    for (int i = 0; i < _g.nV; i++)
    {
        for (int j = 0; j < _g.edges[i].size(); j++)
        {
            if (i < _g.edges[i][j].first)                                                          // 중복 방지
                d.push_back(make_pair(_g.edges[i][j].second, make_pair(i, _g.edges[i][j].first))); // weight, (src, dest)
        }
    }

    sort(d.begin(), d.end()); // weight 작은 순서대로 정렬

    Disjoint_set s(_g.nV);
    WT ret = (WT)0;

    for (int i = 0; i < d.size(); i++)
    {
        int x = d[i].second.first;
        int y = d[i].second.second;
        WT w = d[i].first;

        if (s.Find_Set(x) != s.Find_Set(y))
        {
            printf("%2d - %2d (Weight: %.2lf)\n", x + 1, y + 1, (double)w);
            ret += w;
            s.Union_Set(x, y);
        }
    }
    return ret;
}

/*
다음에 다룰 다익스트라 알고리즘은
두 정점 사이의 최단거리를 구하는 알고리즘이다.
*/
int main(int argc, char **argv)
{
    Graph<int> g(argv[1]);
    int cost_prim = Prim<int>(g);
    printf("Cost (Prim) = %d\n", cost_prim);

    int cost_kruskal = Kruskal<int>(g);
    printf("Cost (Kruskal) = %d\n", cost_kruskal);

    return 1;
}