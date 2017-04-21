#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define UNUSED(x) ((void)(x))

#define MAX_WORDS_COUNT 1000
#define WATCHDOG_ALARM  10000
#define MIN_BOARD_SIZE  11

#define COMMAND_EXIT    'q'
#define COMMAND_HELP    '?'
#define COMMAND_SELECT  's'


typedef enum DIRECTIONtag {
    HORIZONTAL,
    VERTICAL,
    DOWN_UP,
    UP_DOWN,
    UNDEFINED
} DIRECTION, *PDIRECTION;

typedef struct POINTtag {
    unsigned int X;
    unsigned int Y;
} POINT, *PPOINT;

typedef struct WORDtag {
    char * word;
    unsigned int length;
    DIRECTION direction;
    int isFound;
    POINT start;
    POINT end;
} WORD, *PWORD;

typedef struct LETTERtag {
    char letter;
    int isFound;
    PWORD word;
} LETTER, *PLETTER;

typedef struct BOARDtag {
    unsigned int height;
    unsigned int width;
    PLETTER * letters;
} BOARD, *PBOARD;

PBOARD createBoard(unsigned int width, unsigned int height);
void destroyBoard(PBOARD board);
void fillBoard(PBOARD board);
void throwError(char * message);
int canBePlacedInBoard(PPOINT point, PBOARD board);
void initTheGame(unsigned int width, unsigned int height, unsigned int * dictLength, PBOARD * board);
char * getDirectionString(DIRECTION dir);
DIRECTION giveMeMyDirection(PPOINT start, PPOINT end);
void printBoard(PBOARD board);
void debugPrintWords(PWORD words, unsigned int wordsCount);
void calculateWordEnd(PPOINT start, PPOINT end, unsigned int len, DIRECTION dir);
int isPointInBoard(PBOARD board, PPOINT point);
int insertWordIntoBoard(PBOARD board, unsigned int index, DIRECTION dir, PPOINT start, PWORD wordInList);
unsigned int min(unsigned int a, unsigned int b);
unsigned int max(unsigned int a, unsigned int b);
PWORD addWordToList(PBOARD board, PWORD words, unsigned int yetAdded, unsigned int index, PPOINT start, DIRECTION dir);
int addWordToTheGame(PBOARD board, PWORD words, unsigned int yetAdded, unsigned int dictLength);
void waitForEnter();
void clearStdin();
void clearConsole();
void letsTheGameBegin(PBOARD board, PWORD words, unsigned int wordsCount);
void handleWrongCommand();
unsigned int addManyWordsToTheGame(unsigned int maxCount, unsigned int dictLength, PBOARD board, PWORD words);
PWORD getWordOnCoord(PBOARD board, PPOINT start, PPOINT end);
int selectWord(PBOARD board, PPOINT start, PPOINT end);

char* dict[] = {
    "hat",
	"with",
	"they",
	"have",
	"this",
	"from",
	"word",
	"what",
	"some",
	"other",
	"were",
	"there",
	"when",
	"your",
	"said",
	"each",
	"which",
	"their",
	"time",
	"will",
	"about",
	"many",
	"then",
	"them",
	"write",
	"would",
	"like",
	"these",
	"long",
	"make",
	"thing",
	"look",
	"more",
	"could",
	"come",
	"number",
	"sound",
	"most",
	"people",
	"over",
	"know",
	"water",
	"than",
	"call",
	"first",
	"down",
	"side",
	"been",
	"find",
	"work",
	"part",
	"take",
	"place",
	"made",
	"live",
	"where",
	"after",
	"back",
	"little",
	"only",
	"round",
	"year",
	"came",
	"show",
	"every",
	"good",
	"give",
	"under",
	"name",
	"very",
	"through",
	"just",
	"form",
	"sentence",
	"great",
	"think",
	"help",
	"line",
	"differ",
	"turn",
	"cause",
	"much",
	"mean",
	"before",
	"move",
	"right",
	"same",
	"tell",
	"does",
	"three",
	"want",
	"well",
	"also",
	"play",
	"small",
	"home",
	"read",
	"hand",
	"port",
	"large",
	"spell",
	"even",
	"land",
	"here",
	"must",
	"high",
	"such",
	"follow",
	"change",
	"went",
	"light",
	"kind",
	"need",
	"house",
	"picture",
	"again",
	"animal",
	"point",
	"mother",
	"world",
	"near",
	"build",
	"self",
	"earth",
	"father",
	"head",
	"stand",
	"page",
	"should",
	"country",
	"found",
	"answer",
	"school",
	"grow",
	"study",
	"still",
	"learn",
	"plant",
	"cover",
	"food",
	"four",
	"between",
	"state",
	"keep",
	"never",
	"last",
	"thought",
	"city",
	"tree",
	"cross",
	"farm",
	"hard",
	"start",
	"might",
	"story",
	"draw",
	"left",
	"late",
	"while",
	"press",
	"close",
	"night",
	"real",
	"life",
	"north",
	"open",
	"seem",
	"together",
	"next",
	"white",
	"children",
	"begin",
	"walk",
	"example",
	"ease",
	"paper",
	"group",
	"always",
	"music",
	"those",
	"both",
	"mark",
	"often",
	"letter",
	"until",
	"mile",
	"river",
	"feet",
	"care",
	"second",
	"book",
	"carry",
	"took",
	"science",
	"room",
	"friend",
	"began",
	"idea",
	"fish",
	"mountain",
	"stop",
	"once",
	"base",
	"hear",
	"horse",
	"sure",
	"watch",
	"color",
	"face",
	"wood",
	"main",
	"enough",
	"plain",
	"girl",
	"usual",
	"young",
	"ready",
	"above",
	"ever",
	"list",
	"though",
	"feel",
	"talk",
	"bird",
	"soon",
	"body",
	"family",
	"direct",
	"pose",
	"leave",
	"song",
	"measure",
	"door",
	"product",
	"black",
	"short",
	"numeral",
	"class",
	"wind",
	"question",
	"happen",
	"complete",
	"ship",
	"area",
	"half",
	"rock",
	"order",
	"fire",
	"south",
	"problem",
	"piece",
	"told",
	"knew",
	"pass",
	"since",
	"whole",
	"king",
	"space",
	"heard",
	"best",
	"hour",
	"better",
	"true",
	"during",
	"hundred",
	"five",
	"remember",
	"step",
	"early",
	"hold",
	"west",
	"ground",
	"interest",
	"reach",
	"fast",
	"verb",
	"sing",
	"listen",
	"table",
	"travel",
	"less",
	"morning",
	"simple",
	"several",
	"vowel",
	"toward",
	"against",
	"pattern",
	"slow",
	"center",
	"love",
	"person",
	"money",
	"serve",
	"appear",
	"road",
	"rain",
	"rule",
	"govern",
	"pull",
	"cold",
	"notice",
	"voice",
	"unit",
	"power",
	"town",
	"fine",
	"certain",
	"fall",
	"lead",
	"dark",
	"machine",
	"note",
	"wait",
	"plan",
	"figure",
	"star",
	"noun",
	"field",
	"rest",
	"correct",
	"able",
	"pound",
	"done",
	"beauty",
	"drive",
	"stood",
	"contain",
	"front",
	"teach",
	"week",
	"final",
	"gave",
	"green",
	"quick",
	"develop",
	"ocean",
	"warm",
	"free",
	"minute",
	"strong",
	"special",
	"mind",
	"behind",
	"clear",
	"tail",
	"produce",
	"fact",
	"street",
	"inch",
	"multiply",
	"nothing",
	"course",
	"stay",
	"wheel",
	"full",
	"force",
	"blue",
	"object",
	"decide",
	"surface",
	"deep",
	"moon",
	"island",
	"foot",
	"system",
	"busy",
	"test",
	"record",
	"boat",
	"common",
	"gold",
	"possible",
	"plane",
	"stead",
	"wonder",
	"laugh",
	"thousand",
	"check",
	"game",
	"shape",
	"equate",
	"miss",
	"brought",
	"heat",
	"snow",
	"tire",
	"bring",
	"distant",
	"fill",
	"east",
	"paint",
	"language",
	"among",
	"grand",
	"ball",
	"wave",
	"drop",
	"heart",
	"present",
	"heavy",
	"dance",
	"engine",
	"position",
	"wide",
	"sail",
	"material",
	"size",
	"vary",
	"settle",
	"speak",
	"weight",
	"general",
	"matter",
	"circle",
	"pair",
	"include",
	"divide",
	"syllable",
	"felt",
	"perhaps",
	"pick",
	"sudden",
	"count",
	"square",
	"reason",
	"length",
	"represent",
	"subject",
	"region",
	"energy",
	"hunt",
	"probable",
	"brother",
	"ride",
	"cell",
	"believe",
	"fraction",
	"forest",
	"race",
	"window",
	"store",
	"summer",
	"train",
	"sleep",
	"prove",
	"lone",
	"exercise",
	"wall",
	"catch",
	"mount",
	"wish",
	"board",
	"winter",
	"written",
	"wild",
	"instrument",
	"kept",
	"glass",
	"grass",
	"edge",
	"sign",
	"visit",
	"past",
	"soft",
	"bright",
	"weather",
	"month",
	"million",
	"bear",
	"finish",
	"happy",
	"hope",
	"flower",
	"clothe",
	"strange",
	"gone",
	"jump",
	"baby",
	"eight",
	"village",
	"meet",
	"root",
	"raise",
	"solve",
	"metal",
	"whether",
	"push",
	"seven",
	"paragraph",
	"third",
	"shall",
	"held",
	"hair",
	"describe",
	"cook",
	"floor",
	"either",
	"result",
	"burn",
	"hill",
	"safe",
	"century",
	"consider",
	"type",
	"coast",
	"copy",
	"phrase",
	"silent",
	"tall",
	"sand",
	"soil",
	"roll",
	"temperature",
	"finger",
	"industry",
	"value",
	"fight",
	"beat",
	"excite",
	"natural",
	"view",
	"sense",
	"else",
	"quite",
	"broke",
	"case",
	"middle",
	"kill",
	"lake",
	"moment",
	"scale",
	"loud",
	"spring",
	"observe",
	"child",
	"straight",
	"consonant",
	"nation",
	"dictionary",
	"milk",
	"speed",
	"method",
	"organ",
	"section",
	"dress",
	"cloud",
	"surprise",
	"quiet",
	"stone",
	"tiny",
	"climb",
	"cool",
	"design",
	"poor",
	"experiment",
	"bottom",
	"iron",
	"single",
	"stick",
	"flat",
	"twenty",
	"skin",
	"smile",
	"crease",
	"hole",
	"trade",
	"melody",
	"trip",
	"office",
	"receive",
	"mouth",
	"exact",
	"symbol",
	"least",
	"trouble",
	"shout",
	"except",
	"wrote",
	"seed",
	"tone",
	"join",
	"suggest",
	"clean",
	"break",
	"lady",
	"yard",
	"rise",
	"blow",
	"blood",
	"touch",
	"grew",
	"cent",
	"team",
	"wire",
	"cost",
	"lost",
	"brown",
	"wear",
	"garden",
	"equal",
	"sent",
	"choose",
	"fell",
	"flow",
	"fair",
	"bank",
	"collect",
	"save",
	"control",
	"decimal",
	"gentle",
	"woman",
	"captain",
	"practice",
	"separate",
	"difficult",
	"doctor",
	"please",
	"protect",
	"noon",
	"whose",
	"locate",
	"ring",
	"character",
	"insect",
	"caught",
	"period",
	"indicate",
	"radio",
	"spoke",
	"atom",
	"human",
	"history",
	"effect",
	"electric",
	"expect",
	"crop",
	"modern",
	"element",
	"student",
	"corner",
	"party",
	"supply",
	"bone",
	"rail",
	"imagine",
	"provide",
	"agree",
	"thus",
	"capital",
	"chair",
	"danger",
	"fruit",
	"rich",
	"thick",
	"soldier",
	"process",
	"operate",
	"guess",
	"necessary",
	"sharp",
	"wing",
	"create",
	"neighbor",
	"wash",
	"rather",
	"crowd",
	"corn",
	"compare",
	"poem",
	"string",
	"bell",
	"depend",
	"meat",
	"tube",
	"famous",
	"dollar",
	"stream",
	"fear",
	"sight",
	"thin",
	"triangle",
	"planet",
	"hurry",
	"chief",
	"colony",
	"clock",
	"mine",
	"enter",
	"major",
	"fresh",
	"search",
	"send",
	"yellow",
	"allow",
	"print",
	"dead",
	"spot",
	"desert",
	"suit",
	"current",
	"lift",
	"rose",
	"continue",
	"block",
	"chart",
	"sell",
	"success",
	"company",
	"subtract",
	"event",
	"particular",
	"deal",
	"swim",
	"term",
	"opposite",
	"wife",
	"shoe",
	"shoulder",
	"spread",
	"arrange",
	"camp",
	"invent",
	"cotton",
	"born",
	"determine",
	"quart",
	"nine",
	"truck",
	"noise",
	"level",
	"chance",
	"gather",
	"shop",
	"stretch",
	"throw",
	"shine",
	"property",
	"column",
	"molecule",
	"select",
	"wrong",
	"gray",
	"repeat",
	"require",
	"broad",
	"prepare",
	"salt",
	"nose",
	"plural",
	"anger",
	"claim",
	"continent",
	"oxygen",
	"sugar",
	"death",
	"pretty",
	"skill",
	"women",
	"season",
	"solution",
	"magnet",
	"silver",
	"thank",
	"branch",
	"match",
	"suffix",
	"especially",
	"afraid",
	"huge",
	"sister",
	"steel",
	"discuss",
	"forward",
	"similar",
	"guide",
	"experience",
	"score",
	"apple",
	"bought",
	"pitch",
	"coat",
	"mass",
	"card",
	"band",
	"rope",
	"slip",
	"dream",
	"evening",
	"condition",
	"feed",
	"tool",
	"total",
	"basic",
	"smell",
	"valley",
	"double",
	"seat",
	"arrive",
	"master",
	"track",
	"parent",
	"shore",
	"division",
	"sheet",
	"substance",
	"favor",
	"connect",
	"post",
	"spend",
	"chord",
	"glad",
	"original",
	"share",
	"station",
	"bread",
	"charge",
	"proper",
	"offer",
	"segment",
	"slave",
	"duck",
	"instant",
	"market",
	"degree",
	"populate",
	"chick",
	"dear",
	"enemy",
	"reply",
	"drink",
	"occur",
	"support",
	"speech",
	"nature",
	"range",
	"steam",
	"motion",
	"path",
	"liquid",
	"meant",
	"quotient",
	"teeth",
	"shell",
	"neck",
    NULL
};

PBOARD createBoard(unsigned int width, unsigned int height)
{
    int allocatedRows = 0;

    if(width < MIN_BOARD_SIZE)
        width = MIN_BOARD_SIZE;
    if(height < MIN_BOARD_SIZE)
        height = MIN_BOARD_SIZE;

    PBOARD board = (PBOARD)malloc(sizeof(BOARD));
    if(NULL != board) {
        board->letters = (PLETTER*)malloc(sizeof(PLETTER) * height);
        if(NULL != board->letters) {
            board->width = width;
            board->height = height;

            for(allocatedRows = 0; allocatedRows < (int)height; ++allocatedRows) {
                printf("%u\n", allocatedRows);
                board->letters[allocatedRows] = (PLETTER)malloc(sizeof(LETTER) * width);
                if(NULL == board->letters[allocatedRows]) {
                    break;
                }
                for(unsigned int i = 0; i < width; ++i) {
                    board->letters[allocatedRows][i].isFound = 0;
                    board->letters[allocatedRows][i].word = NULL;
                }
            }
            if(NULL != board->letters[allocatedRows - 1])
                return board;

            for(;allocatedRows >= 0; --allocatedRows) free(board->letters[allocatedRows]);
            free(board->letters);
        }
        puts("free(board)");
        free(board);
    }
    puts("nope");
    return NULL;
}

void destroyBoard(PBOARD board)
{
    for(unsigned int i = 0; i < board->height; ++i) {
        free(board->letters[i]);
    }
    free(board->letters);
    free(board);
    return;
}

void fillBoard(PBOARD board)
{
    for(unsigned int y = 0; y < board->height; ++y) {
        for(unsigned int x = 0; x < board->width; ++x) {
            board->letters[y][x].letter = rand() % ('z' - 'a') + 'a';
        }
    }
}

void throwError(char * message)
{
    fprintf(stderr, "%s", message);
    exit(1);
}

int canBePlacedInBoard(PPOINT point, PBOARD board)
{
    if(point->X >= board->height || point->Y >= board->width)
        return 0;
    return 1;
}

void initTheGame(unsigned int width, unsigned int height, unsigned int * dictLength, PBOARD * board)
{
    srand(time(NULL));
    for(*dictLength = 0; NULL != dict[*dictLength]; ++*dictLength);
    *board = createBoard(width, height);
    if(NULL == *board) {
        throwError("Error while initializing the game. Terminating.");
    }
    fillBoard(*board);
}

char * getDirectionString(DIRECTION dir)
{
    switch(dir) {
        case HORIZONTAL:    return "horizontal";   break;
        case VERTICAL:      return "vertical";     break;
        case UP_DOWN:       return "up-down";      break;
        case DOWN_UP:       return "down-up";      break;
        default:            return "undefined!!";
    }
}

DIRECTION giveMeMyDirection(PPOINT start, PPOINT end)
{
    if(start->Y == end->Y) {
        return HORIZONTAL;
    }
    if(start->X == end->X) {
        return VERTICAL;
    }
    if(start->X < end->X && start->Y < end->Y) {
        return UP_DOWN;
    }
    if(start->X < end->X && start->Y > end->Y) {
        return DOWN_UP;
    }
    return UNDEFINED;
}

void printBoard(PBOARD board)
{
    PLETTER character = NULL;
    printf("    "); for(unsigned int i = 0; i < board->width; ++i)      {if(i / 10 > 0) putchar(i / 10 + '0'); else putchar(' '); putchar(' '); } putchar('\n');
    printf("    "); for(unsigned int i = 0; i < board->width; ++i)      {putchar(i % 10 + '0'); putchar(' '); } putchar('\n');
    printf("    "); for(unsigned int i = 0; i < board->width * 2; ++i)   putchar('\xC4'); putchar('\n');
    for(unsigned int y = 0; y < board->height; ++y) {
        printf("%-2u \xB3", y);
        for(unsigned int x = 0; x < board->width; ++x) {
            character = &(board->letters[y][x]);
            if(character->isFound) {
                putchar('\xB0');
                putchar('\xB0');
            }
            else {
                putchar(tolower(character->letter));
                putchar(' ');
            }
        }
        puts("\xB3");
    }
    printf("    "); for(unsigned int i = 0; i < board->width * 2; ++i) putchar('\xC4'); putchar('\n');
    return;
}

void debugPrintWords(PWORD words, unsigned int wordsCount)
{
    char * direction = NULL;
    puts("\nword        len direction  x : y - x : y  isFound");
    for(unsigned int i = 0; i < wordsCount; ++i) {
        direction = getDirectionString(words[i].direction);
        printf("%-11s %-3u %-10s %-2u:%-2u - %-2u:%-2u  %i\n",
                words[i].word,
                words[i].length,
                direction,
                words[i].start.X,
                words[i].start.Y,
                words[i].end.X,
                words[i].end.Y,
                words[i].isFound);
    }
}

void calculateWordEnd(PPOINT start, PPOINT end, unsigned int len, DIRECTION dir)
{
    switch(dir) {
        case HORIZONTAL:
            end->X = start->X + len - 1;
            end->Y = start->Y;
            break;
        case VERTICAL:
            end->X = start->X;
            end->Y = start->Y + len - 1;
            break;
        case UP_DOWN:
            end->X = start->X + len - 1;
            end->Y = start->Y + len - 1;
            break;
        case DOWN_UP:
            end->X = start->X + len - 1;
            end->Y = start->Y - len + 1;
            break;
        default:
            end->X = start->X;
            end->Y = start->Y;
            break;
    }
    return;
}

int isPointInBoard(PBOARD board, PPOINT point)
{
    if(point->X < board->width && point->Y < board->height) {
        return 1;
    }
    return 0;
}

int insertWordIntoBoard(PBOARD board, unsigned int index, DIRECTION dir, PPOINT start, PWORD wordInList)
{
    POINT current = {start->X, start->Y};
    if(NULL == wordInList)
        return 0;
    for(unsigned int i = 0; i < strlen(dict[index]); ++i) {
        board->letters[current.Y][current.X].letter = dict[index][i];
        board->letters[current.Y][current.X].word = wordInList;
        switch(dir) {
            case HORIZONTAL:
                ++current.X;
                break;
            case VERTICAL:
                ++current.Y;
                break;
            case UP_DOWN:
                ++current.X;
                ++current.Y;
                break;
            case DOWN_UP:
                ++current.X;
                --current.Y;
                break;
            default:
                return 0;
        }
    }
    return 1;
}

unsigned int min(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

unsigned int max(unsigned int a, unsigned int b)
{
    return (a > b) ? a : b;
}

int areWordsOverlaping(PBOARD board, PPOINT start, unsigned int len, DIRECTION dir)
{
    POINT current = {start->X, start->Y};
    for(unsigned int i = 0; i < len; ++i) {
        if(board->letters[current.Y][current.X].word != NULL) {
            return 1;
        }
        switch(dir) {
            case HORIZONTAL:
                ++current.X;
                break;
            case VERTICAL:
                ++current.Y;
                break;
            case UP_DOWN:
                ++current.X;
                ++current.Y;
                break;
            case DOWN_UP:
                ++current.X;
                --current.Y;
                break;
            default:
                return 0;
        }
    }
    return 0;
}

PWORD addWordToList(PBOARD board, PWORD words, unsigned int yetAdded, unsigned int index, PPOINT start, DIRECTION dir)
{
    if(yetAdded < MAX_WORDS_COUNT) {
        words[yetAdded].word = dict[index];
        words[yetAdded].start.X = start->X;
        words[yetAdded].start.Y = start->Y;
        words[yetAdded].length = strlen(dict[index]);
        words[yetAdded].direction = dir;
        calculateWordEnd(start, &(words[yetAdded].end), words[yetAdded].length, dir);
        for(unsigned int i = 0; i < yetAdded; ++i) {
            if(areWordsOverlaping(board, &words[yetAdded].start, words[yetAdded].length, words[yetAdded].direction))
                return NULL;
        }
        return &(words[yetAdded]);
    }
    return NULL;
}

int addWordToTheGame(PBOARD board, PWORD words, unsigned int yetAdded, unsigned int dictLength)
{
    POINT start = {rand() % board->width, rand() % board->height};
    POINT end = {0, 0};
    DIRECTION dir = HORIZONTAL;
    switch(rand() % 4)
    {
        case 0:
            dir = HORIZONTAL;
            break;
        case 1:
            dir = VERTICAL;
            break;
        case 2:
            dir = UP_DOWN;
            break;
        case 3:
            dir = DOWN_UP;
            break;
    }
    unsigned int index = rand() % dictLength;
    unsigned int len = strlen(dict[index]);

    calculateWordEnd(&start, &end, len, dir);
    if(     isPointInBoard(board, &start) &&
            isPointInBoard(board, &end) &&
            insertWordIntoBoard(board, index, dir, &start, addWordToList(board, words, yetAdded, index, &start, dir))) {
        return 1;
    }
    return 0;
}

unsigned int addManyWordsToTheGame(unsigned int maxCount, unsigned int dictLength, PBOARD board, PWORD words)
{
    unsigned int reallyAdded = 0;
    for(unsigned int watchDog = 0; watchDog < WATCHDOG_ALARM && reallyAdded < maxCount; ++watchDog) {
        if(addWordToTheGame(board, words, reallyAdded, dictLength)) {
            ++reallyAdded;
        }
    }
    return reallyAdded;
}

void waitForEnter()
{
    clearStdin();
    getchar();
}

void printHelp()
{
    clearConsole();
    puts("\n\nyawn by Igor Bagnucki");
    puts("Copyright 2016-2016\n");
    puts("q -- Exit yawn");
    puts("? -- Show this help");
    puts("s <start X> <start Y> <end X> <end Y> -- Select word");
    waitForEnter();
    clearConsole();
    clearStdin();
    return;
}

void clearStdin()
{
    fseek(stdin, 0, SEEK_END);
}

void clearConsole()
{
    puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

PWORD getWordOnCoord(PBOARD board, PPOINT start, PPOINT end)
{
    PWORD wordAtBegin = board->letters[start->Y][start->X].word;
    PWORD wordAtEnd = board->letters[end->Y][end->X].word;
    if(wordAtBegin == wordAtEnd)
        return wordAtBegin;
    return NULL;
}

void selectWordInBoard(PBOARD board, PWORD word)
{
    POINT current = {0, 0};
    current.X = word->start.X;
    current.Y = word->start.Y;
    for(unsigned int i = 0; i < word->length; ++i) {
        board->letters[current.Y][current.X].isFound = 1;
        switch(word->direction) {
            case HORIZONTAL:
                ++current.X;
                break;
            case VERTICAL:
                ++current.Y;
                break;
            case UP_DOWN:
                ++current.X;
                ++current.Y;
                break;
            case DOWN_UP:
                ++current.X;
                --current.Y;
                break;
            default:
                return;
        }
    }
    return;
}

int selectWord(PBOARD board, PPOINT start, PPOINT end)
{
    PWORD selectedWord = getWordOnCoord(board, start, end);
    if(NULL == selectedWord) {
        return 0;
    }
    selectedWord->isFound = 1;
    selectWordInBoard(board, selectedWord);
    return 1;
}

void handleWrongCommand()
{
    clearConsole();
    puts("Undefined command. Try \"?\".");
    waitForEnter();
    return;
}

void victory()
{
    puts("Congratulations! You found all words!");
    clearStdin();
    waitForEnter();
    return;
}

void letsTheGameBegin(PBOARD board, PWORD words, unsigned int wordsCount)
{
    UNUSED(words);
    int playing = 1;
    unsigned int wordsStillInBoard = wordsCount;
    char command = '\0';
    POINT selectStart = {0, 0};
    POINT selectEnd = {0, 0};
    while(playing) {
        clearConsole();
        if(wordsStillInBoard == 0) {
            victory();
            break;
        }
        printBoard(board);
#ifdef DEBUG
        debugPrintWords(words, wordsCount);
#endif //DEBUG
        if(wordsStillInBoard > 0) {
            printf("There are still %u words waiting for you!\n", wordsStillInBoard);
        }
        printf("(yawn) ");
        scanf(" %c", &command);
        switch(command) {
            case COMMAND_EXIT:
                playing = 0;
                break;
            case COMMAND_HELP:
                printHelp();
                break;
            case COMMAND_SELECT:
                scanf("%u%u%u%u", &selectStart.X, &selectStart.Y, &selectEnd.X, &selectEnd.Y);
                if(selectWord(board, &selectStart, &selectEnd))
                    --wordsStillInBoard;
                break;
            default:
                handleWrongCommand();
        }
    }
    return;
}

void getGameSize(unsigned int * width, unsigned int * height)
{
    unsigned int choose = 0;
    do {
        puts("What size of board do you want:\n[0] 11x11\n[1] 15x11\n[2] 11x15\n[3] 20x20\n[4] 20x15\n[5] custom");
        scanf("%u", &choose);
        switch(choose) {
            case 0:
                *width = 11;
                *height = 11;
                break;
            case 1:
                *width = 15;
                *height = 11;
                break;
            case 2:
                break;
                *width = 11;
                *height = 15;
            case 3:
                *width = 20;
                *height = 20;
                break;
            case 4:
                *width = 20;
                *height = 15;
                break;
            case 5:
                puts("What size of board do you want? Minimum is 11 x 11. Gimme X Y.");
                scanf("%u%u", width, height);
                break;
        }
    } while(choose > 5);
}

int main()
{
    unsigned int width = MIN_BOARD_SIZE;
    unsigned int height = MIN_BOARD_SIZE;
    unsigned int dictLength = 0;
    PBOARD board = NULL;
    WORD words[MAX_WORDS_COUNT];
    unsigned int wordsCount = 0;

#ifndef DEBUG_XSIZE
    getGameSize(&width, &height);
#else //!defined(DEBUG_XSIZE)
    width = DEBUG_XSIZE;
    height = DEBUG_YSIZE;
#endif //!defined(DEBUG_XSIZE)
    initTheGame(width, height, &dictLength, &board);
    wordsCount = addManyWordsToTheGame(rand() % 15 + 5, dictLength, board, words);
    letsTheGameBegin(board, words, wordsCount);
    destroyBoard(board);
    board = NULL;
    return 0;
}
