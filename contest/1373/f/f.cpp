#include<map>
#include<set>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>
#include<string>
#include<functional>
#include<queue>
#include<iostream>
#include<list>
#include<tuple>
using namespace std;

class segment_t {
    public:
    int l, r;

    segment_t( int l, int r ){
        this->l = l;
        this->r = r;
    }

    int mid(){
        return ( l + r ) / 2;
    }

    segment_t left(){
        return segment_t( l, mid() );
    }

    segment_t right(){
        return segment_t( mid(), r );
    }

    bool unit(){
        return l + 1 == r;
    }
};

template<class T>
class segment_tree {
    class node_t : segment_t {
        public:
        int i;

        node_t(int i, int l, int r)
        : segment_t( l, r ) {
            this->i = i;
        }

        node_t(int i, segment_t seg)
        : segment_t( seg.l, seg.r ) {
            this->i = i;
        }

        node_t left(){
            return node_t( 2 * i, segment_t::left() );
        }

        node_t right(){
            return node_t( 2 * i + 1, segment_t::right() );
        }

    };
    
    private:

    vector<T> nodes;
    node_t root;

    function<T (T, T)> min;

    T query(node_t node, segment_t query){
        if( node.unit() ){
            return nodes[ node.i ];
        }else{
            if( query.r <= node.mid() ){
                return query( node.left(), query );
            }else if( node.mid() <= query.l ){
                return query( node.right(), query );
            }else{
                return this->min(
                    query( node.left(),  segment_t( query.l, node.mid() )),
                    query( node.right(), segment_t( node.mid(), query.r ))
                );
            }
        }
    }

    T build( node_t node, function<T (segment_t)> unit_value ){
        if( node.unit() ){
            nodes[ node.i ] = unit_value( node );
        }else{
            nodes[ node.i ] = this->min(
                build( node.left(),  unit_value ),
                build( node.right(), unit_value )
            );
        }
        return nodes[ node.i ];
    }

    public:

    segment_tree( int l, int r, function<T(int)> units, function<T (T, T)> min ){
            
        segment_t full_seg( l, r );

        for( int i = 0; i < 2 * units.size(); i ++ ){
            nodes.push_back( 0 );
        }

        this->min = min;
        this->root = node_t( 1, full_seg );

        build(
            root,
            [&]( segment_t seg ) -> T {
                return units( seg.l );
            }
        );
    }

    T query( int l, int r ){
        return query( root, segment_t( l, r ) );
    }
};

template<T>
T bin_search(T min, T max, function<bool(T)> check ){
    while( max - min > 1 ){
        T mid = ( min + max ) / 2;
        if( check( mid ) ){
            max = mid;
        }else{
            min = mid;
        }
    }
    return max;
}

int main(){
    int T;
    cin >> T;
    while( T --> 0 ){
        int n;
        cin >> n;

        vector<int> demand, supply;

        for( int i = 0; i < n; i ++ ){
            int input_demand;
            cin >> input_demand;
            demand.push_back( input_demand );
        }

        for( int i = 0; i < n; i ++ ){
            int input_supply;
            cin >> input_supply;
            supply.push_back( input_supply );
        }

        vector<long long> aggr;
        long long aggr_sum = 0;

        for( int i = 0; i < n; i ++ ){
            aggr.push_back( aggr_sum );
            aggr_sum += supply[ i ] - demand[ i ] 
        }

        segment_tree<long long> min_aggr( 0, n,
            [&](int i){ return aggr[ i ] },
            []( long long a, long long b ){ return min(a, b) }
        );

        segment_tree<long long> max_aggr( 0, n,
            [&]( int i ){ return aggr[ i ] - demand[ i ] },
            []( long long a, long long b ){ return max(a, b) }
        );
        
        int last_full_i = 0;
        auto over_supply = [&]( int init_supply ){
            last_full_i = bin_search<int>( last_full_i, n, [&]( int v ){
                return init_supply + max_aggr.query( 0, v ) > 0;
            })
        }

        /*
        function<tuple<bool, int>(int)> over_supply = 
        [&]( int init_supply ){
            bool over = true;
            int supply = init_supply;

            for( int i = 0; i < n; i ++ ){
                if( supply + supply[ i ] < demand[ i ] ){
                    over = false;
                    break;
                }else{
                    supply = supply[ i ] - min( supply, demand[ i ] );
                }
            }

            return make_tuple( over, supply );
        }

        priority_queue<int, vector<int>, greater<int>> linear_delta;
        int linear_n = 0, linear_aggr = 0;

        function<bool(int)> aggr_over_supply =
        [&]( int init_supply ){
            for(; linear_n < n &&
                linear_aggr + init_supply < demand[ linear_n ]; linear_n ++ ){

                linear_delta.push( linear_aggr );
                linear_aggr += supply[ linear_n ] - demand[ linear_n ];
            }
            if(! linear_delta.empty() && linear_delta.top() + init_supply >= 0 ){
                return true;
            }else{
                return false;
            }
        }

        auto[ over, supply ] = over_supply( b[ n-1 ] );

        if( ! over ){
            cout << "NO" << endl;
            continue;
        }

        int min = -1, max = b[ n-1 ];
        
        while( max - min > 1 ){
            int mid = ( min + max ) / 2;
            if( aggr_over_supply( mid ))
        }


    }
    */
	return 0;
}
