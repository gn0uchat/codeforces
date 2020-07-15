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

        for( int i = 0; i < 2 * ( r - l ); i ++ ){
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
        T mid = ( min + max + 1 ) / 2;
        if( check( mid ) ){
            max = mid;
        }else{
            min = mid;
        }
    }
    return min;
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
            aggr.push_back( aggr_sum - demand[ i ] );
            aggr_sum += supply[ i ] - demand[ i ] 
        }

        segment_tree<long long> min_aggr( 0, n,
            [&](int i){ return aggr[ i ] },
            []( long long a, long long b ){ return min(a, b) }
        );

        segment_tree<long long> max_aggr( 0, n,
            [&]( int i ){ return aggr[ i ] },
            []( long long a, long long b ){ return max(a, b) }
        );
        
        auto over_supply = [&]( int init_supply ) -> tuple<bool, long long> {
            bool over = true;
            long long supply = init_supply;
            for( int i = 0; i < n && over; i ++ ){
                if( supply + supply[ i ] >= demand[ i ] ){
                    over = false;
                }else{
                    supply = supply[ i ] - min(supply, demand[ i ]);
                }
            }
            return make_tuple( over, supply );
        }

        auto( cond_over, cond_supply ) = over_supply( supply[ n-1 ] );
        if( ! cond_over ){
            cout << "NO" << endl;
            continue;
        }

        auto quick_over_supply = [&]( int init_supply ) -> bool {
            int last_full_i = bin_search<int>( -1, n, [&]( int v ){
                return init_supply + max_aggr.query( 0, v ) >= 0; }
            );
            return init_supply + min_aggr.query( 0, last_full_i ) >= 0
        }

        int opt_supply = bin_search<int>( -1, supply[ n-1 ], [&]( int v ){
            quick_over_supply( v )
        })

        auto( over, supply ) = over_suply( opt_supply )

        if( over && supply + ( supply[ n-1 ] + opt_supply ) >= demand[ n-1 ] ){
            cout << "YES" << endl;
        }else{
            cout << "NO" << endl;
        }

        return 0;
}
