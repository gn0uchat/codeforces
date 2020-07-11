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

int mod( int v, int m ){
	v = v % m;
	return v < 0 ? v + m : v;
}

int main(){
	int n, m;
	cin >> n >> m;

	vector< vector< int > > perm( n );

	for( int i = 0; i < n; i ++ ){
		for( int j = 0; j < m; j ++ ){
			int v;
			cin >> v;
			perm[ i ].push_back( v );
		}
	}

	vector< int > rot_match( n, 0 );
	int min_op = 0;
	for( int c = 0; c < m; c ++ ){
		int min_op_c = n;

		for( int r = 0; r < n; r ++ ){
			rot_match[ r ] = 0;
		}

		for( int r = 0; r < n; r ++ ){
			int v = perm[ r ][ c ] - ( c + 1 );
			if( v % m  == 0 ){
				int i = v / m;
				if( i < n ){
					rot_match[ mod( r - i, n ) ] ++;
				}
			}
		}
		for( int r = 0; r < n; r ++ ){
			int op = r + ( n - rot_match[ r ]);
			if( op < min_op_c ){
				min_op_c = op;
			}
		}
		
		min_op += min_op_c;
	}

	cout << min_op << endl;
	return 0;
}
