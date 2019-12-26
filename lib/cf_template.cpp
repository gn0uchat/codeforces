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

int ceil( int num, int dem ){
	return ( num % dem == 0 ? num / dem : 1 + ( num / dem ) );
}

int mod( int v, int base ){
	return ( ( v % base ) + base ) % base;
}

int fd_add( int a, int b, int prime ){
	int r = ( a - prime) + b;
	return r < 0 ? r + prime: r;
}

int fd_sub( int a, int b, int prime ){
	int r = a - b;
	return r < 0 ? r + prime : r;
}

int fd_mul( int a, int b, int prime ){
	long long int big_a = a;
	long long int big_b = b;
	
	long long int r = big_a * big_b;

	return ((int)( ( big_a * big_b ) % prime ));
}

int fd_div( int dividend, int divisor, int prime ){
	int t_inv = 0, inv = 1;
	for( int v = prime, r = divisor; r > 1; ){
		int q = v / r;
		int new_r = v % r;
		int new_inv = sub( t_inv, mul( q, inv ) );

		t_inv = inv;
		inv = new_inv;

		v = r;
		r = new_r;
	}

	return mul( dividend, inv );
}

int main(){
	return 0;
}
