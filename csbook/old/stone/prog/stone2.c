/* program:  賢いアルゴリズム
   file:     stone2.c (プログラム) --> stone2 (コマンド)
   date:     1998/9/1 by O.W.，
             修正 1998/11/19, 1999/11/8

   usage:
   stone2 <n> <m> < <file name>
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
    目標の重さ（m 個目）                                              */


/* ここからプログラム．ただし，「ここからアルゴリズムの記述」までは，
   おまじないと思って読み飛ばしてよい．                                */

# include <stdio.h>

# define MAXSTONES 30                 /* 石の数の上限 */
# define MAXSIZE 32768 + 5            /* 表の大きさの上限 */
# define MAX_HABA 6                   /* 衝突回数（表の幅）の上限 */
# define TRUE 1
# define FALSE 0
# define MAXNUM 2147483647            /* 整数の上限 */

int wa_kazu1, wa_kazu2;               /* 和の回数 */
long exp(int x);
long wa1(long a, long b);             /* 和の計算: 表を作るとき用 */
long wa2(long a, long b);             /* 和の計算: 表を探索するとき用 */
int bittest(long p, int i);

main(int argc, char *argv[])
{
  long omosa[MAXSTONES], mokuhyo, souwa, nokori, genkai1, genkai2;
  long pattern1, pattern2;
  long hyo1[MAXSIZE][MAX_HABA], hyo2[MAXSIZE][MAX_HABA];
  int ishi_kazu, mondai_kazu, zenhan_kazu, kohan_kazu, jyoyo, basho;
  int wa_kazu;
  int mitsukatta, mitsukaranai;
  int i, j, t;

/* コマンドの入力 */
  if(argc == 3) {
    ishi_kazu = atoi(argv[1]);
    mondai_kazu = atoi(argv[2]);
  }
  else {
    printf("Usage:\n   stone2 <n> <k>\n");
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

/* ここからアルゴリズムの記述: 前処理 */

  wa_kazu = 0;

  kohan_kazu = ishi_kazu / 2;           /* 後半の石の数 */
  zenhan_kazu = ishi_kazu - kohan_kazu; /* 前半の石の数 */
  /* 剰余（割る数）を決める（+5 はおまじない） */
  jyoyo = exp(kohan_kazu) + 5;

  /* hyo1 の初期化: 使う範囲は hyo1[0..(jyoyo-1)][0..(MAX_HABA-1)]  */
  for(i = 0; i < jyoyo; i++) {
    for(j = 0; j < MAX_HABA; j++) {
      hyo1[i][j] = -1;
      hyo2[i][j] = -1;
    }
  }

  genkai2 = exp(kohan_kazu);  /* genkai2 = 2 の kohan_kazu 乗 */
  for(pattern2 = 0; pattern2 < genkai2; pattern2++) {
    souwa = 0;
    for(i = 1; i <= kohan_kazu; i++) {
        /* pattern2 の i ビット目が 1 かを調べる．
	   i ビット目が 1 の場合には後半 i 番目の石を選ぶことにする */
      if(bittest(pattern2, i)) {
	souwa = wa1(souwa, omosa[zenhan_kazu + i]);
      }
    }
    /* souwa に応じて表にしまう */
    basho = souwa % jyoyo;
    mitsukaranai = TRUE;
    /* hyo1[basho] の中で空いている所を探し，そこに情報を入れる */
    for(j = 0; j < MAX_HABA; j++) {
      if(hyo2[basho][j] == souwa) {
	/* すでに souwa と同じになる組み合わせがあったときは無視 */
	mitsukaranai = FALSE;
	break;
      }
      if(hyo1[basho][j] == -1) {
	hyo1[basho][j] = pattern2;
	hyo2[basho][j] = souwa;
	mitsukaranai = FALSE;
	break;
      }
    }
    if(mitsukaranai) { /* 空いている場所がなかった場合 */
      printf("Too many collisions!\n");
      exit(0);
    }
  }

/* デバック用出力: 表の出来具合 */
/*
for(i = 0; i < jyoyo; i++) {
  for(j = 0; j < MAX_HABA; j++) {
    printf("%d, %d;  ", hyo1[i][j], hyo2[i][j]);
  }
  printf("\n");
}
*/
/* デバック用出力（終り） */

  for(t = 0; t < mondai_kazu; t++) {
    scanf("%d", &mokuhyo);
    printf("   目標の重さ = %d\n", mokuhyo);

/* ここから本番！: 与えられた mokuhyo 値になる選び方を求める */

    genkai1 = exp(zenhan_kazu);  /* genkai1 = 2 の zenhan_kazu 乗 */
    for(pattern1 = 0; pattern1 < genkai1; pattern1++) {
      souwa = 0;
      for(i = 1; i <= zenhan_kazu; i++) {
        /* pattern1 の i ビット目が 1 かを調べる．
	   i ビット目が 1 の場合に i 番目の石を選ぶことにする */
	if(bittest(pattern1, i)) {
	  souwa = wa2(souwa, omosa[i]);
	}
      }
      /* nokori を求める．
         後半で nokori になるような選び方があるかを表を使って調べる */
      nokori = mokuhyo - souwa;
      basho = nokori % jyoyo;
      mitsukatta = FALSE;
      /* hyo2[basho] の中でちょうど nokori になっているものを探す */
      for(j = 0; j < MAX_HABA; j++) {
	if(hyo1[basho][j] == -1) break;
	if(hyo2[basho][j] == nokori) {
	  pattern2 = hyo1[basho][j];
	  mitsukatta = TRUE;
	  break;
	}
      }

      if(mitsukatta) break;
    }
    /* 答えの出力 */
    if(mitsukatta) { /* 見つかった場合 */
      printf("   答え: ");
      /* pattern1, pattern2 で規定された選び方を答えとして出力する */
      for(i = 1; i <= zenhan_kazu; i++) {
	/* pattern1 の i ビット目が 1 かを調べる．
	   i ビット目が 1 の場合に i 番目の石が選ばれている */
	if(bittest(pattern1, i)) {
	  printf("%d, ", i);
	}
      }
      for(i = 1; i <= kohan_kazu; i++) {
	/* pattern2 の i ビット目が 1 かを調べる．
	   i ビット目が 1 の場合に zenhan_kazu + i 番目の石が選ばれている */
	if(bittest(pattern2, i)) {
	  printf("%d, ", zenhan_kazu + i);
	}
      }
      printf("\n");
    }
    else { /* 見つからなかった場合 */
      printf("   解なし\n");
    }
  }

  /* 計算時間の出力 */
  wa_kazu = wa_kazu1 * mondai_kazu + wa_kazu2;
  printf("   -----\n");
  printf("   計算時間（加算回数）: 総数 %d 回, 平均 %d 回\n",
	 wa_kazu, wa_kazu / mondai_kazu);
  return 0;
}

/* 補助のプログラム群（サブルーチン群） */

long wa1(long a, long b)
/* 重さの和を計算し，その回数を記録するプログラム: 表作成用 */
{
  wa_kazu1++;
  return a + b;
}

long wa2(long a, long b)
/* 重さの和を計算し，その回数を記録するプログラム: 表探索用 */
{
  wa_kazu2++;
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
