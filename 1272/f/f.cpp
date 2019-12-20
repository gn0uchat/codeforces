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
string s, t;
int sn, tn;

int _mcs[ 200 + 1][ 200 ][ 200 ];
bool _mcs_valid[ 200 + 1][ 200 ][ 200 ];

int inc_ifeq( string& s, int i, char c ){
	return s[ i ] == c ? i + 1 : i;
}

int next_left( char c, int left ){
	return c == '(' ? left + 1 : left - 1;
}

int mcs( int left, int si, int ti ){
	if( si == sn && ti == tn ){
		return left;
	}else if( _mcs_valid[ left ][ si ][ ti ] ){
		return _mcs[ left ][ si ][ ti ];
	}else{
		int opt;
		if( left > 0 ){
			if( si == sn ){
				opt = mcs( next_left( t[ ti ], left ), si, ti + 1 ); 
			}else if( ti == tn ){
				opt = mcs( next_left( s[ si ], left ), si + 1, ti ); 
			}else if( s[ si ] == t[ ti ] ){
				char c = s[ si ];
				opt = mcs( next_left( s[ si ], left ), si + 1, ti + 1); 
			}else{
				opt = min( mcs( left + 1,
						inc_ifeq( s, si, '('),
						inc_ifeq( t, ti, '(')),
					   mcs( left - 1,
						inc_ifeq( s, si, ')'),
						inc_ifeq( t, ti, ')')));
			}
		}else{
			opt = mcs( left + 1,
				   inc_ifeq( s, si, '(' ),
				   inc_ifeq( t, ti, '(' ));
		}
		_mcs[ left ][ si ][ ti ] = 1 + opt;
		_mcs_valid[ left ][ si ][ ti ] = true;
		return _mcs[ left ][ si ][ ti ];
	}
}



int main(){
	cin >> s >> t;

	sn = s.length();
	tn = t.length();

	int left, si, ti;
	for( left = 0, si = 0, ti = 0; si < sn || ti < tn || left != 0 ;){
		char c;
		if( si == sn && ti == tn ){
			c = ')';
		}else if( left == 0 ){
			c = '(';
		}else{
			if( si == sn ){
				c = t[ ti ];
			}else if( ti == tn ){
				c = s[ si ];
			}else if( s[ si ] == t[ ti ] ){
				c = s[ si ];
			}else{
				int lc = mcs( left + 1,
					      inc_ifeq( s, si, '('),
					      inc_ifeq( t, ti, '('));

				int rc = mcs( left - 1,
					      inc_ifeq( s, si, ')'),
					      inc_ifeq( t, ti, ')'));
				c = lc < rc ? '(' : ')';
			}
		}

		cout << c;

		left = next_left( c, left );
		si = inc_ifeq( s, si, c );
		ti = inc_ifeq( t, ti, c );
	}
	cout << endl;
	return 0;
}
