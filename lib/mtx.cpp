template<class T, int n>
class mtx_t{
private:
public:
	T entry[ n ][ n ];

	mtx_t(){
	}

	mtx_t( initializer_list< initializer_list< T >> init_l ){
		int i = 0;
		for( auto v : init_l ){
			int j = 0;
			for( auto s : v ){
				this->entry[ i ][ j ] = s;
				j ++;
			}
			i ++;
		}
	}

	friend mtx_t<T, n> operator* ( const mtx_t<T,n> &a, const mtx_t< T, n>&b ){
		mtx_t< T, n > r;
		for( int i = 0; i < n; i ++ ){
			for( int j = 0; j < n; j ++ ){
				for( int k = 0; k < n; k ++ ){
					r.entry[ i ][ j ] = r.entry[ i ][ j ] + ( a.entry[ i ][ k ] * b.entry[ k ][ j ] );
				}
			}
		}
		return r;
	}
	
	T sum(){
		auto acc = T();
		for( int i = 0; i < n; i ++ ){
			for( int j = 0; j < n; j ++ ){
				acc = acc + this->entry[ i ][ j ];
			}
		}
		return acc;
	}
};
