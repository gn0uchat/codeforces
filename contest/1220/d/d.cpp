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

int pow_fact_2( long long val ){
	if( val == 0 ){
		return 0;
	}else{
		if( val % 2 == 0 ){
			return 1 + pow_fact_2( val / 2 );
		}else{
			return 0;
		}
	}
}

int main(){
	int n;

	cin >> n;

	vector<long long> b(n);

	for( int i = 0; i < n; i ++ ){
		cin >> b[ i ];
	}

	vector<int> pow_count( 64 );

	for( int i = 0; i < n; i ++ ){
		pow_count[ pow_fact_2( b[ i ] ) ] ++;
	}

	int pick_pow = 0;
	for( int i = 0; i < 64; i ++ ){
		if( pow_count[ i ] > pow_count[ pick_pow ] ){
			pick_pow = i;
		}
	}

	int erase_n = n - pow_count[ pick_pow ];

	cout << erase_n << endl;

	for( int i = 0; i < n; i ++ ){
		if( pow_fact_2( b[ i ] ) != pick_pow ){
			cout << b[ i ] << endl;
		}
	}

	return 0;
}
