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

vector<bool> in_tree;
int max_li;

int max_out_tree(){
	for(; max_li > 0 && in_tree[ max_li ]; max_li -- );
	return max_li;
}

int main(){
	int n;
	cin >> n;

	in_tree.resize( n + 1, false );
	vector<int> tree_father( n + 1, 0 );
	vector<int> wire_ml;

	for( int i = 0; i < n - 1; i ++ ){
		int v;
		cin >> v;
		wire_ml.push_back( v );
	}

	max_li = n;

	int last = 0;
	for( auto ml : wire_ml ){
		if( in_tree[ ml ] ){
			int max_l = max_out_tree();

			tree_father[ max_l ] = last;
			in_tree[ max_l ] = true;
		}else{
			tree_father[ ml ] = last;
			in_tree[ ml ] = true;
		}
		last = ml;
	}

	tree_father[ max_out_tree() ] = last;

	cout << wire_ml[ 0 ] << endl;
	for( int l = 1; l <= n; l ++ ){
		if( tree_father[ l ] != 0 ){
			printf( "%d %d\n", l, tree_father[ l ] );
		}
	}

	return 0;
}
