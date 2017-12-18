/* program:  �������르�ꥺ��
   file:     stone2t.c (�ץ����) --> stone2t (���ޥ��)
   date:     1999/11/8 by O.W.

   usage:
   stone2t <n> <m> < <file name>

   �׻����֤Τߤ���ϡ�

   ���������ե����� <file name> ����Ȥ�

    1 ���ܤ��ФνŤ�
    2 ���ܤ��ФνŤ�
    3 ���ܤ��ФνŤ�
    ...
    n ���ܤ��ФνŤ�
    �ʶ��ԡ�
    ��ɸ�νŤ���1 ���ܡ�
    ��ɸ�νŤ���2 ���ܡ�
    ...
    ��ɸ�νŤ���m ���ܡ�                                              */


/* ��������ץ���ࡥ���������֤������饢�르�ꥺ��ε��ҡפޤǤϡ�
   ���ޤ��ʤ��Ȼפä��ɤ����Ф��Ƥ褤��                                */

# include <stdio.h>

# define MAXSTONES 30                 /* �Фο��ξ�� */
# define MAXSIZE 32768 + 5            /* ɽ���礭���ξ�� */
# define MAX_HABA 6                   /* ���Ͳ����ɽ�����ˤξ�� */
# define TRUE 1
# define FALSE 0
# define MAXNUM 2147483647            /* �����ξ�� */

int wa_kazu1, wa_kazu2;               /* �¤β�� */
long exp(int x);
long wa1(long a, long b);             /* �¤η׻�: ɽ����Ȥ��� */
long wa2(long a, long b);             /* �¤η׻�: ɽ��õ������Ȥ��� */
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

/* ���ޥ�ɤ����� */
  if(argc == 3) {
    ishi_kazu = atoi(argv[1]);
    mondai_kazu = atoi(argv[2]);
  }
  else {
    printf("Usage:\n   stone2t <n> <k>\n");
    exit(0);
  }

/* �Ť������� */

  for(i = 0; i < ishi_kazu; i++) {
    scanf("%d", &omosa[i + 1]);
  }

/* �������饢�르�ꥺ��ε���: ������ */

  wa_kazu = 0;

  kohan_kazu = ishi_kazu / 2;           /* ��Ⱦ���Фο� */
  zenhan_kazu = ishi_kazu - kohan_kazu; /* ��Ⱦ���Фο� */
  /* ��;�ʳ����ˤ�����+5 �Ϥ��ޤ��ʤ��� */
  jyoyo = exp(kohan_kazu) + 5;

  /* hyo1 �ν����: �Ȥ��ϰϤ� hyo1[0..(jyoyo-1)][0..(MAX_HABA-1)]  */
  for(i = 0; i < jyoyo; i++) {
    for(j = 0; j < MAX_HABA; j++) {
      hyo1[i][j] = -1;
      hyo2[i][j] = -1;
    }
  }

  genkai2 = exp(kohan_kazu);  /* genkai2 = 2 �� kohan_kazu �� */
  for(pattern2 = 0; pattern2 < genkai2; pattern2++) {
    souwa = 0;
    for(i = 1; i <= kohan_kazu; i++) {
        /* pattern2 �� i �ӥå��ܤ� 1 ����Ĵ�٤롥
	   i �ӥå��ܤ� 1 �ξ��ˤϸ�Ⱦ i ���ܤ��Ф����֤��Ȥˤ��� */
      if(bittest(pattern2, i)) {
	souwa = wa1(souwa, omosa[zenhan_kazu + i]);
      }
    }
    /* souwa �˱�����ɽ�ˤ��ޤ� */
    basho = souwa % jyoyo;
    mitsukaranai = TRUE;
    /* hyo1[basho] ����Ƕ����Ƥ�����õ���������˾��������� */
    for(j = 0; j < MAX_HABA; j++) {
      if(hyo2[basho][j] == souwa) {
	/* ���Ǥ� souwa ��Ʊ���ˤʤ��Ȥ߹�碌�����ä��Ȥ���̵�� */
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
    if(mitsukaranai) { /* �����Ƥ����꤬�ʤ��ä���� */
      printf("Too many collisions!\n");
      exit(0);
    }
  }

/* �ǥХå��ѽ���: ɽ�ν����� */
/*
for(i = 0; i < jyoyo; i++) {
  for(j = 0; j < MAX_HABA; j++) {
    printf("%d, %d;  ", hyo1[i][j], hyo2[i][j]);
  }
  printf("\n");
}
*/
/* �ǥХå��ѽ��ϡʽ���� */

  for(t = 0; t < mondai_kazu; t++) {
    scanf("%d", &mokuhyo);

/* �����������֡�: Ϳ����줿 mokuhyo �ͤˤʤ������������ */

    genkai1 = exp(zenhan_kazu);  /* genkai1 = 2 �� zenhan_kazu �� */
    for(pattern1 = 0; pattern1 < genkai1; pattern1++) {
      souwa = 0;
      for(i = 1; i <= zenhan_kazu; i++) {
        /* pattern1 �� i �ӥå��ܤ� 1 ����Ĵ�٤롥
	   i �ӥå��ܤ� 1 �ξ��� i ���ܤ��Ф����֤��Ȥˤ��� */
	if(bittest(pattern1, i)) {
	  souwa = wa2(souwa, omosa[i]);
	}
      }
      /* nokori ����롥
         ��Ⱦ�� nokori �ˤʤ�褦�������������뤫��ɽ��Ȥä�Ĵ�٤� */
      nokori = mokuhyo - souwa;
      basho = nokori % jyoyo;
      mitsukatta = FALSE;
      /* hyo2[basho] ����Ǥ��礦�� nokori �ˤʤäƤ����Τ�õ�� */
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
  }

  /* �׻����֤ν��� */
  wa_kazu = wa_kazu1 * mondai_kazu + wa_kazu2;
  printf("%d\n", wa_kazu / mondai_kazu);
  return 0;
}

/* ����Υץ���ෲ�ʥ��֥롼���󷲡� */

long wa1(long a, long b)
/* �Ť����¤�׻��������β����Ͽ����ץ����: ɽ������ */
{
  wa_kazu1++;
  return a + b;
}

long wa2(long a, long b)
/* �Ť����¤�׻��������β����Ͽ����ץ����: ɽõ���� */
{
  wa_kazu2++;
  return a + b;
}

long exp(int x) /* 2 �� x ������ץ���� */
{
  long ans;
  int i;

  ans = 1;
  for(i = 0; i < x; i++) {
    ans = ans * 2;
  }
  return ans;
}

int bittest(long p, int i) /* p �� i �ӥå��ܤ� 1 ���ɤ�����Ĵ�٤�ץ���� */
{
  if((p & (1 << (i - 1))) != 0)
    return TRUE;
  else
    return FALSE;
}
