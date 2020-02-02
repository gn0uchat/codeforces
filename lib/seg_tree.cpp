template<class T>
class seg_tree{
private:
	typedef tuple<int, int> segment;

	int alloc_i;
	vector< tuple< int, int, T > > mem;
	function<T(const T&, const T&)> op;
	function<T(int)> init_v;
	segment full_seg;

	bool unit( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		return ( nd_ub - nd_lb ) == 1;
	}

	segment left_seg( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		int mid = ( nd_lb + nd_ub ) / 2;
		return { nd_lb, mid };
	}

	segment right_seg( segment nd_seg ){
		auto [ nd_lb, nd_ub ] = nd_seg;
		int mid = ( nd_lb + nd_ub ) / 2;
		return { mid, nd_ub };
	}

	T node_init( segment nd_seg, int nd_i ){
		auto&[ li, ri, v ] = mem[ nd_i ];

		if( unit( nd_seg ) ){
			auto[ lb, ub ] = nd_seg;
			v = this->init_v( lb );
			return v;
		}else{
			li = this->alloc_i ++;	
			ri = this->alloc_i ++;

			T lv = node_init( left_seg ( nd_seg ), li );
			T rv = node_init( right_seg( nd_seg ), ri );

			v = this->op( lv, rv );
			return v;
		}
	}

	T node_query( segment nd_seg, segment qy_seg, int nd_i ){
		auto&[ li, ri, v ] = mem[ nd_i ];
		auto [ nd_lb, nd_ub ] = nd_seg;
		auto [ qy_lb, qy_ub ] = qy_seg;

		if( nd_seg == qy_seg ){
			return v;
		}else{
			int mid = ( nd_lb + nd_ub ) / 2;
			if( qy_ub <= mid ){
				return this->node_query( left_seg( nd_seg ),  qy_seg, li );
			}else if( mid <= qy_lb ){
				return this->node_query( right_seg( nd_seg ), qy_seg, ri );
			}else{
				T lv = this->node_query( left_seg( nd_seg ) , { qy_lb, mid }, li );
				T rv = this->node_query( right_seg( nd_seg ), { mid, qy_ub }, ri );
				return this->op( lv, rv );
			}
		}
	}

	T node_update( segment nd_seg, int p, int nd_i, const T& update_v ){
		auto&[ li, ri, v ] = mem[ nd_i ];
		auto[ lb, ub ] = nd_seg;

		if( unit( nd_seg ) ){
			v = update_v;
			return v;
		}else{
			int mid = ( lb + ub ) / 2;	
			if( p < mid ){
				T lv = this->node_update( left_seg( nd_seg ), p, li, update_v );
				T rv = this->node_query( right_seg( nd_seg ), right_seg( nd_seg ), ri );
				v = this->op( lv, rv );
			}else{
				T lv = this->node_query( left_seg( nd_seg ), left_seg( nd_seg ), li );
				T rv = this->node_update( right_seg( nd_seg), p, ri, update_v );
				v = this->op( lv, rv );
			}
			return v;
		}
	}

public:
	seg_tree( int lb, int ub, function<T(int)> init_v, function<T( const T&, const T& )> op ){
		int mem_n = 4 * ( ub - lb );

		this->op = op;
		this->init_v = init_v;
		this->alloc_i = 0;
		this->full_seg = { lb, ub };
		this->mem.resize( mem_n );

		this->node_init( this->full_seg, this->alloc_i ++ );
	}

	T query( int lb, int ub ){
		return this->node_query( this->full_seg, {lb, ub}, 0 );
	}

	T update( int p, const T& update_v ){
		return this->node_update( this->full_seg, p, 0, update_v );
	}

};
