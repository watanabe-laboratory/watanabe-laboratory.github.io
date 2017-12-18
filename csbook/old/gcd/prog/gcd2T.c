/* program:  �������������ץ����ʥ桼����åɤθ߽�ˡ��
   file:     gcd2.c�ʥץ�����--> gcd2�ʥ��ޥ�ɡ�
   date:     2000/10/2 by K.H

   usage:
   gcd1 <x> <y>
             x �� y �κ������������Ѥ��� mod �β�������   */

#include <stdio.h>
#include <stdlib.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int x, y;    /*  ���ϤΤ������줾���礭��,�������� */
  int steps;   /* ��;�黻�β��*/
  int k, r;
  int ans;
  
  /* ����� */
  x = atoi(argv[1]);   /* �������� x ������ */
  y = atoi(argv[2]);   /* ������� �� y ������ */
  steps = 0;

  steps = steps + 1;
  r = x % y;
  
  while(r > 0) {
    x = y;
    y = r;
    steps = steps + 1;
    r = x % y;
  }

  ans = y;

  /* �ʲ����
  printf("GCD of %s and %s is %d \n", argv[1],argv[2], ans);
  printf("%d steps \n", steps);
  �����ޤǺ�� */

  printf("%d\n", steps);
  exit(0);
}
