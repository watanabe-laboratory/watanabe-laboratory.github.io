/* program:  単純なアルゴリズム
   file:     stone1.c (プログラム) --> stone1 (コマンド)
   date:     1998/9/1 by O.W.

   usage:
   stone1 <n> <k> < <file name>
   ただし，ファイル <file name> の中身は

    1 番目の石の重さ
    2 番目の石の重さ
    3 番目の石の重さ
    ...
    n 番目の石の重さ
    （空行）
    目標の重さ（1 個目）
    目標の重さ（2 個目）
    ...
    目標の重さ（k 個目）                                              */


/* ここからプログラム．ただし，「ここからアルゴリズムの記述」までは，
   おまじないと思って読み飛ばしてよい．                                */

# include <stdio.h>

# define MAXSTONES 30                 /* 石の数の上限 */
# define TRUE 1
# define FALSE 0
# define MAXNUM 2147483647            /* 整数の上限 */

int wa_kazu;
long exp(int x);
long wa(long a, long b);
int bittest(long p, int i);

main(int argc, char *argv[])
{
  long omosa[MAXSTONES], mokuhyo, souwa, genkai;
  long pattern;
  int ishi_kazu, mondai_kazu;
  int i, t;

/* コマンドの入力 */
  if(argc == 3) {
    ishi_kazu = atoi(argv[1]);
    mondai_kazu = atoi(argv[2]);
  }
  else {
    printf("Usage:\n   stone1 <n> <k>\n");
    exit(0);
  }

/* 重さの入力 */

  printf("*** 石選び問題 ***\n\n");
  printf("   石の数は %d, 問題例の数は = %d\n",
	 ishi_kazu, mondai_kazu);
  printf("   石の重さ:");

  for(i = 0; i < ishi_kazu; i++) {
    if(i % 5 == 0) printf("\n     ");
    scanf("%d", &omosa[i + 1]);
    printf("%d, ", omosa[i + 1]);
  }
  printf("\n   -----\n");

  wa_kazu = 0;
  for(t = 0; t < mondai_kazu; t++) {
    scanf("%d", &mokuhyo);
    printf("   目標の重さ = %d\n", mokuhyo);

/* ここからアルゴリズムの記述: mokuhyo 値になる選び方を求める */

    genkai = exp(ishi_kazu);  /* genkai = 2 の ishi_kazu 乗 */
    for(pattern = 0; pattern < genkai; pattern++) {
      souwa = 0;
      for(i = 1; i <= ishi_kazu; i++) {
        /* pattern の i ビット目が 1 かを調べる．
	   i ビット目が 1 の場合に i 番目の石を選ぶことにする */
	if(bittest(pattern, i)) {
	  souwa = wa(souwa, omosa[i]);
	}
      }
      /* souwa が mokuhyou 値になったかを調べ，その場合は直ちにやめる */
      if(souwa == mokuhyo) break;
    }

    if (souwa == mokuhyo) { /* souwa が mokuhyou 値になっている場合 */
      printf("   答え: ");
      /* pattern で規定された選び方を答えとして出力する */
      for(i = 1; i <= ishi_kazu; i++) {
        /* pattern の i ビット目が 1 かを調べる．
	   i ビット目が 1 の場合に i 番目の石が選ばれている */
	if(bittest(pattern, i)) {
	  printf("%d, ", i);
	}
      }
      printf("\n");
    }
    else { /* souwa が mokuhyou 値になっていない場合: 答えがなかった場合 */
      printf("   解なし\n");
    }
  }

  /* 計算時間の出力 */
  printf("   -----\n");
  printf("   計算時間（加算回数）: 総数 %d 回, 平均 %d 回\n",
	 wa_kazu, wa_kazu / mondai_kazu);
  return 0;
}

/* 補助のプログラム群（サブルーチン群） */

long wa(long a, long b) /* 重さの和を計算し，その回数を記録するプログラム */
{
  wa_kazu++;
  return a + b;
}

long exp(int x) /* 2 の x 乗を求めるプログラム */
{
  long ans;
  int i;

  ans = 1;
  for(i = 0; i < x; i++) {
    ans = ans * 2;
  }
  return ans;
}

int bittest(long p, int i) /* p の i ビット目が 1 かどうかを調べるプログラム */
{
  if((p & (1 << (i - 1))) != 0)
    return TRUE;
  else
    return FALSE;
}
