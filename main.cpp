
/* main.cpp */


#include "graph.hpp"
#include <iostream>
#include <string>
#include <vector>

typedef char T;
typedef int R;

char vertices[] = {'A','B','C','D','E','F'};

R hw( T * const p ) {
    std::cout << *p << std::endl;
    return 0;
}

Graph<char> geeksForGeeks();

int main() {
    std::cout << "start" << std::endl;

    Graph<char> g = geeksForGeeks();
    g.print();
    g.print_arrays();

    std::cout << "BFS from A" << std::endl;
    g.BFS( hw, vertices+0 );

    std::cout << "DFS from D" << std::endl;
    g.BFS( hw, vertices+3 );

    return 0;
}

Graph<char> geeksForGeeks() {

    Graph<char> g;

    for ( uint8_t i; i < 6; i++ ) {
        g.addVertex( vertices+i );
    }

    g.addEdge( vertices+0, vertices+1 );
    g.addEdge( vertices+0, vertices+3 );
    g.addEdge( vertices+0, vertices+4 );
    g.addEdge( vertices+1, vertices+2 );
    g.addEdge( vertices+2, vertices+4 );
    g.addEdge( vertices+2, vertices+5 );
    g.addEdge( vertices+3, vertices+4 );
    g.addEdge( vertices+4, vertices+5 );

    return g;
}
