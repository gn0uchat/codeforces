template< typename X_T, typename Y_T >
class line_t{
private:
public:
	Y_T a, b;
	line_t(){}
	line_t( Y_T a, Y_T b ):a( a ), b( b ){}
	Y_T operator() ( X_T x ) const {
		return a * ( ( Y_T ) x ) + b;
	}
};

void add_line( vector<line_t<int, number>> &bag, int& bag_i, line_t<int, number> line ){
	if( bag.size() < 1 ){
		bag.push_back( line );
	}else{
		if( bag.back().a == line.a ){
			if( bag.back().b < line.b ){
				line = bag.back();
				bag.pop_back();
			}else{
				bag.pop_back();
			}
		}
		while( bag.size() >= 2 ){
			line_t<int, number> last_l = bag.back();
			bag.pop_back();

			if( ( ( line.b - last_l.b ) * ( bag.back().a - last_l.a ) ) > 
					( ( last_l.b - bag.back().b ) * ( last_l.a - line.a ) ) ){
				bag.push_back( last_l );
				break;
			}
		}
		bag.push_back( line );
	}

	if( bag_i >= bag.size() ){
		bag_i = bag.size() - 1;
	}
}

number query_line( const vector<line_t<int, number>> & bag, int& bag_i, int x ){
	for(; bag_i + 1 < bag.size(); bag_i ++ ){
		int next_i = bag_i + 1;
		if( ( bag[ next_i ].b - bag[ bag_i ].b ) > ( ( bag[ bag_i ].a - bag[ next_i ].a ) * ((number) x) ) ){
			break;
		}
	}
	return bag[ bag_i ]( x );
}

