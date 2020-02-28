template< typename T>
class segm_t{
public:
	T l, r;
	segm_t(){}
	segm_t( T b ):
		l( b ), r( b + 1 ){}
	segm_t( T l, T r ):
		l( l ), r( r ){}

	bool is_null() const {
		return this->l == this->r;
	}

	bool contain( const segm_t<T>& s ) const {
		return ( this->l <= s.l ) && ( this->r >= s.r );
	}

	T mid() const {
		return ( this->l + this->r ) / 2;
	}

	tuple<segm_t<T>, segm_t<T>> split_half() const {
		return { segm_t<T>( this->l, this->mid() ), segm_t<T>( this->mid(), this->r ) };
	}

	bool is_unit() const {
		return ( this->l + 1 == this->r );
	}

};

template< typename X_T, typename Y_T >
class line_t{
private:
	Y_T a, b;
public:
	line_t(){}
	line_t( Y_T a, Y_T b ):a( a ), b( b ){}
	Y_T operator() ( X_T x ) const {
		return a * ( ( Y_T ) x ) + b;
	}
};

template< typename VAR_T, typename COEF_T>
class cht_tree{
private:
	typedef line_t<VAR_T, COEF_T> LN_T;

	vector<tuple<bool, LN_T>> tree_node;
	segm_t< VAR_T > full_segm;

	bool line_ge( segm_t<VAR_T> bnd, const LN_T& b, const LN_T& s ){
		return s( bnd.l ) <= b( bnd.l ) && s( bnd.r - 1 ) <= b( bnd.r - 1 );
	}

	tuple<bool, LN_T> tree_query( int i, const segm_t<VAR_T>& bnd, VAR_T x ){
		if( bnd.is_unit() ){
			return tree_node[ i ];
		}else{
			auto [ set, segm_l ] = tree_node[ i ];

			if( set ){
				segm_t<VAR_T> next_bnd;
				int next_i;

				auto[ l_bnd, r_bnd ] = bnd.split_half();

				if( l_bnd.contain( segm_t<VAR_T>( x ) ) ){
					next_bnd = l_bnd;
					next_i = 2 * i;
				}else{ 
					next_bnd = r_bnd;
					next_i = 2 * i + 1;
				}

				auto[ sb_set, sb_l ] = tree_query( next_i, next_bnd, x );
				if( sb_set && ( segm_l( x ) < sb_l( x ) ) ){
					segm_l = sb_l;
				}
			}
			return { set, segm_l };
		}
	}

	void tree_insert( int i, const segm_t<VAR_T>& bnd, const LN_T& l ){
		auto& [ set, segm_l ] = tree_node[ i ];

		if( ! set ){
			set = true;
			segm_l = l;
		}else{
			if( line_ge( bnd, segm_l, l ) ){
			}else if( line_ge( bnd, l, segm_l ) ){
				segm_l = l;
			}else{
				auto[ l_bnd, r_bnd ] = bnd.split_half();
				VAR_T m = bnd.mid();

				LN_T up_l = l(m) < segm_l(m) ? segm_l : l;
				LN_T dn_l = l(m) < segm_l(m) ? l : segm_l;

				segm_l = up_l;

				if( up_l( bnd.l ) < dn_l( bnd.l ) ){
					tree_insert( 2 * i, l_bnd, dn_l );
				}else{
					tree_insert( 2 * i + 1, r_bnd, dn_l );
				}
			}
		}
	}

public:
	cht_tree(){}
	cht_tree( VAR_T l, VAR_T r ) : full_segm( l, r ), tree_node( 4 * ( r - l ), {false, LN_T()} ){}

	void insert( COEF_T a, COEF_T b ){
		tree_insert( 1, full_segm, LN_T( a, b ) );
	}

	auto query( VAR_T x ){
		auto[ set, l ] = tree_query( 1, full_segm, x );
		return l( x );
	}
};
