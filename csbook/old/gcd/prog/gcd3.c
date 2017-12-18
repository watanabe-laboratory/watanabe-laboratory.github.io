/* program:  �������������ץ����
             �ʱ�����̾���á� ¼�塤���귯�Υ����ǥ���
   file:     gcd3.c�ʥץ�����--> gcd3�ʥ��ޥ�ɡ�
   date:     Implemented on 2000/10/2 by O.W

   usage:
   gcd1 <x> <y>
             x �� y �κ������������Ѥ��� mod �β�������   */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int x, y;     /*  ���� */
  int steps;    /* ��;�黻�β��*/
  int k, d, N, K;
  int ans;
  
  /* ����� */
  x = atoi(argv[1]);   /* �������� x ������ */
  y = atoi(argv[2]);   /* ������� �� y ������ */
  steps = 0;

  N = min(x,y);
  K = sqrt(N) + 1;

  for(k = 1; k <= K; k++){
    steps = steps + 1; /* ���ιԤν�;�黻�򥫥���� */
    if (x % k == 0) {
      steps = steps + 1; /* ���ιԤν�;�黻�򥫥���� */
      d = x / k;
      steps = steps + 1; /* ���ιԤν�;�黻�򥫥���� */
      if (y % d == 0) {
	ans = d;
	break;
      }
      steps = steps + 1; /* ���ιԤν�;�黻�򥫥���� */
      if (y % k == 0) {
	ans = k;
      }
    }
  }

  printf("GCD of %d and %d is %d \n", x,y, ans);
  printf("%d steps \n", steps);
  exit(0);
}
      
int min(x,y)
     int x,y;
{
  if(x <= y){
    return(x);
  }
  else{
    return(y);
  }
}



