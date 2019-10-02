#include<iostream>
#include<functional>

using namespace std;

template <typename T>
class lazy_list{
	private:

	T head;
	
	lazy_list<T> *tail = NULL;

	public:

	function< lazy_list<T>& () > delay;

	lazy_list(){}

	lazy_list( const T& head ){
		this->head = head;
	}

	lazy_list( const T& head, const function< lazy_list<T>& () >& delay ){
		this->head = head;
		this->delay = delay;
	}

	lazy_list( const T& head, lazy_list<T>& tail ){
		this->head = head;
		this->tail = & tail;
	}

	lazy_list( const lazy_list<T>& same ){
		this->head = same.head;
		this->tail = same.tail;
		this->delay = same.delay;
	}

	const T& car(){
		return this->head;
	}

	lazy_list<T>& cdr(){
		if( this->tail == NULL ){
			this->tail = new lazy_list<T>;
			this->tail = & this->delay();
		}
		return * this->tail;
	}

	lazy_list<T>& map( const function< T& (const T&) >& lambda ){

		function< lazy_list<T>& () >
		delay = [&]() -> lazy_list<T>& {
			return this->cdr( lambda ); };

		lazy_list<T> *list_mem =
			new lazy_list<T>( lambda( car() ), delay );

		return * list_mem;
	}

	lazy_list<T>& filter( const function< bool(const T&) >& lambda ){

		if( lambda( this->car() ) ){
			function< lazy_list<T> () >
			delay = [&]() -> lazy_list<T>& {
				return this->cdr().filter( lambda ); };
			return *( new lazy_list<T>( this->car(), delay ) );
		}else{
			return this->cdr().filter( lambda );
		}
	}

	void until( 	const function< void( const T& ) >& lambda,
			const function< bool( const T& ) >& stop_cond ){
		if( ! stop_cond( this->car() ) ){
			lambda( this->car() );
			this->cdr().until( lambda, stop_cond );
		}
	}
};

template <typename T>
lazy_list<T>& list_op( lazy_list<T>& a, lazy_list<T>& b, const function<T(T,T)>& op ){
	T head = op( a.car(), b.car() );
	function<lazy_list<T>&()> delay =
		[&]() -> lazy_list<T>& { return list_op( a.cdr(), b.cdr(), op ); };

	lazy_list<T> *list_mem = new lazy_list<T>( head, delay );
	return *list_mem;
}

template <typename T>
T list_ref( lazy_list<T>& l, int n ){
	if( n == 0 ){
		return l.car();
	}else{
		return list_ref( l.cdr(), n - 1 );
	}
}

template <typename T>
lazy_list<T>& list_interleave( lazy_list<T> &list_front, lazy_list<T> &list_rear ){
	auto delay = [&]() -> lazy_list<T>& { return list_interleave( list_rear, list_front.cdr() ); }
	return *( new lazy_list<T>( list_front.car(), delay ) );
}

template <typename T>
lazy_list<T>& list_concat( lazy_list<T> &list_front, lazy_list<T> &list_rear ){
	if( list_front.empty() ){
		return list_rear;
	}else{
		auto delay = [&]() -> lazy_list<T>& { return list_concat( list_front.cdr(), list_rear ); }
		return *( new lazy_list<T>( list_front.car(), delay ) );
	}
}


int main(){

	int i = 0;

	lazy_list<int> fib1( 1 );

	lazy_list<int> fib( 0, [&]()-> lazy_list<int>&{ return fib1; } );

	function<int(int,int)> add = [](int a, int b)->int{ return a + b; };

	fib1.delay = [&]() -> lazy_list<int>& {
		return list_op<int>( fib, fib1, add );
	};

	int n;

	cin >> n;

	fib.until(
		[&](const int& a){
			cout << a << endl;},
		[&](const int& a) -> bool {
			return a >= n;});
	
	///*
	// cout << list_ref( fib, n ) << endl;
	//*/

	/*
	int f[2] = {0, 1};

	for( int i = 0; i <= n - 2; i ++ ){
		f[ i % 2 ] = f[ 0 ] + f[ 1 ];
	}

	cout << f[ n % 2 ];
	*/
	return 0;
}
