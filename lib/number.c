//user define
#define number_t long long int

#define read_number( VAR ) do{ \
	scanf( sizeof(VAR)==sizeof(long long int)? "%lld" : "%d", & (VAR)); }while(0)

#define read_numbers( ARR, N ) do{\
	int i; const char* fmt_str = sizeof(ARR[0])==sizeof(long long int)? "%lld" : "%d";\
	for(i = 0; i < N; i ++ ){scanf( fmt_str, & (ARR[i])); }}while(0)

#define write_number( VAR ) do{\
	printf( sizeof(VAR)==sizeof(long long int)? "%lld" : "%d", (VAR)); }while(0)

#define equal(A, B) ( (A) == (B) )

number_t max( number_t a, number_t b ){ return a > b ? a : b; }
number_t min( number_t a, number_t b ){ return a < b ? a : b; }
