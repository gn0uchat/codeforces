#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>
#include<string>
#include<functional>
#include<queue>
#include<iostream>
using namespace std;

int ceil( int num, int dem ){
	return ( num % dem == 0 ? num / dem : 1 + ( num / dem ) );
}

int mod( int v, int base ){
	return ( ( v % base ) + base ) % base;
}

class graph{
	vector< lazy_list<int> > adj_list;
};

void dfs_traverse(
	int n,
	vector< list< int > > &adj,
	function<void(
		int,
		vector<int>&,
		vector<int>&,
		function<void()>)> &enter_f ){

	vector<int> father( n, n );
	vector<bool> unvisit( n, true );

	function<void(int)> dfs = [&](
		int u ){
		
		std::list<int>::iterator it;

		unvisit[ u ] = false;

		for ( it = adj[ u ].begin(); it != adj[ u ].end(); ++ it ){
			int v = *it;
			if( unvisit[ v ] ){
				father[ v ] = u;
				dfs( v );
			}
		}
	}

	for( int v = 0; v < n; v ++ ){
		dfs( v );
	}

	function<void(int)> traverse = [&](
		int u ){

		function< void() > deeper = [&](){
			std::list<int>::iterator it;
			for ( it = adj[ u ].begin(); it != adj[ u ].end(); ++ it ){
				int v = *it;
				if( father[ v ] == u ){
					traverse( v );
				}
			}
		};
		enter_f( u, father, depth, deeper );
	}

	for( int v = 0; v < n; v ++ ){
		traverse( v );
	}
 }

class dsu{
private:
	int n;
	vector< int > repr;
public:
	construct( int n ){
		this.n = n;
		for( int i = 0; i < n; i ++ ){
			repr.push_back( i );
		}
	}

	int repr( int a ){
		if( repr[ a ] == a ){
			return a;
		}else{
			int b = repr( repr[ a ] );
			repr[ a ] = b;
			return b;
		}
	}

	void join( int a, int b ){
		repr[ repr( b ) ] = repr( a );
	}
}

int main(){
	int n, m;

	cin >> n >> m;

	vector<int> city_value( n );
	vector< list<int> > adj( n );

	for( int i = 0; i < m; i ++ ){
		int u, v;
		cin >> u >> v;
		u --; v --;

		adj[ u ].push_back( v );
		adj[ v ].push_back( u );
	}


	dsu cycle_set( n );

	vector<int> cycle_set_value( n, 0 );
	for( int i = 0; i < n; i ++ ){
		cycle_set_value[ i ] = city_value[ i ];
	}
		

	dfs_traverse( n, adj, [&](
		int u,
		vector<int> &father,
		vector<int> &depth,
		function<void()> deeper ){

		int min_d = depth[ u ];
		std::list<int>::iterator it;
		for ( it = adj[ u ].begin(); it != adj[ u ].end(); ++ it ){
			int v = *it;
			min_d = min( min_d, depth[ v ] );
		}

		if( depth[ u ] - min_d >= 2 ){
			for( int i = u, j = u; depth[ i ] >= min_d;){

				cycle_set.join( i, j );
				cycle_set_value[ i ] += cycle_set_value[ j ];

				j = i;
				i = cycle_set.repr( father[ i ] );
			}
		}

		deeper();});

	return 0;
}
