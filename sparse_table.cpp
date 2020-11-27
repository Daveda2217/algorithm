/* boj.kr/17435 */
/*
희소 테이블 = 공간 안에 상대적으로 굉장히 적은 양의 값만 존재한다.

모든 정점의 나가는 간선이 정확히 1개인 유향 그래프가 있다고 가정하자.
어떤 정점에서 시작해서 정확히 k번 간선을 타고 나가면 
k번의 이동 후 도착점은 유일하게 정해진다.

여기서 k가 굉장히 크다면 시간이 오래걸린다.

만약 우리가 어떤 정점의 바로 다음 정점 말고, 2번 이동했을 때 도달하는 유일한 정점이
어딘지 정점마다 다 기억해두고 있다면, 우리가 만약 3번 이동하고 싶다고 할 때
이 배열을 사용해서 2번 이동을 한번에 뛰고 간선을 1번 타면 2번의 값 참조만으로 도착지를 알 수 있다. 

k값에 구애받지 않기 위해서
1,2,4,8,16, ... 번의 이동에 대한 배열을 모조리 만들자!

k의 최대값이 M이라고 하면 배열의 개수는 O(logM)개 필요하다.
*/

#include <bits/stdc++.h>

using namespace std;

const int MAX = 500001; // 최대 이동 횟수
const int MAX_D = 19;   // 2^19 = 524288

int m; // 정점의 개수
// next_vertex[i][j] => 정점 i에서 2^j번 이동한 후의 정점
int next_vertex[MAX][MAX_D];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> m;

    for (int i = 1; i <= m; i++) // 정점 -> 정점
        cin >> next_vertex[i][0];

    // 배열 채우기
    // 작은 j부터 채워나가자.
    // i에서 2^(j+1)번 이동한 후의 정점은 i에서 2^j번을*2번 이동하는 것이다.
    // next_vertex[i][j+1] = next_vertex[next[i][j]][j]
    for (int j = 1; j < MAX_D; j++)
        for (int i = 1; i <= m; i++)
            next_vertex[i][j] = next_vertex[next_vertex[i][j - 1]][j - 1];

    int query;
    cin >> query; // 쿼리의 개수

    for (int i = 0; i < query; i++)
    {
        int n, x; // x에서 시작해서 n번 이동한다.
        cin >> n >> x;
        for (int j = MAX_D - 1; j >= 0; j--)
        {
            if (n >= 1 << j) // 2^j번 한 번에 이동할 수 있다면
            {
                n -= 1 << j;           // 2^j번 이미 이동했으니 이동횟수에서 제거한다.
                x = next_vertex[x][j]; // 다음 정점으로 이동하자.
            }
        }
        cout << x << "\n"; // n번 이동후의 x 값을 출력한다.
    }
    return 0;
}

/*
예제 입력
5
3 3 5 4 3
5
1 1
2 1
11 3
1000 4
5 1
*/

/*
예제 출력
3
5
5
4
3
*/