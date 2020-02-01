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

template<class T, int m, int n>
class mtx_t{
private:
	vector< vector<T> > entry;

public:
	mtx_t(){
		this->entry.resize( m, vector< T >( n ));
	}

	mtx_t( initializer_list< initializer_list< T >> init_l ){
		for( auto v : init_l ){
			this->entry.push_back( vector<T>( v ));
		}
	}

	mtx_t operator* ( const mtx_t< T, m, n>& mp ){
		mtx_t< T, m, n > r;
		for( int i = 0; i < m; i ++ ){
			for( int j = 0; j < n; j ++ ){
				for( int k = 0; k < n; k ++ ){
					r[ i ][ j ] = r[ i ][ j ] + r[ i ][ k ] * mp[ k ][ j ];
				}
			}
		}
	}

	vector<T>& operator[] ( size_t i ){
		return this->entry[ i ];
	}
};

<template int prime>
class mod_int_fd{
private:
	int elem;
	int mod( int v, int base ){
		return ( ( v % base ) + base ) % base;
	}

	int mul_inv( int a ){
		a = mod( a, prime );

		int t_inv = 0, inv = 1;
		for( int v = prime, r = a; r > 1; ){
			int q = v / r;
			int new_r = v % r;
			int new_inv = sub( t_inv, mul( q, inv ) );

			t_inv = inv;
			inv = new_inv;

			v = r;
			r = new_r;
		}

		return inv;
	}

public:
	int operator* (){
		return this->elem;
	}

	mod_int_fd<prime> operator+ ( const mod_int_fd<prime> &b ){
		a = mod( this->elem, prime );
		b = mod( *b, prime );

		int r = ( a - prime) + b;
		return r < 0 ? r + prime: r;
	}

	mod_int_fd<prime> operator- ( const mod_int_fd<prime> &b ){
		int a = elem;
		int b = *b;
		int r = a - b;
		return r < 0 ? mod_int_fd<prime>( r + prime ): mod_int_fd<prime>( r );
	}

	mod_int_fd<prime> operator*( const mod_int_fd<prime> &b )
		long long int big_a = this->elem;;
		long long int big_b = *b;

		return mod_int_fd<prime>((int)( ( big_a * big_b ) % prime ));
	}

	mod_int_fd<prime> operator/ ( const mod_int_fd<prime> &b )
		return mod_int_fd<prime>( mul( this->elem, *b ) );
	}

	mod_int_fd( int elem ){
		this->elem = mod( elem, prime );
	}
};

template<class T>
class seg_tree{
private:
	typedef tuple<int, int> segment;
	int alloc_i;
	int low_bound, upp_bound;
	vector< tuple< int, int, T > > mem;
	function<T(const T&, const T&)> op;
	function<T(int)> init_v;
	segment full_seg;

	bool unit( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		return ( nd_ub - nd_lb ) == 1;
	}

	segment left_seg( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		int mid = ( nd_lb + nd_ub ) / 2;
		return { nd_lb, mid };
	}

	segment right_seg( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		int mid = ( nd_lb + nd_ub ) / 2;
		return { mid, nd_ub };
	}

	T node_init( segment nd_seg, int nd_i ){
		auto&[ li, ri, v ] = mem[ nd_i ];

		if( unit( nd_seg ) ){
			auto[ lb, ub ] = nd_seg;
			v = this->init_v( lb );
			return v;
		}else{
			li = this->alloc_i ++;	
			ri = this->alloc_i ++;

			T lv = node_init( left_seg ( nd_seg ), li, init_v );
			T rv = node_init( right_seg( nd_seg ), ri, init_v );

			v = this->op( nd_seg, lv, rv );
			return v;
		}
	}

	T node_query( segment nd_seg, segment qy_seg, int nd_i ){
		auto&[ li, ri, v ] = mem[ nd_i ];
		auto [ nd_lb, nd_ub ] = nd_seg;
		auto [ qy_lb, qy_ub ] = qy_seg;

		if( nd_seg == qy_seg ){
			return v;
		}else{
			int mid = ( nd_lb + nd_ub ) / 2;
			if( qy_ub <= mid ){
				return this->node_query( left_seg( nd_seg ),  qy_seg, li );
			}else if( mid <= qy_lb ){
				return this->node_query( right_seg( nd_seg ), qy_seg, ri );
			}else{
				T lv = this->node_query( left_seg( nd_seg ) , { qy_lb, mid }, li );
				T rv = this->node_query( right_seg( nd_seg ), { mid, qy_ub }, ri );
				return this->op( nd_seg, lv, rv );
			}
		}
	}

	T node_update( segment nd_seg, int p, int nd_i, const T& update_v ){
		auto&[ li, ri, v ] = mem[ nd_i ];
		auto[ lb, ub ] = nd_seg;

		if( unit( nd_seg ) ){
			v = update_v;
			return v;
		}else{
			int mid = ( lb + ub ) / 2;	
			if( p < mid ){
				T lv = this->node_update( left_seg( nd_seg ), p, li, update_v );
				T rv = this->node_query( right_seg( nd_seg ), right_seg( nd_seg ), ri );
				v = this->op( nd_seg, lv, rv );
			}else{
				T lv = this->node_query( left_seg( nd_seg ), left_seg( nd_seg ), li );
				T rv = this->node_update( right_seg( nd_seg), p, ri, update_v );
				v = this->op( nd_seg, lv, rv );
			}
			return v;
		}
	}

public:
	seg_tree( int lb, int ub, function<T(int)> init_v, function<T( const segment&, const T&, const T& )> op ){
		int mem_n = 4 * ( ub - lb );

		this->op = op;
		this->init_v = init_v;
		this->alloc_i = 0;
		this->full_seg = { lb, ub };
		this->mem.resize( mem_n );

		this->node_init( this->full_seg, this->alloc_i ++ );
	}

	T query( int lb, int ub ){
		return this->node_query( this->full_seg, {lb, ub}, 0 );
	}

	T update( int p, const T& update_v ){
		return this->node_update( this->full_seg, p, 0, update_v );
	}

};

int main(){
	typedef mod_int_fd< 990244353 > fd_t;
	const fd_t one( 1 );
	int n;
	vector< tuple<int, int, quotient_t> > proton;

	typedef tuple<int, quotient_t, quotient_t> collision_t;

	vector< collision_t > collision;


	cin >> n;
	for( int i = 0; i < n; i ++ ){
		int x, v, p;
		cin >> x >> v >> p;
		proton.push_back( { x, v, fd.div( p, 100 ) } );
	}

	for( int i = 0; i < n - 1; i ++ ){
		auto [ x, v, p ] = proton[ i ];
		auto [ nx, nv, np ] = proton[ i + 1 ];

		quotient_t dt = quotient( nx - x, v + nv );

		int r = fd.mul( p, fd.sub( 1, np ) );

		collision.push_back( { i, r, dt} );

		if( v != nv ){
			if( nv > v ){
				quotient_t dt  = quotient( nx - x, nv - v );
				int r = fd.mul( fd.sub( 1, p ), fd.sub( 1, np ));

				collision.push_back( { i, r, dt });
			}else{
				quotient_t dt  = quotient( nx - x, v - nv );
				int r = fd.mul( p, np );

				collision.push_back( { i, r, dt });
			}
		}
	}

	sort( collision.begin(), collision.end(), []
		(const collision_t &a, const collision_t &b) -> bool {
			auto [ a_i, a_p, a_t ] = a;
			auto [ b_i, b_p, b_t ] = b;
			return lt( a_t, b_t );
		});

	typedef mtx<fd_t, 2, 2> dirc_prob_t;
	const dirc_prob_t sum_row{ {one, one}, {one, one}};
	seg_tree<dirc_prob_t> st( 0, n - 1,
			[ &proton ](int p) -> dirc_prob_t{
				auto [ x, v, p ] = proton[ p ];
				auto [ nx, nv, np ] = proton[ p + 1 ];
				return dirc_prob_t{ { p * np, p * ( one - np )}, { ( one - p) * np, (one - p) * (one - np)} };
			},
			[]( const tuple<int, int> &s, const dirc_prob_t& a, const dirc_prob_t& b ){
				return 
			});

	return 0;
}
