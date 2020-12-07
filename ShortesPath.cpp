#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <queue>
#include <random>

using namespace std;

template <typename WT>
class Graph
{
public:
    int nV, nE;

    double **p; // 좌표
    vector<pair<int, WT>> *edges;

    Graph() {}
    Graph(const char *_filename, int _type = 0)
    {
        if (_type == 0) // 정점이 숫자이다.
        {
            FILE *input = fopen(_filename, "r");
            fscanf(input, "%d", &nV);              // 정점의 개수
            fscanf(input, "%d", &nE);              // 간선의 개수
            edges = new vector<pair<int, WT>>[nV]; // src와 연결되어 있는 간선

            for (int i = 0; i < nE; i++)
            { // 그래프 정보 받기
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
        else if (_type == 1) // 정점이 알파벳이다.
        {
            FILE *input = fopen(_filename, "r");
            fscanf(input, "%d", &nV);
            fscanf(input, "%d", &nE);
            edges = new vector<pair<int, WT>>[nV];

            for (int i = 0; i < nE; i++)
            {
                int vA, vB;
                char a, b;
                double weight;
                fscanf(input, "%c %c %lf", &a, &b, &weight);
                vA = a - 'A';
                vB = b - 'A';
                edges[vA].push_back(make_pair(vB, (WT)weight));
                edges[vB].push_back(make_pair(vA, (WT)weight));
            }
            fclose(input);
        }
        else
        {
            nV = 1000;
            nE = 5000;

            p = new double *[nV];
            for (int i = 0; i < nV; i++)
            {
                p[i] = new double[2];
            }

            FILE *input = fopen("V.txt", "r"); // 죄표 받기
            for (int i = 0; i < nV; i++)
            {
                fscanf(input, "%lf %lf\n", &p[i][0], &p[i][1]);
            }
            fclose(input);

            input = fopen("E.txt", "r");

            for (int i = 0; i < nE; i++)
            {
                int vA, vB;
                double dist;
                fscanf(input, "%d %d %lf\n", &vA, &vB, &dist);
                edges[vA].push_back(make_pair(vB, (WT)dist));
            }
            fclose(input);
        }
    }
};

// 프림 알고리즘 = 모든 노드들을 최소 비용으로 연결시키는 알고리즘
// 다익스트라 = 두 노드 사이의 최단거리 (시작 노드는 보통 정해짐)

template <typename WT>
WT Prim(Graph<WT> &_g)
{
    bool *visit = new bool[_g.nV]; // 정점 방문 여부
    int *parent = new int[_g.nV];  // 어디서 왔는지 확인
    WT *dist = new WT[_g.nV];      // weight 배열

    for (int i = 0; i < _g.nV; i++)
    {
        visit[i] = false;
        parent[i] = -1;
        dist[i] = (WT)9999999999;
    }

    WT ret = (WT)0; // 최단 거리

    int rootIdx = 0;       // 단순히 0부터 시작
    dist[rootIdx] = (WT)0; // 시작점은 weight가 0이다.
    parent[rootIdx] = 0;   // 시작점은 0이라고 생각

    priority_queue<pair<WT, int>, vector<pair<WT, int>>, greater<pair<WT, int>>> pq; // 거리가 낮은 순서대로 push
    pq.push(make_pair((WT)0, rootIdx));                                              // 시작점 push

    for (;;)
    {
        if (pq.empty()) // 큐가 비어있다면 끝낸다.
            break;

        pair<WT, int> x = pq.top(); // dist, destination 가져오기
        pq.pop();

        if (visit[x.second]) // 이미 방문한 destination이면 pass
            continue;

        visit[x.second] = true;
        ret += x.first;

        for (int i = 0; i < _g.edges[x.second].size(); i++)
        {                                          // 현재 정점과 이웃하는 방문하지 않은 정점에 대해 check
            int idx = _g.edges[x.second][i].first; // 이웃 정점
            WT w = _g.edges[x.second][i].second;   // 이웃 정점까지 가는 거리

            if (!visit[idx] && w < dist[idx])
            { // 방문하지 않았고 거리가 더 짧은게 발견되었다면
                pq.push(make_pair(w, idx));
                parent[idx] = x.second; // 이웃 정점의 부모 설정
                dist[idx] = w;          // 거리 업데이트
            }
        }
    }

    printf("Idx\tV\tDist\tParent\n"); // 정점 / 방문여부 / 거리 / 어디서 왔는지
    for (int i = 0; i < _g.nV; i++)
    {
        printf("%2d\t%d\t%.2lf\t%2d\n", i + 1, (int)visit[i], (double)dist[i], parent[i] + 1);
    }

    delete[] visit;
    delete[] parent;
    delete[] dist;

    return ret;
}

template <typename WT>
WT Dijkstra(Graph<WT> &_g, int _stIdx = 0, int _destIdx = 1)
{
    FILE *output = fopen("edges_dijkstra.txt", "w");
    bool *visit = new bool[_g.nV];
    int *prev = new int[_g.nV]; // path을 알아내기 위한 배열
    WT *dist = new WT[_g.nV];   // stIdx와의 거리

    for (int i = 0; i < _g.nV; i++)
    {
        visit[i] = false;
        prev[i] = -1;
        // dist[i] = (WT)9999999999;
        dist[i] = numeric_limits<WT>::max() / (WT)2;
        // prevent from overflow -> divides 2
    }

    dist[_stIdx] = (WT)0;
    prev[_stIdx] = 0;

    priority_queue<pair<WT, int>, vector<pair<WT, int>>, greater<pair<WT, int>>> pq;
    pq.push(make_pair((WT)0, _stIdx));

    for (;;)
    {
        if (pq.empty())
            break;

        pair<WT, int> x = pq.top();
        pq.pop();

        if (visit[x.second])
            continue;
        visit[x.second] = true;

        for (int i = 0; i < _g.edges[x.second].size(); i++)
        {
            int idx = _g.edges[x.second][i].first; // 현재 정점과 이웃한 정점
            WT w = _g.edges[x.second][i].second;   // 현재 정점과 이웃한 정점 사이의 거리
            fprintf(output, "%d %d\n", x.second, idx);
            if (!visit[idx] && dist[idx] < dist[x.second] + w)
            { // 시작 정점 ~ idx 정점까지의 최단거리 업데이트
                pq.push(make_pair(dist[x.second] + w, idx));
                prev[idx] = x.second;
                dist[idx] = dist[x.second] + w;
            }
        }
    }

    // printf("Idx\tV\tDist\tPrev\n");
    // for (int i = 0; i < _g.nV; i++)
    // {
    //     printf("%2c\t%d\t%.2lf\t%2c\n", i + 'A', (int)visit[i], (double)dist[i], prev[i] + 'A');
    // }

    // // print path
    // for (int i = 0; i < _g.nV; i++)
    // {
    //     vector<int> p;
    //     p.push_back(i);
    //     while (1)
    //     {
    //         if (p[p.size() - 1] == _stIdx)
    //             break;
    //         p.push_back(prev[p[p.size() - 1]]);
    //     }

    //     printf("Path from %c to %c: ", _stIdx + 'A', i + 'A');
    //     for (int j = p.size() - 1; j >= 0; j--)
    //     {
    //         printf(" %c ", 'A' + p[j]);
    //     }
    //     printf("\n");
    // }

    printf("Dist from (0,0) to (1,1) = %lf\n", dist[1]);
    delete[] visit;
    delete[] parent;
    delete[] dist;

    fclose(output);
}

template <typename WT>
WT AStar(Graph<WT> &_g, int _stIdx = 0, int _destIdx = 1)
{
    FILE *output = fopen("edges_astar.txt", "w");

    bool *visit = new bool[_g.nV];
    WT *dist = new WT[_g.nV];
    WT *w = new WT[_g.nV];
    WT *h = new WT[_g.nV]; // 휴리스틱

    for (int i = 0; i < _g.nV; i++)
    {
        visit[i] = false;
        // dist[i] = (WT)9999999999;
        dist[i] = numeric_limits<WT>::max() / (WT)2;
        // prevent from overflow -> divides 2

        w[i] = numeric_limits<WT>::max() / (WT)2;

        h[i] = (_g.p[i][0] - _g.p[_destIdx][0]) * (_g.p[i][0] - _g.p[_destIdx][0]);
        h[i] = (_g.p[i][1] - _g.p[_destIdx][1]) * (_g.p[i][1] - _g.p[_destIdx][1]);
        h[i] = sqrt(h[i]);
    }

    dist[_stIdx] = (WT)0;
    w[_stIndx] = (WT)0;

    priority_queue<pair<WT, int>, vector<pair<WT, int>>, greater<pair<WT, int>>> pq;
    pq.push(make_pair((WT)0, _stIdx));

    for (;;)
    {
        if (pq.empty())
            break;

        pair<WT, int> x = pq.top();
        pq.pop();

        if (visit[x.second])
            continue;

        visit[x.second] = true;

        if (x.second == _destIdx)
            break;

        for (int i = 0; i < _g.edges[x.second].size(); i++)
        {
            int idx = _g.edges[x.second][i].first;
            WT edge_weight = _g.edges[x.second][i].second;
            fprintf(output, "%d %d\n", x.second, idx);

            if (!visit[idx] && w[idx] > dist[x.second] + edge_weight + h[idx])
            {
                pq.push(make_pair(idx, w[idx]));
                dist[idx] = dist[x.second] + edge_weight;
                w[idx] = dist[x.second] + edge_weight + h[idx];
            }
        }
    }

    printf("Dist from (%lf,%lf) to (%lf,%lf) = %lf\n", _g.p[_stIdx][0], _g.p[_stIdx][1], _g.p[_destIdx][0], _g.p[_destIdx][1], dist[1]);
    delete[] visit;
    delete[] dist;

    fclose(output);
}

// 여기서 솔직히 height는 왜 두는지 잘 모르겠다.

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
        { // 간선이 작은 순서대로 거리 측정
            printf("%2d - %2d (Weight: %.2lf)\n", x + 1, y + 1, (double)w);
            ret += w;
            s.Union_Set(x, y);
        }
    }
    return ret;
}

void Generate_Map()
{
    random_device rd;
    default_random_engine gen(rd());
    uniform_real_distribution<double> dist((double)0, (double)1);

    int nV = 1000;
    int nE = 10 * nV;

    pair<double, double> *p = new pair<double, double>[nV]; // (x,y)
    p[0] = make_pair((double)0, (double)0);                 // start
    p[1] = make_pair((double)1, (double)1);                 // destination

    for (int i = 2; i < nV; i++)
    {
        // Get random #
        p[i] = make_pair(dist(gen), dist(gen));
    }

    FILE *output = fopen("V.txt", "w");
    for (int i = 0; i < nV; i++)
    {
        // write all vertices
        fprintf(output, "%lf\t%lf\n", p[i].first, p[i].second);
    }

    fclose(output);

    output = fopen("E.txt", "w");

    pair<double, int> *d = new pair<double, int>[nV]; // (distance,index)

    for (int i = 0; i < nV; i++)
    {
        for (int j = 0; j < nV; j++)
        {
            // Get distance (i와 j사이의 거리)
            d[j].first = (p[i].first - p[j].first) * (p[i].first - p[j].first);
            d[j].first += (p[i].second - p[j].second) * (p[i].second - p[j].second);
            d[j].first = sqrt(d[j].first);
            // Get index
            d[j].second = j;
        }
        // 거리가 짧은 순대로 정렬
        sort(&d[0], &d[nV]);

        // 모든 정점끼리의 거리에서 짧은거 10개만 간선으로 채택한다.
        for (int j = 1; j < 11; j++)
        {
            fprintf(output, "%d %d %lf\n", i, d[j].second, d[j].first); // 정점 -> 정점, 거리
        }
    }
}

int main(int argc, char **argv)
{
    // Graph<int> g(argv[1]);
    // int cost_prim = Prim<int>(g);
    // printf("Cost (Prim) = %d\n", cost_prim);

    // int cost_kruskal = Kruskal<int>(g);
    // printf("Cost (Kruskal) = %d\n", cost_kruskal);

    // Graph<int> g(argv[1], 1);

    // Dijkstra<int>(g, 'K' - 'A');

    // Generate_Map();

    Graph<double> g("", 2);
    Dijkstra<double>(g);
    printf("\n");
    AStar<double>(g);
    return 1;
}