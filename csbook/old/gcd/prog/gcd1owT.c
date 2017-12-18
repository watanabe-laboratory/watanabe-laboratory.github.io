/* program:  �������������ץ����ʥ���ߥĥ֥�ˡ���ɷ���
   file:     gcd1ow.c�ʥץ�����--> gcd1ow�ʥ��ޥ�ɡ�
   date:     2000/10/2 by K.H

   usage:
   gcd1 <x> <y>
             x �� y �κ������������Ȥ��˻��Ѥ��� mod �β��   */

#include <stdio.h>
#include <stdlib.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int x, y;     /*  ���� */
  int steps;    /* ��;�黻�β��*/
  int k, N;
  int ans;
  
  /* ����� */
  x = atoi(argv[1]);   /* �������� x ������ */
  y = atoi(argv[2]);   /* ������� �� y ������ */
  steps = 0;

  N = min(x,y);
  
  for(k = N; k >= 1; k--){
    steps = steps + 2; /* ���ιԤν�;�黻�򥫥���� */
    if ((x % k == 0) && (y % k == 0)){
      ans = k;
      break;
    }
  }

  /* �ʲ����
  printf("GCD of %s and %s is %d \n", argv[1],argv[2], ans);
  printf("%d steps \n", steps);
  �����ޤǺ�� */

  printf("%d\n", steps);
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

