sealed class thunk<T>(){
	abstract fun eval(): inf_list<T>
}

class eager_thunk<T>( val value: inf_list<T> ): thunk<T>(){
	override fun eval(): inf_list<T> {
		return value;
	}
}

class lazy_thunk<T>( var lambda: () -> inf_list<T> ): thunk<T>(){
	override fun eval(): inf_list<T>{
		return lambda();
	}
}

class inf_list<T>( val head: T, var tail: thunk<T> ){

	constructor( head: T, lambda: () -> inf_list<T> ) :
		this( head, lazy_thunk<T>( lambda ) )

	constructor( head: T, tail: inf_list<T> ) :
		this( head, eager_thunk<T>( tail ) )

	fun car(): T { return this.head; }

	fun cdr(): inf_list<T> {
		val set_and_eval = fun( delay: lazy_thunk<T> ): inf_list<T>{
			val v = delay.eval()
			this.tail = eager_thunk<T>( v )
			return v
		}

		return when ( this.tail ){
			is eager_thunk<T> -> this.tail.eval()
			is lazy_thunk<T>  -> set_and_eval( this.tail )}
	}

	fun set_cdr( lambda: () -> inf_list<T> ){
		this.tail = lazy_thunk<T>( lambda )
	}

	fun set_cdr( il: inf_list<T> ){
		this.tail = eager_thunk<T>( il )
	}
}

class repeat_list<T>( )

fun <T> list_op( il1: inf_list<T>, il2: inf_list<T>, op: (T, T) -> T ) : inf_list<T> {
	val head   = op( il1.car(), il2.car() )
	val lambda = {-> list_op<T>( il1.cdr(), il2.cdr(), op ) }
	return inf_list<T>( head, lambda )
}

fun main( args: Array<String> ){

	val add = fun( a: Int, b: Int ) : Int { return a + b; }

	var fib = inf_list<Int>( 0 )

	fib.set_cdr( inf_list<Int>( 1, {-> list_op<Int>( fib, fib.cdr(), add )}))

	println( fib.car() )
}
