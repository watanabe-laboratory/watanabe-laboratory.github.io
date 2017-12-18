/* program:    sum
   file:       sumall.c (source) --> sumall (executable)
   date:       1998/11/12 by O.W.
               1999/11/8 $B6u9T$,F~$k$h$&$K$7$?(B

   usage:
   (1) sumall <n> $B$r<B9T!%(B
       $B$?$@$7!$(Bn: $B@P$N?t(B
   (2) $B<!$NNc$N$h$&$K(B n $B8D$N@P$N=E$5$rF~NO$9$k!%(B

       345732
       1234
       1457
       1111900
       .
       .
       234567 <-- n $B8DL\(B
   (3) $B$9$k$H!$$=$N9g7W$N=E$5$,=PNO$5$l$k!%(B   */

#include <stdio.h>

#define MAXSTONES 30
# define TRUE 1
# define FALSE 0

main(int argc, char *argv[])
{
  long weight, sum;
  int n;

  int i;

/* $B%3%^%s%I$NF~NO(B */
  if(argc == 2) {
    n = atoi(argv[1]);
  }
  else {
    printf("Usage:\n   sum <n>\n");
    exit(0);
  }

  printf("  \n"); /* $B6u9T$N=PNO(B */

  /* $B@P$N=E$5$NF~NO(B */
  sum = 0;
  for(i = 0; i < n; i++) {
    scanf("%d", &weight);
    sum = sum + weight;
  }
  printf("%d\n", sum);
}
