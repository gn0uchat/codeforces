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
					for( int j = i + i; j <= max_prime; not_prime[ j ] = true, j += i );
				}
			}
		}
		return _prime;
	}

public:
	vector <factr_t> factr;

	fnumb_t(){}
	fnumb_t( int v ){
		for( auto f : prime() ){
			int p = 0;
			if( v <= 1 ){
				break;
			}
			for(; v > 1 && (v % f == 0 ); p ++, v = v / f );
			factr.push_back( { f, p } );
		}
		reverse( factr.begin(), factr.end() );
	}


	friend bool operator== ( const fnumb_t& a, const fnumb_t& b ){
		if( a.factr.size() != b.factr.size() ){
			return false;
		}else{
			auto a_it = a.factr.begin();
			auto b_it = b.factr.begin();
			for(; a_it != a.factr.end() && b_it != b.factr.end(); ++ a_it, ++ b_it ){
				auto[ a_prm, a_pow ] = *a_it;
				auto[ b_prm, b_pow ] = *b_it;
				if( a_prm != b_prm || a_pow != b_pow ){
					return false;
				}
			}	
			return true;
		}
	}

	friend bool eq_after ( const fnumb_t& a, const fnumb_t& b, int prm_i ){
		if( a.factr.size() != b.factr.size() ){
			return false;
		}else{
			int n = a.factr.size();
			for( int i = prm_i; i < n; i ++ ){
				auto[ a_prm, a_pow ] = a.factr[ i ];
				auto[ b_prm, b_pow ] = b.factr[ i ];
				if( a_prm != b_prm || a_pow != b_pow ){
					return false;
				}
			}	
			return true;
		}
	}

	friend bool is_suffix( const fnumb_t& s, const fnumb_t& b, int prm_i ){
		if( s.factr.size() != b.factr.size() - prm_i ){
			return false;
		}else{
			for( int i = 0; i < s.factr.size(); i ++ ){
				auto[ s_prm, s_pow ] = s.factr[ i ];
				auto[ b_prm, b_pow ] = b.factr[ i + prm_i ];
				if( s_prm != b_prm || s_pow != b_pow ){
					return false;
				}
			}
			return true;
		}
	}

	friend fnumb_t operator/ ( const fnumb_t& a, const fnumb_t& b ){
		fnumb_t c;
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
int prm_size() const {
		return this->factr.size();
	}

	int size() const {
		int l = 0;
		for( auto f : this->factr ){
			auto [ prm , pow ] = f;
			l += pow;
		}
		return l;
	}

	friend fnumb_t common_factr_after( const fnumb_t& a, const fnumb_t& b, int prm_i ){
		fnumb_t c;
		int n = min( a.factr.size(), b.factr.size() );
		for( int i = prm_i; i < n; i ++ ){
			auto[ a_prm, a_pow ] = a.factr[ i ];
			auto[ b_prm, b_pow ] = b.factr[ i ];
			if( a_prm == b_prm ){
				if( a_pow != b_pow ){
					c.factr.push_back( { a_prm, min( a_pow, b_pow ) } );
					break;
				}else{
					c.factr.push_back( { a_prm, a_pow } );
				}
			}else{
				break;
			}
		}
		return c;
	}

	string str() const{
		string s;
		char buf[ 36 ];
		for( auto[ f, p ] : this->factr ){
			sprintf( buf, "%d^%d ", f, p );
			s.append( buf );
		}
		return s;
	}


};

template<int max_prime>
vector<int> fnumb_t<max_prime> :: _prime = vector<int>(0);

int main(){

	typedef fnumb_t< 5000 > fnumb_t;

	int n;
	cin >> n;

	int max_k = 0;
	map<int, int> fragm_ks;

	for( int i = 0; i < n; i ++ ){
		int k;
		cin >> k;
		fragm_ks[ k ] ++;
		max_k = max( max_k, k );
	}

	vector<fnumb_t> factrl( max_k + 1, fnumb_t( 1 ) );
	for( int k = 1; k <= max_k; k ++ ){
		factrl[ k ] = factrl[ k - 1 ] * fnumb_t( k );
	}

	typedef tuple<fnumb_t, int, int, list<int>> node_t;

	int nd_n = 0;
	int max_nd_n = 2 * fragm_ks.size() + 1;
	vector< node_t >nd_mem( max_nd_n + 1 );

	nd_mem[ nd_n ++ ] = { fnumb_t( 1 ), 0, 0, list<int>() };

	function<void(int, const fnumb_t&, int, int)> insert_node;
	insert_node = [&]( int nd_i, const fnumb_t& v, int agg, int prm_i ){
		auto& [ nd_v, nd_c, nd_fth, nd_cld ] = nd_mem[ nd_i ];
		if( eq_after( nd_v, v, prm_i ) ){
			nd_c = nd_c + agg;
		}else{
			nd_c = nd_c + agg;

			bool done = false;
			for( auto &i : nd_cld ){
				auto& [ sb_v, sb_c, sb_fth, sb_cld ] = nd_mem[ i ];

				auto nd_cm_v = common_factr_after( nd_v, v, prm_i );
				auto cm_v = common_factr_after( sb_v, v, prm_i );

				if( nd_cm_v.size() != cm_v.size() ){
					if( is_suffix( cm_v, sb_v, prm_i ) ){
						done = true;

						insert_node( i, v, agg, prm_i + cm_v.prm_size() - 1 );
						break;
					}else{
						done = true;

						int j = nd_n ++;
						auto full_cm_v = common_factr_after( sb_v, v, 0 );;
						nd_mem[ j ]= { full_cm_v, sb_c, nd_i, list<int>{i}};

						i = j;
						sb_fth = j;
						insert_node( j, v, agg, prm_i + cm_v.prm_size() - 1 );
						break;
					}
				}
				
			}

			if( done == false ){
				int j = nd_n ++;
				nd_mem[ j ] = { v, agg, nd_i, list<int>() };
				nd_cld.push_back( j );
			}
		}
	};

	vector< tuple<bool, long long int> > _nd_cost( max_nd_n, {false, 0} );
	vector< tuple<bool, long long int> > _nd_up_cost( max_nd_n, {false, 0} );

	function<long long int(int)> nd_cost, nd_up_cost;

	auto edge_cost = [&]( int nd_i ) -> long long int {
		auto[ v, c, fth, cld ] = nd_mem[ nd_i ];
		auto[ fth_v, fth_c, fth_fth, fth_cld ] = nd_mem[ fth ];
		return ( ((long long int) v.size() ) - ( (long long int) fth_v.size()) );
	};

	nd_cost = [&]( int nd_i ) -> long long int {
		auto& [ done, v ] = _nd_cost[ nd_i ];
		if( done == true ){
			return v;
		}else{
			done = true;
			auto& [ nd_v, nd_c, nd_fth, nd_cld ] = nd_mem[ nd_i ];
			v = 0;

			for( auto& cld_i : nd_cld ){
				auto[ cld_v, cld_c, cld_fth, cld_cld ] = nd_mem[ cld_i ];
				v = v + nd_cost( cld_i ) + ( ( long long int ) cld_c ) * edge_cost( cld_i );
			}
			return v;
		}
	};

	nd_up_cost = [&]( int nd_i ) -> long long int {
		auto& [ done, v ] = _nd_up_cost[ nd_i ];

		if( done == true ){
			return v;
		}else{
			done = true;
			auto& [ nd_v, nd_c, nd_fth, nd_cld ] = nd_mem[ nd_i ];

			if( nd_fth == nd_i ){
				v = 0;
			}else{
				auto& [ fth_v, fth_c, fth_fth, fth_cld ] = nd_mem[ nd_fth ];
				long long int sb_cost =
					nd_cost( nd_fth ) - ( nd_cost( nd_i ) + edge_cost( nd_i ) * ( (long long int) nd_c ));

				v =  edge_cost( nd_i ) * ( (long long int) ( n - nd_c ))
					+ sb_cost
					+ nd_up_cost( nd_fth );
			}
			return v;
		}
	};

	for( auto [ k, c ] : fragm_ks ){
		insert_node( 0, factrl[ k ], c, 0 );
	}

	long long int min_cost = nd_cost( 0 ) + nd_up_cost( 0 );
	for( int i = 1; i < nd_n; i ++ ){
		min_cost = min( min_cost, nd_cost( i ) + nd_up_cost( i ) );
	}

	cout << min_cost << endl;

	return 0;
}
