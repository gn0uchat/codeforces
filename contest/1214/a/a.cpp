//user define
#include<stdlib.h>
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
int gcd( number_t a, number_t b ){
	if( equal( b, 0 ) ){
		return a;
	}
	return a < b ? gcd( b, a ) : gcd( b, a % b );
}
#define heap_arr_size( SIZE )   ((SIZE) + 1)
#define heap_node_father( IDX ) ( (IDX) / 2 )
#define heap_node_lchild( IDX ) ( 2 * (IDX) )
#define heap_node_rchild( IDX ) ( 2 * (IDX) + 1 )
#define swap_int(A, B) do{ int tmp = (A); (A) = (B); (B) = tmp;}while(0)
#define heap_cmp_lt( A, B ) ((A) < (B))

int heap_valid_node( int* arr, int i ){
	int arr_n = arr[ 0 ];
	return ( i > 0 ) && ( i <= arr_n );
}

int heap_min_child( int* arr, int i ){
	int tmp;
	int min_i = 0;

	if( heap_valid_node( arr, heap_node_lchild( i ) ) ){
		min_i = heap_node_lchild( i );
	}
	if( heap_valid_node( arr, heap_node_rchild( i ) ) ){
		min_i = heap_cmp_lt( arr[ heap_node_rchild( i ) ], arr[ heap_node_lchild( i ) ] )
			? heap_node_rchild( i ) : heap_node_lchild( i );
	}
	return min_i;
}

int heap_sink( int* arr, int i ){
	int swap_i;

	for(; swap_i = heap_min_child( arr, i ), heap_valid_node( arr, swap_i ) ; i = swap_i ){
		if( heap_cmp_lt( arr[ swap_i ], arr[ i ] ) ){
			swap_int( arr[ i ], arr[ swap_i ] );
		}else{ 
			break;
		}
	}
	return i;
}

int heap_float( int* arr, int i ){
	int swap_i;
	for(; swap_i = heap_node_father( i ), heap_valid_node( arr, swap_i ); i = swap_i ){
		if( heap_cmp_lt( arr[ i ], arr[ swap_i ] ) ){
			swap_int( arr[ i ], arr[ swap_i ] );
		}else{
			break;
		}
	}
	return i;
}


int heap_push( int* arr, int val ){
	int i, arr_n;

	arr[ 0 ] ++;
	arr_n = arr[ 0 ];
	arr[ arr_n ] = val;

	i = heap_float( arr, arr_n );
	heap_sink( arr, i );

	return 0;
}

int heap_pop( int* arr ){
	int val;
	int arr_n = arr[ 0 ];

	val = arr[ 1 ];
	swap_int( arr[ 1 ], arr[ arr_n ] );
	arr[ 0 ] --;

	heap_sink( arr, 1 );

	return val;
}

int heap_empty( int* arr ){
	return arr[ 0 ] == 0;
}

#include<stdio.h>

int main(){
	number_t n, d, e;

	read_number( n );
	read_number( d );
	read_number( e );

	number_t ml;

	for( ml = n; n >= 0; n -= 5 * e ){
		ml = min( ml, n % d );
	}

	write_number( ml );
	return 0;
}
