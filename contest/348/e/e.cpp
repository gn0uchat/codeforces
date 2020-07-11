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

template<int n, typename V_T, typename TAG_T>
class nmaxset{
private:
	int nset_n;
public:
	tuple<V_T, TAG_T> nset[ n ];
	nmaxset(): nset_n( 0 ){}

	int size() const { return nset_n; }

	nmaxset<n, V_T, TAG_T> filter_tag( TAG_T tag ){
		nmaxset<n, V_T, TAG_T> outset;
		for( int i = 0; i < size(); i ++ ){
			auto[ e_v, e_tag ] = nset[ i ];
			if( !( e_tag == tag ) ){
				outset.insert( e_v, e_tag );
			}
		}
		return outset;
	}

	tuple<V_T, TAG_T> max() const {
		if( size() == 0 ){
			return { V_T(), TAG_T() };
		}else{
			return nset[ 0 ];
		}
	}

	V_T max_value() const {
		if( size() == 0 ){
			return V_T();
		}else{
			auto[ v, t ] = nset[ 0 ];
			return v;
		}
	}

	int value_count( V_T qv ) const {
		int c = 0;
		for( int i = 0; i < size(); i ++ ){
			auto[ v, tag ] = nset[i];
			if( v == qv ){ c ++; }
		}
		return c;
	}
	
	void insert( V_T new_v, TAG_T tag ){
		bool do_swap = false;
		tuple<V_T, TAG_T> tmp;

		tmp = { new_v, tag };

		for( int i = 0 ; i < size(); i ++ ){
			auto& [ e_v, e_i ] = nset[ i ];
			if( do_swap ){
				swap( tmp, nset[ i ] );
			}else if( e_v < new_v ){
				do_swap = true;
				swap( tmp, nset[ i ] );
			}
		}

		if( size() < n ){
			nset[ nset_n ++ ] = tmp;
		}
	}
};

class cd_node_t{
public:
	int n;
	int fth;
	list<tuple<int,int, int>> cld;
};

tuple< int, vector<cd_node_t> > build_centroid_tree( const vector<list<tuple<int, int>>>& adj ){
	int node_n = adj.size();
	vector<cd_node_t> cd_tree( node_n );

	vector<int> sub_tree_size( node_n, 0 );
	vector<bool> picked( node_n, false );

	function<int(int, int)>
	dfs_sub_tree_size = [&]( int i, int fi ) -> int {
		sub_tree_size[ i ] = 1;
		for( auto [ j, ignore ] : adj[ i ] ){
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
		for( auto [ j, cost ] : adj[ i ] ){
			if( picked[ j ] ){ continue; }
			int cd_j = search_centroid( j, i, dfs_sub_tree_size( j, i ));
			the_tree.cld.push_back({ j, cd_j, cost });
			cd_tree[ cd_j ].fth = i;
		}
		return i;
	};

	search_centroid = [&]( int i, int fi, int tree_size ) -> int {
		for( auto [ j, ignore ] : adj[ i ] ){
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

class flex_v{
public:
	bool exist;
	int  value;

	flex_v(): exist( false ), value(0){}
	flex_v( int v ): exist( true ), value( v ) {} 

	bool operator< ( const flex_v& r ) const {
		if( this->exist && r.exist ){
			return this-> value < r.value;
		}else{
			return !( this->exist );
		}
	}

	bool operator> ( const flex_v& r ) const {
		if( this->exist && r.exist ){
			return this-> value > r.value;
		}else{
			return this->exist;
		}
	}


	bool operator== ( const flex_v& r ) const {
		return ( exist == r.exist ) && ( value == r.value );
	}

	bool operator== ( int v ) const {
		if( exist ){ return v == value; }
		else{ return false; }
	}

	flex_v operator+ ( int v ) const {
		if( this->exist ){
			return flex_v( this->value + v );
		}else{
			return flex_v();
		}
	}
};

int main(){

	int n, m;
	cin >> n >> m;
	vector<list<tuple<int,int>>> adj( n );
	vector< tuple< bool, int > > node( n, { false, 0 } );

	for( int i = 0; i < m; i ++ ){
		int d;
		cin >> d;
		d --;
		node[ d ] = { true, 0 };
	}

	for( int i = 0; i < n - 1; i ++ ){
		int a, b, c;
		cin >> a >> b >> c;
		a --; b --;
		adj[ a ].push_back( { b, c } );
		adj[ b ].push_back( { a, c } );
	}

	vector< bool > mark( n, false );
	vector< bool > mark_m( n, false );
	vector< int > inters( n, 0 );

	vector<flex_v> dn_max_path( n, flex_v() );

	function<void(int, const vector<cd_node_t>&)>
	trav_cd = [&]( int ci, const vector<cd_node_t>& cd_tree ){
		auto& [ live_m, angry ] = node[ ci ];
		const auto& cd_child = cd_tree[ ci ].cld;

		mark[ ci ] = true;
		{
			vector<int> out_path_within( cd_child.size() + 1, 0 );

			function<void(int,int)>
			dfs_dn_max_path = [&]( int u, int f ){
				auto[ live_m, angry ] = node[ u ];
				inters[ u ] = 0;
				if( live_m ){
					dn_max_path[ u ] = flex_v( 0 );
				}else{
					dn_max_path[ u ] = flex_v();
				}

				for( auto [ v, cost ] : adj[ u ] ){
					if( mark[ v ] || ( v == f ) ){ continue; }
					dfs_dn_max_path( v, u );
					dn_max_path[ u ] = max( dn_max_path[ u ], dn_max_path[ v ] + cost );
				}
			};

			function<int(int, int, flex_v, flex_v, int, int )>
			dfs_angry_out = [&]( int u, int f, flex_v up_path, flex_v out_path, int out_path_tag, int up_path_tag ) -> int {
				auto&[ live_m, angry ] = node[ u ];

				auto collect_sb_path = [&]() -> nmaxset<2, flex_v, int>{
					nmaxset<2,flex_v,int> sb_path;
					if( live_m ){
						sb_path.insert( flex_v( 0 ), u );
					}else{
						sb_path.insert( flex_v(), u );
					}

					for( auto [ v, cost ] : adj[ u ] ){
						if( mark[ v ] || ( v == f ) ){ continue; }
						sb_path.insert( dn_max_path[ v ] + cost, v );
					}
					return sb_path;
				};

				nmaxset<2, flex_v, int> sb_path_pool = collect_sb_path();

				int out_path_c = 0;
				if( live_m && ! mark_m[ u ] ){
					auto opt_dn_path = sb_path_pool.max_value();
					if( out_path > max( opt_dn_path, up_path ) ){
						mark_m[ u ] = true;
						out_path_c ++;
						out_path_within[ out_path_tag ] ++;
					}else if( out_path == max( opt_dn_path, up_path ) ){
						mark_m[ u ]= true;
						if( out_path == opt_dn_path ){
						}else{
							out_path_c ++;
							inters[ up_path_tag ] ++;
						}
					}
				}

				for( auto [ v, cost ] : adj[ u ] ){
					if( mark[ v ] || ( v == f ) ){ continue; }
					auto opt_dn_path = sb_path_pool.filter_tag( v ).max_value();
					if( opt_dn_path > up_path ||
						opt_dn_path == up_path ){
						out_path_c += dfs_angry_out( v, u, opt_dn_path + cost, out_path + cost,
							out_path_tag, u );
					}else{
						out_path_c += dfs_angry_out( v, u, up_path + cost, out_path + cost,
							out_path_tag, up_path_tag );
					}
				}
				angry += out_path_c;
				out_path_c -= inters[ u ];
				return out_path_c;
			};

			function<void(int, int, int)>
			dfs_angry_within = [&]( int u, int f, int c ){
				auto& [ live_m, angry ] = node[ u ];
				angry += c;
				if( dn_max_path[ u ] == 0 ){
					return;
				}else{
					int max_path_v = 0;
					int max_path_c = 0;
					for( auto[ v, cost ] : adj[ u ] ){
						if( mark[ v ] || ( v == f ) ){ continue; }
						if( dn_max_path[ v ] + cost == dn_max_path[ u ] ){
							max_path_v = v;
							max_path_c ++;
						}
					}
					if( max_path_c > 1 ){
						return;
					}else{
						dfs_angry_within( max_path_v, u, c );
					}
				}
			};


			auto collect_out_path = [&]() -> nmaxset<3, flex_v, int>{
				nmaxset<3, flex_v, int> r_out_path;
				if( live_m ){
					r_out_path.insert( flex_v( 0 ), 0 );
				}else{
					r_out_path.insert( flex_v(), 0 );
				}

				int sb_i = 1;
				for( auto[ v, cd_v, cost ] : cd_child ){
					r_out_path.insert( dn_max_path[ v ] + cost, sb_i );
					sb_i ++;
				}
				return r_out_path;
			};

			dfs_dn_max_path( ci, ci );

			nmaxset<3, flex_v, int> r_out_path = collect_out_path();

			if( live_m && ! mark_m[ ci ] ){
				auto [ the_max_path, the_max_path_tag ]= r_out_path.max();
				if( the_max_path.exist ){
					mark_m[ ci ] = true;
					if( r_out_path.value_count( the_max_path ) == 1 ){
						out_path_within[ the_max_path_tag ] ++;
					}else{
						out_path_within[ 0 ] ++;
					}
				}
			}

			int sb_i = 1;
			for( auto[ v, cd_v, cost ] : cd_child ){
				auto [ the_max_path, the_max_path_tag ]= r_out_path.filter_tag( sb_i ).max();
				if( the_max_path.exist ){
					if( r_out_path.filter_tag( sb_i ).value_count( the_max_path ) == 1 ){
						dfs_angry_out( v, ci, flex_v(), the_max_path.value + cost, the_max_path_tag, ci );
					}else{
						dfs_angry_out( v, ci, flex_v(), the_max_path.value + cost, 0, ci );
					}
				}
				sb_i ++;
			}

			angry += out_path_within[ 0 ];
			sb_i = 1;
			for( auto[ v, cd_v, cost ] : cd_child ){
				if( out_path_within[ sb_i ] > 0 ){
					angry += out_path_within[ sb_i ];
					dfs_angry_within( v, ci, out_path_within[ sb_i ] );
				}
				sb_i ++;
			}
		}

		for( auto[ v, cd_v, cost ] : cd_child ){
			trav_cd( cd_v, cd_tree );
		}
	};

	auto[ cd_r, cd_tree ] = build_centroid_tree( adj );

	trav_cd( cd_r, cd_tree );
	
	int angry_max = 0, angry_max_n = 0;
	for( auto [ live_m, v ] : node ){
		if( live_m ){ continue; }
		angry_max = max( angry_max, v );
	}

	for( auto [ live_m, v ] : node ){
		if( live_m ){ continue; }
		if( v == angry_max ){ angry_max_n ++ ;}
	}

	printf( "%d %d\n", angry_max, angry_max_n );

	return 0;
}

