/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";
int list1[20];  // The assembly allocates 80 bytes for both lists and an integer is 4 bytes so the array size is 80/4=20
int list2[20];
int counter = 0;

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}
void copycodes(char *text, int *list, int *counter){
while (*text !=0){
  *list = *text; // lb	t0,0(a0) copies the value from text and stores it in the list
  text +=1; // in assembly it add 1 because char is 1 byte 
  list +=1; // in assembly it add 4 because int is 4 bytes
  *counter +=1;
}
}
void work(){
    copycodes(text1, list1, &counter); //jal	copycodes
    copycodes(text2, list2, &counter);  //jal	copycodes
}
int main(void){
 
    work();
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);

    endian_proof((char*) &counter);
}
/* gcc pointers.c -o pointers
./pointers 105 */