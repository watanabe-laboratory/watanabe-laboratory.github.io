/* program:  与えられた複数の数値データを昇順に並び替える（マージソート）
   file:     msort.c（プログラム）--> msort（コマンド）
   date:     2001/10/19 by Takeru Sasou
             2001/10/25 modified by O.W
*/

#include <stdio.h>
#include <float.h>

float *a;
int count;

void merge(int s1, int t1, int s2, int t2)
/* 配列 a の [s1..t1] と [s2..t2] をマージし，[s..t] へしまう
   注： s = s1, t1 + 1 = s2, t2 = t
        [s1..t1] の方が [s2..t2] より長い
*/
{
  int t1tmp = t1 - s1;
  float A[t1tmp + 1];
  int i, j, k;

  /* [s1..t1] を作業領域に退避 */
  for(i = s1; i <= t1; i++) {
    A[i - s1] = a[i];
  }

  i = 0;  j = s2;
  for(k = s1; k <= t2; k++) {
    count++;
    if(A[i] <= a[j]){
      a[k] = A[i];
      i++;
      if(i > t1tmp) {       /* 前半が先に終了．後半の残りの処理 */
	for(; j <= t2; j++) {
	  k++;
	  a[k] = a[j];
	}
	break;
      }
    }
    else{
      a[k] = a[j];
      j++;
      if(j > t2) {           /* 後半が先に終了．前半の残りの処理 */
	for(; i <= t1tmp; i++) {
	  k++;
	  a[k] = A[i];
	}
	break;
      }
    }
  }
}

/* マージソート 
   a[] : 入/出力配列
   n   : aの要素数
*/
void sortM(int s, int t)
{
  int length, s1, s2, t1, t2;
  int i, j;

  length = t - s + 1;

  /* 配列 a の要素が 1 つの場合, 終了 */
  if( length == 1 ) {
    return;
  }

  s1 = s;  t1 = s + (length + 1) / 2 - 1;  /* 前半の長さ >= 後半の長さ */
  s2 = t1 + 1;  t2 = t;

  sortM(s1, t1); /* 前半部分 [s1..t1] のソート */
  sortM(s2, t2); /* 後半部分 [s2..t2] のソート */

  /* [s1..t1] と [s2..t2] をマージして a[s..t] に戻す */
  merge(s1, t1, s2, t2);
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
    fscanf(stdin, "%f", &a[i] );
  }
  
  count = 0;
  /* 配列のソート */
  sortM(0, n - 1);

  /* ソートした結果を出力 */
  for(i = 0; i < n; i++) {
    printf("%f\n", a[i]);
  }
  printf("Number of comparisons = %d\n", count);

  /* メモリ領域の開放 */
  free(a);

  exit(0);
}

