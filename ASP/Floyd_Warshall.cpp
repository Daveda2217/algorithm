// 모든 정점에서 모든 정점으로 가는 최소비용을 구하는 알고리즘이다.

// 예를 들어
// X에서 Y로 가는 최소 비용 vs X에서 노드 1로 가는 비용 + 노드 1에서 Y로 가는 비용

// 위의 과정을 노드 N만큼 반복해서 구해주면 끝이다.

//-------------------------------------------------------------------------------

// <pseudo code> //

int N; // 정점의 개수
int matrix[N][N];

void Floyd_Warshall()
{
    int answer_matrix[N][N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            answer_matrix[i][j] = matrix[i][j];
        }
    }

    for (int k = 0; k < N; k++)
    { // 거쳐가는 노드
        for (int i = 0; i < N; i++)
        { // 출발 노드
            for (int j = 0; j < N; j++)
            { // 도착 노드
                if (answer_matrix[i][k] + answer_matrix[k][j] < answer_matrix[i][j])
                { // update
                    answer_matrix[i][j] = answer_matrix[i][k] + answer_matrix[k][j];
                }
            }
        }
    }
}
