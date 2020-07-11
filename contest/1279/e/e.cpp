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

#define INF 1000000000000000001LL

#define MAX_N (50 + 1)

tuple<bool, long long int> _comb_cycle[ MAX_N ][ MAX_N ];

long long int mul( long long int a, long long int b ){
	if( a == 0 || b == 0 ){
		return 0;
	}else{
		long long int c = a * b;
		if( a == c / b ){
			return c;
		}else{
			return INF;
		}
	}
}

long long int add( long long int a, long long int b ){
	if( a >= INF || b >= INF ){
		return INF;
	}else{
		if( a + b >= INF ){
			return INF;
		}else{
			return a + b;
		}
	}
}

long long int comb_cycle( int n, int chain_n ){
	auto& [ solved, result ] = _comb_cycle[ n ][ chain_n ];

	if( solved ){
		return result;
	}else{
		solved = true;
		if( chain_n == 0 ){
			if( n <= 1 ){
				result = 1;
			}else{
				result = comb_cycle( n - 1, 1 );
			}
		}else if( n <= 1 ){
			result = 1;
		}else{
			int free_n = n - chain_n;
			if( free_n == 0 ){

				result =
				  mul( chain_n - 1, comb_cycle( n - 1, chain_n - 1 ) );

			}else if( free_n == 1 ){

				result =
				  mul( chain_n, comb_cycle( n - 1, chain_n ));

			}else if( free_n > 1 ){

				result =
				  add( mul( chain_n, comb_cycle( n - 1, chain_n )),
				       mul( free_n - 1, comb_cycle( n - 1, chain_n + 1 )));
			}

		}
		return result;
	}
}

tuple<bool, long long int> _comb_perm[ MAX_N ];
long long int comb_perm( int n ){
	auto& [ solved, result ] = _comb_perm[ n ];
	if( solved ){
		return result;
	}else{
		solved = true;
		if( n <= 1 ){
			result = 1;
		}else{
			result = 0;
			for( int i = 1; i <= n; i ++ ){
				// result += comb_cycle( i, 0 ) * comb_perm( n - i );
				result = add( result, mul( comb_cycle( i, 0 ), comb_perm( n - i )));
			}
		}

		return result;
	}
}

int set_cycle_size( int n, long long int& k ){
	int cycle_n;
	for( cycle_n = 1; cycle_n <= n; cycle_n ++ ){
		// long long int d = comb_cycle( cycle_n, 0 ) * comb_perm( n - cycle_n );
		long long int d = mul( comb_cycle( cycle_n, 0 ), comb_perm( n - cycle_n ));
		if( d >= k ){
			break;
		}else{
			k -= d;
		}
	}
	return cycle_n;
}

void it_cand_at( int cycle_i, int cycle_n, const vector<int>& cycle_map, set<int>& cand, function<bool(int)> proc ){
	for( auto e : cand ){
		int p;
		bool qual;

		if( cycle_i == cycle_n ){
			qual = true;
		}else{
			for( p = e; cycle_map[ p ] != p; p = cycle_map[ p ]);
			qual = p != cycle_i;
		}

		if( qual ){
			if( proc( e ) == false ){
				break;
			}
		}
	}
	return;
}

int set_cycle( int cycle_n, long long int comb_remain, long long int& k, vector<int>& cycle ){
	vector<bool> chain_endp;
	set<int> cand;
	vector<int> cycle_map;

	for( int i = 0; i <= cycle_n; i ++ ){
		cycle_map.push_back( i );
		chain_endp.push_back( false );
	}

	for( int i = 1; i <= cycle_n; i ++ ){
		cand.insert( i );
	}

	int chain_n = 0;

	cycle_map[ 1 ] = cycle_n;
	cand.erase( cycle_n );
	chain_endp[ cycle_n ] = true;
	chain_n = 1;

	for( int i = 2; i <= cycle_n; i ++ ){
		it_cand_at( i, cycle_n, cycle_map, cand, [&]( int e ){
			int update_chain_n;

			if( chain_endp[ i ] ){
				if( e < i ){
					update_chain_n = chain_n - 1;
				}else{
					update_chain_n = chain_n;
				}
			}else{
				if( e < i ){
					update_chain_n = chain_n;
				}else{
					update_chain_n = chain_n + 1;
				}
			}

			// long long int d = comb_cycle( cycle_n - i, update_chain_n ) * comb_remain;
			long long int d = mul( comb_cycle( cycle_n - i, update_chain_n ), comb_remain );
			if( d >= k ){
				cand.erase( e );
				cycle_map[ i ] = e;
				chain_endp[ e ] = true;
				chain_n = update_chain_n;
				return false;
			}else{
				k -= d;
				return true;
			}
		});
	}

	for( int i = 1; i <= cycle_n; i ++ ){
		cycle.push_back( cycle_map[ i ] );
	}

	return 0;
}

int main(){
	int t;
	cin >> t;
	while( t --> 0 ){
		int n;
		long long int k;
		cin >> n >> k;

		if( comb_perm( n ) < k ){
			cout << "-1" << endl;
		}else{
			vector< int > good_perm;

			int perm_i = 0;
			while( perm_i < n ){

				vector<int> cycle;
				int cycle_n = set_cycle_size( n - perm_i, k );

				set_cycle( cycle_n, comb_perm( n - perm_i - cycle_n ), k, cycle );

				for( auto e : cycle ){
					good_perm.push_back( e + perm_i );
				}

				perm_i += cycle_n;
			}

			bool begin_flag = true;
			for( auto e : good_perm ){
				if( begin_flag ){
					begin_flag = false;
				}else{
					cout << " ";
				}
				cout << e;
			}
			cout << endl;
		}
	}
	return 0;
}
