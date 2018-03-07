#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tweeter
{
    /* Struct to act as a linked list node
     * containing information about each twitter
     * user */

    /* User name */
    char* name;
    /* Number of tweets */
    int tweetCount;

} Tweeter;

typedef struct TweetCSV
{
    /* Struct that holds information about the
    *  passed CSV and its contents. Holds a linked
    *  list of Tweeter structs. */

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
        printf("Valid open file: %s\n", csvInfo->filename);
    else
    {
        printf("No open file\n");
        return;
    }

    printf("Name column: %d\n", csvInfo->nameCol);
    printf("Number of users: %d\n", csvInfo->numTweeters);

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
    /* Add a new Tweeter to a TweetCSV's array */
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

    /* Check if there are not too many unique tweeters */
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

void checkExtension(char* filename)
{
    /* Validate the extention is ".csv" with
       any variation on capitalization */

    char* ch;
    char* comp = "csv";
    int res = 0;

    /* Find the last '.' in the string */
    ch = strrchr(filename, '.') + 1;

    /* Compare the extension to "csv" by walking the extension.
    /* For each character */
    for (; *ch || *comp; ch++, comp++)
    {
        /* If at the end on only one string, they are different lengths */
        if((*ch && !*comp) || (*comp && !*ch)) { inputError(); }
        /* Convert current characters to lowercase and subtract character ascii values */
        res += tolower(*ch) - *comp;
        /* res will only be 0 if the characters are the same */
        if (res != 0) { inputError(); }
    }
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

/* void findNameCol(TweetCSV* csvInfo)
{
    const char* column;
    //for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    for (tok = strtok(line, ","); ; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
} */

void storeConsts(TweetCSV* csvInfo)
{
    /* Set up TweetCSV object */

    /* Store constants known from HW3 */
    csvInfo->maxRows = 20000;
    csvInfo->maxLineLength = 183;
    csvInfo->maxTweeters = 6227;

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

    /* Check for the correct file extension */
    checkExtension(csvInfo->filename);

    /* Find the names column */
    /* findNameCol(csvInfo); */
}

int main (int argc, char* argv[])
{
    /* Struct to collect info throughout execution */
    TweetCSV* csvInfo;

    /* Dynamically allocate a new object */
    csvInfo = (TweetCSV*) malloc(sizeof(TweetCSV));

    /* Map CSV structural information */
    mapCSV(argc, argv, csvInfo);
    checkTweeter(csvInfo, "Test");
    checkTweeter(csvInfo, "Test2");
    checkTweeter(csvInfo, "Test2");
    printTweeters(csvInfo);

    return 0;
}