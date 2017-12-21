/* program:    sum
   file:       sumall.c (source) --> sumall (executable)
   date:       1998/11/12 by O.W.
               1999/11/8 空行が入るようにした

   usage:
   (1) sumall <n> を実行．
       ただし，n: 石の数
   (2) 次の例のように n 個の石の重さを入力する．

       345732
       1234
       1457
       1111900
       .
       .
       234567 <-- n 個目
   (3) すると，その合計の重さが出力される．   */

#include <stdio.h>

#define MAXSTONES 30
# define TRUE 1
# define FALSE 0

main(int argc, char *argv[])
{
  long weight, sum;
  int n;

  int i;

/* コマンドの入力 */
  if(argc == 2) {
    n = atoi(argv[1]);
  }
  else {
    printf("Usage:\n   sum <n>\n");
    exit(0);
  }

  printf("  \n"); /* 空行の出力 */

  /* 石の重さの入力 */
  sum = 0;
  for(i = 0; i < n; i++) {
    scanf("%d", &weight);
    sum = sum + weight;
  }
  printf("%d\n", sum);
}
