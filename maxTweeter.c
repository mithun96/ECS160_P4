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
    /* Pointer to next node in list */
    struct Tweeter* next;

} Tweeter;

typedef struct TweetCSV
{
    /* Struct that holds information about the
    *  passed CSV and its contents. Holds a linked
    *  list of Tweeter structs. */

    /* A pointer to the CSV file */
    FILE* csvFile;
    /* The file's name */
    char* filename;
    /* The number of the name column */
    int nameCol;
    /* The number of the tweet column */
    int tweetCol; /* HOW DO YOU FIND THIS? */
    /* Linked list head */
    Tweeter* head;
    /* Number of list elements */
    int numTweeters;

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
    printf("Tweet column: %d\n", csvInfo->tweetCol);
    printf("Number of users: %d\n", csvInfo->numTweeters);

    printf("----------- CSV FILE INFO -----------\n");
}

/* --------------------------------------------------- DEBUG ----------------------------------------------------- */

void inputError()
{
    /* Error to be thrown if input is invalid */

    printf("InvalidInputFormat\n");
    exit(1);
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
        if(*ch && !*comp || *comp && !*ch){ inputError(); }
        /* Convert current characters to lowercase and subtract character ascii values */
        res += tolower(*ch) - tolower(*comp);
        /* res will only be 0 if the characters are the same */
        if (res != 0) { inputError(); }
    }
}

FILE* openCSV(int argc, char* argv[])
{
    /* Open and return the CSV file if valid */

    FILE* fp;

    printf("Open()\n");

    /* Check for the correct number of arguments */
    if(argc != 2) { inputError(); }

    /* Attempt to open the file */
    if((fp = fopen(argv[1], "r")) == NULL) { inputError(); }

    return fp;
}

void mapCSV(int argc, char* argv[], TweetCSV* csvInfo)
{
    /* Collect and validate information about the
    *  structure of the CSV file */

   /* Open and store the file, as well as its name */
    csvInfo->csvFile = openCSV(argc, argv);
    csvInfo->filename = argv[1];

    /* Check for the correct file extension */
    checkExtension(csvInfo->filename);
}

int main (int argc, char* argv[])
{
    /* Struct to collect info throughout execution */
    TweetCSV* csvInfo;

	printf("Main()\n");

    /* Dynamically allocate a new object */
    csvInfo = (TweetCSV*) malloc(sizeof(TweetCSV));
    /* Map CSV structural information */
    mapCSV(argc, argv, csvInfo);


    printCSVInfo(csvInfo); /* DEBUG - Check file information */

    return 0;
}