/* program:  最大公約数を求めるプログラム（シラミツブシ法改良型）
   file:     gcd1ow.c（プログラム）--> gcd1ow（コマンド）
   date:     2000/10/2 by K.H

   usage:
   gcd1 <x> <y>
             x と y の最大公約数を求めるときに使用した mod の回数   */

#include <stdio.h>
#include <stdlib.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int x, y;     /*  入力 */
  int steps;    /* 除余演算の回数*/
  int k, N;
  int ans;
  
  /* 初期化 */
  x = atoi(argv[1]);   /* 第一引数を x に代入 */
  y = atoi(argv[2]);   /* 第二引数 を y に代入 */
  steps = 0;

  N = min(x,y);
  
  for(k = N; k >= 1; k--){
    steps = steps + 2; /* 次の行の除余演算をカウント */
    if ((x % k == 0) && (y % k == 0)){
      ans = k;
      break;
    }
  }

  /* 以下削除
  printf("GCD of %s and %s is %d \n", argv[1],argv[2], ans);
  printf("%d steps \n", steps);
  ここまで削除 */

  printf("%d\n", steps);
  exit(0);
}
      
int min(x,y)
     int x,y;
{
  if(x <= y){
    return(x);
  }
  else{
    return(y);
  }
}

