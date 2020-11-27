#include <bits/stdc++.h>

using namespace std;

void set_parent(int parent[], int n)
{
    // 자기 자신을 부모로 설정하자!
    for (int i = 1; i <= n; i++)
        parent[i] = i;
}

int find_parent(int parent[], int x)
{ // 노드 x의 부모를 찾는 함수
    if (parent[x] == x)
        return x;
    else
        return parent[x] = find_parent(parent, parent[x]);
}

void set_union(int parent[], int x, int y)
{                               // 노드 x와 y를 합쳐주는 함수
    x = find_parent(parent, x); // 먼저 x의 부모를 찾고
    y = find_parent(parent, y); // y의 부모를 찾아준다.
    if (x != y)                 // 만약 두 노드의 부모가 서로 다르다면
    {
        parent[y] = x; // 어느 한쪽 노드의 부모를 연결되는 다른 한쪽 노드로 설정해버림.
    }                  // 이 과정을 통해 노드 x의 부모도 x, 노드 y의 부모도 x로 부모가 같아진다 !
}

bool same_parent(int parent[], int x, int y) // vertex x와 y가 서로 같은 부모를 갖는지 아닌지 판단해주는 함수
{
    x = find_parent(parent, x); // 노드 x의 부모 찾기
    y = find_parent(parent, y); // 노드 y의 부모 찾기
    if (x == y)
        return true; // 두 부모가 같은 부모라면, true를 반환
    else
        return false; // 두 부모가 서로 다른 부모라면, false를 반환
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int v, e;
    cin >> v >> e;

    int *parent = new int[v + 1];
    set_parent(parent, v);

    vector<pair<int, pair<int, int>>> E; // cost, src, dest
    vector<int> mst;

    for (int i = 0; i < e; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        E.push_back({c, {a, b}});
    }

    sort(E.begin(), E.end()); // cost가 작은 순대로 정렬하자

    int fmst = 0; // first minimum spanning tree

    for (int i = 0; i < e; i++) // 간선의 수 만큼 순회를 돌면서
    {
        if (!same_parent(parent, E[i].second.first, E[i].second.second))
        { // cycle이 없다면 간선을 추가해준다.
            fmst += E[i].first;
            set_union(parent, E[i].second.first, E[i].second.second);
            mst.push_back(i);
        }
    }

    int smst = 0; // second minimum spanning tree
    int cnt = 0;  // how many edges new mst
    int mv = INT_MAX;
    for (int i = 0; i < mst.size(); i++)
    {
        cnt = 0;
        smst = 0;
        set_parent(parent, v);
        for (int j = 0; j < e; j++)
        {
            if (j == mst[i]) // 기존 mst 간선에서 1개씩 제거한 후 mst를 만들자!
                continue;
            if (!same_parent(parent, E[j].second.first, E[j].second.second))
            {
                smst += E[j].first;
                set_union(parent, E[j].second.first, E[j].second.second);
                cnt++;
            }
        }
        if (cnt != v - 1) // mst는 간선의 개수가 v-1개여야한다!
            continue;

        if (mv > smst)
            mv = smst;
    }

    if (mv == INT_MAX)
        cout << -1 << "\n";
    else
        cout << mv << "\n";

    delete[] parent;

    return 0;
}

/* 더 간단한 mst 구하는 방법 */
/*
int union_parent(int i, int sum){
    int a,b;
    a = find_parent(E[i].second.first);
    b = find_parent(E[i].second.second);
    
    if(a != b){
        parent[b] = a;
        sum+= E[i].first;
    }
    return sum;
}

for(int i=0; i < m;i++){
    fmst = union_parent(i, fmst);
}
*/