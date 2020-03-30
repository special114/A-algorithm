/* Shortest path algorithm A*
 * This algoithm is more efficient than Dijkstra's algorithms
 * because includes heuristic
 * author: Rafał Surdej
 * 01.2020
 */


#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define N 7

typedef struct Vertex{
    int x;
    int y;
    int cost;
    int g_score;
    int h_score;
    int f_score;
    Vertex* prev;
} Vertex;


int** path(Vertex* start, Vertex* goal)
{

    int** out_board = new int*[N];
    for (int i = 0; i < N; ++i)
    {
        out_board[i] = new int[N];
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            out_board[i][j] = -1;
        }
    }
    Vertex* tmp = goal;
    while (tmp != start)
    {
        out_board[tmp->x][tmp->y] = tmp->cost;
        tmp = tmp->prev;
    }
    out_board[start->x][start->y] = start->cost;
    return out_board;
}

vector<Vertex*> findNeighbours( Vertex (&board)[N][N], Vertex* tmp )
{
    vector<Vertex*> neighbours;
    neighbours.clear();
    if (tmp->x - 1 >= 0 )
        neighbours.push_back(&board[tmp->x - 1][tmp->y]);
    if (tmp->x + 1 <= 6)
        neighbours.push_back(&board[tmp->x + 1][tmp->y]);
    if (tmp->y + 1 <= 6)
        neighbours.push_back(&board[tmp->x][tmp->y + 1]);
    if (tmp->y - 1 >= 0)
        neighbours.push_back(&board[tmp->x][tmp->y - 1]);

    return neighbours;
}


int** a_alg( Vertex (&board)[N][N], Vertex &start, Vertex &goal )
{
    vector<Vertex*> closedset;
    vector<Vertex*> openset;
    vector<Vertex*>::iterator it;
    openset.push_back( &start );
    start.g_score = 0;

    while( openset[0] != nullptr ){
        size_t i;
        Vertex *tempVert = openset[0];
        it = openset.begin();

        for( i=1; i < openset.size(); ++i ){
            if( tempVert->f_score > openset[i]->f_score ){
                tempVert = openset[i];
                it = openset.begin() + (i - 1);
            }
        }
        if( tempVert->x == goal.x && tempVert->y == goal.y ){
            return path(&start, &goal);
        }
        openset.erase( it );
        closedset.push_back( tempVert );
        vector<Vertex*> neighbours = findNeighbours( board, tempVert );
        for( Vertex* neigh: neighbours )
        {
            for( i=0; i < closedset.size() && closedset[i] != neigh; ++i );
            if ( i == closedset.size() )
            {
                int temp_g_score  = tempVert->g_score + neigh->cost;
                bool is_better = false;
                size_t j;
                for( j=0; j < openset.size() && openset[j] != neigh; ++j );
                if ( j == openset.size() ){
                    openset.push_back(neigh);
                    neigh->h_score = abs(goal.x - neigh->x) + abs(goal.y - neigh->y);
                    is_better = true;
                }
                else if ( temp_g_score < neigh->g_score )
                    is_better = true;
                if ( is_better ){
                    neigh->prev = tempVert;
                    neigh->g_score = temp_g_score;
                    neigh->f_score = neigh->g_score + neigh->h_score;
                }
            }
         }
      }
    return nullptr;
}

void createTable( Vertex (&board)[N][N] ){
    int insert_table[N*N]{ 9, 9, 9, 9, 9, 9, 0,
                           9, 9, 9, 9, 9, 9, 1,
                           9, 9, 9, 9, 1, 1, 1,
                           9, 9, 9, 9, 1, 9, 9,
                           9, 9, 9, 9, 1, 1, 9,
                           9, 9, 9, 9, 9, 1, 9,
                           0, 1, 1, 1, 1, 1, 9};

    for( int i=0; i<N; ++i ){
        for(int j=0; j<N; ++j){
            board[i][j].cost = insert_table[N*i+j];
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].g_score = 0;
            board[i][j].f_score = 0;
        }
    }
}



int main()
{

    Vertex board[N][N];
    createTable(board);
    int** output = a_alg(board, board[0][6], board[6][0] );
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (output[i][j] != -1)
                cout << output[i][j];
            else
                cout << " ";
            cout << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < N; ++i)
    {
        delete [] output[i];
    }
    delete [] output;
    return 0;
}
