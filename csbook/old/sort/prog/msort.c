/* program:  Ϳ����줿ʣ���ο��ͥǡ����򾺽���¤��ؤ���ʥޡ��������ȡ�
   file:     msort.c�ʥץ�����--> msort�ʥ��ޥ�ɡ�
   date:     2001/10/19 by Takeru Sasou
             2001/10/25 modified by O.W
*/

#include <stdio.h>
#include <float.h>

float *a;
int count;

void merge(int s1, int t1, int s2, int t2)
/* ���� a �� [s1..t1] �� [s2..t2] ��ޡ�������[s..t] �ؤ��ޤ�
   �� s = s1, t1 + 1 = s2, t2 = t
        [s1..t1] ������ [s2..t2] ���Ĺ��
*/
{
  int t1tmp = t1 - s1;
  float A[t1tmp + 1];
  int i, j, k;

  /* [s1..t1] �����ΰ������ */
  for(i = s1; i <= t1; i++) {
    A[i - s1] = a[i];
  }

  i = 0;  j = s2;
  for(k = s1; k <= t2; k++) {
    count++;
    if(A[i] <= a[j]){
      a[k] = A[i];
      i++;
      if(i > t1tmp) {       /* ��Ⱦ����˽�λ����Ⱦ�λĤ�ν��� */
	for(; j <= t2; j++) {
	  k++;
	  a[k] = a[j];
	}
	break;
      }
    }
    else{
      a[k] = a[j];
      j++;
      if(j > t2) {           /* ��Ⱦ����˽�λ����Ⱦ�λĤ�ν��� */
	for(; i <= t1tmp; i++) {
	  k++;
	  a[k] = A[i];
	}
	break;
      }
    }
  }
}

/* �ޡ��������� 
   a[] : ��/��������
   n   : a�����ǿ�
*/
void sortM(int s, int t)
{
  int length, s1, s2, t1, t2;
  int i, j;

  length = t - s + 1;

  /* ���� a �����Ǥ� 1 �Ĥξ��, ��λ */
  if( length == 1 ) {
    return;
  }

  s1 = s;  t1 = s + (length + 1) / 2 - 1;  /* ��Ⱦ��Ĺ�� >= ��Ⱦ��Ĺ�� */
  s2 = t1 + 1;  t2 = t;

  sortM(s1, t1); /* ��Ⱦ��ʬ [s1..t1] �Υ����� */
  sortM(s2, t2); /* ��Ⱦ��ʬ [s2..t2] �Υ����� */

  /* [s1..t1] �� [s2..t2] ��ޡ������� a[s..t] ���᤹ */
  merge(s1, t1, s2, t2);
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
    fscanf(stdin, "%f", &a[i] );
  }
  
  count = 0;
  /* ����Υ����� */
  sortM(0, n - 1);

  /* �����Ȥ�����̤���� */
  for(i = 0; i < n; i++) {
    printf("%f\n", a[i]);
  }
  printf("Number of comparisons = %d\n", count);

  /* �����ΰ�γ��� */
  free(a);

  exit(0);
}

