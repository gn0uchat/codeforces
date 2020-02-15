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

template<int max_prime>
class fnumb_t{
private:
	typedef tuple<int, int> factr_t;
	static vector<int> _prime;

	static const vector<int>& prime(){
		if( _prime.size() == 0 ){
			vector<bool> not_prime( max_prime + 1,  false );
			for( int i = 2; i <= max_prime; i ++ ){
				if( not_prime[ i ] == false ){
					_prime.push_back( i );
					for( int j = i + i; j <= max_prime; not_prime[ j ] = true, j ++ );
				}
			}
		}
		return _prime;
	}

public:
	list <factr_t> factr;

	fnumb_t(){}
	fnumb_t( int v ){
		for( auto f : prime() ){
			int p = 0;
			if( v <= 1 ){
				break;
			}
			for(; v > 1 && (v % f == 0 ); p ++, v = v / f );
			factr.push_front( { f, p } );
		}
	}

	friend bool operator== ( const fnumb_t& a, const fnumb_t& b ){
		if( a.factr.size() != b.factr.size() ){
			return false;
		}else{
			auto a_it = a.factr.begin();
			auto b_it = b.factr.begin();
			for( a_it != a.factr.end() && b_it != b.factr.end(); ++ a_it, ++ b_it ){
				auto[ a_prm, a_pow ] = *a_it;
				auto[ b_prm, b_pow ] = *b_it;
				if( a_prm != b_prm || a_pow != b_pow ){
					return false;
				}
			}	
			return true;
		}
	}

	friend fnumb_t operator/ ( const fnumb_t& a, const fnumb_t& b ){
		auto a_it = a.factr.begin();
		auto b_it = b.factr.begin();
		while( a_it != a.factr.end() && b_it != b.factr.end() ){
			auto[ a_prm, a_pow ] = *a_it;
			auto[ b_prm, b_pow ] = *b_it;
			if( a_prm == b_prm ){
				if( a_pow - b_pow != 0 ){
					c.factr.push_back( { a_prm, a_pow - b_pow } );
				}
				++ a_it;
				++ b_it;
			}else{
				if( a_prm > b_prm ){
					c.factr.push_back( { a_prm, a_pow } );
					++ a_it;
				}else{
					c.factr.push_back( { b_prm, - b_pow } );
					++ b_it;
				}
					
			}
		}	

		for(; a_it != a.factr.end(); c.factr.push_back( *a_it ), ++ a_it );
		for(; b_it != b.factr.end(); ++ b_it ){
			auto[ b_prm, b_pow ] = *b_it;
			c.factr.push_back( { b_prm, - b_pow } );
		}
	}

	friend fnumb_t operator*( const fnumb_t& a, const fnumb_t& b ){
		fnumb_t c;
		int i, j;

		auto a_it = a.factr.begin();
		auto b_it = b.factr.begin();
		while( a_it != a.factr.end() && b_it != b.factr.end() ){
			auto[ a_prm, a_pow ] = *a_it;
			auto[ b_prm, b_pow ] = *b_it;
			if( a_prm == b_prm ){
				c.factr.push_back( { a_prm, a_pow + b_pow } );
				++ a_it;
				++ b_it;
			}else{
				if( a_prm > b_prm ){
					c.factr.push_back( *a_it );
					++ a_it;
				}else{
					c.factr.push_back( *b_it );
					++ b_it;
				}
					
			}
		}

		for(; a_it != a.factr.end(); c.factr.push_back( *a_it ), ++ a_it );
		for(; b_it != b.factr.end(); c.factr.push_back( *b_it ), ++ b_it );
		return c;
	}

	int size() const {
		int l = 0;
		for( auto f : this->factr ){
			auto [ prm , pow ] = f;
			l += pow;
		}
		return l;
	}

	friend fnumb_t common_factr( const fnumb_t& a, const fnumb_t& b ){
		fnumb_t c;
		auto a_it = a.factr.begin();
		auto b_it = b.factr.begin();
		for( a_it != a.factr.end() && b_it != b.factr.end(); ++ a_it, ++ b_it ){
			auto[ a_prm, a_pow ] *a_it;
			auto[ b_prm, b_pow ] *b_it;
			if( a_prm == b_prm ){
				if( a_pow != b_pow ){
					c.push_back( { a_prm, min( a_pow, b_pow ) } );
					break;
				}else{
					c.push_back( { a_prm, a_pow } );
				}
			}else{
				break;
			}
		}
		return c;
	}


};

template<int max_prime>
vector<int> fnumb_t<max_prime> :: _prime = vector<int>(0);

int main(){

	typedef fnumb_t< 5000 > fnumb_t;

	int n;
	cin >> n;

	int max_k = 0;
	vector<int> fragm_k;
	for( int i = 0; i < n; i ++ ){
		int k;
		cin >> k;
		fragm_k.push_back( k );
		max_k = max( max_k, k );
	}

	vector<fnumb_t> factrl( max_k + 1, fnumb_t( 1 ) );
	for( int k = 1; k <= max_k; k ++ ){
		factrl[ k ] = factrl[ k - 1 ] * fnumb_t( k );
	}

	typedef tuple<fnumb_t, int, int, list<int>> node_t;

	int nd_mem_i = 0;
	vector< node_t >nd_mem( 2 * n + 1 );

	nd_mem[ nd_mem_i ++ ] = { fnumb_t( 1 ), 0, 0, list<int>() };

	funciton<void(const fnumb_t&)> insert_node = [&]( int nd_i, const fnumb_t& v, int agg ){
		auto& [ nd_v, nd_c, nd_fth, nd_cld ] = nd_mem[ nd_i ];
		if( nd_v == v ){
			nd_c = nd_c + agg;
		}else{
			bool done = false;
			for( auto &i : nd_cld ){
				auto& [ sb_v, sb_c, sb_fth, sb_cld ] = nd_mem[ i ];
				auto cm_v = common_factr( sb_v, v );
				if( cm_v == sb_v ){
					done = true;

					insert_node( i, v );
					break;
				}else if( !( cm_v == nd_v) ){
					done = true;

					int j = nd_mem_i;
					nd_mem[ nd_mem_i ++ ]= { cm_v, sb_c + 1, nd_i, list<int>{i}};

					i = j;
					sb_fth = j;
					insert_node( j, v );
					break;
				}
			}

			if( done == false ){
				int j = nd_mem_i;
				nd_mem[ nd_mem_i ++ ] = { v, 1, nd_i, list<int>() };
				nd_cld.push_back( j );
			}
		}
	};

	vector< tuple<bool, int> > _nd_cost( 2 * n + 1 );
	vector< tuple<bool, int> > _nd_up_cost( 2 * n + 1 );

	function<int(int)> nd_cost, nd_up_cost;

	nd_cost = [&]( int nd_i ) -> int {
		auto& [ done, v ] = _nd_cost[ nd_i ];
		if( done == true ){
			return v;
		}else{
			done = true;
			auto& [ nd_v, nd_c, nd_fth, nd_cld ] = nd_mem[ nd_i ];
			v = 0;

			for( auto& i : nd_cld ){
				auto[ v, c, fth, cld ] = nd_mem[ i ];
				v = v + nd_cost( i ) + c * ( v.size() - nd_v.size() );
			}
			return v;
		}
	};

	nd_up_cost = [&]( int nd_i ) -> int {
		auto& [ done, v ] = _nd_up_cost[ nd_i ];

		if( done == true ){
			return v;
		}else{
			done = true;
			auto& [ nd_v, nd_c, nd_fth, nd_cld ] = nd_mem[ nd_i ];

			if( nd_fth == nd_i ){
				v = 0;
			}else{
				v = 0;

				auto& [ v, c, fth, cld ] = nd_mem[ nd_fth ];

				for( auto i : cld ){
					if( i != nd_i ){
						v = v + nd_cost( i );
					}
				}

				v = v + nd_up_cost( nd_fth );
			}
			return v;
		}
	}

	

	for( auto k : fragm_k ){
		insert_node( 0, factrl( k ) );
	}


	return 0;
}
