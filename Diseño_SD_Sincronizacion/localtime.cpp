#include <stdio.h>
#include <time.h>
int main () {
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   printf("Current local time and date: %s", asctime(info));
   return(0);
}