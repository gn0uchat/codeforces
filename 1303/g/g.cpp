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
	int tree_n = tree.size();
	vector<cd_node_t> cd_tree( tree_n, {0, list<int>});

	vector<bool> visit( tree_n, false );
	vector<int> sub_tree_n( tree_n, 0 );
	vector<bool> picked( tree_n, false );

	function<int(int)> dfs;
	dfs = [&]( int nd_i ){
	
		visit[ nd_i ] = true;
		int& tree_n = sub_tree_n[ nd_i ];
		tree_n = 1;
		for( auto j : adj[ nd_i ] ){
			if( picked[ j ] ){ continue; }
			if( visit[ j ] == true ){ continue; }
			tree_n = tree_n + dfs( j );
		}
		visit[ nd_i ] = false;
	};

	function<int(int, int)> search_centroid;
	function<int(int)> find_centroid;

	search_centroid = [&]( int nd_i, int prev_i, int prev_n ) -> int {
		int threshold = ( sub_tree_n[ nd_i ] + prev_n ) / 2;
		int overweight = false;
		int ow_i = 0;

		for( auto j : adj[ nd_i ] ){
			if( picked[ j ] == true ){ continue; }
			if( j == prev_i ){ continue; }
			if( sub_tree_n[ j ] > threshold ){
				overweight = true;
				ow_i = j;
				break;
			}
		}

		int tree_n = sub_tree_n[ nd_i ] + prev_n;
		if( overweight == true ){
			return search_centroid( ow_i, nd_i, tree_n - sub_tree_n[ ow_i ] );
		}else{
			return find_centroid( ow_i, tree_n );
		}
	}

	find_centroid = [&]( int nd_i, int n ) -> int {
		cd_node_t& nd = cd_tree[ nd_i ];

		picked[ nd_i ] = true;

		nd.n = n;
		for( auto j : adj[ nd_i ] ){
			if( picked[ j ] == true ){ continue; }

			dfs( j );
			int sub_cd = search_centroid( j, nd_i, 0 );
			cd_node_t& sub_nd = cd_tree[ sub_cd ];

			sub_nd.fth = nd_i;
			nd.cld.push_back( { j, sub_cd } );
		}
		return nd_i;
	};

	int r = search_centroid( 0, 0, 0 );
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
		return { segm_t( this->l, this->mid() ), segm_t( this->mid(), this->r ) };
	}

	bool unit() const {
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
	Y_T operator() ( X_T x ){
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
		return s( bnd.l ) < b( bnd.l ) && s( bnd.r - 1 ) < b( bnd.r - 1 );
	}

	tuple<bool, LN_T> tree_query( int i, const segm_t<VAR_T>& bnd, VAR_T x ){
		if( unit( bnd ) ){
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
				if( sb_set && segm_l( x ) < sb_l( x ) ){
					segm_l = sb_l;
				}
			}

			return { set, segm_l };
		}
		
	}

	void tree_insert( int i, const segm_t<VAR_T>& bnd, const LN_T& l ){
		auto& [ set, segm_l ] = tree_node[ i ];

		if( set == false ){
			set = true;
			segm_l = l;
		}else{
			if( line_ge( bnd, segm_l, l ) ){
			}else if( line_ge( bnd, l, segm_l ) ){
				segm_l = l;
			}else{
				auto[ l_bnd, r_bnd ] = bnd.split_half();
				VAR_T mid = bnd.mid();

				LN_T up_l = l(mid) < segm_l(mid) ? segm_l : l;
				LN_T dn_l = l(mid) < segm_l(mid) ? l : segm_l;

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
	cht_tree( VAR_T l, VAR_T r ) : full_segm( l, r ), tree_node( 4 * ( r - l ), {false, LN_T()} ){}

	void insert( COEF_T a, COEF_T b ){
		tree_insert( 0, full_segm, LN_T( a, b ) );
	}

	auto query( VAR_T x ){
		auto[ set, l ] = tree_query( 0, full_segm, x );
		return l( x );
	}
};

int main(){
	int n;
	cin >> n;

	vector<list<int>> adj(n);
	for( int i = 0; i < n-1; i ++ ){
		cin >> u >> v;
		u --;
		v --;
		adj[ u ].push_back( v );
		adj[ v ].push_back( u );
	}

	vector<int> value( n );

	for( int i = 0; i < n; i ++ ){
		cin >> value[ i ];
	}

	auto[ cd_r, cd_tree ] = build_centroid_tree( adj );

	vector<bool> visit( n, false );

	function<long long int(int)>
	cd_tree_dfs = [&]( int i ) -> long long int{
		
		typedef tuple<int, long long int, long long int > path_t;
		list<path_t> in_path, out_path;
		list<int> path_n

		function<int(int, path_t, path_t)>
		find_path = [&]( int i, path_t op, path_t ip ) -> int {
			long long int v = value[ i ];
			auto [ on, osum, opf ] = op;
			auto [ in, isum, ipf ] = ip;
			path_t the_ip = { in + 1, isum + v, ipf + v * ( (long long int) ( in + 1 )) };
			path_t the_op = { on + 1, osum + v, opf + osum + v };

			visit[ i ] = true;

			int r = 0;
			for( auto j : adj[ i ] ){
				if( visit[ j ] ){ continue; }
				r += find_path( j, the_op, the_ip );
			}

			if( r == 0 ){
				out_path.push_back( the_op );
				in_path.push_back( the_ip )
				return 1;
			}else{
				return r;
			}

			visit[ i ] = false;
		};

		long long max_cost = 0;

		visit[ i ] = true;
		for( auto[ oj, j ] : cd_tree[ i ].cld ){
			max_cost = max( max_cost, cd_tree_dfs( j ) );
		}

		for( auto[ oj, j ] : cd_tree[ i ].cld ){
			path_n.push_back( find_path( oj, { 0, 0, 0 }, { 0, 0, 0 } ) );
		}
		visit[ i ] = false;

		long long int v = value[ i ];

		auto itr_path = [&](){
			cht_t<int, long long int> bag( 0, n + 1 );

			auto f = in_path.begin();
			auto g = out_path.begin();
			for( auto n : path_n ){
				for( int i = 0; i < n; i ++ ){
					auto[ pn, ps, pf ] = *g;
					long long int cn = pn + 1;
					long long int cf = pf + v * ((long long int) ( pn + 1 ));

					long cost = bag.query( cn ) + cf;
					max_cost = max( max_cost, cost );

					++ g;
				}

				for( int i = 0; i < n; i ++ ){
					auto[ pn, ps, pf ] = *f;
					bag.insert( ps, pf );
					++ f;
				}
			}
		};

		itr_path();

		in_path.reverse();
		out_path.reverse();
		path_n.reverse();
		itr_path();

		return max_cost;
	};

	cout << cd_tree_dfs( cf_r ) << endl;

	return 0;
}
