/* program:  �ǡ��������Υץ����
   file:     genr.c�ʥץ�����--> genr�ʥ��ޥ�ɡ�
   date:     2000/10/2 by K.H

   usage:
   genfib <t>
             t, t+1 ���ܤΥե��ܥʥå�������롥
             ���Σ��ʷ�����֤�                                  */

#include <stdio.h>

int main(argc,argv)
     int argc;
     int *argv[];
{
  int i, j, k;
  int t;

  t = atoi(argv[1]); /* ������ t ������ */
  i = 1;
  j = 0;
  
  for(k = 1; k <= t; k++){
    j = i + j;
    i = j - i;
  }
  
  printf("%d\t",j); /* t ���ܤΥե��ܥʥå������֤� */
  
  j = i + j;     /* t+1 ���ܤΥե��ܥʥå��� */
  i = j - i;     /* t ���ܤΥե��ܥʥå��� */

  printf("%d\t",j); /* t+1 ���ܤΥե��ܥʥå������֤� */
  
  printf("%d\t", getbinlen(i)); /* t ���ܤΥե��ܥʥå����Σ��ʷ�� */
  printf("%d\n", getbinlen(j)); /* t + 1 ���ܤΥե��ܥʥå����Σ��ʷ�� */
  exit(0);
}

int getbinlen(x)   /* ������ x �Σ���ɽ���ˤ����������֤� */
     int x;
{
  int i,y;
  
  i = 0;
  y = x;

  while ( y != 0) {
    y =(int) y /2;
    i = i + 1;
  }
  
  return(i);

}
  


