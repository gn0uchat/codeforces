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

#define for_all_char( CHAR_VAR ) for( char CHAR_VAR = 'a'; CHAR_VAR <= 'z'; CHAR_VAR ++ )

int ceil( int num, int dem ){
	return ( num % dem == 0 ? num / dem : 1 + ( num / dem ) );
}

int query( int l, int r, vector< vector<string> > &sub ){
	printf( "? %d %d\n", l, r );
	fflush( stdout );
	
	int n = r - l + 1;

	for( int i = 0; i < n * ( n + 1 ) / 2; i ++ ){
		string s;
		cin >> s;
		sub[ s.length() ].push_back( s );
	}

	return 0;
}

int guess( const string &s ){
	printf( "! %s\n", s.c_str() );
	fflush( stdout );
	return 0;
}

map<char, int> distr_add( map<char, int> f, map<char, int> g ){
	map<char, int> h;
	for_all_char( c ){
		h[ c ] = f[ c ] + g[ c ];
	}
	return h;
}

map<char, int> distr_sub( map<char, int> f, map<char, int> g ){
	map<char, int> h;
	for_all_char( c ){
		h[ c ] = f[ c ] - g[ c ];
	}
	return h;
}

map<char, int> str2distr( const string &s ){
	map<char, int> h;
	for( auto c : s ){
		h[ c ] ++;
	}
	return h;
}
void print_distr( map<char, int> h ){
	printf( "distr{ ");
	for_all_char( c ){
		printf( "%d ", h[ c ]);
	}
	printf( "}\n");
}
string distr2str( map<char, int> h ){
	string s;
	for_all_char( c ){
		for( int i = 0; i < h[ c ]; i ++ ){
			s += c;
		}
	}

	return s;
}



map<char, int> qstr2distr( const vector<string> &q_str ){
	map<char, int> d;
	for( auto s : q_str ){
		d = distr_add( d, str2distr( s ));
	}
	return d;
}

int main(){
	int n;
	cin >> n;

	if( n == 1 ){
		vector<vector<string>> s( 2 );
		query( 1, n, s );
		guess( s[ 1 ][ 0 ] );

	}else if( n == 2 ){
		vector<vector<string>> s( 2 ), t( 2 );
		query( 1, 1, s );
		query( 2, 2, t );
		guess( s[ 1 ][ 0 ] + t[ 1 ][ 0 ] );

	}else{
		int half_n = ceil( n, 2 );
		vector<vector<string>> whole( n + 1 );
		vector<vector<string>> half( half_n + 1 );
		vector<vector<string>> halfsub( half_n );

		query( 1, n, whole );
		query( 1, half_n, half );
		query( 2, half_n, halfsub );

		string first_half, second_half;

		map<char, int> last_d;

		last_d.clear();
		for( int l = half_n; l > 0; l -- ){
			map<char, int> delta;
			delta = distr_sub( qstr2distr( half[ l ] ), qstr2distr( halfsub[ l - 1 ]));
			first_half += distr2str( distr_sub( delta, last_d ));
			last_d = delta;
		}


		last_d.clear();
		for( int l = half_n , j = half_n - 1; j >= 0 ; l --, j -- ){
			map<char, int> delta, remove;
			string s = string( first_half.substr( j, 1 ) );

			delta = distr_sub( qstr2distr( whole[ l ] ), qstr2distr( whole[ l - 1 ] ) );
			remove = distr_add( str2distr( s ), last_d );
			second_half += distr2str( distr_sub( delta, remove ) );

			last_d = delta;
		}

		guess( first_half + second_half );

	}
	return 0;
}
