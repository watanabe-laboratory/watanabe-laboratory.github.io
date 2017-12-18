/* program:  �ǡ��������Υץ����ʥ������ѡ�
   file:     rands.c�ʥץ�����--> rands�ʥ��ޥ�ɡ�
   date:     2001/11/8 by O.W

   usage:
   rands <a> <b> <seed> <n>
             [a, b] ������� n �ĵ����a <= b �� 0 �ʾ�Ȥ����
             seed ������ν����                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int rand_seed, n;
  double a, b, x;
  int i;

  /* ����� */
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  rand_seed = atoi(argv[3]);
  n = atoi(argv[4]);
  sgenrand(rand_seed);

  /* ������ʿ������� */
  for(i = 1; i <= n; i++) {
    x = a + genrand() * (b - a);
    printf("%f\n", x);
  }

  exit(0);
}
