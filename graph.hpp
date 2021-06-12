
/* graph.hpp */


#ifndef __CSR_GRAPH_H
#define __CSR_GRAPH_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <queue>




/* ================================================= */
/* ================== Declarations ================= */
/* ================================================= */

class NodeDoesNotExist { };
class NotYetSupported { };

template <class T>
class Graph {
    public:
        Graph() : n(0), m(0) {
            vertex_array.push_back(0);
        }

        T * addVertex( T * const );
        void addEdge( T * const, T * const );

        void removeVertex( T * const ); // not yet supported
        void removeEdge( T * const, T * const );

        bool hasVertex( T * const p ) const { return ( ids.find( p ) != ids.end() ); }
        bool hasEdge( T * const, T * const ) const;

        void print_arrays() const;
        void print() const;

        template <class R>
        std::unordered_map<T*,R> BFS( R(* const action)(T * const), T * start = 0 );
        template <class R>
        std::unordered_map<T*,R> DFS( R(* const action)(T * const), T * start = 0 );

    private:
        std::vector<size_t> edge_array, vertex_array;
        std::unordered_map<T*,size_t> ids;
        std::unordered_map<size_t,T*> values;
        size_t n, m;

        size_t getId( T * const p ) const {
            if ( !hasVertex(p) )
                throw NodeDoesNotExist();
            return ids.at( p );
        }
        T * getValue( size_t id ) const {
            if ( id >= n )
                throw NodeDoesNotExist();
            return values.at(id);
        }
        template <class R>
        void DFS_call( R(* const action)(T * const), T * p, std::vector<bool> & visited, std::unordered_map<T*,R> & out );
};





/* ================================================= */
/* ================= Implementations =============== */
/* ================================================= */


/* ======== Modifiers ========*/

template <class T>
T * Graph<T>::addVertex( T * const p ) {
    // ensure vertex has not been added yet
    if ( hasVertex(p) ) return p;

    std::cout << "Adding vertex for " << *p << "\n";

    // give vertex id
    ids[p] = n;
    values[n] = p;
    n++;

    // add vertex to vertecies list
    vertex_array.pop_back();
    vertex_array.push_back(2*m);
    vertex_array.push_back(2*m);

    return p;
}

template <class T>
void Graph<T>::addEdge( T * const p1, T * const p2 ) {
    addVertex(p1);
    addVertex(p2);

    if (
        hasEdge(p1,p2)
        || p1 == p2
    ) return;

    size_t edge_offset = vertex_array.at( getId(p1)+1 );
    edge_array.insert( edge_array.begin() + edge_offset, getId(p2) );
    for ( size_t i = getId(p1)+1; i < vertex_array.size(); i++ ) {
        vertex_array.at(i)++;
    }

    edge_offset = vertex_array.at( getId(p2)+1 );
    edge_array.insert( edge_array.begin() + edge_offset, getId(p1) );
    for ( size_t i = getId(p2)+1; i < vertex_array.size(); i++ ) {
        vertex_array.at(i)++;
    }

    m++;
}

template <class T>
void Graph<T>::removeEdge( T * const p1, T * const p2 ) {
    if (
        !hasEdge(p1,p2)
        || p1 == p2
    ) return;

    print_arrays();
    // delete endpoint 1
    size_t first = vertex_array.at( getId(p1) );
    size_t last = vertex_array.at( getId(p1)+1 );
    edge_array.erase( find( edge_array.begin() + first, edge_array.begin() + last, getId(p2) ) );
    for ( size_t i = getId(p1)+1; i < vertex_array.size(); i++ ) {
        vertex_array.at(i)--;
    }

    print_arrays();

    // delete endpoint 2
    first = vertex_array.at( getId(p2) );
    last = vertex_array.at( getId(p2)+1 );
    edge_array.erase( find( edge_array.begin() + first, edge_array.begin() + last, getId(p1) ) );
    for ( size_t i = getId(p2)+1; i < vertex_array.size(); i++ ) {
        vertex_array.at(i)--;
    }
    print_arrays();

}

// inccomplete
template <class T>
void Graph<T>::removeVertex( T * const p ) {
    if ( !hasVertex(p) ) return;

    throw NotYetSupported();

    std::vector<T*> neighbors;

    for ( size_t i = vertex_array.at(getId(p)); i < vertex_array.at(getId(p)+1); i++ ) {
        neighbors.push_back( getValue( edge_array.at(i) ) );
    }

    for ( const auto p2 : neighbors ) {
        removeEdge( p, p2 );
    }

}

/* ======== Getters ========*/

template <class T>
bool Graph<T>::hasEdge( T * const p1, T * const p2 ) const {
    bool out = false;
    for ( size_t edge_id = vertex_array.at(getId(p1)); edge_id < vertex_array.at(getId(p1)+1); edge_id++ ) {
        if ( edge_array.at(edge_id) == getId(p2) )
            out = true;
    }
    return out;
}

/* ======== Viewers ========*/

template <class T>
void Graph<T>::print_arrays() const {
    std::cout << "edge_array\n";
    for ( const auto e : edge_array )
        std::cout << e << " ";
    std::cout << std::endl;
    std::cout << "vertex_array\n";
    for ( const auto v : vertex_array )
        std::cout << v << " ";
    // std::cout << std::endl;
    // for ( const auto id : ids )
    //     std::cout << *id.first << " " << id.second << " ";
    std::cout <<"\n"<< std::endl;
}

template <class T>
void Graph<T>::print() const {
    for ( size_t vertex_id = 0; vertex_id < vertex_array.size()-1; vertex_id++ ) {
        for ( size_t edge_id = vertex_array.at(vertex_id); edge_id < vertex_array.at(vertex_id+1); edge_id++ ) {
            size_t neighbor_id = edge_array.at(edge_id);
            if ( neighbor_id < vertex_id )
                std::cout << "( " << *( values.at(neighbor_id) ) << ", " << *( values.at(vertex_id) ) << " )\n";
        }
    }
    std::cout << std::endl;
}




/* ================================================= */
/* ================== Algorithms =================== */
/* ================================================= */

template <class T>
template <class R>
std::unordered_map<T*,R> Graph<T>::BFS( R(* const action)(T * const), T * start ) {
    std::unordered_map<T*,R> out;

    if ( n == 0 ) return out;

    std::vector<bool> visited( n, false );
    std::queue<size_t> bfs_queue;

    if ( start == 0 )
        for ( size_t i = 0; i < n; i++ ) {
            bfs_queue.push(i);
            visited[i] = true;
        }
    else {
        bfs_queue.push( getId(start) );
        visited[getId(start)] = true;
    }

    while( !bfs_queue.empty() ) {
        size_t id = bfs_queue.front();
        bfs_queue.pop();
        T * p = getValue( id );
        out[p] = action(p);

        for ( size_t edge_id = vertex_array.at(id); edge_id < vertex_array.at(id+1); edge_id++ ) {
            size_t neighbor_id = edge_array.at(edge_id);
            if ( !visited[neighbor_id] ) {
                bfs_queue.push( neighbor_id );
                visited[neighbor_id] = true;
            }
        }
    }

    return out;
}


template <class T>
template <class R>
std::unordered_map<T*,R> Graph<T>::DFS( R(* const action)(T * const), T * start ) {
    std::unordered_map<T*,R> out;
    std::vector<bool> visited( n, false );

    if ( n == 0 )
        return out;

    if ( start == 0 ) {
        for ( size_t i = 0; i < n; i++ ) {
            if ( !visited[i] )
                DFS_call( action, getValue(i), visited, out );
        }
    } else {
        DFS_call( action, start, visited, out );
    }

    return out;
}

template <class T>
template <class R>
void Graph<T>::DFS_call( R(* const action)(T * const), T * p, std::vector<bool> & visited, std::unordered_map<T*,R> & out ) {

    size_t id = getId(p);
    visited[id] = true;
    out[p] = action(p);

    for ( size_t edge_id = vertex_array.at(id); edge_id < vertex_array.at(id+1); edge_id++ ) {
        size_t neighbor_id = edge_array.at(edge_id);
        if ( !visited[neighbor_id] ) {
            DFS_call( action, getValue(neighbor_id), visited, out );
        }
    }
}


#endif
