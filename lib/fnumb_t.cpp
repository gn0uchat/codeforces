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
