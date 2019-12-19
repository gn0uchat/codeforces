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

int ceil( int num, int dem ){
	return ( num % dem == 0 ? num / dem : 1 + ( num / dem ) );
}

int mod( int v, int base ){
	return ( ( v % base ) + base ) % base;
}

long long int _count[ 19 ][ 1 << 19 ];
bool _count_valid[ 19 ][ 1 << 19 ];
list<int> fp;
int max_div;

///*
int print_state(int n, int exclude ){
	printf( "(%d, ", n );
	printf( "{");
	int mask = 1;
	for( auto v : fp ){
		if( (exclude & mask ) == 0 ){
			printf( "%d, ", v );
		}
		mask = mask << 1;
	}
	printf( "}\n");
	return 0;
}
//*/

long long int count( int n, int exclude ){
	if( n == 1 ){
		return 1;
	}else if( _count_valid[ (n-2) ][ exclude ] ){
		return _count[ (n-2) ][ exclude ];
	}else{
		list<tuple<int,int>> elem_mask_list;
		int mask = 1;
		for( auto elem: fp ){
			elem_mask_list.push_back( make_tuple( elem, mask ));
			mask = mask << 1;
		}

		elem_mask_list.remove_if( [ exclude ]
		( tuple<int,int> elem_mask){
			auto[ elem, mask ] = elem_mask;
			return (exclude & mask) != 0;
		});

		if( n == max_div ){
			elem_mask_list.remove_if( []
			( tuple<int, int> elem_mask){
				auto[ elem, mask ] = elem_mask;
				return elem == 0;
			});
		}

		int last = max_div;
		long long int counting = 0;

		for( auto [ elem, mask ] : elem_mask_list ){
			if( elem >= n ) break;
			if( elem != last ){
				counting += count( n - 1, exclude | mask );
			}
			last = elem;
		}

		_count[ (n-2) ][ exclude ] = counting;
		_count_valid[ (n-2) ][ exclude ] = true;

		return _count[ (n-2) ][ exclude ];
	}
}

int main(){
	int t;
	cin >> t;
	while( t --> 0 ){
		long long int k;
		cin >> k;

		fp.clear();
		for( int d = 2; k != 0; k /= d, d ++ ){
			fp.push_back( k % d );
		}

		fp.sort( [](int a, int b){ return a > b;} );

		int fp_n = fp.size();
		max_div = fp.size() + 1;

		for( int i = 0; i < fp_n; i ++ ){
			fill( _count_valid[i], _count_valid[i] + (1 << fp_n), false );
		}

		cout << count( max_div, 0 ) - 1 << endl;
	}
		
	return 0;
}
