/* program:  �ǡ��������Υץ����
   file:     gen.c�ʥץ�����--> gen�ʥ��ޥ�ɡ�
   date:     2000/10/2 by K.H and O.W.

   usage:
   gen <n>   ���ʿ��� n ��ο��Τ�������Τ�Τ����
             �Ĥޤꡤ���� = 2**n-1                        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(argc,argv)
     int argc;
     char *argv[];
{
  int n, ans;
  
/* ���ޥ�ɤ����� */
  n = atoi(argv[1]);

/* �׻� */

  ans = power(2, n) - 1;
  printf("%d \n", ans) ;
  exit(0);
}
    
  
int power(x,y)       /* x �� y ���׻� (x ,y �� ������)*/
     int x,y;
{
  int i;
  int ans;
  
  ans = 1;

  for(i = 1;i <= y; i++){
    ans = ans * x;
  }
  return(ans);
}

