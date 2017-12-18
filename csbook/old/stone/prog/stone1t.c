/* program:  ñ��ʥ��르�ꥺ��
   file:     stone1t.c (�ץ����) --> stone1t (���ޥ��)
   date:     1999/11/8 by O.W.

   usage:
   stone1t <n> <k> < <file name>

   ʿ�Ѥη׻����֤��������Ϥ���롥

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
    ��ɸ�νŤ���k ���ܡ�                                              */

/* ��������ץ���ࡥ���������֤������饢�르�ꥺ��ε��ҡפޤǤϡ�
   ���ޤ��ʤ��Ȼפä��ɤ����Ф��Ƥ褤��                                */

# include <stdio.h>

# define MAXSTONES 30                 /* �Фο��ξ�� */
# define TRUE 1
# define FALSE 0
# define MAXNUM 2147483647            /* �����ξ�� */

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

/* ���ޥ�ɤ����� */
  if(argc == 3) {
    ishi_kazu = atoi(argv[1]);
    mondai_kazu = atoi(argv[2]);
  }
  else {
    printf("Usage:\n   stone1t <n> <k>\n");
    exit(0);
  }

/* �Ť������� */

  for(i = 0; i < ishi_kazu; i++) {
    scanf("%d", &omosa[i + 1]);
  }

  wa_kazu = 0;
  for(t = 0; t < mondai_kazu; t++) {
    scanf("%d", &mokuhyo);

/* �������饢�르�ꥺ��ε���: mokuhyo �ͤˤʤ������������ */

    genkai = exp(ishi_kazu);  /* genkai = 2 �� ishi_kazu �� */
    for(pattern = 0; pattern < genkai; pattern++) {
      souwa = 0;
      for(i = 1; i <= ishi_kazu; i++) {
        /* pattern �� i �ӥå��ܤ� 1 ����Ĵ�٤롥
	   i �ӥå��ܤ� 1 �ξ��� i ���ܤ��Ф����֤��Ȥˤ��� */
	if(bittest(pattern, i)) {
	  souwa = wa(souwa, omosa[i]);
	}
      }
      /* souwa �� mokuhyou �ͤˤʤä�����Ĵ�١����ξ���ľ���ˤ��� */
      if(souwa == mokuhyo) break;
    }
  }

  /* �׻����֤ν��� */
  printf("%d\n", wa_kazu / mondai_kazu);
  return 0;
}

/* ����Υץ���ෲ�ʥ��֥롼���󷲡� */

long wa(long a, long b) /* �Ť����¤�׻��������β����Ͽ����ץ���� */
{
  wa_kazu++;
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
