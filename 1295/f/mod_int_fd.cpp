template< int prime >
class mod_int_fd{
private:
	int elem;
	int mod( long long int v ) const {
		v = v % prime;
		return v < 0 ? v + prime : v;
	}
public:

	int operator* () const {
		return this->elem;
	}

	mod_int_fd<prime> operator+ ( const mod_int_fd<prime> &b ) const{
		int x = this->elem;
		int y = *b;
		int r = ( x - prime ) + y;
		return r < 0 ? mod_int_fd( r + prime ) : mod_int_fd( r );
	}

	mod_int_fd<prime> operator- ( const mod_int_fd<prime> &b ) const {
		int x = elem;
		int y = *b;
		int r = x - y;
		return r < 0 ? mod_int_fd<prime>( r + prime ): mod_int_fd<prime>( r );
	}

	mod_int_fd<prime> operator*( const mod_int_fd<prime> &b ) const {
		long long int big_a = this->elem;
		long long int big_b = *b;

		return mod_int_fd<prime>(( big_a * big_b ) % prime );
	}

	mod_int_fd<prime> mul_inv() const {
		int a = this->elem;

		int t_inv = 0, inv = 1;
		for( int v = prime, r = a; r > 1; ){
			int q = v / r;
			int new_r = v % r;
			int new_inv = mod( ((long long int) t_inv) - ( ((long long int) q) * ((long long int) inv)) );

			t_inv = inv;
			inv = new_inv;

			v = r;
			r = new_r;
		}

		return mod_int_fd<prime>( inv );
	}

	mod_int_fd<prime> operator/ ( const mod_int_fd<prime> &b ) const {
		return ( * this ) * b.mul_inv();
	}

	friend mod_int_fd<prime > operator* ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) * b; }
	friend mod_int_fd<prime > operator/ ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) / b; }
	friend mod_int_fd<prime > operator+ ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) + b; }
	friend mod_int_fd<prime > operator- ( int v, const mod_int_fd<prime> &b ){ return mod_int_fd<prime>( v ) - b; }
	mod_int_fd<prime> operator * ( int v ) const { return this->elem * mod_int_fd<prime>( v ); }
	mod_int_fd<prime> operator / ( int v ) const { return this->elem / mod_int_fd<prime>( v ); }
	mod_int_fd<prime> operator + ( int v ) const { return this->elem + mod_int_fd<prime>( v ); }
	mod_int_fd<prime> operator - ( int v ) const { return this->elem - mod_int_fd<prime>( v ); }

	mod_int_fd(){
		this->elem = 0;
	}

	mod_int_fd( long long int elem ){
		this->elem = mod( elem );
	}
};
