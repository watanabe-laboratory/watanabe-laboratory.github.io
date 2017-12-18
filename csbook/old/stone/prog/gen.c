/* program:  generator
   file:     gen.c (source) --> gen (executable)
   date:     1998/8/28 by O.W.
             修正 1999/10/28

   usage:
   1. gen -1 <n> <s>          超上昇列を用い n 個の重さを生成する．
   2. gen -2 <n> <x> <s>      ランダムに n 個の重さ（x 以下）を生成する．
   3. gen -3 <n> <s>          超上昇列を用い n 個の重さを生成する．
                              超上昇列等の情報も出力する．
   ただし，いずれの場合も乱数のシードとして s を与える．

   scope:  1 <= n <= 30, 1 <= m <= 100,
           s = 自分の学籍番号（98_xxxx-y の xxxx の部分） */
   
# include <stdio.h>

# define MAXSTONES 30
# define TRUE 1
# define FALSE 0
# define DANGEROUSNUM 4630            /* 2 乗すると上限を越えてしまう数 */
# define VERYDANGEROUSNUM 1073741824  /* 2 倍すると上限を越えてしまう数 */
# define MAXNUM 2147483647            /* 整数の上限 */

int easyprime(long p);
long modmult(long u, long v, long p);
long invert(long u, long p);

main(int argc, char *argv[])
{
  long limit, inc[MAXSTONES], weight[MAXSTONES],
       sum, p, mult, invmult, range;
  int num;
  int flg_r, flg_o;
  unsigned seed;

  int i, t, val;

/* コマンドの入力 */
  if((argc == 4) && (argv[1][0] == '-') && (argv[1][1] == '1')) {
    flg_r = FALSE;  flg_o = FALSE;
    num = atoi(argv[2]);
    seed = atoi(argv[3]);
    if(num > MAXSTONES) {
      printf("Usage:\n   gen -1 <n> <s>\n   gen -2 <n> <x> <s>\n   gen -3 <n> <s>\nwhere <n> <= 30.\n");
    exit(0);
    }
  }
  else if((argc == 5) && (argv[1][0] == '-') && (argv[1][1] == '2')) {
    flg_r = TRUE;  flg_o = FALSE;
    num = atoi(argv[2]);
    range = atoi(argv[3]);
    seed = atoi(argv[4]);
    limit = MAXNUM / num;
    if(num > MAXSTONES) {
      printf("Usage:\n   gen -1 <n> <s>\n   gen -2 <n> <x> <s>\n   gen -3 <n> <s>\nwhere <n> <= 30.\n");
    exit(0);
    }
    if(range > limit) {
      printf("The parameter <x> is too large!\n");
      exit(0);
    }
  }
  else if((argc == 4) && (argv[1][0] == '-') && (argv[1][1] == '3')) {
    flg_r = FALSE;  flg_o = TRUE;
    num = atoi(argv[2]);
    seed = atoi(argv[3]);
    if(num > MAXSTONES) {
      printf("Usage:\n   gen -1 <n> <s>\n   gen -2 <n> <x> <s>\n   gen -3 <n> <s>\nwhere <n> <= 30.\n");
    exit(0);
    }
  }
  else {
    printf("Usage:\n   gen -1 <n> <s>\n   gen -2 <n> <x> <s>\n   gen -3 <n> <s>\n");
    exit(0);
  }

/* 重さの作成 */

  if(flg_r) { /* ランダムに作成する場合 */

    srandom(seed);
    for(i = 0; i < num; ++i) {
      val = random() % range;
      printf("%d\n", val);
    }
  }
  else { /* 超上昇列を用いて作成する場合 */

    srandom(seed);
    sum = 1;
    for(i = 0; i < num; ++i) {
      val = random() % 5;
      inc[i] = sum + val;
      sum = sum + inc[i];
    }

    /* 法（素数 p）とかける数の選択 */

    p = sum * 2 + 1;
    val = random() % 1000;
    p = p + val;
    if(p % 2 == 0) p = p + 1;
    while(!easyprime(p)) p = p + 2;

    mult = random();
    invmult = invert(mult, p);

    /* 重さの計算 */

    for(i = 0; i < num; ++i)
      weight[i] = modmult(inc[i], mult, p);

    if(flg_o) { /* 重さと諸データの出力 */
      for(i = 0; i < num; ++i) printf("%d\n", weight[i]);
      printf("\n重さ作成に関する緒データ\n");
      printf("   超上昇列 = ");
      for(i = 0; i < num; ++i) printf("%d, ", inc[i]);
      printf("\n");
      printf("   秘密データ P = %d, M = %d, Y = %d\n",
	     p, mult, invmult);
    }
    else { /* 重のみ出力 */
      for(i = 0; i < num; ++i) printf("%d\n", weight[i]);
    }
  }
}

long modmult(long u, long v, long p)
{
  long ans, tmp;


  if(u < 0) u = u + p;
  if(v < 0) v = v + p;
  u = u % p;  v = v % p;
  if(u > v) {
    tmp = v;  v = u;  u = tmp;
  }
  if(v < DANGEROUSNUM) return (u * v) % p;

  /* u * v が大きくなりそうな場合 */
  ans = 0;
  while(v > 0) {
    while(v % 2 == 0) {
      u = (u + u) % p;
      v = v / 2;
    }
    ans = (ans + u) % p;
    v = v - 1;
  }
  return ans;
}

int easyprime(long p)
{
  long x, y, t, v;

  if(p < 0 || p >= VERYDANGEROUSNUM) {
    printf("Prime p becomes too big.  Try with another random seed.\n");
    exit(0);
  }

  t = 1;  v = p - 1;
  while(v > 1) {
    x = 1;  y = 2;
    do {
      y = modmult(y, y, p);
      x = x * 2;
    } while(x * 2 <= v);
    t = modmult(t, y, p);
    v = v - x;
  }
  if(v == 1) t = modmult(t, 2, p);
  if(t == 1) return 1; else return 0;
}

long invert(long u, long p)
{
  long x0, x1, q, r, a0, a1, a2;

  x0 = p;  x1 = u;  a0 = 0;  a1 = 1;
  q = x0 / x1;  r = x0 % x1;
  while(r != 0) {
    a2 = a0 - q * a1;
    x0 = x1;  x1 = r;
    a0 = a1;  a1 = a2;
    q = x0 / x1;  r = x0 % x1;
  }
  return a2;
}
