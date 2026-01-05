#include <stdio.h>
#include <limits.h> // for INT_MAX
#include "ticktack.h"

//
//  ticktack関数の使い方のサンプル
//
//  実行方法：
//    make
//           # 数秒ほど待つと結果が出る。
//
//  コラッツ予想を1千万までの整数 n について
//  確認する。その際、
//
//    グループ1: n = 1, 4, 7, 11, ...
//    グループ2: n = 2, 5, 8, 12, ...
//    グループ3: n = 3, 6, 9, 13, ...
//
//  の3つのグループの中でどれが一番計算
//  に時間がかかるかを見つける。
//
//  なお、このサンプルコードをコンパイルする
//  ときには最適化オプションを切ること。
//
//     例： gcc -O0
//
//  そうでないとコンパイラが「最適化」の
//  結果、一部の計算を「賢く」省略してしまう。
//

void collatz( long int start, long int end, long int skip )
{
  //   printf( "LONG_MAX = %ld\n", LONG_MAX );

  for ( long int n0=start; n0<=end; n0+=skip ) {
    long int n = n0;
    while ( n > 1 ) {
      if ( n % 2 == 0 ) {
        n = n / 2;
      }
      else {
        n = 3 * n + 1;
      }
    }
  }
}

int main( void ) 
{
  __tick_;         // メイン関数の計時開始

  __tick( "1st group" ); // 最初のグループ
  collatz(        1L, 10000000L, 3L );
  __tack( "1st group" );

  __tick( "2nd group" ); // 2番めのグループ
  // collatz( 10000002L, 20000001L, 2L );
  collatz(        2L, 10000000L, 3L );
  __tack( "2nd group" );

  __tick( "3rd group" ); // 3番めのグループ
  // collatz( 20000003L, 30000002L, 2L );
  collatz(        3L, 10000000L, 3L );
  __tack( "3rd group" );

  __tack_;         // メイン関数の計時終了
  __toe_;          // 計時結果を出力
  return 0;
}
