class cd_node_t{
public:
	int n;
	int fth;
	list<tuple<int,int>> cld;
};

tuple< int, vector<cd_node_t> > build_centroid_tree( const vector<list<int>>& adj ){
	int node_n = adj.size();
	vector<cd_node_t> cd_tree( node_n );

	vector<int> sub_tree_size( node_n, 0 );
	vector<bool> picked( node_n, false );

	function<int(int, int)>
	dfs_sub_tree_size = [&]( int i, int fi ) -> int {
		sub_tree_size[ i ] = 1;
		for( auto j : adj[ i ] ){
			if( picked[ j ] || ( j == fi ) ){ continue; }
			sub_tree_size[ i ] += dfs_sub_tree_size( j, i );
		}
		return sub_tree_size[ i];
	};

	function<int(int, int, int)> search_centroid;
	function<int(int, int)> found_centroid;

	found_centroid = [&]( int i, int tree_size ) -> int {
		auto& the_tree = cd_tree[ i ];
		picked[ i ] = true;
		the_tree.n = tree_size;
		for( auto j : adj[ i ] ){
			if( picked[ j ] ){ continue; }
			int cd_j = search_centroid( j, i, dfs_sub_tree_size( j, i ));
			the_tree.cld.push_back({ j, cd_j });
			cd_tree[ cd_j ].fth = i;
		}
		return i;
	};

	search_centroid = [&]( int i, int fi, int tree_size ) -> int {
		for( auto j : adj[ i ] ){
			if( picked[ j ] || ( j == fi ) ){ continue; }
			if( sub_tree_size[ j ] > tree_size / 2 ){
				return search_centroid( j, i, tree_size );
			}
		}
		return found_centroid( i, tree_size );
	};

	int r = search_centroid( 0, 0, dfs_sub_tree_size( 0, 0 ) );
	cd_tree[ r ].fth = r;

	return { r, cd_tree };
}
