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
	int tree_n = adj.size();
	vector<cd_node_t> cd_tree( tree_n );

	vector<bool> visit( tree_n, false );
	vector<int> sub_tree_n( tree_n, 0 );
	vector<bool> picked( tree_n, false );

	function<int(int)> dfs;
	dfs = [&]( int nd_i ) -> int {

		/*
		list<int> acyc;
		acyc.push_back( nd_i );
		for( auto it = acyc.begin(); it != acyc.end(); ++ it ){
			auto i = * it;
			visit[ i ] = true;
			for( auto j : adj[ i ] ){
				if( picked[ j ] || visit[ j ] ){ continue; }
				acyc.push_back( j );
			}
		}

		for( auto it = acyc.rbegin(); it != acyc.rend(); ++ it ){
			auto i = * it;
			sub_tree_n[ i ] = 1;
			for( auto j : adj[ i ] ){
				if( picked[ j ] || visit[ j ] ){ continue; }
				sub_tree_n[ i ] += sub_tree_n[ j ];
			}
			visit[ i ] = false;
		}
		*/

		///*
		visit[ nd_i ] = true;
		int& tree_n = sub_tree_n[ nd_i ];
		tree_n = 1;
		for( auto j : adj[ nd_i ] ){
			if( picked[ j ] || visit[ j ]){ continue; }
			tree_n = tree_n + dfs( j );
		}
		visit[ nd_i ] = false;
		//*/
		return sub_tree_n[ nd_i ];
	};

	function<int(int, int, int)> search_centroid;
	function<int(int, int)> find_centroid;

	search_centroid = [&]( int nd_i, int prev_i, int tree_n ) -> int {
		int threshold = tree_n / 2;
		bool overweight = false;
		int i = nd_i, pi = prev_i;
		do{
			overweight = false;
			for( auto j : adj[ i ] ){
				if( ( picked[ j ] == true ) || ( j == pi ) ){ continue; }
				if( sub_tree_n[ j ] > threshold ){
					overweight = true;
					pi = i;
					i = j;
					break;
				}
			}
		}while( overweight );
		return find_centroid( i, tree_n );
	};

	find_centroid = [&]( int nd_i, int n ) -> int {
		cd_node_t& nd = cd_tree[ nd_i ];

		picked[ nd_i ] = true;

		nd.n = n;
		for( auto j : adj[ nd_i ] ){
			if( picked[ j ] == true ){ continue; }

			dfs( j );
			int sub_cd = search_centroid( j, nd_i, sub_tree_n[ j ] );
			cd_node_t& sub_nd = cd_tree[ sub_cd ];

			sub_nd.fth = nd_i;
			nd.cld.push_back( { j, sub_cd } );
		}
		return nd_i;
	};

	dfs( 0 );
	int r = search_centroid( 0, 0, sub_tree_n[ 0 ] );
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
		return s( bnd.l ) < b( bnd.l ) && s( bnd.r - 1 ) < b( bnd.r - 1 );
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
	cht_tree( VAR_T l, VAR_T r ) : full_segm( l, r ), tree_node( 4 * ( r - l ), {false, LN_T()} ){}

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
	for( int i = 0; i < n-1; i ++ ){
		int u, v;
		cin >> u >> v;
		u --;
		v --;
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

	vector<bool> visit( n, false );

	function<long long int(int)>
	cd_tree_dfs = [&]( int i ) -> long long int{
		auto nd = cd_tree[ i ];
		
		typedef tuple<int, long long int, long long int > path_t;
		list<path_t> in_path, out_path;
		list<int> path_n;

		stack<tuple<int, path_t, path_t>> path_stack;

		function<int(int, path_t, path_t)>
		find_path = [&]( int i, path_t op, path_t ip ) -> int {
		// auto find_path = [&]( int i ) -> int {

			/*
			int find_path_n = 0;
			path_stack.push({ i, { 0, 0, 0 }, { 0, 0, 0 } });

			while( ! path_stack.empty() ){
				auto[ i, op, ip ] = path_stack.top();
				path_stack.pop();

				if( visit[ i ] ){
					visit[ i ] = false;
				}else{
					long long int v = value[ i ];
					auto [ on, osum, opf ] = op;
					auto [ in, isum, ipf ] = ip;
					path_t the_ip = { in + 1, isum + v, ipf + v * ( (long long int) ( in + 1 )) };
					path_t the_op = { on + 1, osum + v, opf + osum + v };

					visit[ i ] = true;
					path_stack.push( { i, op, ip } );

					bool is_leaf = true;

					for( auto j : adj[ i ] ){
						if( visit[ j ] ){ continue; }
						is_leaf = false;
						path_stack.push({ j, the_op, the_ip });
					}

					if( is_leaf ){
						out_path.push_back( the_op );
						in_path.push_back( the_ip );
						find_path_n ++;
					}
				}
			}

			return find_path_n;
			*/
			

			///*
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

			visit[ i ] = false;

			if( r == 0 ){
				out_path.push_back( the_op );
				in_path.push_back( the_ip );
				return 1;
			}else{
				return r;
			}
			//*/
		};

		long long int max_cost = 0;

		visit[ i ] = true;
		for( auto[ oj, j ] : cd_tree[ i ].cld ){
			max_cost = max( max_cost, cd_tree_dfs( j ) );
		}

		in_path.push_back( { 0, 0, 0 } );
		out_path.push_back( { 0, 0, 0 } );
		path_n.push_back( 1 );
		
		for( auto[ oj, j ] : cd_tree[ i ].cld ){
			//path_n.push_back( find_path( oj, { 0, 0, 0 }, { 0, 0, 0 } ) );
			path_n.push_back( find_path( oj ) );
		}
		visit[ i ] = false;

		long long int v = value[ i ];

		auto itr_path = [&](){
			cht_tree<int, long long int> bag( 0, ( nd.n / 2 + 1 ) + 1 );
			bag.insert( 0, 0 );

			auto f = in_path.begin();
			auto g = out_path.begin();
			for( auto n : path_n ){
				for( int i = 0; i < n; i ++ ){
					auto[ pn, ps, pf ] = *g;
					long long int cn = pn + 1;
					long long int cf = pf + v * ((long long int) ( pn + 1 ));

					max_cost = max( max_cost, bag.query( cn ) + cf );

					++ g;
				}

				for( int i = 0; i < n; i ++ ){
					auto[ pn, ps, pf ] = *f;
					bag.insert( ps, pf );
					++ f;
				}
			}
		};

		/*
		auto ritr_path = [&](){
			cht_tree<int, long long int> bag( 0, ( nd.n / 2 + 1 ) + 1 );
			bag.insert( 0, 0 );

			auto f = in_path.rbegin();
			auto g = out_path.rbegin();
			for( auto it = path_n.rbegin(); it != path_n.rend(); ++ it ){
				auto n = * it;
				for( int i = 0; i < n; i ++ ){
					auto[ pn, ps, pf ] = *g;
					long long int cn = pn + 1;
					long long int cf = pf + v * ((long long int) ( pn + 1 ));

					max_cost = max( max_cost, bag.query( cn ) + cf );

					++ g;
				}

				for( int i = 0; i < n; i ++ ){
					auto[ pn, ps, pf ] = *f;
					bag.insert( ps, pf );
					++ f;
				}
			}
		};
		*/
		// ritr_path();

		itr_path();

		in_path.reverse(); out_path.reverse(); path_n.reverse();
		itr_path();

		return max_cost;
	};

	cout << cd_tree_dfs( cd_r ) << endl;

	return 0;
}
