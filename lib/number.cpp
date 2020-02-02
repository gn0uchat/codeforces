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
	int d = gcd( a, b );
	return d * ( a / d ) * ( b / d );
}

typedef tuple<int, int> quotient_t;

bool gt( const quotient_t &a, const quotient_t &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;
	
	return ( (long long int) a_nom ) * ( (long long int) b_denm ) > 
	       ( (long long int) b_nom ) * ( (long long int) a_denm );
}

bool lt( const quotient_t &a, const quotient_t &b ){
	return ( a != b ) && gt( b, a );
}


quotient_t quotient( int nom, int dem ){
	int d = gcd<int>( nom, dem );
	return { nom / d, dem / d };
}

quotient_t add( const quotient_t &a, const quotient_t &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;

	int denm = lcm( a_denm, b_denm );
	int nom = ( denm / a_denm ) * a_nom + ( denm / b_denm ) * b_nom;

	return quotient( nom, denm );
}

quotient_t sub( const quotient_t &a, const quotient_t &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;

	int denm = lcm( a_denm, b_denm );
	int nom = ( denm / a_denm ) * a_nom - ( denm / b_denm ) * b_nom;

	return quotient( nom, denm );
}

quotient_t mul( const quotient_t &a, const quotient_t &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;
	long long nom = ( (long long int) a_nom ) * ( (long long int) b_nom );
	long long denm = ( (long long int) a_denm ) * ( (long long int) b_denm );
	long long int d = gcd<long long int>( nom, denm );
	return quotient( (int) ( nom / d ), (int) ( denm / d ) );
}

quotient_t div( const quotient_t &a, const quotient_t &b ){
	auto [ a_nom, a_denm ] = a;
	auto [ b_nom, b_denm ] = b;
	long long nom = ( (long long int) a_nom ) * ( (long long int) b_denm );
	long long denm = ( (long long int) a_denm ) * ( (long long int) b_nom );
	long long int d = gcd<long long int>( nom, denm );
	return quotient( (int) ( nom / d ), (int) ( denm / d ) );
}

