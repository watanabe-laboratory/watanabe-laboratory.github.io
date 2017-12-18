/* program:  データ生成のプログラム（ソート用）
   file:     randfloat.c（プログラム）--> randfloat（コマンド）
   date:     2001/10/24 by O.W

   usage:
   randfloat <a> <b> <seed>
             [a, b] の乱数を 1 つ求める（a <= b は 0 以上とする）
             seed は乱数の初期値                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int rand_seed;
  double a, b, x;
  int i;

  /* 初期化 */
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  rand_seed = atoi(argv[3]);
  sgenrand(rand_seed);

  /* ランダムな数の生成 */
  x = a + genrand() * (b - a);
  printf("%f\n", x);
  exit(0);
}
