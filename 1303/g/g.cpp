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
#include<stack>
using namespace std;

class node_t{
public:
	long long v;
	int fth;
	list<int> chld;
	int n; 
};

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
		auto& this_size = sub_tree_size[ i ];
		this_size = 1;
		for( auto j : adj[ i ] ){
			if( picked[ j ] || ( j == fi ) ){ continue; }
			this_size += dfs_sub_tree_size( j, i );
		}
		return this_size;
	};

	function<int(int, int, int)> search_centroid;
	function<int(int, int)> found_centroid;

	found_centroid = [&]( int i, int tree_size ) -> int {
		//fprintf( stderr, "[debug] found centroid [ %d %d]\n", i, tree_size );
		auto& the_tree = cd_tree[ i ];
		picked[ i ] = true;
		the_tree.n = tree_size;
		for( auto j : adj[ i ] ){
			if( picked[ j ] ){ continue; }
			int sub_size = dfs_sub_tree_size( j, i );
			int cd_j = search_centroid( j, i, sub_size );
			the_tree.cld.push_back({ j, cd_j });
			cd_tree[ cd_j ].fth = i;
		}
		return i;
	};

	search_centroid = [&]( int i, int fi, int tree_size ) -> int {
		for( auto j : adj[ i ] ){
			if( picked[ j ] || ( j == fi ) ){ continue; }
			if( sub_tree_size[ j ] > ( tree_size / 2 ) ){
				return search_centroid( j, i, tree_size );
			}
		}
		return found_centroid( i, tree_size );
	};

	int tree_size = dfs_sub_tree_size( 0, 0 );
	int r = search_centroid( 0, 0, tree_size );
	cd_tree[ r ].fth = r;

	return { r, cd_tree };
}

template< typename T>
class segm_t{
public:
	T l, r;
	segm_t(){}
	segm_t( T b ):
		l( b ), r( b + 1 ){}
	segm_t( T l, T r ):
		l( l ), r( r ){}

	bool is_null() const {
		return this->l == this->r;
	}

	bool contain( const segm_t<T>& s ) const {
		return ( this->l <= s.l ) && ( this->r >= s.r );
	}

	T mid() const {
		return ( this->l + this->r ) / 2;
	}

	tuple<segm_t<T>, segm_t<T>> split_half() const {
		return { segm_t<T>( this->l, this->mid() ), segm_t<T>( this->mid(), this->r ) };
	}

	bool is_unit() const {
		return ( this->l + 1 == this->r );
	}

};

template< typename X_T, typename Y_T >
class line_t{
private:
	Y_T a, b;
public:
	line_t(){}
	line_t( Y_T a, Y_T b ):a( a ), b( b ){}
	Y_T operator() ( X_T x ) const {
		return a * ( ( Y_T ) x ) + b;
	}
};

template< typename VAR_T, typename COEF_T>
class cht_tree{
private:
	typedef line_t<VAR_T, COEF_T> LN_T;

	vector<tuple<bool, LN_T>> tree_node;
	segm_t< VAR_T > full_segm;

	bool line_ge( segm_t<VAR_T> bnd, const LN_T& b, const LN_T& s ){
		return s( bnd.l ) <= b( bnd.l ) && s( bnd.r - 1 ) <= b( bnd.r - 1 );
	}

	tuple<bool, LN_T> tree_query( int i, const segm_t<VAR_T>& bnd, VAR_T x ){
		if( bnd.is_unit() ){
			return tree_node[ i ];
		}else{
			auto [ set, segm_l ] = tree_node[ i ];

			if( set ){
				segm_t<VAR_T> next_bnd;
				int next_i;

				auto[ l_bnd, r_bnd ] = bnd.split_half();

				if( l_bnd.contain( segm_t<VAR_T>( x ) ) ){
					next_bnd = l_bnd;
					next_i = 2 * i;
				}else{ 
					next_bnd = r_bnd;
					next_i = 2 * i + 1;
				}

				auto[ sb_set, sb_l ] = tree_query( next_i, next_bnd, x );
				if( sb_set && ( segm_l( x ) < sb_l( x ) ) ){
					segm_l = sb_l;
				}
			}
			return { set, segm_l };
		}
	}

	void tree_insert( int i, const segm_t<VAR_T>& bnd, const LN_T& l ){
		auto& [ set, segm_l ] = tree_node[ i ];

		if( ! set ){
			set = true;
			segm_l = l;
		}else{
			if( line_ge( bnd, segm_l, l ) ){
			}else if( line_ge( bnd, l, segm_l ) ){
				segm_l = l;
			}else{
				auto[ l_bnd, r_bnd ] = bnd.split_half();
				VAR_T m = bnd.mid();

				LN_T up_l = l(m) < segm_l(m) ? segm_l : l;
				LN_T dn_l = l(m) < segm_l(m) ? l : segm_l;

				segm_l = up_l;

				if( up_l( bnd.l ) < dn_l( bnd.l ) ){
					tree_insert( 2 * i, l_bnd, dn_l );
				}else{
					tree_insert( 2 * i + 1, r_bnd, dn_l );
				}
			}
		}
	}

public:
	cht_tree(){}
	cht_tree( VAR_T l, VAR_T r ) : full_segm( l, r ), tree_node( 4 * ( r - l + 1 ), {false, LN_T()} ){}

	void insert( COEF_T a, COEF_T b ){
		tree_insert( 1, full_segm, LN_T( a, b ) );
	}

	auto query( VAR_T x ){
		auto[ set, l ] = tree_query( 1, full_segm, x );
		return l( x );
	}
};

int main(){
	int n;

	cin >> n;

	vector<list<int>> adj(n);
	for( int i = 0; i < n - 1; i ++ ){
		int u, v;
		cin >> u >> v;
		u --; v --;
		adj[ u ].push_back( v );
		adj[ v ].push_back( u );
	}

	vector<int> value( n );

	for( int i = 0; i < n; i ++ ){
		int v;
		cin >> v;
		value[ i ] = v;
	}

	auto[ cd_r, cd_tree ] = build_centroid_tree( adj );

	vector<bool> mark( n, false );

	function<long long int(int)>
	centroid_max_cost = [&]( int ci ) -> long long int {
		typedef tuple<int, long long int, long long int > path_t;

		int cld_n = cd_tree[ ci ].cld.size();
		vector< list< path_t >> head_set( cld_n + 1 ), tail_set( cld_n + 1 );

		head_set[0].push_back( { 0, 0, 0 } );
		tail_set[0].push_back( { 0, 0, 0 } );

		function<void(int,int,int, path_t, path_t )>
		dfs_search_path = [&]( int i, int fi, int set_i, path_t head, path_t tail ){
			bool is_leaf = true;

			long long int v = value[ i ];
			auto[ hn, hs, hp ] = head;
			auto[ tn, ts, tp ] = tail;

			path_t the_head = { hn + 1, hs + v, hp + v * ((long long int)( hn + 1 ))};
			path_t the_tail = { tn + 1, ts + v, tp + ts + v };

			for( auto j : adj[ i ] ){
				if( mark[ j ] || ( j == fi ) ){ continue; }
				is_leaf = false;
				dfs_search_path( j, i, set_i, the_head, the_tail );
			}

			if( is_leaf ){
				head_set[ set_i ].push_back( the_head );
				tail_set[ set_i ].push_back( the_tail );
			}
		};

		mark[ ci ] = true;

		long long int max_cost = value[ ci ];

		int set_i = 1;
		for( auto[ nb_j, cd_j ] : cd_tree[ ci ].cld ){
			dfs_search_path( nb_j, ci, set_i , {0, 0, 0}, {0, 0, 0} );
			set_i ++;
		}

		
		auto comb_head_tail = [&](){
			cht_tree<int, long long int> bag( 0, ( (cd_tree[ ci ].n / 2) + 1 ) + 1 );
			bag.insert( 0, 0 );

			long long int v = value[ ci ];
			for( int i = 0; i < tail_set.size(); i ++ ){
				for( auto& tail : tail_set[ i ] ){
					auto[ n, a, b ] = tail;
					long long int path_cost = b + v * ( (long long int) ( n + 1 ));
					path_cost += bag.query( n + 1 );
					max_cost = max( max_cost, path_cost );
				}
				
				for( auto head : head_set[ i ] ){
					auto[ n, a, b ] = head;
					bag.insert( a, b );
				}
			}
		};

		comb_head_tail();

		reverse( head_set.begin(), head_set.end() );
		reverse( tail_set.begin(), tail_set.end() );
		comb_head_tail();

		head_set.clear();
		tail_set.clear();

		for( auto[ nb_j, cd_j ] : cd_tree[ ci ].cld ){
			max_cost = max( max_cost, centroid_max_cost( cd_j ) );
		}

		return max_cost;
	};

	cout << centroid_max_cost( cd_r ) << endl;

	return 0;
}
