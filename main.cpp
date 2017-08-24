#include "bits/stdc++.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

class MazeGenerator{
    public:
        typedef bitset <8> _BYTE;
        _BYTE* _world;
        int _s, _ptX, _ptY;
        MazeGenerator(int s){
            _world = 0;
            _s = s;
            memset(visit, 0, sizeof visit);
            generate();
        }

    private:
        static const int maxn = 10001;

        vector<int>adj[maxn];
        vector<int>path;
        bool visit[maxn];

        enum directions { NONE, NOR = 1, EAS = 2, SOU = 4, WES = 8 };
        const int CELL_SIZE = 16;

        void generate(){
            _world = new _BYTE[_s * _s];
            ZeroMemory( _world, _s * _s );
            _ptX = rand() % _s; _ptY = rand() % _s;
            carve();
            draw_maze();
            draw_solution(_s);
        }

        void carve(){
            while(true){
                int d = getDirection();
                if( d < NOR ) return;
                switch( d ){
                    case NOR:
                    _world[_ptX + _s * _ptY] |= NOR; _ptY--;
                    _world[_ptX + _s * _ptY] = SOU | SOU << 4;
                break;
                    case EAS:
                    _world[_ptX + _s * _ptY] |= EAS; _ptX++;
                    _world[_ptX + _s * _ptY] = WES | WES << 4;
                break;
                case SOU:
                    _world[_ptX + _s * _ptY] |= SOU; _ptY++;
                    _world[_ptX + _s * _ptY] = NOR | NOR << 4;
                break;
                case WES:
                    _world[_ptX + _s * _ptY] |= WES; _ptX--;
                    _world[_ptX + _s * _ptY] = EAS | EAS << 4;
                }
            }
        }

        int getDirection(){
            int d = 1 << rand() % 4;
            while( true ){
                for( int x = 0; x < 4; x++ ){
                    if( testDir( d ) ) return d;
                    d <<= 1;
                    if( d > 8 ) d = 1;
                }
                d = ( _world[_ptX + _s * _ptY].to_ulong() & 0xf0 ) >> 4;
                if( !d ) return -1;
                switch( d ){
                    case NOR: _ptY--; break;
                    case EAS: _ptX++; break;
                    case SOU: _ptY++; break;
                    case WES: _ptX--; break;
                }
                d = 1 << rand() % 4;
            }
        }

        bool testDir(int d){
            switch(d){
                case NOR: return ( _ptY - 1 > -1 && !_world[_ptX + _s * ( _ptY - 1 )].to_ulong() );
                case EAS: return ( _ptX + 1 < _s && !_world[_ptX + 1 + _s * _ptY].to_ulong() );
                case SOU: return ( _ptY + 1 < _s && !_world[_ptX + _s * ( _ptY + 1 )].to_ulong() );
                case WES: return ( _ptX - 1 > -1 && !_world[_ptX - 1 + _s * _ptY].to_ulong() );
            }
            return false;
        }

        void draw(int x1, int y1, int x2, int y2){
            glBegin(GL_LINES);
             glPointSize(30);
                glColor3f(1.0f, 0.f, 0.0f);
                glVertex2i(x1, y1);glVertex2i(x2, y2);
            glEnd();
        }

        void draw_maze(){
            for( int y = 0; y < _s; y++ ){
                int yy = y * _s;
                for( int x = 0; x < _s; x++ ){
                    _BYTE b = _world[x + yy];
                    int nx = x * CELL_SIZE,
                        ny = y * CELL_SIZE;

                    if(!(b.to_ulong() & NOR ))draw(nx, ny, nx + CELL_SIZE + 1, ny);
                    else adj[x + _s * y].push_back(x + _s * (y - 1));

                    if(!(b.to_ulong() & EAS))draw(nx + CELL_SIZE, ny, nx + CELL_SIZE + 1, ny + CELL_SIZE + 1);
                    else adj[x + _s*y].push_back((x + 1) + _s * y);

                    if(!(b.to_ulong() & SOU))draw(nx, ny + CELL_SIZE, nx+CELL_SIZE + 1, ny + CELL_SIZE);
                    else adj[x + _s * y].push_back(x + _s * (y + 1));

                    if(!(b.to_ulong() & WES))draw(nx, ny, nx, ny + CELL_SIZE + 1);
                    else adj[x + _s * y].push_back((x - 1) + _s * y);
                }
            }
        }
        int dfs(int i, int sz){
            visit[i] = 1;
            path.push_back(i);
            if(i == sz * sz -1){
                return 1;
            }
            for(int k = 0; k < adj[i].size(); k++){
                if(visit[adj[i][k]])continue;
                    if(dfs(adj[i][k], sz))return 1;
            }
            path.pop_back();
            return 0;
        }

        void draw_solution(int sz){
            dfs(0, _s);
            int x1, y1, x2, y2;
            for(int i = 0; i < path.size() - 1; i++ ){
                x1 = (path[i] % sz) * CELL_SIZE;
                y1 = (path[i] / sz) * CELL_SIZE;
                x2 = (path[i + 1] % sz) * CELL_SIZE;
                y2 =  (path[i + 1] / sz) * CELL_SIZE;
                glBegin(GL_LINES);
                glLineWidth(500);
                    glColor3f(0.0f, 1.2f, 0.f);
                    glVertex2i(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2);
                    glVertex2i(x2 + CELL_SIZE / 2, y2 + CELL_SIZE / 2);
                    Sleep(10);
                glEnd();
                glFlush();
            }
        }
};

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    int _s = 50;
    srand(GetTickCount());
    MazeGenerator mg(_s);
    glFlush();
}

void init(){
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-50, 1024, -50, 1024);
}

int main(int argc ,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Maze");

    init();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
