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

template< typename X_T, typename Y_T >
class line_t{
private:
public:
	Y_T a, b;
	line_t(){}
	line_t( Y_T a, Y_T b ):a( a ), b( b ){}
	Y_T operator() ( X_T x ) const {
		return a * ( ( Y_T ) x ) + b;
	}
};

auto qnum_gt = []( auto a, auto b  ) -> bool {
	auto[ a_nom, a_dem ] = a;
	auto[ b_nom, b_dem ] = b;

	if( ( a_nom / a_dem ) > ( b_nom / b_dem ) ){
		return true;
	}else if( ( a_nom / a_dem ) == ( b_nom / b_dem ) ){
		return ( a_nom % a_dem ) * b_dem > ( b_nom % b_dem ) * a_dem;
	}else{
		return false;
	}
};

template< typename VAR_T, typename COEF_T >
class mono_cht{
private:
	vector<line_t<VAR_T, COEF_T>> bag;
	int bag_i;
public:
	mono_cht(): bag_i(0){}

	void insert( COEF_T a, COEF_T b ){
		line_t<VAR_T, COEF_T> line( a, b );

		if( bag.size() < 1 ){
			bag.push_back( line );
		}else{
			if( bag.back().a == line.a ){
				if( bag.back().b < line.b ){
					line = bag.back();
					bag.pop_back();
				}else{
					bag.pop_back();
				}
			}
			while( bag.size() >= 2 ){
				line_t<VAR_T, COEF_T> last_l = bag.back();
				bag.pop_back();

				if( qnum_gt( make_tuple( line.b - last_l.b, last_l.a - line.a ),
						make_tuple(last_l.b - bag.back().b, bag.back().a - last_l.a) )){
					bag.push_back( last_l );
					break;
				}
			}
			bag.push_back( line );
		}

		if( bag_i >= bag.size() ){
			bag_i = bag.size() - 1;
		}
	}

	COEF_T query( VAR_T x ){
		for(; bag_i + 1 < bag.size(); bag_i ++ ){
			int next_i = bag_i + 1;
			if( ( bag[ next_i ].b - bag[ bag_i ].b ) > ( ( bag[ bag_i ].a - bag[ next_i ].a ) * ((COEF_T) x) ) ){
				break;
			}
		}
		return bag[ bag_i ]( x );
	}
};

typedef long long int number;



int main(){
	int n;
	cin >> n;

	vector<number> tree_h, tree_p;

	for( int i = 0; i < n; i ++ ){
		int v;
		cin >> v;
		tree_h.push_back( v );
	}

	for( int i = 0; i < n; i ++ ){
		int v;
		cin >> v;
		tree_p.push_back( v );
	}

	vector< number > opt_cost( n );
	mono_cht<int, number> bag;

	opt_cost[ n - 1 ] = 0;
	bag.insert( tree_h[ n - 1 ], opt_cost[ n - 1 ] );

	for( int i = n - 2; i >= 0; i -- ){
		opt_cost[ i ] = bag.query( tree_p[ i ] );
		bag.insert( tree_h[ i ], opt_cost[ i ] );
	}

	cout << opt_cost[ 0 ] << endl;
	return 0;
}
