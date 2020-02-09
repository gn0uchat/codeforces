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

typedef mod_int_fd<998244353> fd;

fd fac( int x ){
	fd v( 1 );
	for( int i = x; i > 0; i = i - 1 ){
		v = v * i;
	}
	return v;
}

fd pow( int b, int p ){
	fd v( 1 );
	for(; p > 0; p >>= 1, b = b * b ){
		if( p & 1 ){
			v = v * b;
		}
	}
	return v;
}

fd comb_rg( int x, int d ){
	fd f( 1 );
	for( int i = d; i < d + x; i ++ ){
		f = f * i;
	}
	return f / ( pow( d, x ) * fac( x ) );
}

const bool LB = false;
const bool UB = true;

typedef tuple<int,bool> pt;
typedef tuple<pt, pt> rg;

vector< tuple<int, int> >bndr;
int bndr_n;

int pt_val( pt p ){
	auto[ i, b ] = p;
	auto[ ub, lb ] = bndr[ i ];
	return b == UB ? ub : lb;
}

bool pt_ord( pt p ){
	auto[ i, b ] = p;
	return i;
}

<bool, rg> inters( rg a, rg b ){
	auto[ a_lb, a_ub ] = a;
	auto[ b_lb, b_ub ] = b;

	pt int_lb = pt_val( a_lb ) > pt_val( b_lb ) ? a_lb : b_lb;
	pt int_ub = pt_val( a_ub ) < pt_val( b_ub ) ? a_ub : b_ub;

	if( pt_val( int_ub ) <= pt_val( int_lb ) ){
		return { false, { int_lb, int_ub } };
	}else{
		return { true, { int_lb, int_ub } };
	}
}

int rg_len( rg r ){
	auto[ lb, ub ] = r;
	return pt_val( ub ) - pt_val( lb );
}

tuple<bool, fd> _good[ 2 ][ 2 ][ 50 ][ 50 ][ 50 ];

fd good( rg bnd_rg, int pb_i, int bnd_n ){
	if( pb_i == bndr.size() ){
		return comb_rg( bnd_n, rg_len( bnd_rg ));
	}else{
		auto [ bnd_rg_lb, bnd_rg_ub ] = bnd_rg;
		int i, b, c;

		if( pt_ord( bnd_rg_lb ) <= pt_ord( bnd_rg_ub ) ){
			c = 0;
			[ i, b ] = bnd_rg_lb;
		}else{
			c = 1;
			[ i, b ] = bnd_rg_ub;
		}

		auto& [ done, v ] = _good[ c ][ b ][ i ][ pb_i ][ bnd_n ];

		if( done ){
			return v;
		}else{
			done = true;

			pt pb_rg_lb = { pb_i, LB };
			pt pb_rg_ub = { pb_i, UB };
			rg pb_rg = { pb_rg_lb, pb_rg_ub };
			auto [ int_b, int_rg ] = inters( pb_rg, bnd_rg );

			if( int_b == false ){
				if( pt_val( pb_rg_ub ) <= pt_val( bnd_rg_lb ) ){
					v = comb_rg( bnd_n, rg_len( bnd_rg )) * good( pb_rg, pb_i, 0 );
				}else{
					v = fd( 0 );
				}
			}else{
				v = fd( 0 );
				auto[ int_rg_lb, int_rg_ub ] = int_rg;
				
				if( pt_val( int_rg_lb ) > pt_val( pb_rg_lb ) ){
					auto new_bnd_rg = { pb_rg_lb, int_rg_lb };
					v = comb_rg( bnd_n, rg_len( bnd_rg ) ) * good( new_bnd_rg, pb_i, 0 );
				}

				if( pt_val( bnd_rg_ub ) > pt_val( int_rg_ub ) ){
					for( int i = 1; i <= bnd_n; i ++ ){
						auto ex_rg = { int_rg_ub, bnd_rg_ub };
						v = v + comb_rg( i, rg_len( ex_rg ) ) * good( int_rg, pb_i, bnd_n - i );
					}
				}

				v = v + good( int_rg, pb_i, bnd_n + 1 );
			}
			return v;
		}
	}
}

int main(){
	int n;
	cin >> n;

	for( int i = 0; i < n; i ++ ){
		for( int j = 0; j < n; j ++ ){
			for( int k = 0; k < n; k ++ ){
				_good[0][0][i][j][k] = { false, fd(0) };
				_good[0][1][i][j][k] = { false, fd(0) };
				_good[1][0][i][j][k] = { false, fd(0) };
				_good[1][1][i][j][k] = { false, fd(0) };
			}
		}
	}

	for( int i = 0; i < n; i ++ ){
		int lb, ub;
		cin >> lb >> ub;
		bndr.push_back( { lb, ub + 1 } );
	}

	cout << good( {{ 0, LB }, { 1, UB }}, 0, 0 ) << endl;
	return 0;
}
