/* program:    sum
   file:       sum.c (source) --> sum (executable)
   date:       1998/8/28 by O.W.
               1999/11/8 空行が入るようにした

   usage:
   (1) sum <n> <k> <s> を実行．
       ただし，n: 石の数，k: 目標の重さの数
               s: 乱数のシード（自分の学籍番号（6桁））
   (2) 次の例のように n 個の石の重さを入力する．
       345732
       1234
       1457
       1111900
       .
       .
       234567 <-- n 個目
   (3) すると，その中から適当に選んで合計した重さが k 通り，次の
       ように出力される．

       346966
       2691
       .
       .
       82345789 <-- k 個目

    注） sum -a <n> <k> <s> とすると，答えも一緒に出力される．  */

#include <stdio.h>

#define MAXSTONES 30
# define TRUE 1
# define FALSE 0

main(int argc, char *argv[])
{
  long weight[MAXSTONES];
  long sum;
  int slist[MAXSTONES];
  int n, m;
  unsigned seed;
  int flg;

  int i, t;

/* コマンドの入力 */
  if(argc == 4) {
    flg = FALSE;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    seed = atoi(argv[3]);
  }
  else if((argc == 5) && (argv[1][0] == '-') && (argv[1][1] == 'a')) {
    flg = TRUE;
    n = atoi(argv[2]);
    m = atoi(argv[3]);
    seed = atoi(argv[4]);
  }
  else {
    printf("Usage:\n   sum <n> <k> <s>\
            \n   sum -a <n> <k> <s>\n");
    exit(0);
  }

  /* 石の重さの入力 */
  for(i = 0; i < n; i++) scanf("%d", &weight[i]);

  srandom(seed);
  printf("  \n"); /* 空行の出力 */

  for(t = 0; t < m; t++){

    /* 石選び */
    for(i = 0; i < n; i++) slist[i] = random() % 2;
    
    /* 重さの総和の計算と出力 */
    sum = 0;
    for(i = 0; i < n; i++)
      if(slist[i]) sum = sum + weight[i];
    printf("%d", sum);
    if(flg) {
      printf(": ");
      for(i = 0; i < n; i++) printf("%d,", slist[i]);
    }
    printf("\n");
  }
}
