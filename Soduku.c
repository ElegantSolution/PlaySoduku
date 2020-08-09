/**
 Copyright (c) 2020 Yue Zhao @ElegantSolution <eesoto@foxmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

/**
 
 This C program generates a sudoku game:
 displays the solution game board on the console,
 saves the puzzle game board to file "PlaySudoku.txt".
 
 Compile with a command line tool and run!
 ("PlaySudoku.txt" in the same directory where you run the command line, NOT the output C file directory)
 
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 9
#define S 3

int genSudokuNumbers(int grid[ ]);
int checkDups(int nums[ ], int search, int n);
void clearArray(int nums[ ], int n);
void genSudokuBoard(int grid[ ], int display[ ]);
void getRandNum(int low, int high, int *pn);
void displaySudokuBoard(int grid[ ], int bigOh);
void printSudokuBoardToFile(int display[ ], const char *fileName);

int main( ) {
    int grid[N*N];
    int display[N*N];
    int loops, i;
    char fileName[]="PlaySudoku.txt";

    clearArray(grid, N*N);
    clearArray(display, N*N);

    loops = genSudokuNumbers(grid);
    genSudokuBoard(grid,display);

    displaySudokuBoard(grid,loops);
    printSudokuBoardToFile(display, fileName);

    return 0;
}

int checkDups(int nums[ ], int search, int n) {
   int i;
   for(i=0;i<n && nums[i]!=search; i++) /*If there's a duplicated number, exit the loop and return 1*/
        ;
   if(i==n)
    return 0;
        else
            return 1;
}

void clearArray(int nums[ ], int n) {
    int i;
    for(i=0;i<n;i++){
        nums[i]=0;
    }
}

int genSudokuNumbers(int grid[ ]) {
    int c, i, j, rowNum, colNum, blockNum;
    int test[N], dup, temp, valid, cnt, iterations=0;

    srand(time(NULL));

    for(c=0; c<N*N; c++){
    iterations++;
    temp = rand( ) % N + 1;
    valid = dup = cnt = 0;

        while(!valid) {
        iterations++;

        clearArray(test, N);

        rowNum = c / N;
        colNum = c % N;
        blockNum = (rowNum / S) * S + (colNum / S);

        for(j=0; j<colNum; j++) {
            test[j] = grid[rowNum*N+j];
            iterations++;
        }
        dup += checkDups(test, temp, colNum);

        if(!dup) {
            clearArray(test, N);

            for(j=0; j<rowNum; j++) {
                test[j] = grid[j*N + colNum];
                iterations++;
            }
            dup += checkDups(test, temp, rowNum);

                if(!dup) {
                    clearArray(test, N);

                    for(j=0; j<N; j++) {
                    test[j] = grid[((blockNum/S)*N*S) + (colNum/S)*S + (j/S)*N + j%S];
                    iterations++;
                    }

                    dup += checkDups(test, temp, N);
                }
        }
        if(!dup) {
        grid[c] = temp;
        valid = 1;
        cnt = 0;
        }
            else {
            temp = rand( ) % N + 1;
            dup = 0;
            cnt++;
            }

        if(cnt > N*N) {
        clearArray(grid, N*N);
        valid = 1;
        c = -1;
        }

        }

    }
    return iterations;
}

void genSudokuBoard(int grid[ ], int display[ ]){
    int const f=4,low=0,high=8; /*constants numbers used for equation*/
    int i,j,c,k=0,temp;
    int game[N*f],test[f];

    for (i=0;i<N*f;i++){  /*generate a 36-number array and assign a random value of 0 ~ 8 to every 4 members*/
        if(i%f==0)
            clearArray(test,f);
        do{
        getRandNum(low,high,&temp);
        }while(checkDups(test,temp,i%f));

        game[i]=temp;
        test[i%f]=temp;
    }

    for(i=0;i<N*N;i+=(N*S)){    /*update every 4 values to 4 block index numbers*/
        for(j=i;j<i+N;j+=S){
            for(c=0;c<f;c++){
                temp=game[k];
                game[k]=j+(temp/S)*N+temp%S;
                k++;
            }
        }
    }

    for(i=0;i<N*N;i++){     /*assign the index's corresponding value to "display" array, others remain 0*/
        temp=(i/(N*S))*S+(i%N)/S;
        temp*=f;
        for(j=0;j<f;j++){
            if(i==game[temp+j]){
                display[i]=grid[i];
            }
        }
    }
}

void getRandNum(int low, int high, int *pn){
    *pn=low+rand()%(high+1-low);
}

void printSudokuBoardToFile(int display[ ], const char *fileName){
    const int e=18,x=6; /*constants used for printing borders*/
    int i, j;
    FILE *fp=NULL;

    fp=fopen(fileName,"w");

    if(fp==NULL)
        printf("Fail to open %s!",fileName);

    else{
        fprintf(fp,"PLAY SUDOKU\n");
        for(i=0;i<N*N;i++){ /*the first line of border*/
            if (i==0){
                for(j=0;j<e+1;j++){
                    if (j==e)
                    fprintf(fp,"+\n|");
                        else if(j%x==0)
                            fprintf(fp,"+");
                                else fprintf(fp,"-");
                }
            }
                else if (i%(N*S)==0){ /*horizontal borders for every 3 blocks*/
                    fprintf(fp,"|\n");
                    for(j=0;j<e+1;j++){
                    if (j==e)
                    fprintf(fp,"+\n|");
                        else if(j%x==0)
                            fprintf(fp,"+");
                                else fprintf(fp,"-");
                    }
                }
                    else if(i%N==0) /*outer vertical borders*/
                        fprintf(fp,"|\n|");

                        else if(i%S==0) /*vertical borders for every 3 numbers*/
                            fprintf(fp,"|");

            if(i%S==2){ /*if the value is 0 leave a blank*/
                if(display[i]==0)
                fprintf(fp," ");
                    else fprintf(fp,"%d",display[i]);

            }
                else {                   /*adjusting space*/
                    if(display[i]==0)
                    fprintf(fp,"  ");
                        else fprintf(fp,"%d ",display[i]);

                }


            if(i==N*N-1){   /*the bottom line of border*/
                fprintf(fp,"|\n");
                for(j=0;j<e+1;j++){
                if (j==e)
                fprintf(fp,"+\n");
                    else if(j%x==0)
                    fprintf(fp,"+");
                        else fprintf(fp,"-");
                }
            }
        }
        printf("(Sudoku board was printed to file: %s)\n",fileName);
        fclose(fp);
    }
}

void displaySudokuBoard(int grid[ ], int bigOh){
    const int e=18,x=6;   /*very similar to the "printSudokuToFile" function*/
    int i,j;
    printf("PLAY SUDOKU\n");
    for(i=0;i<N*N;i++){
        if (i==0){
            for(j=0;j<e+1;j++){
                if (j==e)
                printf("+\n|");
                    else if(j%x==0)
                    printf("+");
                        else printf("-");
            }
        }
            else if (i%(N*S)==0){
                printf("|\n");
                for(j=0;j<e+1;j++){
                    if (j==e)
                    printf("+\n|");
                        else if(j%x==0)
                        printf("+");
                            else printf("-");
                }
            }
                else if(i%N==0)
                    printf("|\n|");

                    else if(i%S==0)
                        printf("|");

        if(i%S==2)
        printf("%d",grid[i]);
            else printf("%d ",grid[i]);

        if(i==N*N-1){
        printf("|\n");
            for(j=0;j<e+1;j++){
                if (j==e)
                printf("+\n");
                    else if(j%x==0)
                    printf("+");
                        else printf("-");
            }
        }
    }
    printf("Total Instructions:%d\n",bigOh);
}
