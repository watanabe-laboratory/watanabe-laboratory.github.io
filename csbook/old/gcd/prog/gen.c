/* program:  データ生成のプログラム
   file:     gen.c（プログラム）--> gen（コマンド）
   date:     2000/10/2 by K.H and O.W.

   usage:
   gen <n>   ２進数で n 桁の数のうち最大のものを出力
             つまり，出力 = 2**n-1                        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int n, ans;
  
/* コマンドの入力 */
  n = atoi(argv[1]);

/* 計算 */

  ans = power(2, n) - 1;
  printf("%d \n", ans) ;
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

