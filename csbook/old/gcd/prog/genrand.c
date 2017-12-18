/* program:  データ生成のプログラム
   file:     genrand.c（プログラム）--> genrand（コマンド）
   date:     2000/10/2 by K.H, modified by O.W

   usage:
   genrand <n> <seed>
             ２進で n 桁の数をランダムに求める．
             seed は乱数の初期値                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int n;  /* ２進数での桁数 */
  int x;
  int rand_seed; 

  /* 初期化 */
  n = atoi(argv[1]);
  rand_seed = atoi(argv[2]);
  sgenrand(rand_seed);

  /* n 桁のランダムな自然数の生成 */
  x = (int)power(2, n - 1)
      + genrand() * (power(2, n) -1 - power(2, n - 1));
  printf("%d\n",x);
  exit(0);

}

int power(x,y)       /* x の y 乗を計算 (x ,y は 正整数)*/
     int x,y;
{
  int i;
  int ans;
  
  ans = 1;

  for(i = 1;i <= y; i++){
    ans = ans * x;
  }
  return(ans);
}
