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

class mod_int_fd{
private:
	int prime;

public:
	int add( int a, int b ){
		int r = ( a - prime) + b;
		return r < 0 ? r + prime: r;
	}

	int sub( int a, int b ){
		int r = a - b;
		return r < 0 ? r + prime : r;
	}

	int mul( int a, int b ){
		long long int big_a = a;
		long long int big_b = b;
		
		long long int r = big_a * big_b;

		return ((int)( ( big_a * big_b ) % prime ));
	}

	int div( int dividend, int divisor ){
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

	int qnt2fd( quotient q ){
		auto[ nom, denm ] = q;
		return div( nom, denm );
	}

	mod_int_fd( int prime ){
		this->prime = prime;
	}
};

template<type T>
T gcd( T a, T b ){
	T tmp;
	if( a < b ){
		tmp = a;
		a = b;
		b = tmp;
	}

	for(; b > 0; tmp = a % b, a = b, b = tmp );

	return a;
}

template<type T>
T lcm( T a, T b ){
	int d = gcd( a, b );
	return d * ( a / d ) * ( b / d );
}

typedef tuple<int, int> quotient;

bool gt( const quotient &a, const quotient &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;
	
	return ( (long long int) a_nom ) * ( (long long int) b_denm ) > 
	       ( (long long int) b_nom ) * ( (long long int) a_denm );
}

quotient quotient( int nom, int dem ){
	int d = gcd<int>( nom, dem );
	return { nom / d, dem / d };
}

quotient add( const quotient &a, const quotient &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;

	int denm = lcm( a_denm, b_denm );
	int nom = ( denm / a_denm ) * a_nom + ( denm / b_denm ) * b_nom;

	return quotient( nom, denm );
}

quotient sub( const quotient &a, const quotient &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;

	int denm = lcm( a_denm, b_denm );
	int nom = ( denm / a_denm ) * a_nom - ( denm / b_denm ) * b_nom;

	return quotient( nom, denm );
}

quotient mul( const quotient &a, const quotient &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;
	long long nom = ( (long long int) a_nom ) * ( (long long int) b_nom );
	long long denm = ( (long long int) a_denm ) * ( (long long int) b_denm );
	long long int d = gcd<long long int>( nom, denm );
	return quotient( (int) ( nom / d ), (int) ( denm / d ) );
}

quotient div( const quotient &a, const quotient &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;
	long long nom = ( (long long int) a_nom ) * ( (long long int) b_dem );
	long long denm = ( (long long int) a_denm ) * ( (long long int) b_nom );
	long long int d = gcd<long long int>( nom, denm );
	return quotient( (int) ( nom / d ), (int) ( denm / d ) );
}

template<class T>
class seg_tree{
private:
	typedef segment tuple<int, int>
	int alloc_i;
	int low_bound, upp_bound;
	vector< tuple< bool, int, int, T > > mem;
	function<T(T, T)> value_comb;

	bool unit( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		return ( nd_ub - nd_lb ) == 1;
	}

	int left_seg( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		int mid = ( nd_lb + nd_ub ) / 2;
		return { nd_lb, mid };
	}

	int right_seg( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		int mid = ( nd_lb + nd_ub ) / 2;
		return { mid, nd_ub };
	}

	T node_init( segment nd_seg, int nd_i, init_v ){
		auto&[ li, ri, v ] = mem[ nd_i ];

		if( unit( nd_seg ) ){
			v = init_v;
			return v;
		}else{
			int mid = ( lb + ub ) / 2;
			li = this->alloc_i ++;	
			ri = this->alloc_i ++;

			int lv = node_init( left_seg ( nd_seg ), li, init_v );
			int rv = node_init( right_seg( nd_seg ), ri, init_v );

			v = this->value_comb( lv, rv );
			return v;
		}
	}

	T node_query( segment nd_seg, segment qy_seg, int nd_i ){
		auto&[ li, ri, v ] = mem[ i ];
		auto [ nd_lb, nd_ub ] = nd_seg;
		auto [ qy_lb, qy_ub ] = qy_seg;

		if( nd_seg == qy_seg ){
			return v;
		}else{
			int mid = ( nd_lv + nd_ub ) / 2;
			if( qy_ub <= mid ){
				return this->node_query( left_seg( nd_seg ),  query, li );
			}else if( mid <= qy_li ){
				return this->node_query( right_seg( nd_seg ), query, ri );
			}else{
				T lv = this->node_query( left_seg( nd_seg ) , { qy_lb, mid }, li );
				T rv = this->node_query( right_seg( nd_seg ), { mid, qy_ub }, ri );
				return this->value_comb( lv, rv );
			}
		}
	}

	T node_update( segment nd_seg, int p, int nd_i, int update_v ){
		auto&[ li, ri, v ] = mem[ nd_i ];
		auto[ lb, ub ] = nd_seg;

		if( unit( nd_seg ) ){
			v = update_v;
			return v;
		}else{
			int mid = ( lb + ub ) / 2;	
			if( p < mid ){
				T lv = this->node_update( left_seg( nd_seg ), p, li, update_v );
				T rv = this->node_query( right_seg( nd_seg ), ri );
				v = this->value_comb( lv, rv );
			}else{
				T lv = this->node_query( left_seg( nd_seg ), li );
				T rv = this->node_update( right_seg( nd_seg), p, ri, update_v );
				v = this->value_comb( lv, rv );
			}
			return v;
		}
	}

public:
	seg_tree( int lb, int ub, T init_v, function<T( const T&, const T&)> value_comb ){
		int mem_n = 4 * ( ub - lb );

		this->value_op = value_comb;
		this->mem_i = 0;
		this->full_seg = { lb, ub };
		this->mem.resize( mem_n, { 0, 0, init_v });

		this->init( this->full_seg, this->mem_i ++, init_v );
	}

	T query( int lb, int ub ){
		return this->node_query( this->full_seg, {lb, ub}, 0 );
	}

	T update( int p, int update_v ){
		return this->node_update( this->full_seg, p, 0, update_v );
	}

};

int main(){
	mod_int_fd fd( 998244353 );
	int n;
	vector< tuple<int, int, int> > proton;

	typedef tuple<int, quotient, quotient> collision_t;
	vector< collision_t > collision;


	cin >> n;
	for( int i = 0; i < n; i ++ ){
		int x, v, p;
		cin >> x >> v >> p;
		proton.push_back( { x, v, quotient( p, 100 ) } );
	}

	for( int i = 0; i < n - 1; i ++ ){
		auto [ x, v, p ] = proton[ i ];
		auto [ nx, nv, np ] = proton[ i + 1 ];

		int dx, dv, r;

		dx = nx - xx;

		dv = v + nv;
		r = mul( p, sub( quotient( 1, 1 ), np ));

		collision.push_back( { i, r, { dx, dv }} );

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
	seg_tree< int > seg_prob( 0, n - 1, 1, fd.mul );

	int exp_t = 0;
	for( auto[ i, p, t ] : collision ){
		int prev = seg_prob.query( 0, n - 1 );	

		ex_colsn_prob[ i ] = sub( ex_colsn_prob[ i ], p );

		auto [pn, pd] = ex_colsn_prob[ i ];
		int t_prob = seg_prob.update( i, fd.qnt2fd( ex_colsn_prob[ i ] ) ) - prev;
		
		exp_t = add( expt_t, mul( t_prob, fd.qnt2fd( t )));
	}


	auto[ nom, denm ] = exp_t;

	cout << fd_mul( mod( nom, prime ), mod( denm, prime ), prime );

	return 0;
}
