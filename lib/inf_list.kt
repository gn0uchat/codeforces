sealed class thunk<T>(){
	abstract fun eval() : inf_list<T>
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

class inf_list<T>( val head: T, var tail: thunk<T>, var delay: Boolean ){

	constructor( head: T, lambda: () -> inf_list<T> ) :
		this( head, lazy_thunk<T>( lambda ), true )

	constructor( head: T, tail: inf_list<T> ) :
		this( head, eager_thunk<T>( tail ), false )

	constructor( head: T ) :
		this( head, lazy_thunk<T>( {-> inf_list<T>( head )}), true )

	fun car() : T { return head }

	fun cdr() : inf_list<T> {
		if( delay ){
			val v = tail.eval()
			delay = false
			tail = eager_thunk<T>( v )
			return v
		}else{
			return tail.eval()
		}
	}

	fun set_cdr( lambda: () -> inf_list<T> ){
		tail = lazy_thunk<T>( lambda )
		delay = true
	}

	fun set_cdr( il: inf_list<T> ){
		tail = eager_thunk<T>( il )
		delay = false
	}

	fun map( lambda: (T) -> T ) : inf_list<T> {
		return inf_list<T>( lambda( car() ), {-> cdr().map( lambda )} )
	}

	fun filter( pass_cond: (T) -> Boolean ) : inf_list<T> {
		if( pass_cond( car() ) ){
			return inf_list<T>( car(), {-> cdr().filter( pass_cond )})
		}else{
			return cdr().filter( pass_cond )
		}
	}

	fun until( lambda: (T) -> Unit, stop_cond: (T) -> Boolean ){
		if( ! stop_cond( car() ) ){
			lambda( car() )
			cdr().until( lambda, stop_cond )
		}
	}
}

fun <T> list_op( il1: inf_list<T>, il2: inf_list<T>, op: (T, T) -> T ) : inf_list<T> {
	val head   = op( il1.car(), il2.car() )
	val lambda = {-> list_op<T>( il1.cdr(), il2.cdr(), op ) }
	return inf_list<T>( head, lambda )
}

fun <T> list_ref( il: inf_list<T>, n: Int ) : T {
	if( n == 0 ){
		return il.car()
	}else{
		return list_ref( il.cdr(), n - 1 )
	}
}

fun main( args: Array<String> ){
	val n = 16
	var adj = Array< inf_list<Int> >( n ){ i -> inf_list<Int>(i) }
	dddddddddddd
	adj[ a ] = list_interleave( adj[ a ], adj[ b ] )
	adj[ a ] = 
}
