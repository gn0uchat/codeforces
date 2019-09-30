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

bool repeat_gt( vector<int>& rep, queue<int>& tail ){

	while( tail.size() > 0 ){
		for( int i = 0; i < rep.size(); i ++, tail.pop() ){
			int d = tail.front();
			if( rep[ i ] > d ){ return true; }
			else if( rep[ i ] < d ){ return false; }
		}
	}

	return false;
}

bool plus_one_stop( vector<int>& rep ){
	bool stop_carry = false;
	for( int i = rep.size() - 1; ( i >= 0 ) && ( ! stop_carry ); i -- ){
		if( rep[ i ] < 9 ){
			rep[ i ] ++;
			stop_carry = true;
 		}else{
			rep[ i ] = 0;
		}
	}
	return stop_carry;
}

int main(){
	int L, S;
	string A;

	cin >> L >> A;

	int rep_n = ceil( A.length(), L );
	int zero_n = mod( - A.length(), L );

	vector<int> rep(L, 0);
	if( zero_n > 0 ){
		rep[ 0 ] = 1;
	}else{
		for( int i = 0; i < L; i ++ ){
			rep[ i ] = ( (int) A[ i ] - (int) '0' );
		}
	}

	queue<int> tail;

	for( int i = 0; i < zero_n; i ++ ){
		tail.push( 0 );
	}

	for( int i = 0; i < A.length(); i ++ ){
		tail.push( (int) A[ i ] - (int) '0' );
	}

	function<void(int)> print_rep = [&]( int n ){
		for( int i = 0; i < n; i ++ ){
			for(int i = 0; i < rep.size(); i ++ ){
				printf( "%d", rep[ i ] );
			}
		}
	};

	if( repeat_gt( rep, tail ) ){
		print_rep( rep_n );
	}else{
		if( plus_one_stop( rep ) ){
			print_rep( rep_n );
		}else{
			rep[ 0 ] = 1;
			print_rep( 1 + rep_n );
		}
	}

	return 0;
}
