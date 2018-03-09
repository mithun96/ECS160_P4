#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tweeter
{
    /* Struct containing information about each twitter
     * user */

    /* User name */
    char* name;
    /* Number of tweets */
    int tweetCount;

} Tweeter;

typedef struct TweetCSV
{
    /* Struct that holds information about the
    *  passed CSV and its contents. Holds an array
    *  of Tweeter structs. */

    /* File information */
    /* A pointer to the CSV file */
    FILE* csvFile;
    /* The file's name */
    char* filename;
    /* The number of the name column */
    int nameCol;
    /* Array of Tweeters */
    Tweeter** tweeters;
    /* Number of Tweeters */
    int numTweeters;
    /* Number of columns */
    int numCols;
    /* Position in the file */
    int filePos;
    /* Current field */
    int fieldPos;

    /* Info calculated from HW3 csv per project specs */
    /* Maximum number of rows */
    int maxRows;
    /* Maximum numver of tweeters */
    int maxTweeters;
    /* Maximum line length */
    int maxLineLength;

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
    printf("Max line length: %d characters\n", csvInfo->maxLineLength);
    printf("Max number of unique tweeters: %d\n\n", csvInfo->maxTweeters);
    printf("Name column: %d\n", csvInfo->nameCol);
    printf("Total columns: %d\n", csvInfo->numCols);
    printf("Number of recorded users: %d\n", csvInfo->numTweeters);

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

void addTweeter(TweetCSV* csvInfo, char* username)
{
    /* Add a new Tweeter to a TweetCSV tweeters array */

    Tweeter* newTweeter;
    /* Dynamically allocate a new Tweeter object */
    newTweeter = (Tweeter*) malloc(sizeof(Tweeter));
    /* Set its count to 1 (first tweet) */
    newTweeter->tweetCount = 1;
    /* Allocate space to store the username in the Tweeter */
    newTweeter->name = (char*) malloc(sizeof(username));
    /* Copy the name */
    strcpy(newTweeter->name, username);

    /* Store a pointer to the new object in the array */
    csvInfo->tweeters[csvInfo->numTweeters++] = newTweeter;
    /* Check if there are too many unique tweeters */
    if(csvInfo->numTweeters > csvInfo->maxTweeters) /* TEST >= TOO!!!! */
        inputError();
}

void checkTweeter(TweetCSV* csvInfo, char* username)
{
    /* Check if a Tweeter is in a TweetCSV's array */

    int i;
    /* Add Tweeter if the array is empty */
    if(csvInfo->numTweeters == 0)
    {
        addTweeter(csvInfo, username);
        return;
    }

    /* Else, search the array for the user, update count if found */
    for(i = 0; i < csvInfo->numTweeters; i++)
        if(strcmp(csvInfo->tweeters[i]->name, username) == 0)
        {
            csvInfo->tweeters[i]->tweetCount++;
            return;
        }

    /* If not found */
    if(i == csvInfo->numTweeters)
        addTweeter(csvInfo, username);
}

FILE* openCSV(int argc, char* argv[])
{
    /* Open and return the CSV file if valid */

    FILE* fp;

    /* Check for the correct number of arguments */
    if(argc != 2) { inputError(); }

    /* Attempt to open the file */
    if((fp = fopen(argv[1], "r")) == NULL) { inputError(); }

    return fp;
}

void getTweeters(TweetCSV* csvInfo)
{

    int count = 0, col = 0;
    int pos = 0;
    char* str1 = "\0", *str2, *firstRow;
    const char c[2] = ",";

    /* Allocate space for a word buffer */
    firstRow = (char*) malloc(sizeof(char) * csvInfo->maxLineLength);
    firstRow = fgets(firstRow, sizeof(char) * csvInfo->maxLineLength + 1, csvInfo->csvFile);

    /* While there is a next token without an ending newline */
    while(str1)
    {
        str1 = strrchr(str1, ',');
        if(str1){str1++;}
        pos = strlen(str1);
        printf("%s\n", str1);

        count++;
    }

    printf("%d\n", count);
       count = 0;
}

void findNameColbk(TweetCSV* csvInfo)
{
    /* Find the name column of a CSV file */

    char* firstRow, *token = "\0";
    int pos = 0, col = 0, flag = 0;
    const char c[2] = ",";

    /* Allocate space for a wocrd buffer */
    firstRow = (char*) malloc(sizeof(char) * csvInfo->maxLineLength);

    /* While there is a next token without an ending newline */
    while(token && token[strlen(token) - 1] != '\n')
    {
        /* Get the next chunk of characters from the current position */
        if(! fgets(firstRow, sizeof(char) * csvInfo->maxLineLength + 1, csvInfo->csvFile))
            inputError();

        /* Check CSV structure for quotes around cells */
        if((strcmp(token, "\0") == 0) && firstRow[0] == ',')
        {
            csvInfo->numCols = 1; csvInfo->nameCol = 1;
        }

        /* Get the next field from the chunk of characters */
        token = strtok(firstRow, c);

        /* Check if the field is "name" */
        if(strcmp("\"name\"", token) == 0 || strcmp("name", token) == 0)
        {
            csvInfo->nameCol += col;
            flag = 1;
        }

        /* Otherwise, update position to the end of the field and continue */
        pos += (strlen(token) + 1);
        rewind(csvInfo->csvFile);
        fseek(csvInfo->csvFile, pos, 1);
        col++;
    }

    /* If not found, error */
    if(flag == 0)
        inputError();

    csvInfo->numCols += col;
    rewind(csvInfo->csvFile);
}

char* getNextChunk(TweetCSV* csvInfo)
{
    /* Get the next chunk of 240 characters from a file */
    char* chunk;
    chunk = (char*) malloc(sizeof(char) * 240);

    /* Rewind and set to correct file position */
    rewind(csvInfo->csvFile);
    fseek(csvInfo->csvFile, csvInfo->filePos, 1);

    /* Get the chunk */
    if(! fgets(chunk, sizeof(char) * 240, csvInfo->csvFile))
        inputError();

    return chunk;
}

char* getNextField(char* chunk, TweetCSV* csvInfo)
{
    /* Get the next field from a chunk */

    char* nextComma;
    char* field;
    int diff;

    /* Move to end of last field */
    chunk = chunk + csvInfo->fieldPos;

    /* Get next comma */
    nextComma = strchr(chunk, ',');

    /* If last field */
    if(! nextComma)
        return NULL;

    /* Else */
    /* Get length of string between commas */
    diff = strlen(chunk) - strlen(nextComma);

    /* Copy string between commas */
    field = (char*) malloc(sizeof(char) * diff + 1);
    memcpy(field, chunk, diff);

    /* Update the field position */
    csvInfo->fieldPos += (diff+ 1);

    return field;
}

int getFieldType(char* field)
{
    /* Check if a field ends a row */
    /* DEBUG THIS */
    int qtCount = 0, i = 0;

    for(; i < strlen(field); i++ )
        if(field[i] == '\"')
            qtCount++;

    if(qtCount % 2 == 0)
        return 1;
    else
        return 0;
}

int findNameCol(TweetCSV* csvInfo)
{
    /* Find the name column of a CSV file */

    int col = 0;
    char* chunk;
    char* field = " ";

    /* While there are more columns, get a chunk of the header */
    while(chunk = getNextChunk(csvInfo))
    {
        /* Split the chunk into fields */
        while(field = getNextField(chunk, csvInfo))
        {
            /* Compare the field with "name" */
            if(strcmp("name", field)  == 0 || strcmp("\"name\"", field) == 0)
                csvInfo->nameCol = col;
            else
                col++;
        }

        /* If end of row */
        if(getFieldType(chunk + csvInfo->fieldPos) == 1)
        {
            csvInfo->numCols = col + 1;
            return;
        }

        /* Move FP to end of last processed field */
        csvInfo->fieldPos += strlen(chunk + csvInfo->fieldPos);
        csvInfo->filePos = csvInfo->fieldPos;
        csvInfo->fieldPos = 0;
    }

}

void storeConsts(TweetCSV* csvInfo)
{
    /* Set up TweetCSV object */

    /* Store constants known from HW3 */
    csvInfo->maxRows = 20000;
    csvInfo->maxLineLength = 183;
    csvInfo->maxTweeters = 6227;
    csvInfo->filePos = 0;
    csvInfo->fieldPos = 0;

    /* Allocate memory for array of Tweeters w/ extra space */
    csvInfo->tweeters = (Tweeter**) malloc(sizeof(Tweeter*) * 8000);
}

void mapCSV(int argc, char* argv[], TweetCSV* csvInfo)
{
    /* Collect and validate information about the
    *  structure of the CSV file */

    /* Configure the datastructure with known constants */
    storeConsts(csvInfo);

   /* Open and store the file, as well as its name */
    csvInfo->csvFile = openCSV(argc, argv);
    csvInfo->filename = argv[1];

    /* Find the names column */
    findNameCol(csvInfo);
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
    /* getTweeters(csvInfo); */

    printCSVInfo(csvInfo);

    return 0;
}