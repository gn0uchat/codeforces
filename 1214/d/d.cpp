#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>
#include<string>
#include<functional>
#include<iostream>
using namespace std;

int main(){
	int n, m;

	cin >> n >> m;

	vector<vector<bool>> island(n, vector<bool>(m));

	for( int i = 0; i < n; i ++ ){
		string row;
		cin >> row;
		for( int j = 0; j < m; j ++ ){
			island[i][j] = ( row[ j ] == '.' ? true : false );
		}
	}

	function<bool()> walk = [&]() -> bool {
		vector<vector<bool>> reach(n, vector<bool>(m));
		for( int i = n - 1; i >= 0; i -- ){
			for( int j = m - 1; j >= 0; j -- ){
				bool reach_c = false;

				if( i == n - 1 && j == m - 1 ){
					reach_c = true;
				}else{
					if( ! island[ i ][ j ] ){
						reach_c = false;
					}else{
						if( i < n - 1 ){
							reach_c = reach_c || reach[ i + 1 ][ j ];
						}
						if( j < m - 1 ){
							reach_c = reach_c || reach[ i ][ j + 1 ];
						}
					}
				}
				reach[ i ][ j ] = reach_c;
			}
		}

		if( reach[ 0 ][ 0 ] ){
			for( int i = 0, j = 0; !( i == n - 1 && j == m - 1 ); ){
				island[ i ][ j ] = false;
				if( i < n - 1 && reach[ i + 1 ][ j ] ){
					i = i + 1;
				}else{
					j = j + 1;
				}
			}
			island[ 0 ][ 0 ] = true;
			return true;
		}

		return false;
	};

	int flow = 0;
	while( walk() ){ flow++; }

	printf( "%d\n", flow );
	return 0;
}
