/* program:  最大公約数を求めるプログラム（シラミツブシ法）
   file:     gcd1.c（プログラム）--> gcd1（コマンド）
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
  int x, y;     /*  入力 */
  int steps;    /* 除余演算の回数*/
  int k, N;
  int ans;
  
  /* 初期化 */
  x = atoi(argv[1]);   /* 第一引数を x に代入 */
  y = atoi(argv[2]);   /* 第二引数 を y に代入 */
  steps = 0;

  N = min(x,y);
  
  for(k = 1; k <= N; k++){
    steps = steps + 2; /* 次の行の除余演算をカウント */
    if ((x % k == 0) && (y % k == 0)){
      ans = k; 
    }
  }

  printf("GCD of %d and %d is %d \n", x,y, ans);
  printf("%d steps \n", steps);
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

