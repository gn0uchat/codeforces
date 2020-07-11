#include<cstdio>
#include<iostream>
#include<list>
#include<functional>
using namespace std;
int bit_length( long long int v ){
	int n = 0;
	for(; v != 0; n ++, v = v >> 1);
	return n;
}

long long int two_pow( int pow ){
	long long int val = 1;
	return ( val << pow );
}

long long int path_comb( long long int n, long long int v ){
	if( v == n ){
		return 1;
	}
	int entropy_length = bit_length( n ) - bit_length( v );
	long long int n_prefix = n >> entropy_length;
	long long int v_prefix_comb;

	if( v == n_prefix ){
		v_prefix_comb = two_pow( entropy_length )
				+ ( n - (n_prefix << entropy_length) );
	}else{
		if( v > n_prefix ){
			v_prefix_comb = two_pow( entropy_length ) - 1;
		}else{
			v_prefix_comb = two_pow( entropy_length + 1 ) - 1;
		}
	}

	if( v % 2 == 0 ){
		return v_prefix_comb + path_comb( n, v + 1 );
	}else{
		return v_prefix_comb;
	}
};

int main(){
	long long int n, k;
	cin >> n >> k;

	if( path_comb( n, n ) >= k ){
		cout << n << endl;
	}else{
		long long int solution = 0;
		for( int length = bit_length( n ); solution == 0; length -- ){
			long long int full = two_pow( length ) - 1;
			long long int n_prefix = n >> ( bit_length( n ) - length );
			list<long long int> searching_v;

			searching_v.push_back( full );
			searching_v.push_back( full - 1 );
			searching_v.push_back( n_prefix );
			searching_v.push_back( n_prefix - 1 );
			searching_v.push_back( n_prefix - 2 );
			searching_v.push_back( n_prefix - 3 );

			searching_v.remove_if([length, n]( auto v ){
				return ( v <= 0 ) || ( v > n ) || ( bit_length( v ) != length );
			});

			for( auto v : searching_v ){
				
				if( path_comb( n, v ) >= k ){
					solution = v;
					break;
				}
			}
		}
		cout << solution << endl;
	}

	return 0;
}
