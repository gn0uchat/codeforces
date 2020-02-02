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

template<class T, int n>
class mtx_t{
private:
public:
	T entry[ n ][ n ];

	mtx_t(){
	}

	mtx_t( initializer_list< initializer_list< T >> init_l ){
		int i = 0;
		for( auto v : init_l ){
			int j = 0;
			for( auto s : v ){
				this->entry[ i ][ j ] = s;
				j ++;
			}
			i ++;
		}
	}

	friend mtx_t<T, n> operator* ( const mtx_t<T,n> &a, const mtx_t< T, n>&b ){
		mtx_t< T, n > r;
		for( int i = 0; i < n; i ++ ){
			for( int j = 0; j < n; j ++ ){
				for( int k = 0; k < n; k ++ ){
					r.entry[ i ][ j ] = r.entry[ i ][ j ] + ( a.entry[ i ][ k ] * b.entry[ k ][ j ] );
				}
			}
		}
		return r;
	}
	
	T sum(){
		auto acc = T();
		for( int i = 0; i < n; i ++ ){
			for( int j = 0; j < n; j ++ ){
				acc = acc + this->entry[ i ][ j ];
			}
		}
		return acc;
	}
};

template< int prime >
class mod_int_fd{
private:
	int elem;
	int mod( long long int v ) const {
		v = v % prime;
		return v < 0 ? v + prime : v;
	}
public:

	int operator* () const {
		return this->elem;
	}

	mod_int_fd<prime> operator+ ( const mod_int_fd<prime> &b ) const{
		int x = this->elem;
		int y = *b;
		int r = ( x - prime ) + y;
		return r < 0 ? mod_int_fd( r + prime ) : mod_int_fd( r );
	}

	mod_int_fd<prime> operator- ( const mod_int_fd<prime> &b ) const {
		int x = elem;
		int y = *b;
		int r = x - y;
		return r < 0 ? mod_int_fd<prime>( r + prime ): mod_int_fd<prime>( r );
	}

	mod_int_fd<prime> operator*( const mod_int_fd<prime> &b ) const {
		long long int big_a = this->elem;
		long long int big_b = *b;

		return mod_int_fd<prime>(( big_a * big_b ) % prime );
	}

	mod_int_fd<prime> mul_inv() const {
		int a = this->elem;

		int t_inv = 0, inv = 1;
		for( int v = prime, r = a; r > 1; ){
			int q = v / r;
			int new_r = v % r;
			int new_inv = mod( ((long long int) t_inv) - ( ((long long int) q) * ((long long int) inv)) );

			t_inv = inv;
			inv = new_inv;

			v = r;
			r = new_r;
		}

		return mod_int_fd<prime>( inv );
	}

	mod_int_fd<prime> operator/ ( const mod_int_fd<prime> &b ) const {
		return ( * this ) * b.mul_inv();
	}

	friend mod_int_fd<prime > operator* ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) * b; }
	friend mod_int_fd<prime > operator/ ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) / b; }
	friend mod_int_fd<prime > operator+ ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) + b; }
	friend mod_int_fd<prime > operator- ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) - b; }
	mod_int_fd<prime> operator * ( int v ) const { return this->elem * mod_int_fd<prime>( v ); }
	mod_int_fd<prime> operator / ( int v ) const { return this->elem / mod_int_fd<prime>( v ); }
	mod_int_fd<prime> operator + ( int v ) const { return this->elem + mod_int_fd<prime>( v ); }
	mod_int_fd<prime> operator - ( int v ) const { return this->elem - mod_int_fd<prime>( v ); }

	mod_int_fd(){
		this->elem = 0;
	}

	mod_int_fd( long long int elem ){
		this->elem = mod( elem );
	}
};

template<class T>
class seg_tree{
private:
	typedef tuple<int, int> segment;

	int alloc_i;
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

			T lv = node_init( left_seg ( nd_seg ), li );
			T rv = node_init( right_seg( nd_seg ), ri );

			v = this->op( lv, rv );
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
				return this->op( lv, rv );
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
				v = this->op( lv, rv );
			}else{
				T lv = this->node_query( left_seg( nd_seg ), left_seg( nd_seg ), li );
				T rv = this->node_update( right_seg( nd_seg), p, ri, update_v );
				v = this->op( lv, rv );
			}
			return v;
		}
	}

public:
	seg_tree( int lb, int ub, function<T(int)> init_v, function<T( const T&, const T& )> op ){
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
	typedef mod_int_fd< 998244353 > fd_t;
	typedef tuple< int, tuple<int, int>, tuple<int, int>> collision_t;
	typedef tuple< int, int, fd_t > proton_t;
	typedef mtx_t<fd_t, 2> dirc_prob_t;

	int n;

	vector< proton_t > proton;
	vector< collision_t > collision;

	cin >> n;
	for( int i = 0; i < n; i ++ ){
		int x, v, p;
		cin >> x >> v >> p;
		proton.push_back( { x, v, fd_t( p ) / 100 } );
	}

	for( int i = 0; i < n - 1; i ++ ){
		auto [ x, v, p ] = proton[ i ];
		auto [ nx, nv, np ] = proton[ i + 1 ];

		collision.push_back( { i, {0, 1}, { nx - x, v + nv } } );

		if( v != nv ){
			if( nv > v ){
				collision.push_back( { i, {1, 1}, { nx - x, nv - v } });
			}else{
				collision.push_back( { i, {0, 0}, { nx - x, v - nv } });
			}
		}
	}

	sort( collision.begin(), collision.end(), []
		(const collision_t &a, const collision_t &b) -> bool {
			auto [ a_i, a_dirc, a_t ] = a;
			auto [ b_i, b_dirc, b_t ] = b;
			auto [ atn, atd ] = a_t;
			auto [ btn, btd ] = b_t;
			return  ((long long int)atn) * ((long long int)btd)  <
				 ((long long int)btn) * ((long long int)atd) ;
		});

	seg_tree<dirc_prob_t> st( 0, n,
			[ &proton ](int i) -> dirc_prob_t{
				auto [ x, v, p ] = proton[ i ];
				return dirc_prob_t{ { p , p }, { (1 - p), (1 - p) } };
			},
			[]( const dirc_prob_t& a, const dirc_prob_t& b ) -> dirc_prob_t {
				return a * b;
			});

	fd_t exp_t( 0 );
	fd_t prob_prev = st.query( 0, n ).sum() / 2;

	for( auto [ i, dirc, qt ] : collision ){
		auto[ qt_n, qt_d ] = qt;
		auto[ mi, mj ] = dirc;

		fd_t t = fd_t( qt_n ) / fd_t( qt_d );
		
		dirc_prob_t m = st.query( i, i + 1 );

		m.entry[ mi ][ mj ] = fd_t( 0 );

		st.update( i, m );
		fd_t prob_ex_this = st.query( 0, n ).sum() / 2;

		exp_t = exp_t + ( ( prob_prev - prob_ex_this ) * t );

		prob_prev = prob_ex_this;
	}

	cout << *(exp_t) << endl;

	return 0;
}
