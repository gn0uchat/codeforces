#include<map>
#include<set>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>
#include<string>
#include<functional>
#include<queue>
#include<iostream>
#include<list>
#include<tuple>
using namespace std;

tuple<int, int> edge( int a, int b ){
	if( a < b ){
		return { a, b };
	}else{
		return { b, a };
	}
}

vector< tuple<int, int, int> > tri;
vector< map< tuple<int, int>, int> > adj;
list< int > cut, order;
vector< bool > visit;

int opp_pnt( int tri_i, tuple<int, int> e ){
	auto [ x, y ] = e;
	auto [ a, b, c ] = tri[ tri_i ];
	if( a != x && a != y ){ return a; }
	else if( b != x && b != y ){ return b; }
	else{ return c; }
}

int dfs( int a, int b, int c, int tri_i ){
	visit[ tri_i ] = true;

	vector<tuple<int, int>> edges;
	edges.push_back( make_tuple( a, b ));
	edges.push_back( make_tuple( b, c ));
	edges.push_back( make_tuple( c, a ));

	for( auto [ x, y ] : edges ){
		tuple<int, int> e = edge( x, y );
		if( adj[ tri_i ].count( e ) > 0 ){
			int adj_tri_i = adj[ tri_i ][ e ];

			if( visit[ adj_tri_i ] == false ){
				dfs( x, opp_pnt( adj_tri_i, e ), y, adj_tri_i );
			}

		}else{
			order.push_back( y );
		}
	}

	cut.push_back( tri_i + 1 );

	return 0;
}

int main(){
	int t;
	cin >> t;
	while( t --> 0 ){
		int n, tri_n;
		map<tuple<int, int>, vector<int>> edge2tri;

		cin >> n;
		tri_n = n - 2;

		visit.resize( tri_n );
		adj.resize( tri_n );

		for( int i = 0; i < tri_n; i ++ ){
			int a, b, c;
			cin >> a >> b >> c;
			tri.push_back( make_tuple( a, b, c ) );
		}

		for( int i = 0; i < tri_n; i ++ ){
			auto [a, b, c] = tri[ i ];
			edge2tri[ edge( a, b ) ].push_back( i );
			edge2tri[ edge( a, c ) ].push_back( i );
			edge2tri[ edge( b, c ) ].push_back( i );
		}

		for( auto const& [ e, tri ] : edge2tri ){
			if( tri.size() == 2 ){
				int u = tri[ 0 ], v = tri[ 1 ];
				adj[ u ][ e ] = v;
				adj[ v ][ e ] = u;		
			}
		}


		auto [ a, b, c ]  = tri[ 0 ];
		dfs( a, b, c, 0 );

		bool flag;

		flag = true;
		for( auto p : order ){
			if( flag ){
				flag = false;
			}else{
				cout << " ";
			}
			cout << p;
		}
		cout << endl;

		flag = true;
		for( auto p : cut ){
			if( flag ){
				flag = false;
			}else{
				cout << " ";
			}
			cout << p;
		}
		cout << endl;

		tri.clear();
		visit.clear();
		adj.clear();
		cut.clear();
		order.clear();
	}
	return 0;
}
