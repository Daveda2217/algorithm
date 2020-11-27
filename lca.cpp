/* boj.kr/11438 */
/*
최소 공통 조상
1) sparse table 관리
parent[u][k] == 정점 u의 2^k번째 부모를 뜻한다.
sparse_table에서와 마찬가지로
정점 u의 2^(k+1)의 부모는 2^k번을 2번 이동 시켜주는 것과 같다.
=> parent[u][k+1] = parent[parent[u][k]][k]

2) 깊이
두 정점 깊이 차이는 항상 depth[u] >= depth[v]라고 가정한다.
깊이가 같아 질 때까지 정점 u의 부모를 올려준다.
하지만 깊이가 같아져도 조상이 다를 수 있다.
이럴땐 두 정점 동시에 부모를 올려주면서 같아질때까지 과정을 반복해준다.
*/

#include <bits/stdc++.h>

using namespace std;

const int MAX = 18; // log(2,100000) = 262144
int n, m;           // n개의 정점, m개의 답을 찾아야하는 노드 쌍
int parent[100001][MAX];
int depth[100001];       // 정점의 깊이
vector<int> adj[100001]; // 인접한 리스트

void make_tree_dfs(int curr)
{
    for (int adj_element : adj[curr])
    { //curr과 인접한 모든 element를 순회하자.
        if (depth[adj_element] == -1)
        { // 깊이가 -1이라는 것은 자식 element라는 뜻이다.
            parent[adj_element][0] = curr;
            depth[adj_element] = depth[curr] + 1;
            make_tree_dfs(adj_element);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n; // 트리 노드 개수

    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v; // 트리 정보 입력

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 배열 초기화
    memset(parent, -1, sizeof(parent));
    fill(depth, depth + n + 1, -1); // memset해도 상관없다.
    depth[1] = 0;                   // 루트의 depth는 0이다.
    //parent[1][0] = 1;               // 루트의 부모는 자기자신으로 설정하자.
    make_tree_dfs(1);

    // parent 나머지 부분 채우기
    for (int j = 0; j < MAX - 1; j++)
    {
        for (int i = 2; i <= n; i++)
        {
            if (parent[i][j] != -1) // 부모가 존재한다는 가정하에
                parent[i][j + 1] = parent[parent[i][j]][j];
        }
    }

    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        // 항상 depth[u] >= depth[v]로 설정하자
        if (depth[u] < depth[v])
            swap(u, v);
        int diff = depth[u] - depth[v];

        for (int j = 0; diff; j++) // j < diff가 아닌 점을 유의하자!
        {
            if (diff % 2)
                u = parent[u][j];
            diff /= 2;
        }

        if (u != v) // 깊이는 같지만 lca가 다르다면
        {
            for (int j = MAX - 1; j >= 0; j--)
            {
                if (parent[u][j] != -1 && parent[u][j] != parent[v][j])
                { // 조상이 존재하면서 두 조상이 다르다면(최소 조상을 구하기 위해서)
                    u = parent[u][j];
                    v = parent[v][j];
                }
            }
        }

        if (u != v) // 두 정점이 다르다면 그 다음 부모가 lca가 된다.
            cout << parent[u][0] << "\n";
        else // 두 정점이 같다면 lca이므로 그대로 출력한다.
            cout << u << "\n";
    }

    return 0;
}

/*
예제 입력
15
1 2
1 3
2 4
3 7
6 2
3 8
4 9
2 5
5 11
7 13
10 4
11 15
12 5
14 7
6
6 11
10 9
2 6
7 6
8 13
8 15
*/

/*
예제 출력
2
4
2
1
3
1
*/