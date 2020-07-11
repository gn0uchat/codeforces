#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>
#include<string>
using namespace std;

int ceil( int num, int dem ){
	return ( num % dem == 0 ? num / dem : 1 + ( num / dem ) );
}

int main(){
	int t;
	scanf( "%d", &t );
	while( t --> 0 ){
		int n, x;
		scanf( "%d %d", &n, &x );
		int max_d = 0, max_dec = 0;
		for( int i = 0; i < n; i ++ ){
			int d, h;
			scanf( "%d %d", &d, &h );
			max_d = max( max_d, d );
			max_dec = max( max_dec, d - h );
		}

		int move = 0;

		if( max_d >= x ){
			move = 1;
		}else{
			if( max_dec > 0 ){
				move = 1 + ceil( x - max_d, max_dec );
			}else{
				move = -1;
			}
		}
		printf( "%d\n", move );
	}
	return 0;
}
