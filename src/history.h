
#include "common.h"
#define HISTNAME "/.lsh_hist"

FILE* histFile;
int histCount;

void writeHist(char *line);
void initHist();
void printHist();
int getHistItem(int pos, char **buffer);
void closeHist();
