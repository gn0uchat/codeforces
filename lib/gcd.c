int gcd( number_t a, number_t b ){
	if( equal( b, 0 ) ){
		return a;
	}
	return a < b ? gcd( b, a ) : gcd( b, a % b );
}
