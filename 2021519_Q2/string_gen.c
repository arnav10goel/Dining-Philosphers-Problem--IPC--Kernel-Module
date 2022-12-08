#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    char **listofstrings = malloc(50 * sizeof(char *));
    char listofalphabets[26][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
                                "w", "x", "y", "z"};
    
    for(int i = 0; i < 50; i++){
        listofstrings[i] = malloc(5 * sizeof(char));
        for(int j = 0; j < 5; j++){
            listofstrings[i][j] = listofalphabets[rand()%26][0];
        }
        
    }
    for(int i = 0; i < 50; i++){
        printf("%s\n", listofstrings[i]);
    }
}