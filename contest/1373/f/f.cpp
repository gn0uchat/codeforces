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

int main(){
    int t;
    cin >> t;
    while( t --> 0 ){
        int n;
        vector<int> a, b;

        cin >> n;

        for( int i = 0; i < n; i ++ ){
            int house_holds;
            cin >> house_holds;
            a.push_back( house_holds );
        }

        for( int i = 0; i < n; i ++ ){
            int capacity;
            cin >> capacity;
            b.push_back( capacity );
        }

        function<tuple<bool, int>(int, int)> over_supply = [&]
        (int i, int previous_supply) -> tuple<bool, int> {
            bool over = true;
            int supply = previous_supply;

            for( int i = 0; i < n - 1; i ++ ){
                if( supply + b[ i ] < a[ i ] ){
                    over = false;
                    break;
                }
                int satisfied = min( supply, a[ i ] );
                supply = b[ i ] - ( a[ i ] - satisfied );
            }
            return make_tuple( over, supply );
        /*
            if( i == n - 1 ){
                return true;
            }else{
                if( previous_supply + b[ i ] < a[ i ] ){
                    return false;
                }else{
                    int satisfied = min( previous_supply, a[ i ] );
                    int next_supply = b[ i ] - ( a[ i ] - satisfied );
                    return over_supply( i + 1, next_supply );
                }
            }
        */
        };

        function<bool(int, int, int)> valid_supply =
        [&](int init_supply, int i, int previous_supply) -> bool {
            int back_supply = b[ n-1 ] - init_supply;
            auto[ over, supply ] = over_supply( 0, init_supply );

            return over && back_supply + supply >= a[ n-1 ];
            
        /*
            if( i == n - 1 ){
                int back_supply = b[ n-1 ] - init_supply;
                return previous_supply + back_supply >= a[ n-1 ];
            }else{
                if( previous_supply + b[ i ] < a[ i ] ){
                    return false;
                }else{
                    int satisfied = min( previous_supply, a[ i ] );
                    int next_supply = b[ i ] - ( a[ i ] - satisfied );
                    return valid_supply( init_supply, i + 1, next_supply );
                }
            }
        */
        };

        int min = -1, max = b[ n-1 ]; 

        while( max - min > 1 ){
            int mid = ( min + max ) / 2;
            auto [over, supply] = over_supply( 0, mid );
            if( over ){
                max = mid;
            }else{
                min = mid;
            }
        }

        int opt_supply = max;

        if( valid_supply( opt_supply, 0, opt_supply ) ){
            cout << "YES" << endl;
        }else{
            cout << "NO" << endl;
        }
    }

	return 0;
}
