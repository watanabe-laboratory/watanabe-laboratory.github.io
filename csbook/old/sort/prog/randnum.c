/* program:  �ǡ��������Υץ����ʥ������ѡ�
   file:     randnum.c�ʥץ�����--> randnum�ʥ��ޥ�ɡ�
   date:     2001/11/8 by O.W

   usage:
   randnum <a> <b> <seed>
             [a, b] ������� 1 �����������a <= b �� 0 �ʾ�Ȥ����
             seed ������ν����                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int rand_seed, num;
  double a, b, x;

  /* ����� */
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  rand_seed = atoi(argv[3]);
  sgenrand(rand_seed);

  /* ������ʿ������� */
  do {
    x = genrand() * (b + 1 - a);
    num = a + x;
  } while(num > b);
  printf("%d\n", num);

  exit(0);
}
