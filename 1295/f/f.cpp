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
typedef tuple<int, int, const vector<int>&> rg;
typedef tuple<int, int, const vector< rg >& > prob_t;

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
	if( x == 0 ){
		return 1;
	}else{
		fd f( 1 );
		for( int i = d; i < d + x; i ++ ){
			f = f * i;
		}
		return f / ( pow( d, x ) * fac( x ) );
	}
}

tuple<bool, rg> intrs( rg a, rg b ){
	auto[ a_lb, a_ub ] = a;
	auto[ b_lb, b_ub ] = b;
	if( a_lb >= b_ub || a_ub <= b_lb ){
		return { false, { 0, 0 } };
	}else{
		return { true, { max( a_lb, b_lb ), min( a_ub, b_ub ) } };
	}
}

int rg_len( rg r ){
	auto[ lb, ub ] = r;
	return ub - lb;
}

bool rg_empty( rg r ){
	auto [ lb, ub ] = r;
	return lb >= ub;
}

tuple< rg, rg > split( rg s, rg d ){
	auto[ s_lb, s_ub ] = s;
	auto[ d_lb, d_ub ] = d;
	return { { d_ub, s_ub }, { s_lb, d_lb } };
}

tuple<bool, fd> _prob_good[50][50][50];
vector< rg > prob_bndr;
rg full_rg;

fd prob_good( int pb_i, int int_h, int int_n ){
	int int_i = pb_i - int_h;

	rg cr = int_i == 0 ? full_rg : { 0, get<0>( prob_bndr[ int_i - 1 ] )};

	for( int i  = int_i; i < pb_i; i ++ ){
		tie( ignore, cr ) = intrs( cr, prob_bndr[ i ] );
	}

	if( pb_i == prob_bndr.size() ){
		return comb_rg( int_n, rg_len( cr ) );
	}else{
		auto& [ done, v ] = _prob_good[ pb_i ][ int_i ][ int_n ];
		if( done == true ){
			return v;
		}else{
			done = true;

			auto[ is_int, int_rg ] = intrs( cr, prob_bndr[ pb_i ] );
			if( is_int == false ){
				auto[ cr_lb, cr_ub ] = cr;
				auto[ pb_lb, pb_ub ] = prob_bndr[ pb_i ];
				if( pb_ub <= cr_lb ){
					v = comb_rg( int_r, rg_len( cr ) ) * prob_good( pb_i + 1, 0, 0 );
				}else{
					v = 0;
				}
			}else{
				auto[ pbr_up, pbr_dn ] = split( prob_bndr[ pb_i ], int_rg );
				if( rg_empty( prb_dn ) == false ){
					v = v + comb_rg( int_n, cr_len( cr ) ) * prob_bood( pb_i + 1, 1, 1);
				}

				auto[ cr_ub, cr_dn ] = split( cr, int_rg );
				if( rg_empty( cr_ub ) == false ){
					for( int i = 1; i <= int_n; i ++ ){
						int j = int_n - i;
						v = v + comb_rg( i, rg_len( cr_ub ) ) * prob_good( pb_i + 1, int_h + 1, j + 1 ); 
					}	
				}
				v = v + prob_good( pb_i + 1, int_h + 1, int_n + 1 );
			}
			return v;
		}
	}
}

int main(){
	int n;

	cin >> n;
	int max_bndr = 0;

	for( int i = 0; i < n; i ++ ){
		int l, r;
		cin >> l >> r;
		prob_bndr.push_back( { l, r + 1 } );
		if( max_bndr < r + 1 ){
			max_bndr = r + 1;
		}
	}

	full_rg = { 0, max_bndr };

	cout << * prob_good( 0, 0, 0 );

	return 0;
}
