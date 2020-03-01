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
#define debug( args... ) do{fprintf( stderr, "[debug] " );fprintf( stderr, args ); fprintf(stderr, "\n");}while(0)
using namespace std;
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
public:
	Y_T a, b;
	line_t(){}
	line_t( Y_T a, Y_T b ):a( a ), b( b ){}
	Y_T operator() ( X_T x ) const {
		return a * ( ( Y_T ) x ) + b;
	}
};

template< typename VAR_T, typename COEF_T>
class cht_tree{
private:
	typedef segm_t<VAR_T> var_segm_t;
	typedef line_t<VAR_T, COEF_T> cht_line_t;
	size_t elem_cnt;

	class tree_node{
	public:
		cht_line_t line;
		tree_node *left, *right;
		tree_node(): left(NULL), right(NULL){}
		tree_node( const cht_line_t& l ): line(l), left(NULL), right(NULL){}
	};

	tree_node* alloc_node( const cht_line_t& line ){
		elem_cnt ++;
		return new tree_node( line );
	}

	tree_node* root;
	var_segm_t full_segm;

	bool lt( COEF_T a, COEF_T b ){ return a > b; }
	bool ge( COEF_T a, COEF_T b ){ return ! lt( a,  b ); }

	bool line_ge( const cht_line_t& b, const cht_line_t& s, const var_segm_t& segm ){
		return ge( b( segm.l ), s( segm.l ) ) && ge( b( segm.r - 1 ), s( segm.r - 1 ) );
	}

	void node_clear( tree_node* nd ){
		if( nd->left ){
			node_clear( nd->left );
		}
		if( nd->right ){
			node_clear( nd->right );
		}
		delete nd;
	}

	void node_insert( tree_node* nd, const var_segm_t& nd_segm, const cht_line_t& new_line ){
		if( line_ge( nd->line, new_line, nd_segm ) ){
		}else if( line_ge( new_line, nd->line, nd_segm ) ){
			nd->line = new_line;
		}else{
			auto dn_line = lt( new_line( nd_segm.mid() ), nd->line( nd_segm.mid() ) )
					? new_line : nd->line;
			auto up_line = lt( new_line( nd_segm.mid() ), nd->line( nd_segm.mid() ) )
					? nd->line : new_line;

			nd->line = up_line;
			if( ! nd_segm.is_unit() ){
				auto[ left_segm, right_segm ] = nd_segm.split_half();
				if( lt( up_line( nd_segm.l ), dn_line( nd_segm.l ) ) ){
					if( nd->left ){
						node_insert( nd->left, left_segm, dn_line );
					}else{
						nd->left = alloc_node( dn_line );
					}
				}else{
					if( nd->right ){
						node_insert( nd->right, right_segm, dn_line );
					}else{
						nd->right = alloc_node( dn_line );
					}
				}
			}
		}
	}

	const cht_line_t& node_query( tree_node* nd, const var_segm_t& nd_segm, VAR_T x ){
		if( nd_segm.is_unit() ){
			return nd->line;
		}else{
			auto[ left_segm, right_segm ] = nd_segm.split_half();
			if( x <= nd_segm.mid() ){
				if( nd->left ){
					auto sub_l = node_query( nd->left, left_segm, x );
					return lt( sub_l( x ), nd->line( x ) ) ? nd->line: sub_l;
				}else{
					return nd->line;
				}
			}else{
				if( nd->right ){
					auto sub_l = node_query( nd->right, right_segm, x );
					return lt( sub_l( x ), nd->line( x ) ) ? nd->line: sub_l;
				}else{
					return nd->line;
				}
			}
		}
	}

	void node_for_each( tree_node* nd, function<void(COEF_T, COEF_T)> proc ){
		proc( nd->line.a, nd->line.b );
		if( nd->left ){
			node_for_each( nd->left, proc );
		}
		if( nd->right ){
			node_for_each( nd->right, proc );
		}
	}

public:
	cht_tree(): root(NULL), full_segm(0,0), elem_cnt(0){}
	cht_tree( VAR_T l, VAR_T r ): root(NULL), full_segm(l,r), elem_cnt(0){}

	size_t size(){
		return elem_cnt;
	}
	void clear(){
		if( this->root ){
			node_clear( this->root );
			this->root = NULL;
		}
		this->full_segm = var_segm_t( 0, 0 );
		this->elem_cnt = 0;
	}

	void insert( COEF_T a, COEF_T b ){
		if( this->root ){
			node_insert( this->root, this->full_segm, cht_line_t( a, b ) );
		}else{
			this->root = alloc_node( cht_line_t( a, b ) );
		}
	}

	COEF_T query( VAR_T x ){
		if( this->root ){
			auto ql = node_query( this->root, this->full_segm, x );
			return ql( x );
		}else{
			return COEF_T();
		}
	}

	void for_each( function<void(COEF_T, COEF_T)> proc ){
		if( this->root ){
			node_for_each( this->root, proc );
		}
	}

	~cht_tree(){
		clear();
	}
};

auto union_set = []( auto* set_a, auto* set_b ) -> auto*{
	if( set_b->size() < set_a->size() ){
		set_b->for_each( [&]( auto a, auto b ){
			set_a->insert( a, b );
		});
		delete set_b;
		return set_a;
	}else{
		set_a->for_each( [&]( auto a, auto b ){
			set_b->insert( a, b );
		});
		delete set_a;
		return set_b;
	}
};

typedef long long int number;
int main(){
	int n;
	vector<int> a, b;
	cin >> n;

	int cost_min = 100000, cost_max = -100000;

	for( int i = 0; i < n; i ++ ){
		int v;
		cin >> v;
		cost_min = min( cost_min, v );
		cost_max = max( cost_max, v );
		a.push_back( v );
	}

	for( int i = 0; i < n; i ++ ){
		int v;
		cin >> v;
		cost_min = min( cost_min, v );
		cost_max = max( cost_max, v );
		b.push_back( v );
	}

	vector<list<int>> adj( n );

	for(int i = 0; i < n - 1; i ++ ){
		int u, v;
		cin >> u >> v; u --; v --;
		adj[ u ].push_back( v );
		adj[ v ].push_back( u );
	}

	vector<number> opt_jump( n );

	function<cht_tree<int, number>* (int, int)>
	tree_dfs = [&]( int u, int fu ) -> cht_tree<int, number>* {
		if( u != 0 && adj[ u ].size() <= 1 ){
			auto t = new cht_tree<int, number>( cost_min, cost_max + 1 );

			opt_jump[ u ] = 0;
			t->insert( b[ u ], opt_jump[ u ] );
			return t;
		}else{
			cht_tree<int, number> *t  = NULL;
			for( auto v : adj[ u ] ){
				if( v == fu ){ continue; }
				if( t == NULL ){
					t = tree_dfs( v, u );
				}else{
					t = union_set( t, tree_dfs( v, u ));
				}
			}

			opt_jump[ u ] = t->query( a[ u ] );
			t->insert( b[ u ],  opt_jump[ u ] );

			return t;
		}

	};

	tree_dfs( 0, 0 );

	bool first = true;
	for( auto v : opt_jump ){
		if( first ){
			first = false;
		}else{
			cout << " ";
		}
		cout << v;
	}
	cout << endl;

	return 0;
}
