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
	int mod( long long int v ) const {
		v = v % prime;
		return v < 0 ? v + prime : v;
	}
public:
	int elem;

	string operator* () const {
		char buf[ 16 ];
		sprintf( buf, "%d", this->elem );
		return string( buf );
	}

	mod_int_fd<prime> operator+ ( const mod_int_fd<prime> &b ) const{
		int x = this->elem;
		int y = b.elem;
		int r = ( x - prime ) + y;
		return r < 0 ? mod_int_fd( r + prime ) : mod_int_fd( r );
	}

	mod_int_fd<prime> operator- ( const mod_int_fd<prime> &b ) const {
		int x = elem;
		int y = b.elem;
		int r = x - y;
		return r < 0 ? mod_int_fd<prime>( r + prime ): mod_int_fd<prime>( r );
	}

	mod_int_fd<prime> operator*( const mod_int_fd<prime> &b ) const {
		long long int big_a = this->elem;
		long long int big_b = b.elem;

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

	mod_int_fd<prime> operator^ ( int p ) const {
		p = mod( p );

		mod_int_fd<prime> v( 1 );
		mod_int_fd<prime> b = this->elem;

		for(; p > 0; p >>= 1, b = b * b ){
			if( p & 1 ){
				v = v * b;
			}
		}
		return v;
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
typedef tuple<int, int> rg;

vector<rg> pb_rg;
vector<int> bndr;

int rg_len( rg r ){
	auto[ lb, ub ] = r;
	return ub - lb;
}

bool rg_within( rg b, rg s ){
	auto[ b_lb, b_ub ] = b;
	auto[ s_lb, s_ub ] = s;
	return ( b_lb <= s_lb ) && ( b_ub >= s_ub );
}

fd fac( int x ){
	fd v( 1 );
	for( int i = x; i > 0; i = i - 1 ){
		v = v * i;
	}
	return v;
}

fd comb_seg( int x, int d ){
	fd f( 1 );
	for( int i = d; i < d + x; i ++ ){
		f = f * i;
	}
	return f / fac( x );
}

tuple<bool, fd> _comb_good[ 50 ][ 100 ];

fd comb_good( int pb_i, int bndr_i ){

	if( pb_i == pb_rg.size() ){
		return 1;
	}else if( bndr_i == 0 ){
		return 0;
	}else{
		auto& [ done, v ] = _comb_good[ pb_i ][ bndr_i ];
		
		if( done == true ){
			return v;
		}else{
			done = true;

			v = fd( 0 );

			v = v + comb_good( pb_i, bndr_i - 1 );

			rg seg_rg = { bndr[ bndr_i - 1 ], bndr[ bndr_i ] };
			for( int i = pb_i, j = 1; i < pb_rg.size() && rg_within( pb_rg[ i ], seg_rg ); i ++, j ++ ){
				v = v + ( comb_seg( j, rg_len( seg_rg ) ) * comb_good( i + 1, bndr_i - 1 ) );
			}

			return v;
		}
	}
}

int main(){
	int n;
	cin >> n;

	for( int i = 0; i < n; i ++ ){
		int lb, ub;
		cin >> lb >> ub;
		pb_rg.push_back( { lb, ub + 1 } );
	}

	set<int> seg_bndr;
	for( auto r : pb_rg ){
		auto [ lb, ub ] = r;
		seg_bndr.insert( lb );
		seg_bndr.insert( ub );
	}

	for( auto b : seg_bndr ){
		bndr.push_back( b );
	}

	for( int i = 0; i < pb_rg.size(); i ++ ){
		for( int j = 0; j < bndr.size(); j ++ ){
			_comb_good[ i ][ j ] = { false, fd( 0 ) };
		}
	}

	fd tot_comb( 1 );

	for( auto r : pb_rg ){
		tot_comb = tot_comb * fd( rg_len( r ) );
	}

	cout << *( comb_good( 0, bndr.size() - 1 ) / tot_comb ) << endl;
	return 0;
}
