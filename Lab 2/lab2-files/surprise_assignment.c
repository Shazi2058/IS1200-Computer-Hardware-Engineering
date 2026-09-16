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
    char *arr = malloc((n + 1) * sizeof(char));
    //Markera allt är inte prim
    for(int i = 0; i <= n; i++)
    {
        arr[i] = 0;
    }
// Samat
    int p = 2;
    while(p <= n){
        //Markera 2p, 3p, 4p,...
        for(int i = 2 * p; i <= n; i += p)
        {
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
// Samat
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

    // Surprise assignment: Calculate the average distance between primes
    int d = 0;
    int sum = 0;
    int x = 2;
    int count = 0;
    for (int i = 3; i <= n; i++)
    {
        if(arr[i] == 0)
        {
          d= i - x;
          sum = sum + d;
          x=i;
          count++;
        }
    }
    float avg = (float)sum / count;
    printf("====================================================\n");
    printf("Sum of distances between primes: %d\n", sum);
    printf("Count of primes: %d\n", count);
    printf("Average distance between primes: %f\n", avg);
    free(arr);
}


int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}

/* gcc surprise_assignment.c -o surprise_assignment
./surprise_assignment 10 */