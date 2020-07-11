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
            cin >> house_holds;
            a.push_back( house_holds );
        }

        for( int i = 0; i < n; i ++ ){
            cin >> capacity;
            b.push_back( capacity )
        }

        function<bool(int, int)> under_dist = [&](int i, int dist) -> bool {
            if( dist < 0 ){
                return true;
            }else{
                if( i + 1 < n ){
                    return over_dist( i+1, dist + ( b[ i+1 ] - a[ i+1 ] ))
                }else{
                    return false;
                }
            }
        }

        int min = -1, max = b[0] + 1;
        while( max - min > 1 ){
            int mid = ( min + max ) / 2;
            if( under_dist( mid ) ){
                min = mid;
            }else{
                max = mid;
            }
        }


        auto valid_dist = [&](int i, int dist) -> bool {
            return 0 <= dist && dist <= b[ i ];
        }

        
        int dist = max;
        bool valid = valid_dist( 0, dist );

        for( int i = 1; i < n && valid ; i ++ ){
            dist = dist + ( b[ i ] - a[ i ] );
            if( ! valid_dist( i, dist ) ){
                valid = false;
            }
        }
        
        cout << valid ? "YES" : "NO" << endl;
    }

	return 0;
}
