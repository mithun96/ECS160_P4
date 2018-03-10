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
    int filePos;
    int fieldPos;

    /* Info calculated from HW3 csv per project specs */
    int maxRows;
    int maxTweeters;

} TweetCSV;

/* --------------------------------------------------- DEBUG ----------------------------------------------------- */
void printCSVInfo(TweetCSV* csvInfo)
{
    printf("----------- CSV FILE INFO -----------\n");

    if(csvInfo->csvFile)
        printf("Valid open file: %s\n\n", csvInfo->filename);
    else
    {
        printf("No open file\n");
        return;
    }

    printf("Max number of rows: %d\n", csvInfo->maxRows);
    printf("Max number of unique tweeters: %d\n\n", csvInfo->maxTweeters);
    printf("Name column: %d\n", csvInfo->nameCol);
    printf("Total columns: %d\n", csvInfo->numCols);
    printf("Number of recorded users: %d\n", csvInfo->numTweeters);
    printf("File position %d\n", csvInfo->filePos);

    printf("----------- CSV FILE INFO -----------\n");
}

void printTweeters(TweetCSV* csvInfo)
{
    int i;
    if(csvInfo->numTweeters == 0)
        printf("NO TWEETER RECORDS\n");

    for(i = 0; i < csvInfo->numTweeters; i++)
    {
        printf("----------- TWEETER RECORD -----------\n");
        printf("Name: %s\n",  csvInfo->tweeters[i]->name);
        printf("Count: %d\n",  csvInfo->tweeters[i]->tweetCount);
        printf("----------- TWEETER RECORD -----------\n");
    }
}

/* --------------------------------------------------- DEBUG ----------------------------------------------------- */

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
    csvInfo->filePos = 0;
    csvInfo->fieldPos = 0;
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

char* getNextChunk(TweetCSV* csvInfo, int throwError)
{
    /* Get the next chunk of 240 characters from a file */

    int count = 0;
    char c = ' ';
    char* chunk, *tempChunk, *addChunk;
    chunk = (char*) malloc(sizeof(char) * 32767 + 1);
    memset(chunk, '\0', strlen(chunk));

    rewind(csvInfo->csvFile);
    fseek(csvInfo->csvFile, csvInfo->filePos, 1);

    if(! fgets(chunk, sizeof(char) * 32767, csvInfo->csvFile))
    {
        if(throwError == 1)
            inputError();
        else
            return NULL;
    }

    if(countChar(chunk, '\"') % 2 != 0 && chunk[0] == '\"')
    {
        rewind(csvInfo->csvFile);
        fseek(csvInfo->csvFile, csvInfo->filePos + 1, 1);

        /* 
            odd number of " or no commas
            when End of File is not set
            */
        while((countChar(chunk, '\"') % 2 != 0 || countChar(chunk, ',') == 0) && ! feof(csvInfo->csvFile))
        {
            c = fgetc(csvInfo->csvFile);
            chunk[count++] = c;

            if(count > 32766)
                inputError();
        }

        csvInfo->filePos += count + 1;
    }


    return chunk;
}

char* getNextField(char* chunk, TweetCSV* csvInfo)
{
    /* Get the next field from a chunk */

    char* nextComma, *field;
    int diff;

    chunk = chunk + csvInfo->fieldPos;
    nextComma = strchr(chunk, ',');

    if(nextComma == NULL) {
        // printf("Returning NULL\n");
        return NULL;
    }
    if (chunk == NULL) {
        // printf("Returning NULL chunk\n");
        return NULL;
    }

    // printf("ChunkSize: %s\n", chunk);
    // printf("NextComma: %s\n", nextComma);

    diff = strlen(chunk) - strlen(nextComma);

    if(chunk[0] == '\"' && chunk[diff - 1] != '\"')
    {
    // printf("NextComma2: %s\n", nextComma);
        nextComma = strchr(nextComma, '\"');

        if(nextComma == NULL) {
            return NULL;
        } else {
            nextComma++;
        }

        if (chunk == NULL) {
            return NULL;
        }        
        diff = strlen(chunk) - strlen(nextComma);
    }

    field = (char*) malloc(sizeof(char) * diff + 1);
    memset(field,'\0', strlen(field));
    memcpy(field, chunk, diff);

    csvInfo->fieldPos += (diff+ 1);

    return field;
}

void getTweeters(TweetCSV* csvInfo)
{
    /* Get the names of all tweeters */

    int col = 0, row = 0;
    char* chunk, *field = " ";

    csvInfo->filePos = 0;
    csvInfo->fieldPos = 0;

    
    while(chunk = getNextChunk(csvInfo, 0))
    {
        while(field = getNextField(chunk, csvInfo))
        {
            if(col == csvInfo->nameCol && row > 0)
                checkTweeter(csvInfo, field);

            csvInfo->filePos += strlen(field) + 1;
            col++;

        }

        csvInfo->filePos += (strlen(chunk + csvInfo->fieldPos) + 1);
        csvInfo->fieldPos = 0;
        col = 0;
        row++;

        if(row > csvInfo->maxRows)
            inputError();

        if(chunk) free(chunk);
        if(field) free(field);
    }
}

void findNameCol(TweetCSV* csvInfo)
{
    /* Find the name column of a CSV file */

    int col = 0;
    char* chunk, *field = " ";
    while(chunk = getNextChunk(csvInfo, 1))
    {

        while(field = getNextField(chunk, csvInfo))
        {

            if(strcmp("name", field)  == 0 || strcmp("\"name\"", field) == 0)
                csvInfo->nameCol = col;
            else
                col++;

        }

        if(chunk[strlen(chunk) + csvInfo->fieldPos - 2] == '\"' ||
            countChar(chunk + csvInfo->fieldPos, '\"') == 0)
        {
            csvInfo->numCols = col + 1;

            free(chunk); if(field) free(field);
            return;
        }

        if(chunk[csvInfo->fieldPos] == '\"')
        {
            csvInfo->filePos = csvInfo->fieldPos;
            continue;
        }

        csvInfo->fieldPos += strlen(chunk + csvInfo->fieldPos);
        csvInfo->filePos = csvInfo->fieldPos;
        csvInfo->fieldPos = 0;

        if(chunk) free(chunk);
        if(field) free(field);
    }

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

    /* Collect garbage */
    collectGarbage(csvInfo);

    return 0;
}
