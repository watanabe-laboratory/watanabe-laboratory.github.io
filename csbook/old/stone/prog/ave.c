/* program:    ave
   file:       ave.c (source) --> ave (executable)
   date:       1999/11/9

   usage:
   ave <n> <k> < <file name>
   ���������ե����� <file name> ����Ȥ� n �Ĥο��� k �ȡ�

       1
       5
       18
       34
       .
       .
       1234
       145749
       1111900  <-- n �Ĥο�
       1
       5
       .
       .
       234567   <-- n �Ĥο�
       ...
       �ʾ夬 k ��

   ����ȡ��б����� k �Ȥ�ʿ�Ѥ� n �Ľ��Ϥ���롥*/

# include <stdio.h>

# define MAXSTONES 30
# define MAXKUMI 20

main(int argc, char *argv[])
{
  long data[MAXSTONES][MAXKUMI], total, average;
  int n, k;

  int i, j;

/* ���ޥ�ɤ����� */
  if(argc == 3) {
    n = atoi(argv[1]);
    k = atoi(argv[2]);
  }
  else {
    printf("Usage:\n   ave <n> <k>\n");
    exit(0);
  }

  /* �ǡ��������� */
  for(j = 0; j < k; j++) {
    for(i = 0; i < n; i++) {
      scanf("%d", &data[i][j]);
    }
  }

  /* ʿ�Ѥη׻� */
  for(i = 0; i < n; i++) {
    total = 0;
    for(j = 0; j < k; j++) {
      total = total + data[i][j];
    }
    average = total / k;
    printf("%d\n", average);
  }
}
