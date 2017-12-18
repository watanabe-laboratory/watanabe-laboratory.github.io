/* program:  $BM?$($i$l$?J#?t$N?tCM%G!<%?$r>:=g$KJB$SBX$($k!JA*Br%=!<%H!K(B
   file:     ssort.c$B!J%W%m%0%i%`!K(B--> ssort$B!J%3%^%s%I!K(B
   date:     2001/10/16 by Takeru Sasou
*/

#include <stdio.h>

float *a;
int count;

/* $BA*Br%=!<%H$GD9$5(B n $B$NG[Ns(B a $B$r>:=g$K%=!<%H$9$k(B */
void sortS(int n)
{
  float a0;  /* $BG[NsCf$N:G>.CM(B */
  int i0;    /* $B:G>.CM$N%$%s%G%C%/%9(B */
  int i, j;
  float tmp;

  for(i = 0; i < n - 1; i++) {
    a0 = a[i];
    i0 = i;

    /* a[i]..a[n] $B$NCf$N:G>.CM$N>l=j(B i0 $B$r5a$a$k(B */
    for(j = i + 1; j < n; j++) {
      count++;
      if(a[j] < a0) {
	a0 = a[j];
	i0 = j;
      }
    }

    /* a[i] $B$H(B a[i0] $B$NCM$r8r49(B */
    tmp = a[i];
    a[i] = a[i0];
    a[i0] = tmp;
  }
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
    fscanf(stdin, "%f", &a[i]);
  }

  count = 0;
  /* $BG[Ns$N%=!<%H(B */
  sortS(n);

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
