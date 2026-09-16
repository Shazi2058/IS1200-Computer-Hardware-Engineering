#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
int counter = 0;

// Abbasi
void print_number(int n){
    printf("%10d ", n);
    counter++;
    if(counter == COLUMNS){
        printf("\n");
        counter = 0;
    }
}
// Samat
void print_sieves(int n){
    char arr[n + 1];
    //Markera allt är inte prim
    for(int i = 0; i <= n; i++){
        arr[i] = 0;
    }
    int p = 2;
    while(p <= n){
        //Markera 2p, 3p, 4p,...
        for(int i = 2 * p; i <= n; i += p){
            arr[i] = 1;
            }
        //Hitta nästa omarkerade nummer större än p
        int next = p + 1;
        while(next <= n && arr[next] == 1){
            next++;
            }
        if(next > n){
            break;
            }
        p = next;
    }

    //Alla omarkerade nummer från 2 till n är primtal
    for(int i = 2; i <= n; i++)
    {
        if(arr[i] == 0)
        {
            print_number(i);
        }
    }
    if(counter != 0)
    {
        printf("\n");
    }
}
int main(int argc, char *argv[]){
    if(argc == 2){
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}
/* gcc sieves.c -o sieves
./sieves 105 */