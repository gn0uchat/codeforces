#include<map>
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

#define PRIME 998244353
const int prime = PRIME;

class edge{
public:
	edge( ){}
	edge( int v, int prio ){
		this->v = v;
		this->prio = prio;
	}
	int prio;
	int v;

};

class node{
public:
	node(){}
	node( int father, int color, int oute_prio ){
		this->father = father;
		this->color = color;
	}
	int father;
	bool color;
	int oute_prio;
};

int n;
vector< list<edge> > adj;
vector< node > tree;

bool leaf( int u ){
	if( tree[ u ].father == u ){
		return adj[ u ].size() == 0;
	}else{
		return adj[ u ].size() == 1;
	}
}


int children_of( int u, function<void(edge)> proc ){
	for( auto e : adj[ u ] ){
		if( e.v != tree[ u ].father ){
			proc( e );
		}
	}
	return 0;
}

int dfs( int u ){
	tree[ u ].color = true;
	for( auto e : adj[ u ] ){
		if( tree[ e.v ].color  == false ){
			tree[ e.v ].father  = u;
			tree[ e.v ].oute_prio = e.prio;
			dfs( e.v );
		}
	}
	return 0;
}

int add( int a, int b ){
	int r = ( a - prime) + b;
	return r < 0 ? r + prime: r;
}

int sub( int a, int b ){
	int r = a - b;
	return r < 0 ? r + prime : r;
}

int mul( int a, int b ){
	long long int big_a = a;
	long long int big_b = b;
	
	long long int r = big_a * big_b;

	return ((int)( ( big_a * big_b ) % prime ));
}

int mul( int a, int b, int c ){
	return mul( mul(a, b), c );
}

void opset( int &var, int(*op)(int, int), int val ){
	var = op( var, val );
}

int divp( int dividend, int divisor ){
	int t_inv = 0, inv = 1;
	for( int v = prime, r = divisor; r > 1; ){
		int q = v / r;
		int new_r = v % r;
		int new_inv = sub( t_inv, mul( q, inv ) );

		t_inv = inv;
		inv = new_inv;

		v = r;
		r = new_r;
	}

	return mul( dividend, inv );
}

vector< tuple< bool, int >> _subtree_state[3];

int comb_indp(int);
int remain_bef_oute(int);
int chosen(int);

int chosen_bef_oute( int u ){
	auto& [ solved, result ] = _subtree_state[ 0 ][ u ];
	if( solved ){
		return result;
	}else{
		solved = true;
		if( leaf( u ) ){
			result = 0;
		}else{
			int indp_after = 1;
			children_of( u, [&]( edge e ){
				opset( indp_after, mul, comb_indp( e.v ));
			});

			int remain_root_before = 1;
			int acc = 0;
			children_of( u, [&]( edge e ){
				if( e.prio < tree[ u ].oute_prio ){
					opset( indp_after, divp, comb_indp( e.v ));

					opset( acc, add,
					  mul( remain_root_before, remain_bef_oute( e.v ), indp_after ));

					opset( remain_root_before, mul, chosen( e.v ));
				}
			});

			result = acc;
		}
		return result;
	}
}

int chosen_at_oute( int u ){
	auto& [ solved, result ] = _subtree_state[ 1 ][ u ];
	if( solved ){
		return result;
	}else{
		solved = true;

		if( leaf( u ) ){
			result = 1;
		}else{ int prod = 1;
			children_of( u, [&]( edge e ){
				if( e.prio < tree[ u ].oute_prio ){
					opset( prod, mul, chosen( e.v ));
				}else{
					opset( prod, mul, comb_indp( e.v ));
				}
			});
			result = prod;
		}

		return result;
	}
}

int chosen( int u ){
	return
	  add( chosen_bef_oute( u ), chosen_at_oute( u ) );
}


int remain_bef_oute_chosen_aft( int u ){
	int remain_root_before = 1;
	children_of( u, [&]( edge e ){
		if( e.prio < tree[ u ].oute_prio ){
			opset( remain_root_before, mul, chosen( e.v ));
		}
	});

	int indp_after = 1;
	children_of( u, [&]( edge e ){
		if( e.prio > tree[ u ].oute_prio ){
			opset( indp_after, mul, comb_indp( e.v ) );
		}
	});

	int acc = 0;
	children_of( u, [&]( edge e ){
		if( e.prio > tree[ u ].oute_prio ){
			opset( indp_after, divp, comb_indp( e.v ) );
			
			opset( acc, add,
			  mul( remain_root_before, remain_bef_oute( e.v ) , indp_after ));

			opset( remain_root_before, mul, chosen( e.v ));
		}
	});
	return acc;
}

int remain_bef_oute_remain_aft( int u ){
	int prod = 1;

	children_of( u, [&]( edge e ){
		opset( prod, mul, chosen( e.v ));
	});
	return prod;
}

int remain_bef_oute( int u ){
	auto& [ solved, result ] = _subtree_state[ 2 ][ u ];

	if( solved ){
		return result;
	}else{
		solved = true;
		if( leaf( u ) ){
			result = 1;
		}else{
			result =
			  add( remain_bef_oute_chosen_aft( u ), remain_bef_oute_remain_aft( u ) );
		}
		return result;
	}
}

int comb_indp( int u ){
	int r = add( chosen_bef_oute( u ), remain_bef_oute( u ) );

	return
	  add( chosen_bef_oute( u ), remain_bef_oute( u ) );
}

int main(){
	cin >> n;

	for( int i = 0; i < n; i ++ ){
		adj.push_back( list<edge>() );
		tree.push_back( node( 0, false, 0 ) );
		for( int j = 0; j < 3; j ++ ){
			_subtree_state[ j ].push_back( make_tuple( false, 0 ));
		}
	}

	for( int i = 0; i < n - 1; i ++ ){
		int u, v;
		cin >> u >> v;
		u --;
		v --;

		adj[ u ].push_back( edge( v, i ) );
		adj[ v ].push_back( edge( u, i ) );
	}

	tree[ 0 ].oute_prio = n;
	dfs( 0 );

	cout << comb_indp( 0 ) << endl;
	return 0;
}
