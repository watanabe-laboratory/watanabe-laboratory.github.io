/* program:  与えられた複数の数値データを昇順に並び替える（選択ソート）
   file:     ssort.c（プログラム）--> ssort（コマンド）
   date:     2001/10/16 by Takeru Sasou
*/

#include <stdio.h>

float *a;
int count;

/* 選択ソートで長さ n の配列 a を昇順にソートする */
void sortS(int n)
{
  float a0;  /* 配列中の最小値 */
  int i0;    /* 最小値のインデックス */
  int i, j;
  float tmp;

  for(i = 0; i < n - 1; i++) {
    a0 = a[i];
    i0 = i;

    /* a[i]..a[n] の中の最小値の場所 i0 を求める */
    for(j = i + 1; j < n; j++) {
      count++;
      if(a[j] < a0) {
	a0 = a[j];
	i0 = j;
      }
    }

    /* a[i] と a[i0] の値を交換 */
    tmp = a[i];
    a[i] = a[i0];
    a[i0] = tmp;
  }
}

void main(int argc, char *argv[])
{
  int i, n;

  /* 数値配列の要素数を取得 */
  n = atoi(argv[1]);

  /* 配列データ用のメモリ領域の割り当て */
  a = (float *)malloc(sizeof( float ) * n);

  /* 配列の値を読み込む */
  for(i = 0; i < n; i++) {
    fscanf(stdin, "%f", &a[i]);
  }

  count = 0;
  /* 配列のソート */
  sortS(n);

  /* ソートした結果を出力 */
  for(i = 0; i < n; i++) {
    printf("%f\n", a[i]);
  }
  printf("Number of comparisons = %d\n", count);

  /* メモリ領域の開放 */
  free(a);

  exit(0);
}
