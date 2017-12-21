/* program:  �ǡ��������Υץ����
   file:     genrand.c�ʥץ�����--> genrand�ʥ��ޥ�ɡ�
   date:     2000/10/2 by K.H, modified by O.W

   usage:
   genrand <n> <seed>
             ���ʤ� n ��ο��������˵��롥
             seed ������ν����                                 */

#include <stdio.h>
#include "mt.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int n;  /* ���ʿ��Ǥη�� */
  int x;
  int rand_seed; 

  /* ����� */
  n = atoi(argv[1]);
  rand_seed = atoi(argv[2]);
  sgenrand(rand_seed);

  /* n ��Υ�����ʼ����������� */
  x = (int)power(2, n - 1)
      + genrand() * (power(2, n) -1 - power(2, n - 1));
  printf("%d\n",x);
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
