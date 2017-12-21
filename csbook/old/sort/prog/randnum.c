/* program:  データ生成のプログラム（ソート用）
   file:     randnum.c（プログラム）--> randnum（コマンド）
   date:     2001/11/8 by O.W

   usage:
   randnum <a> <b> <seed>
             [a, b] の乱数を 1 つ生成する（a <= b は 0 以上とする）
             seed は乱数の初期値                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int rand_seed, num;
  double a, b, x;

  /* 初期化 */
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  rand_seed = atoi(argv[3]);
  sgenrand(rand_seed);

  /* ランダムな数の生成 */
  do {
    x = genrand() * (b + 1 - a);
    num = a + x;
  } while(num > b);
  printf("%d\n", num);

  exit(0);
}
