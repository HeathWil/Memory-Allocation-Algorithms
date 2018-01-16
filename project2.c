include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators

double MAX_SIZE;
int remaining;


void listAvailable(char process[]){
        int full=1;

        for(int i=0; i< MAX_SIZE; i++){
                if(process[i] == 'e'){
                        full=0;
                        printf("(%i, %i) ", remaining, i);
                }
        }
        //memory full
        if (full==0){
                fputs("\n", stdout);
        }
        else{
                fputs("FULL\n", stdout);
        }
}
void listAssigned(char process[]){
        int empty=1;

        for (int i=0; i < MAX_SIZE; i++){
                if ((process[i] != 'e') && (process[i] != '$')){
                        empty=0;
                        printf("(%c, %i, %i) ", process[i], remaining,i);
                }
        }

        //memory is empty
        if (empty==0){
                fputs("\n", stdout);
        }
        else{
                fputs("NONE\n", stdout);
        }
}
void find( char process[], char* args[]){
        for (int i=0; i < MAX_SIZE; i++){
                if(process[i] == *args[1]){
                        int size=0;
                        for(int j=i; j<MAX_SIZE; j++){
                                if(process[i]== *args[1]){
                                        size++;
                                }


                        }
                                               printf("%c %i %i\n", *args[1], size, i);
                }
        }
}
int main(int argc, char** argv)
{
        char* args[MAX_ARGS];
        char ** arg;
        char buf[MAX_BUFFER];
        int function=0;
        // set the max size for the process
        if(argv[2]){
                MAX_SIZE =pow(2, atoi(argv[2]));
                remaining= MAX_SIZE;
        }
        char process[(int)MAX_SIZE];
        int block[(int)MAX_SIZE];
        for(int i=0; i< MAX_SIZE; i++){
                process[i]='$';
        }
        process[0]='e';
        if(strcmp(argv[1], "BESTFIT") == 0) {
               function=1;
        } else
        if(strcmp(argv[1], "FIRSTFIT") == 0) {
                function=2;
        } else
        if(strcmp(argv[1], "NEXTFIT") == 0) {
                function=3;
        } else
        if(strcmp(argv[1], "BUDDY") == 0) {
               function=4;
        }
        else{
                fputs("Not a valid command", stdout);
        }
        //open file
        //
        freopen(argv[3], "r", stdin);
        while(!feof(stdin)){
                //read file
                if(fgets (buf, MAX_BUFFER, stdin)){
                        arg=args;
                        *arg++ = strtok(buf, SEPARATORS);
                        while((*arg++ = strtok(NULL, SEPARATORS)));
                        //if theres something there
                        if(args[0]){
                                                       //request command
                                if(!strcmp(args[0], "REQUEST")){
                                        //bestfit
                                        if(function==1){
                                                //bestfit(process,atoi( args[2]),args);
                                                int alloc=-1;
                                                int good=MAX_SIZE;
                                        //      int ideal=0;
                                                //find best location possible
                                                for(int i=0; i< MAX_SIZE; i++){
                                                        if((process[i]=='e') && (remaining > atoi(args[2]) && remaining < good)){
                                                                good=remaining;
                                                                alloc=i;
                                                        }
                                                        else if ((process[i] == 'e') && (remaining == atoi(args[2]))){
                                                                good= remaining;
                                                                alloc=i;
                                                                //ideal=1;
                                                                break;
                                                        }
                                                }
                                                //report not assigned
                                                if(alloc == -1){
                                                        printf("FAIL REQUEST %c %i\n", *args[1], atoi(args[2]));
                                                }
                                                //alloc success
                                                else{
                                                        for(int j = alloc; j < (alloc +atoi(args[2])); j++){
                                                                process[j]= *args[1];
                                                        }
                                                        remaining= remaining - atoi(args[2]);
                                                        process[alloc+ atoi(args[2])]='e';
                                                        printf("ALLOCATED %c %i\n", *args[1], alloc);
                                                }
                                        }
                                        //firstfit
                                        else if(function == 2){
                                                //firstfit(process, block,atoi( args[2]),args);
                                                int alloc=-1;
                                                //find available
                                                for(int i=0; i <MAX_SIZE; i++){
                                                        //found sufficient
                                                        //TODO: deal with perfect size
                                                        if((process[i]=='e') && (remaining > atoi(args[2]))){
                                                                for(int j=i; j < i+ atoi(args[2]); j++){
                                                                                   process[j] = *args[1];
                                                                }
                                                                process[i+atoi(args[2])]='e';
                                                                remaining=remaining- atoi(args[2]);
                                                                alloc=i;
                                                                break;
                                                        }
                                                }
                                                if(alloc== -1){
                                                        printf("FAIL REQUEST %c %i\n", *args[1], atoi(args[2]));
                                                }
                                                else
                                                {
                                                        printf("ALLOCATED %c %i\n", *args[1], alloc);
                                                }
                                        }
                                        //nextfit
                                        else if(function ==3){
                                                //nextfit(process, block,atoi( args[2]),args);
                                                int alloc=-1;
                                                int prev=0;
                                                //TODO: deal with the perfect size
                                                for(int i=prev; i<MAX_SIZE; i++){
                                                        if((process[i]=='e') && (remaining > atoi(args[2]))){
                                                                for(int j=i; j<i+ atoi(args[2]); j++){
                                                                        process[i]= *args[1];
                                                                }
                                                                process[i+atoi(args[2])]='e';
                                                                remaining=remaining-atoi(args[2]);
                                                                alloc=i;
                                                                break;
                                                        }
                                                }
                                                if(alloc==-1){
                                                        for(int i=0; i < prev; i++){
                                                                if ((process[i] =='e') && (remaining > atoi(args[2]))
){
                                                                        for(int j=i; j< i + atoi(args[2]); j++){
                                                                                process[i]= *args[1];
                                                                        }
                                                                        process[i +atoi(args[2])] = 'e';
                                                                        remaining=remaining-atoi(args[2]);
                                                                        alloc=i;
                                                                        break;
                                                                }
                                                        }
                                                }
                                                if(alloc == -1){
                                                        printf("FAIL REQUEST %c %i\n", *args[1], atoi(args[2]));
                                                }
                                                else
                                                {
                                                        printf("ALLOCATED %c %i\n", *args[1], alloc);
                                                }
                                        }
                                                                               //buddy
                                        else if(function ==4){
                                                //buddy(process, block, atoi( args[2]),args);
                                                int begin=0;
                                                int halfway=MAX_SIZE/2;
                                                int iter=halfway;
                                                int alloc=-1;
                                                for( int i=0; i < MAX_SIZE; i++){
                                                        if(atoi(args[2]) < halfway){//upper block
                                                                while(atoi(args[2]) < iter/2 && iter > 1){
                                                                        //check smaller block
                                                                        iter=iter/2;
                                                                }
                                                                //arg requires that space, check allocation
                                                                for(int j=0; j< iter; j++){
                                                                        if(process[j] == 'e'){
                                                                                //already allocated 
                                                                                if(iter*2 < MAX_SIZE){
                                                                                        iter=iter*2;
                                                                                }
                                                                                else{
                                                                                        //outside of block
                                                                                        break;
                                                                                }
                                                                        }
                                                                        else
                                                                        {
                                                                                //allocate
                                                                                for(int i=j; i< j+ atoi(args[2]); i++
){
                                                                                        process[i]= *args[1];
                                                                                }
                                                                                process[j + atoi(args[2])] = 'e';
                                                                                remaining= remaining-atoi(args[2]);
                                                                                alloc=j;
                                                                                break;
                                                                        }
                                                                }
                                                        }
                                                        else{ //lower half of block
                                                                iter= MAX_SIZE;
                                                                while(atoi(args[2]) < iter/2 && iter > halfway){
                                                                        iter=iter/2;
                                                                }
                                                                //args is smaller
                                                                for(int i =halfway; i < iter; i++){
                                                                        //already allocated
                                                                        if(process[i] == 'e'){
                                                                                if(iter*2 < MAX_SIZE){
                                                                                        iter=iter*2;
                                                                                }
                                                                                else{
                                                                                        //outside of block
                                                                                        break;
                                                                                }
                                                                        }
                                                                        else
                                                                        {
                                                                                                                                                        //allocate
                                                                                for(int j=i; j < i + atoi(args[2]); j++){
                                                                                        process[j] = *args[1];
                                                                                }
                                                                                process[i + atoi(args[1])] = 'e';
                                                                                remaining= remaining-atoi(args[2]);
                                                                                alloc=i;
                                                                                break;
                                                                        }
                                                                }
                                                        }
                                                }
                                        }
                                        else{
                                                //error
                                                fputs("Error occurred", stdout);
                                        }
                                }
                                else if(!strcmp(args[0], "RELEASE")){
                                        int space=0;
                                        int size=0;
                                        int location=-1;
                                        int empty = -1;
                                        int proc= -1;
                                        char iter= '$';
                                        int next= -1;
                                        for(int i=0; i < MAX_SIZE; i++){
                                                //process found
                                                if (process[i] == *args[1]){
                                                        //store
                                                        location=i;
                                                        while(process[i] == *args[1]){
                                                                size++;
                                                                i++;
                                                        }
                                                        i=location;
                                                        //check process
                                                        for (int j=0; j< i; j++){
                                                                if(process[j] == 'e'){
                                                                        empty=j;
                                                                }
                                                                else if (process[j] != '$'){
                                                                        proc=j;
                                                                }
                                                        }
                                                        //get the process
                                                        for(int j= i+1; j < MAX_SIZE; j++){
                                                                if (process[j] != '$'){
                                                                        iter=process[j];
                                                                        next=j;
                                                                        break;
                                                                }
                                                                                    }
                                                        //if empty, merge
                                                        if (iter== 'e'){
                                                                process[next]= '$';
                                                                remaining= remaining + next;
                                                                next=-1;
                                                        }
                                                        else{
                                                                process[i]='e';
                                                        }
                                                        space=1;
                                                }
                                        }
                                        if (space ==0){ //check
                                                printf("FAIL RELEASE %c\n", *args[1]);
                                        }
                                        else
                                        {
                                                printf("FREE %c %i %i\n", *args[1], size, location);
                                        }
                                }
                                else if(!strcmp(args[0], "LIST")){
                                        if(!strcmp(args[1], "AVAILABLE")){
                                                listAvailable(process);
                                        }
                                        if(!strcmp(args[1], "ASSIGNED")){
                                                listAssigned(process);
                                        }
                                }
                                else if(!strcmp(args[0], "FIND")){
                                        find(process, args);
                                }
                                else{
                                        fputs("Error occurred", stdout);
                                }
                        }
                }
        }
        return 0;
}
