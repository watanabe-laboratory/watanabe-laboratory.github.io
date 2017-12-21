/* program:  最大公約数を求めるプログラム（ユークリッドの互除法）
   file:     gcd2.c（プログラム）--> gcd2（コマンド）
   date:     2000/10/2 by K.H

   usage:
   gcd1 <x> <y>
             x と y の最大公約数．使用した mod の回数も出力   */

#include <stdio.h>
#include <stdlib.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int x, y;    /*  入力のうちそれぞれ大きい,小さい数 */
  int steps;   /* 除余演算の回数*/
  int k, r;
  int ans;
  
  /* 初期化 */
  x = atoi(argv[1]);   /* 第一引数を x に代入 */
  y = atoi(argv[2]);   /* 第二引数 を y に代入 */
  steps = 0;

  steps = steps + 1;
  r = x % y;
  
  while(r > 0) {
    x = y;
    y = r;
    steps = steps + 1;
    r = x % y;
  }

  ans = y;

  /* 以下削除
  printf("GCD of %s and %s is %d \n", argv[1],argv[2], ans);
  printf("%d steps \n", steps);
  ここまで削除 */

  printf("%d\n", steps);
  exit(0);
}
