/* program:    ave
   file:       ave.c (source) --> ave (executable)
   date:       1999/11/9

   usage:
   ave <n> <k> < <file name>
   ただし，ファイル <file name> の中身は n 個の数が k 組．

       1
       5
       18
       34
       .
       .
       1234
       145749
       1111900  <-- n 個の数
       1
       5
       .
       .
       234567   <-- n 個の数
       ...
       以上が k 組

   すると，対応する k 組の平均が n 個出力される．*/

# include <stdio.h>

# define MAXSTONES 30
# define MAXKUMI 20

main(int argc, char *argv[])
{
  long data[MAXSTONES][MAXKUMI], total, average;
  int n, k;

  int i, j;

/* コマンドの入力 */
  if(argc == 3) {
    n = atoi(argv[1]);
    k = atoi(argv[2]);
  }
  else {
    printf("Usage:\n   ave <n> <k>\n");
    exit(0);
  }

  /* データの入力 */
  for(j = 0; j < k; j++) {
    for(i = 0; i < n; i++) {
      scanf("%d", &data[i][j]);
    }
  }

  /* 平均の計算 */
  for(i = 0; i < n; i++) {
    total = 0;
    for(j = 0; j < k; j++) {
      total = total + data[i][j];
    }
    average = total / k;
    printf("%d\n", average);
  }
}
