/* program:  $BM?$($i$l$?J#?t$N?tCM%G!<%?$r>:=g$KJB$SBX$($k!J%^!<%8%=!<%H!K(B
   file:     msort.c$B!J%W%m%0%i%`!K(B--> msort$B!J%3%^%s%I!K(B
   date:     2001/10/19 by Takeru Sasou
             2001/10/25 modified by O.W
*/

#include <stdio.h>
#include <float.h>

float *a;
int count;

void merge(int s1, int t1, int s2, int t2)
/* $BG[Ns(B a $B$N(B [s1..t1] $B$H(B [s2..t2] $B$r%^!<%8$7!$(B[s..t] $B$X$7$^$&(B
   $BCm!'(B s = s1, t1 + 1 = s2, t2 = t
        [s1..t1] $B$NJ}$,(B [s2..t2] $B$h$jD9$$(B
*/
{
  int t1tmp = t1 - s1;
  float A[t1tmp + 1];
  int i, j, k;

  /* [s1..t1] $B$r:n6HNN0h$KB`Hr(B */
  for(i = s1; i <= t1; i++) {
    A[i - s1] = a[i];
  }

  i = 0;  j = s2;
  for(k = s1; k <= t2; k++) {
    count++;
    if(A[i] <= a[j]){
      a[k] = A[i];
      i++;
      if(i > t1tmp) {       /* $BA0H>$,@h$K=*N;!%8eH>$N;D$j$N=hM}(B */
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
      if(j > t2) {           /* $B8eH>$,@h$K=*N;!%A0H>$N;D$j$N=hM}(B */
	for(; i <= t1tmp; i++) {
	  k++;
	  a[k] = A[i];
	}
	break;
      }
    }
  }
}

/* $B%^!<%8%=!<%H(B 
   a[] : $BF~(B/$B=PNOG[Ns(B
   n   : a$B$NMWAG?t(B
*/
void sortM(int s, int t)
{
  int length, s1, s2, t1, t2;
  int i, j;

  length = t - s + 1;

  /* $BG[Ns(B a $B$NMWAG$,(B 1 $B$D$N>l9g(B, $B=*N;(B */
  if( length == 1 ) {
    return;
  }

  s1 = s;  t1 = s + (length + 1) / 2 - 1;  /* $BA0H>$ND9$5(B >= $B8eH>$ND9$5(B */
  s2 = t1 + 1;  t2 = t;

  sortM(s1, t1); /* $BA0H>ItJ,(B [s1..t1] $B$N%=!<%H(B */
  sortM(s2, t2); /* $B8eH>ItJ,(B [s2..t2] $B$N%=!<%H(B */

  /* [s1..t1] $B$H(B [s2..t2] $B$r%^!<%8$7$F(B a[s..t] $B$KLa$9(B */
  merge(s1, t1, s2, t2);
}

void main(int argc, char *argv[])
{
  int i, n;

  /* $B?tCMG[Ns$NMWAG?t$r<hF@(B */
  n = atoi(argv[1]);

  /* $BG[Ns%G!<%?MQ$N%a%b%jNN0h$N3d$jEv$F(B */
  a = (float *)malloc(sizeof( float ) * n);

  /* $BG[Ns$NCM$rFI$_9~$`(B */
  for(i = 0; i < n; i++) {
    fscanf(stdin, "%f", &a[i] );
  }
  
  count = 0;
  /* $BG[Ns$N%=!<%H(B */
  sortM(0, n - 1);

  /* $B0J2<(B $B>JN,(B
  $B%=!<%H$7$?7k2L$r=PNO(B
  for(i = 0; i < n; i++){
    printf("%f\n", a[i]);
  }
  printf("Number of comparisons = %d\n", count);
  $B$3$3$^$G>JN,(B */

  printf("%d\n", count);

  /* $B%a%b%jNN0h$N3+J|(B */
  free(a);

  exit(0);
}

