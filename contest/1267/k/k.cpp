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
int main(){
	int t;
	cin >> t;
	while( t --> 0 ){

		long long int k;

		cin >> k;

		vector<int> fp;

		for( int d = 2; k != 0; k /= d, d ++ ){
			fp.push_back( k % d );
		}

		sort( fp.begin(), fp.end(), [](int a, int b){ return a < b;} );

		int fp_n = fp.size();
		int max_d = fp.size() + 1;

		long long int r = 1;
		for( int d = 2, i = 0, bc = 0; d <= max_d; d ++, i ++ ){
			for(; bc < fp.size() && fp[ bc ] < d; bc ++);
			r *= ( bc - i );
		}

		if( fp[ 0 ] == 0 ){
			int zn;
			for(zn = 0; fp[ zn ] == 0; zn ++ );

			long long int m = zn;
			for( int d = 2, i = 1, bc = zn; d < max_d; d ++, i ++ ){
				for(; bc < fp.size() && fp[ bc ] < d; bc ++);
				m *= ( bc - i );
			}

			r -= m;
		}

		for( int i = 0; i < fp.size();){
			int j;
			for( j = i; j < fp.size() && fp[ j ] == fp[ i ]; j ++ );
			for( int inv = (j - i); inv > 0; inv --){
				r /= inv;
			}
			i = j;
		}

		cout << r - 1 << endl;
	}
		
	return 0;
}
