/* program:  �������������ץ����ʥ���ߥĥ֥�ˡ��
   file:     gcd1.c�ʥץ�����--> gcd1�ʥ��ޥ�ɡ�
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
  int x, y;     /*  ���� */
  int steps;    /* ��;�黻�β��*/
  int k, N;
  int ans;
  
  /* ����� */
  x = atoi(argv[1]);   /* �������� x ������ */
  y = atoi(argv[2]);   /* ������� �� y ������ */
  steps = 0;

  N = min(x,y);
  
  for(k = 1; k <= N; k++){
    steps = steps + 2; /* ���ιԤν�;�黻�򥫥���� */
    if ((x % k == 0) && (y % k == 0)){
      ans = k; 
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

