/* program:  generator
   file:     gen.c (source) --> gen (executable)
   date:     1998/8/28 by O.W.
             ���� 1999/10/28

   usage:
   1. gen -1 <n> <s>          Ķ�徺����Ѥ� n �ĤνŤ����������롥
   2. gen -2 <n> <x> <s>      ������� n �ĤνŤ���x �ʲ��ˤ��������롥
   3. gen -3 <n> <s>          Ķ�徺����Ѥ� n �ĤνŤ����������롥
                              Ķ�徺�����ξ������Ϥ��롥
   ��������������ξ�������Υ����ɤȤ��� s ��Ϳ���롥

   scope:  1 <= n <= 30, 1 <= m <= 100,
           s = ��ʬ�γ����ֹ��98_xxxx-y �� xxxx ����ʬ�� */
   
# include <stdio.h>

# define MAXSTONES 30
# define TRUE 1
# define FALSE 0
# define DANGEROUSNUM 4630            /* 2 �褹��Ⱦ�¤�ۤ��Ƥ��ޤ��� */
# define VERYDANGEROUSNUM 1073741824  /* 2 �ܤ���Ⱦ�¤�ۤ��Ƥ��ޤ��� */
# define MAXNUM 2147483647            /* �����ξ�� */

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

/* ���ޥ�ɤ����� */
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

/* �Ť��κ��� */

  if(flg_r) { /* ������˺��������� */

    srandom(seed);
    for(i = 0; i < num; ++i) {
      val = random() % range;
      printf("%d\n", val);
    }
  }
  else { /* Ķ�徺����Ѥ��ƺ��������� */

    srandom(seed);
    sum = 1;
    for(i = 0; i < num; ++i) {
      val = random() % 5;
      inc[i] = sum + val;
      sum = sum + inc[i];
    }

    /* ˡ���ǿ� p�ˤȤ������������ */

    p = sum * 2 + 1;
    val = random() % 1000;
    p = p + val;
    if(p % 2 == 0) p = p + 1;
    while(!easyprime(p)) p = p + 2;

    mult = random();
    invmult = invert(mult, p);

    /* �Ť��η׻� */

    for(i = 0; i < num; ++i)
      weight[i] = modmult(inc[i], mult, p);

    if(flg_o) { /* �Ť��Ƚ��ǡ����ν��� */
      for(i = 0; i < num; ++i) printf("%d\n", weight[i]);
      printf("\n�Ť������˴ؤ����ǡ���\n");
      printf("   Ķ�徺�� = ");
      for(i = 0; i < num; ++i) printf("%d, ", inc[i]);
      printf("\n");
      printf("   ��̩�ǡ��� P = %d, M = %d, Y = %d\n",
	     p, mult, invmult);
    }
    else { /* �ŤΤ߽��� */
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

  /* u * v ���礭���ʤꤽ���ʾ�� */
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
