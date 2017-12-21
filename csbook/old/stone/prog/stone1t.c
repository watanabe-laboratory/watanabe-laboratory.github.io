/* program:  単純なアルゴリズム
   file:     stone1t.c (プログラム) --> stone1t (コマンド)
   date:     1999/11/8 by O.W.

   usage:
   stone1t <n> <k> < <file name>

   平均の計算時間だけが出力される．

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
    printf("Usage:\n   stone1t <n> <k>\n");
    exit(0);
  }

/* 重さの入力 */

  for(i = 0; i < ishi_kazu; i++) {
    scanf("%d", &omosa[i + 1]);
  }

  wa_kazu = 0;
  for(t = 0; t < mondai_kazu; t++) {
    scanf("%d", &mokuhyo);

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
  }

  /* 計算時間の出力 */
  printf("%d\n", wa_kazu / mondai_kazu);
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
