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

typedef typle<int, int> quotient;

bool greater( const quotient &a, const quotient &b ){
	long long int [ a_nom, a_denm ] = a;
	long long int [ b_nom, b_denm ] = b;
	
	return a_nom * b_denm > b_nom * a_denm;
}

/*
quotient quotient( int nom, int dem ){
	int c = gcd( nom, dem );
	return { nom / c, dem / c };
}


quotient add( const quotient &a, const quotient &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;

	int denm = lcm( a_denm, b_denm );
	int nom = ( denm / a_denm ) * a_nom + ( denm / b_denm ) * b_nom;

	return quotient( nom, denm );
}

quotient mul( const quotient &a, const quotient &b ){
	return quotient( );
}
*/

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
	int n;
	vector< tuple<int, int, int> > proton;

	typedef tuple<int, quotient, quotient> collision_t;
	vector< collision_t > collision;


	cin >> n;
	for( int i = 0; i < n; i ++ ){
		int x, v, p;
		cin >> x >> v >> p;
		proton.push_back( { s, v, quotient( p, 100 ) } );
	}

	for( int i = 0; i < n - 1; i ++ ){
		auto [ x, v, p ] = proton[ i ];
		auto [ nx, nv, np ] = proton[ i + 1 ];

		int dx, dv, r;

		dx = nx - xx;

		dv = v + nv;
		r = mul( p, sub( quotient( 1, 1 ), np ));

		collision.push_back( { i, r,{ dx, dv }} );

		if( v != nv ){
			dv = ( nv > v ) ? nv - v : v - nv;
			r = ( nv > v ) : mul( sub( { 1, 1 }, p ), sub( { 1, 1 }, np )) : mul( p, np );
			collision.push_back( { i, r, { dx, dv }} );
		}
	}

	sort( collision.begin(), collision.end(), []
		(const collision_t &a, const collision_t &b) -> bool {
			auto [ a_i, a_p, a_t ] = a;
			auto [ b_i, b_p, b_t ] = b;
			return less( a_t, b_t );
		});

	vector< quotient > ex_colsn_prob( n - 1, { 1, 1 } );
	vector< int > segment;

	return 0;
}