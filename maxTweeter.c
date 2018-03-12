#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tweeter
{
    /* Struct containing information about each twitter
     * user */

    char* name;
    int tweetCount;

} Tweeter;

typedef struct TweetCSV
{
    /* Struct that holds information about the
    *  passed CSV and its contents. */

    /* File information */
    FILE* csvFile;
    char* filename;
    int nameCol;
    Tweeter** tweeters;
    int numTweeters;
    int numCols;
    int maxCols;

    /* Info calculated from HW3 csv per project specs */
    int maxRows;
    int maxTweeters;

} TweetCSV;

void inputError()
{
    /* Error to be thrown if input is invalid */

    printf("InvalidInputFormat\n");
    exit(1);
}

int countChar(char* string, char c)
{
    /* Count a character */

    int count = 0, i = 0;

    for(; i < strlen(string); i++ )
        if(string[i] == c)
            count++;

    return count;
}

void collectGarbage(TweetCSV* csvInfo)
{
    /* Free all allocated memory if safe */

    int i;

    if(csvInfo)
    {
        if(csvInfo->tweeters)
        {
            for(i = 0; i < csvInfo->numTweeters; i++)
            {
                if(csvInfo->tweeters[i])
                {
                    if(csvInfo->tweeters[i]->name)
                        free(csvInfo->tweeters[i]->name);

                    free(csvInfo->tweeters[i]);
                }
            }

            free(csvInfo->tweeters);
        }

        if(csvInfo->csvFile)
            fclose(csvInfo->csvFile);

        free(csvInfo);
    }
}

void storeConsts(TweetCSV* csvInfo)
{
    /* Set up TweetCSV object */

    /* Store constants known from HW3 */
    csvInfo->maxRows = 20001;
    csvInfo->maxTweeters = 6230;

    /* Set defaults */
    csvInfo->nameCol = -1;

    /* Allocate memory for array of Tweeters w/ extra space */
    csvInfo->tweeters = (Tweeter**) malloc(sizeof(Tweeter*) * 8000);
}

void addTweeter(TweetCSV* csvInfo, char* username)
{
    /* Add a new Tweeter to a TweetCSV tweeters array */

    Tweeter* newTweeter;
    newTweeter = (Tweeter*) malloc(sizeof(Tweeter));
    newTweeter->tweetCount = 1;
    newTweeter->name = username;
    csvInfo->tweeters[csvInfo->numTweeters++] = newTweeter;

    if(csvInfo->numTweeters > csvInfo->maxTweeters)
        inputError();
}

void checkTweeter(TweetCSV* csvInfo, char* username)
{
    /* Check if a Tweeter is in a TweetCSV tweeter array */

    int i;

    if(strcmp(username, "") == 0)
        return;


    if(csvInfo->numTweeters == 0)
    {
        addTweeter(csvInfo, username);
        return;
    }

    for(i = 0; i < csvInfo->numTweeters; i++)
    {
        if(strcmp(csvInfo->tweeters[i]->name, username) == 0)
        {
            csvInfo->tweeters[i]->tweetCount++;
            return;
        }
    }

    if(i == csvInfo->numTweeters)
        addTweeter(csvInfo, username);
}

void findNameCol(TweetCSV* csvInfo)
{
    /* Find the name column of a CSV file */

    char c = ' '; /* parsing chararacter */
    char cc = ' '; /* forward buffer character */
    int col = 0; /* column number */
    int count = 0; /* row length */
    int open = 0; /* boolean to check if quotes are open/close */
    int fieldLen = 0; /* field length */
    char *field = " "; /* string of field text */
    field = (char*) malloc(sizeof(char) * 3767 + 1);

    c = fgetc(csvInfo->csvFile);
    if(strcmp(&c, "\"") == 0){
        /* If there are quotes around the text fields */

        while(c && !feof(csvInfo->csvFile)){
                
            if(open && strcmp(&c, "\"") == 0){

                cc = fgetc(csvInfo->csvFile);
                if(strcmp(&cc, ",") == 0){
                    open = 0;
                    col = col + 1;

                    if(strcmp("name", field)  == 0 || strcmp("\"name\"", field) == 0){
                        csvInfo->nameCol = col;
                    }
                    
                    memset(field, '\0', strlen(field));
                    fieldLen = 0;
                    
                }
                else if(strcmp(&cc, "\n") == 0){
                    open = 0;
                    col = col + 1;
                    csvInfo->maxCols = col;

                    if(strcmp("name", field)  == 0 || strcmp("\"name\"", field) == 0){
                        csvInfo->nameCol = col;
                    }

                    if(csvInfo->nameCol == -1){
                        inputError();
                    }
                    
                    if(field){free(field);}
                    return;
                }
                else
                    c = cc;
                    continue;
            }
            else if(strcmp(&c, "\"") == 0 && !open){
                open = 1;
            }
            else{
                field[fieldLen] = c;
                field[fieldLen + 1] = '\0';   
                fieldLen++; 
            }

            c = fgetc(csvInfo->csvFile);
        }
    }
    else{
        /* If there are NO quotes around the text fields */

        while(c && !feof(csvInfo->csvFile)){

            if(strcmp(&c, ",") == 0){

                col = col + 1;
                if(strcmp("name", field)  == 0 || strcmp("\"name\"", field) == 0){
                    csvInfo->nameCol = col;
                }
                memset(field, '\0', strlen(field));
                fieldLen = 0;
            }
            else if(strcmp(&c, "\n") == 0){
                col = col + 1;
                csvInfo->maxCols = col;
                if(strcmp("name", field)  == 0 || strcmp("\"name\"", field) == 0){
                    printf("field %s\n", field);
                    csvInfo->nameCol = col;
                }
                if(csvInfo->nameCol == -1){
                    inputError();
                }
                return;

            }
            else {
                field[fieldLen] = c;
                field[fieldLen + 1] = '\0';   
                fieldLen++;
            }                

            c = fgetc(csvInfo->csvFile);
        }
    }
}

void getTweeters(TweetCSV* csvInfo)
{
    /* Get the names of all tweeters */

    int row = 1; /* Number of rows in csv file */
    char c = ' '; /* parsing character */
    char cc = ' '; /* forward buffer character */
    int col = 0; /* column number */
    int count = 0; /* row length */
    int open = 0; /* boolean to check if quotes are open/close */
    int fieldLen = 0; /* field length */
    char *field = " "; /* string of field text */
    field = (char*) malloc(sizeof(char) * 3767 + 1);
    memset(field, '\0', 3768);

    if(feof(csvInfo->csvFile)){
        inputError();
    }
    
    c = fgetc(csvInfo->csvFile);
    if(strcmp(&c, "\"") == 0){
        /* If there are quotes around the text fields */

        while(c && !feof(csvInfo->csvFile)){
            
            /* open => quotes and command */
            if(open && strcmp(&c, "\"") == 0){

                cc = fgetc(csvInfo->csvFile);
                if(strcmp(&cc, ",") == 0){
                    open = 0;
                    col = col + 1;

                    if(col == csvInfo->nameCol){
                        checkTweeter(csvInfo, field);
                        field = (char*) malloc(sizeof(char) * 3767 + 1);

                    }
                    else{
                        memset(field, '\0', strlen(field));
                    }
                    fieldLen = 0;
                }
                else if(strcmp(&cc, "\n") == 0){
                    open = 0;
                    col = col + 1;
                    row = row + 1;

                    if(col != csvInfo->maxCols ){
                        inputError();
                    }

                    if(row > csvInfo->maxRows){
                        inputError();
                    }

                    if(col == csvInfo->nameCol){
                        checkTweeter(csvInfo, field);
                    }
                    memset(field, '\0', strlen(field));
                    fieldLen = 0;
                    col = 0;

                }
                else
                    c = cc;
                    continue;
            }
            else if(strcmp(&c, "\"") == 0 && !open){
                open = 1;
            }
            else{
                field[fieldLen] = c;            
                fieldLen++; 
            }

            c = fgetc(csvInfo->csvFile);
        }
    }
    else{
         /* If there are NO quotes around the text fields */

        while(c && !feof(csvInfo->csvFile)){

            if(strcmp(&c, ",") == 0){

                col = col + 1;
                if(col == csvInfo->nameCol){
                    checkTweeter(csvInfo, field);
                    field = (char*) malloc(sizeof(char) * 3767 + 1);
                }
                else{
                    memset(field, '\0', strlen(field));
                }
                fieldLen = 0;

            }
            else if(strcmp(&c, "\n") == 0){
                col = col + 1;
                row = row + 1;

                if(row > csvInfo->maxRows){
                    inputError();
                }

                if(col == csvInfo->nameCol){
                    checkTweeter(csvInfo, field);
                }
                memset(field, '\0', strlen(field));
                fieldLen = 0;
                col = 0;

            }
            else {
                field[fieldLen] = c;
                field[fieldLen + 1] = '\0';   
                fieldLen++;
            }                

            c = fgetc(csvInfo->csvFile);
        }
    }

    return;
}

FILE* openCSV(int argc, char* argv[])
{
    /* Open and return the CSV file if valid */

    FILE* fp;
    if(argc != 2) { inputError(); }

    if((fp = fopen(argv[1], "r")) == NULL) { inputError(); }

    return fp;
}

void mapCSV(int argc, char* argv[], TweetCSV* csvInfo)
{
    /* Collect and validate information about the
    *  structure of the CSV file */

    storeConsts(csvInfo);

    csvInfo->csvFile = openCSV(argc, argv);
    csvInfo->filename = argv[1];
    
    findNameCol(csvInfo);
    
    if(csvInfo->nameCol == -1)
        inputError();
    
}

void printMaxList(TweetCSV* csvInfo)
{
    /* Print top 10 tweeters */

    Tweeter* currTweeter;
    int curr_max = -1, i = 0, listCount = 0;

    for(; listCount < 10 && listCount < csvInfo->numTweeters; listCount++, curr_max = -1)
    {
        for(i = 0; i < csvInfo->numTweeters; i++)
        {
            if(csvInfo->tweeters[i]->tweetCount > curr_max &&
                csvInfo->tweeters[i]->tweetCount >= 0)
            {
                curr_max = csvInfo->tweeters[i]->tweetCount;
                currTweeter =  csvInfo->tweeters[i];
            }
        }

        if(! currTweeter)
            return;

        printf("%s: %d\n", currTweeter->name, currTweeter->tweetCount);

        currTweeter->tweetCount = -1;
        currTweeter = NULL;
    }
}

int main (int argc, char* argv[])
{
    /* Struct to collect info throughout execution */
    TweetCSV* csvInfo;

    /* Dynamically allocate a new object */
    csvInfo = (TweetCSV*) malloc(sizeof(TweetCSV));

    /* Map CSV structural information */
    mapCSV(argc, argv, csvInfo);

    /* Get a count of the number of times each user tweets */
    getTweeters(csvInfo);

    /* Print the top 10 tweeters */
    printMaxList(csvInfo);

    /*Collect garbage */
    collectGarbage(csvInfo);  
    
    return 0;
}
