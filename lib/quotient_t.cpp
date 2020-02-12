class quotient_t{
	private:

	template<class T>
	T gcd( T a, T b ){
		T tmp;
		if( a < b ){

			tmp = a;
			a = b;
			b = tmp;
		}

		for(; b > 0; tmp = a % b, a = b, b = tmp );

		return a;
	}

	template<class T>
	T lcm( T a, T b ){
		int d = gcd<T>( a, b );
		return d * ( a / d ) * ( b / d );
	}


	public:
	tuple<int, int> q;

	quotient_t( int nom, int denom ){
		this->q = { nom, denom };
	}

	quotient_t( int nom ){
		this->q = { nom, 1 };
	}

	quotient_t(){
		this->q = { 0, 1 };
	}

	string operator* () const {
		char buf[ 36 ];
		auto[ n, d ] = this->q;
		sprintf( buf, "%d/%d", n, d );
		return string( buf );
	}

	quotient_t operator+ ( const quotient_t &b ) const {
		auto [ a_nom, a_denm ] = this->q;
		auto [ b_nom, b_denm ] = b.q;

		int denm = lcm<int>( a_denm, b_denm );
		int nom = ( denm / a_denm ) * a_nom + ( denm / b_denm ) * b_nom;

		return quotient_t( nom, denm );
	}

	quotient_t operator- ( const quotient_t &b ) const {
		auto [ a_nom, a_denm ] = this->q;
		auto [ b_nom, b_denm ] = b.q;

		int denm = lcm<int>( a_denm, b_denm );
		int nom = ( denm / a_denm ) * a_nom - ( denm / b_denm ) * b_nom;

		return quotient_t( nom, denm );
	}

	quotient_t operator* ( const quotient_t &b ) const {
		auto [ a_nom, a_denm ] = this->q;
		auto [ b_nom, b_denm ] = b.q;
		long long nom = ( (long long int) a_nom ) * ( (long long int) b_nom );
		long long denm = ( (long long int) a_denm ) * ( (long long int) b_denm );
		long long int d = gcd<long long int>( nom, denm );
		return quotient_t( (int) ( nom / d ), (int) ( denm / d ) );
	}

	quotient_t operator/ ( const quotient_t &b ) const {
		auto [ a_nom, a_denm ] = this->q;
		auto [ b_nom, b_denm ] = b.q;
		long long nom = ( (long long int) a_nom ) * ( (long long int) b_denm );
		long long denm = ( (long long int) a_denm ) * ( (long long int) b_nom );
		long long int d = gcd<long long int>( nom, denm );
		return quotient_t( (int) ( nom / d ), (int) ( denm / d ) );
	}

	quotient_t operator^ ( int p ) const {
		quotient_t v( 1 );
		quotient_t b = (*this);
		for(; p > 0; p >>= 1, b = b * b ){
			if( p & 1 ){
				v = v * b;
			}
		}
		return v;
	}


	bool operator> ( const quotient_t &b ) const {
		auto [ a_nom, a_denm ] = this->q;
		auto [ b_nom, b_denm ] = b.q;
		
		return ( (long long int) a_nom ) * ( (long long int) b_denm ) > 
		       ( (long long int) b_nom ) * ( (long long int) a_denm );
	}

	bool operator< ( const quotient_t &b ) const {
		auto [ a_nom, a_denm ] = this->q;
		auto [ b_nom, b_denm ] = b.q;
		
		return ( (long long int) a_nom ) * ( (long long int) b_denm ) <
		       ( (long long int) b_nom ) * ( (long long int) a_denm );
	}

};
