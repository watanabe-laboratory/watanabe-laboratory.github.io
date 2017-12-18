/* program:  fire2.rb と同等のプログラム
   file:     fire2.c
   date:     2012/12/19 by O.W

   usage:
   fire2.exe <n> <pint> <bh> <th> <seed>
       pint = prob. * 100 (integer)
       n = size, bh = burnout time, th = simulation time, seed = random seed
*/

#include <stdio.h>
#include "mt.h"
#define N0 200
#define N2 2*N0 + 1
#define SIZE N2 * N2

int main(int argc, char *argv[]) {
  /************/
  /* 変数定義 */
  /************/
  int f[N2+1][N2+1], fnext[N2+1][N2+1];
  int n0, n2, size, ntree, pint, bh, th;
  float p;
  int nubtree, nbtree;
  int newflag, endflag;
  int i, j, di, dj, ii, jj, h;
  int seed;

  /* 変数初期設定 */
  n0 = atoi(argv[1]);
     n2 = 2*n0 + 2;  size = n2 - 1;
     ntree = size * size;
  pint = atoi(argv[2]);  p = (float)pint / 100.0;
  bh   = atoi(argv[3]);
  th   = atoi(argv[4]);
  seed = atoi(argv[5]);  sgenrand(seed);
  /*
  printf("size = %d (# of trees = %d)\n", size, ntree);
  printf("prob. = %f, burn out time = %d, total sim. time = %d, seed = %d\n",
          p, bh, th, seed);
  */

  /* 森林状態保持用 */
  for(i = 0; i <= n2; i++)
    for(j = 0; j <= n2; j++)
      f[i][j] = -1;
  for(i = 0; i <= n2; i++) {
    f[i][0] = f[i][n2] = 0;
    f[0][i] = f[n2][i] = 0;
    fnext[i][0] = fnext[i][n2] = 0;
    fnext[0][i] = fnext[n2][i] = 0;
  }

  /************************/
  /* シミュレーション開始 */
  /************************/
  f[n0+1][n0+1] = 1;
  nbtree = 1;
  h = 0;

  while(1) {
    newflag = 0;
    for(i = 0; i <= n2; i++)
      for(j = 0; j <= n2; j++) {
	if(f[i][j] >= bh) fnext[i][j] = 0;
	else {
	  if(f[i][j] > 0) fnext[i][j] = f[i][j] + 1;
	  else {
	    if(f[i][j] == 0) fnext[i][j] = 0;
	    else {
	      /* まだ木が燃えていない場合 */
	      fnext[i][j] = -1;
	      endflag = 0;
	      for(di = -1; di <= +1; di++) {
		for(dj = -1; dj <= +1; dj++) {
		  ii = i + di;  jj = j + dj;
		  if((f[ii][jj] > 0) && ((genrand() * 100) < pint)) {
	                                      /* ↑確率 p で燃え移る */
		    fnext[i][j] = 1;
		    endflag = 1;
		    break;
		  }
		}
		if(endflag) break;
	      }
	    }
	  }
	}
      }

    /* 結果更新 */
    h = h + 1;
    nubtree = nbtree = 0;
    for(i = 1; i <= n2 - 1; i++)
      for(j = 1; j <= n2 - 1; j++) {
	f[i][j] = fnext[i][j];
	if(f[i][j] > 0) {
	  nbtree = nbtree + 1;  newflag = 1;
	}
	else {
	  if(f[i][j] == 0) nbtree = nbtree + 1;
	  else { /* if(f[i][j] == -1) */
	    nubtree = nubtree + 1;
	  }
	}
      }

    /* 途中結果出力
    printf("time = %d: nub.tree = %d, n.tree = %d\n",
           h, nbtree, nubtree);
    */

    /* 終了条件判定 */
    if(!newflag || (nubtree == 0) || (h >= th)) break;
  }
  /* 最終結果出力 */
  printf("%d,%d,%d\n", h, nbtree, nubtree);
}
