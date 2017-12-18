/* program:    sum
   file:       sum.c (source) --> sum (executable)
   date:       1998/8/28 by O.W.
               1999/11/8 $B6u9T$,F~$k$h$&$K$7$?(B

   usage:
   (1) sum <n> <k> <s> $B$r<B9T!%(B
       $B$?$@$7!$(Bn: $B@P$N?t!$(Bk: $BL\I8$N=E$5$N?t(B
               s: $BMp?t$N%7!<%I!J<+J,$N3X@RHV9f!J(B6$B7e!K!K(B
   (2) $B<!$NNc$N$h$&$K(B n $B8D$N@P$N=E$5$rF~NO$9$k!%(B
       345732
       1234
       1457
       1111900
       .
       .
       234567 <-- n $B8DL\(B
   (3) $B$9$k$H!$$=$NCf$+$iE,Ev$KA*$s$G9g7W$7$?=E$5$,(B k $BDL$j!$<!$N(B
       $B$h$&$K=PNO$5$l$k!%(B

       346966
       2691
       .
       .
       82345789 <-- k $B8DL\(B

    $BCm!K(B sum -a <n> <k> <s> $B$H$9$k$H!$Ez$($b0l=o$K=PNO$5$l$k!%(B  */

#include <stdio.h>

#define MAXSTONES 30
# define TRUE 1
# define FALSE 0

main(int argc, char *argv[])
{
  long weight[MAXSTONES];
  long sum;
  int slist[MAXSTONES];
  int n, m;
  unsigned seed;
  int flg;

  int i, t;

/* $B%3%^%s%I$NF~NO(B */
  if(argc == 4) {
    flg = FALSE;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    seed = atoi(argv[3]);
  }
  else if((argc == 5) && (argv[1][0] == '-') && (argv[1][1] == 'a')) {
    flg = TRUE;
    n = atoi(argv[2]);
    m = atoi(argv[3]);
    seed = atoi(argv[4]);
  }
  else {
    printf("Usage:\n   sum <n> <k> <s>\
            \n   sum -a <n> <k> <s>\n");
    exit(0);
  }

  /* $B@P$N=E$5$NF~NO(B */
  for(i = 0; i < n; i++) scanf("%d", &weight[i]);

  srandom(seed);
  printf("  \n"); /* $B6u9T$N=PNO(B */

  for(t = 0; t < m; t++){

    /* $B@PA*$S(B */
    for(i = 0; i < n; i++) slist[i] = random() % 2;
    
    /* $B=E$5$NAmOB$N7W;;$H=PNO(B */
    sum = 0;
    for(i = 0; i < n; i++)
      if(slist[i]) sum = sum + weight[i];
    printf("%d", sum);
    if(flg) {
      printf(": ");
      for(i = 0; i < n; i++) printf("%d,", slist[i]);
    }
    printf("\n");
  }
}
