/* program:  データ生成のプログラム
   file:     genr.c（プログラム）--> genr（コマンド）
   date:     2000/10/2 by K.H

   usage:
   genfib <t>
             t, t+1 番目のフィボナッチ数を求める．
             その２進桁数も返す                                  */

#include <stdio.h>

int main(argc,argv)
     int argc;
     int *argv[];
{
  int i, j, k;
  int t;

  t = atoi(argv[1]); /* 引数を t に代入 */
  i = 1;
  j = 0;
  
  for(k = 1; k <= t; k++){
    j = i + j;
    i = j - i;
  }
  
  printf("%d\t",j); /* t 番目のフィボナッチ数を返す */
  
  j = i + j;     /* t+1 番目のフィボナッチ数 */
  i = j - i;     /* t 番目のフィボナッチ数 */

  printf("%d\t",j); /* t+1 番目のフィボナッチ数を返す */
  
  printf("%d\t", getbinlen(i)); /* t 番目のフィボナッチ数の２進桁数 */
  printf("%d\n", getbinlen(j)); /* t + 1 番目のフィボナッチ数の２進桁数 */
  exit(0);
}

int getbinlen(x)   /* 正整数 x の２進表記における桁数を返す */
     int x;
{
  int i,y;
  
  i = 0;
  y = x;

  while ( y != 0) {
    y =(int) y /2;
    i = i + 1;
  }
  
  return(i);

}
  


