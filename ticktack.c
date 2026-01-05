#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include "ticktack.h"

/*
 * ticktack.c
 *
 *   - プログラムの特定の範囲の実行時間を測る（計時する）。
 *   - 計時の範囲は文字列（ラベル）で区別する。
 *   - ラベルごとに別個のストップウォッチがあると考えるとよい。
 *   - 同じラベルのプログラム領域が何度も呼ばれると、
 *       そのプログラム領域の実行時間を累積する。
 *   - 一つのプログラム領域は一つの関数全体でもよいし、
 *       関数の中の一部、たとえばループ部分だけでもよい。
 *   - あるプログラム領域を計時するには、その領域の冒頭で
 *          __tick( その領域の固有ラベル );
 *       を呼び、その領域の末尾で
 *          __tack( その領域の固有ラベル );
 *       を呼ぶ。
 *   - 最後に実行時間の割合を表示するために、プログラム全体の
 *       実行時間を必ず測定すること。
 *       main関数の冒頭でたとえば
 *          __tick( "main" );
 *       とし、main関数の最後に
 *          __tack( "main" );
 *       とする。
 *   - 最後に
 *         __ticktack_print();
 *       を呼べば計測結果を標準出力に書き出す。
 *   - その計測結果のリスト表示では最初（0番目）のストップ
 *       ウォッチはプログラム全体の時間計測用と仮定している。
 *   - ラベルは互いに重複しない文字列であれば任意である。
 *   - 関数の実行時間の測定の際にはストップウォッチの文字列
 *       ラベルとしてその関数名を使うのが自然であろう。そう
 *       いう場合にはC言語のマクロ __func__ を使うのが便利
 *       である。
 *   - このマクロ__func__との組み合わせはよく使うので次の
 *       マクロをticktack.hで定義している
 *          __tick_  ( = __tick( __func__ ) )
 *          __tack_  ( = __tack( __func__ ) )
 *          __toe_   ( = __ticktack_print() )
 *
 * 使い方
 *       int main()
 *       {
 *         __tick_; // start main
 *
 *         ...
 *         func1();
 *         ...
 *         func2();
 *         ...
 *         __tack_; // stop main
 *         __toe_;  // print
 *       }
 *
 *       void func1()
 *       {
 *         __tick_;            // start func1
 *         __tack( "part_A" ); // start Part A
 *         .
 *         .
 *         __tack( "part_A" ); // start Part A
 *         __tick( "part_B" ); // start Part B
 *         .
 *         .
 *         __tack( "part_B" ); // stop Part B
 *         __tack_             // stop func1
 *       }
 *
 *
 */


#define MAX_NUM_OF_LAP 100
#define MAX_LEN_OF_LAP_LABEL 200

static int Num_of_registered_lap = 0;
     // 異なるラベルの数（=ストップウォッチの数）
static char Registered_lap_label[MAX_NUM_OF_LAP]
                                [MAX_LEN_OF_LAP_LABEL];
     // ストップウォッチのラベル文字列
static double Registered_lap_time_start[MAX_NUM_OF_LAP];
     // それぞれのストップウォッチの開始ボタンが押された時刻
static double Registered_lap_time_accum[MAX_NUM_OF_LAP] = {0};
     // それぞれのストップウォッチ実行累積時間



/*
 * C言語で時刻を秒単位で取得する定番の方法
 */
static double gettimeofday_in_seconds()
{
  struct timeval tv;

  gettimeofday( &tv, NULL );

  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}


/*
 * label の範囲の実行時間を測るためにストップウォッチ
 * を押し、計時を開始する
 */
void __tick( const char *label )
{
  int this_label_is_nth_in_the_list = -1;
    // このラベルは既に登録されていればリストの何番目か
  for ( int i=0; i<Num_of_registered_lap; i++ ) {
    if ( strcmp( label, Registered_lap_label[i] ) == 0 ) {
      // 既に登録されていたラベル
      this_label_is_nth_in_the_list = i;
      break;
    }
  }
  if ( this_label_is_nth_in_the_list==-1 ) { 
    // このlabelは初めて出てきたのでリストに登録する
    strcpy( Registered_lap_label[Num_of_registered_lap], label );
    this_label_is_nth_in_the_list = Num_of_registered_lap;
    Num_of_registered_lap += 1;
  }

  // このlabelのついたストップウォッチを押して計時開始
  Registered_lap_time_start[this_label_is_nth_in_the_list]
    = gettimeofday_in_seconds(); 
}

/*
 * このラベルのストップウォッチのボタンを押して計時を止め
 * 累積時間を記憶する
 */
void __tack( const char *label )
{
  // このラベルは登録リストの何番目か調べる
  int this_label_is_nth_in_the_list = -1;
  for ( int i=0; i<Num_of_registered_lap; i++ ) {
    if ( strcmp( label, Registered_lap_label[i] ) == 0 ) {
      this_label_is_nth_in_the_list = i;
      break;
    }
  }
  if ( this_label_is_nth_in_the_list==-1 ) {
    // これは想定外。このラベルのついた
    //   ストップウォッチは未登録。つまり開始ボタンを
    //   押していないのに終了ボタンを押そうとしている。
    //   プログラム全体を止めるほどのミスではないので、
    //   メッセージをだして処理は続ける
    printf( "Forgot start? %s (%s)\n", __func__, label );
  }
  else {
    Registered_lap_time_accum[this_label_is_nth_in_the_list]
        += gettimeofday_in_seconds() 
         - Registered_lap_time_start[this_label_is_nth_in_the_list];
  }
}


/*
 * シミュレーションプログラムの終了時に呼ぶ関数
 *   すべてのラベル（ストップウォッチ）の計時結果を
 *   標準出力に書き出す。最初に登録された0番目の
 *   ストップウォッチはメインプログラムと仮定している
 */
void __ticktack_print()
{
  double total_exec_time = Registered_lap_time_accum[0];

  printf( "=========================================================\n" );
  printf( "%27s: %9.3e (sec)\n", Registered_lap_label[0],
                                 total_exec_time );
  printf( "---------------------------------------------------------\n" );
  for ( int i=1; i<Num_of_registered_lap; i++ ) {
    printf( "%27s: %9.3e (sec) [%4.1f%%]\n", 
                   Registered_lap_label[i], 
                   Registered_lap_time_accum[i],
                   Registered_lap_time_accum[i]/total_exec_time*100);
  }
  printf( "=========================================================\n" );
}

