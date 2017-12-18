/* program:  データ生成のプログラム（ソート用）
   file:     rands.c（プログラム）--> rands（コマンド）
   date:     2001/11/8 by O.W

   usage:
   rands <a> <b> <seed> <n>
             [a, b] の乱数を n 個求める（a <= b は 0 以上とする）
             seed は乱数の初期値                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int rand_seed, n;
  double a, b, x;
  int i;

  /* 初期化 */
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  rand_seed = atoi(argv[3]);
  n = atoi(argv[4]);
  sgenrand(rand_seed);

  /* ランダムな数の生成 */
  for(i = 1; i <= n; i++) {
    x = a + genrand() * (b - a);
    printf("%f\n", x);
  }

  exit(0);
}
