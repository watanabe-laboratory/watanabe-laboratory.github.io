/* program:  �ǡ��������Υץ����ʥ������ѡ�
   file:     randfloat.c�ʥץ�����--> randfloat�ʥ��ޥ�ɡ�
   date:     2001/10/24 by O.W

   usage:
   randfloat <a> <b> <seed>
             [a, b] ������� 1 �ĵ����a <= b �� 0 �ʾ�Ȥ����
             seed ������ν����                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int rand_seed;
  double a, b, x;
  int i;

  /* ����� */
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  rand_seed = atoi(argv[3]);
  sgenrand(rand_seed);

  /* ������ʿ������� */
  x = a + genrand() * (b - a);
  printf("%f\n", x);
  exit(0);
}
