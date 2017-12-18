/* program:  Ϳ����줿ʣ���ο��ͥǡ����򾺽���¤��ؤ�������򥽡��ȡ�
   file:     ssort.c�ʥץ�����--> ssort�ʥ��ޥ�ɡ�
   date:     2001/10/16 by Takeru Sasou
*/

#include <stdio.h>

float *a;
int count;

/* ���򥽡��Ȥ�Ĺ�� n ������ a �򾺽�˥����Ȥ��� */
void sortS(int n)
{
  float a0;  /* ������κǾ��� */
  int i0;    /* �Ǿ��ͤΥ���ǥå��� */
  int i, j;
  float tmp;

  for(i = 0; i < n - 1; i++) {
    a0 = a[i];
    i0 = i;

    /* a[i]..a[n] ����κǾ��ͤξ�� i0 ����� */
    for(j = i + 1; j < n; j++) {
      count++;
      if(a[j] < a0) {
	a0 = a[j];
	i0 = j;
      }
    }

    /* a[i] �� a[i0] ���ͤ�� */
    tmp = a[i];
    a[i] = a[i0];
    a[i0] = tmp;
  }
}

void main(int argc, char *argv[])
{
  int i, n;

  /* ������������ǿ������ */
  n = atoi(argv[1]);

  /* ����ǡ����ѤΥ����ΰ�γ������ */
  a = (float *)malloc(sizeof( float ) * n);

  /* ������ͤ��ɤ߹��� */
  for(i = 0; i < n; i++) {
    fscanf(stdin, "%f", &a[i]);
  }

  count = 0;
  /* ����Υ����� */
  sortS(n);

  /* �����Ȥ�����̤���� */
  for(i = 0; i < n; i++) {
    printf("%f\n", a[i]);
  }
  printf("Number of comparisons = %d\n", count);

  /* �����ΰ�γ��� */
  free(a);

  exit(0);
}
