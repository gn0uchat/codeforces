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

#define debug( args... ) do{fprintf( stderr, "[debug] " );fprintf( stderr, args ); fprintf(stderr, "\n");}while(0)

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

int mod( int v, int m ){
	return ( v + m ) % m;
}

typedef long long int number;

void add_line( vector<line_t<int, number>> &bag, int& bag_i, line_t<int, number> line ){
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
			line_t<int, number> last_l = bag.back();
			bag.pop_back();

			if( ( ( line.b - last_l.b ) * ( bag.back().a - last_l.a ) ) > 
					( ( last_l.b - bag.back().b ) * ( last_l.a - line.a ) ) ){
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

number query_line( const vector<line_t<int, number>> & bag, int& bag_i, int x ){
	for(; bag_i + 1 < bag.size(); bag_i ++ ){
		int next_i = bag_i + 1;
		if( ( bag[ next_i ].b - bag[ bag_i ].b ) > ( ( bag[ bag_i ].a - bag[ next_i ].a ) * ((number) x) ) ){
			break;
		}
	}
	return bag[ bag_i ]( x );
}

int main(){
	int n, m, p;
	cin >> n >> m >> p;
	vector<int> hill_d( n );
	vector<int> cat_h( m ), cat_t( m );

	hill_d[ 0 ] = 0;
	for( int i = 1; i < n; i ++ ){
		int v;
		cin >> v; hill_d[ i ] = v + hill_d[ i - 1 ];
	}

	for( int i = 0; i < m; i ++ ){
		int h, t;
		cin >> h >> t;
		h --;
		cat_h[ i ] = h;
		cat_t[ i ] = t;
	}

	vector< vector<number> > opt_cat( 2, vector<number>( m + 1, 0 ) );

	vector< number > cat_delta;
	
	for( int i = 0; i < m; i ++ ){
		cat_delta.push_back( cat_t[ i ] - hill_d[ cat_h[ i ] ] );
	}

	sort( cat_delta.begin(), cat_delta.end() );

	number delta_sum = 0;

	opt_cat[ 1 ][ m ] = 0;
	for( int i = m - 1; i >= 0; i -- ){
		delta_sum += cat_delta[ i ];
		opt_cat[ 1 ][ i ] = cat_delta[ m - 1 ] * ( m - i ) - delta_sum;
	}

	for( int srv = 2; srv <= p; srv ++ ){
		vector<line_t<int,number>> bag;
		int bag_i = 0;

		opt_cat[ mod( srv, 2 ) ][ m ] = 0;

		number delta_sum = 0;
		for( int i = m - 1; i >= 0; i -- ){
			delta_sum += cat_delta[ i ];

			number line_a = cat_delta[ i ];
			number line_b = ( - cat_delta[ i ] ) * ( m - i )
					+ opt_cat[ mod( srv - 1, 2 ) ][ i + 1 ]
					+ delta_sum;

			add_line( bag, bag_i, line_t<int,number>( line_a, line_b ) );
			opt_cat[ mod( srv, 2 ) ][ i ] = query_line( bag, bag_i, m - i )  - delta_sum;
		}
	}

	cout << opt_cat[ mod( p, 2 ) ][ 0 ] << endl;
	return 0;
}
