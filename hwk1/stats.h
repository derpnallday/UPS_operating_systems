
#include <stdio.h>
#include <string.h>
#define ALPHABET_SIZE 26

typedef struct WordStats {
	int histo[ALPHABET_SIZE];
	int wordCount;
	int vowelCount;
	int consonantCount;
} WordStats;

WordStats setHisto(WordStats w);
WordStats update(char str[], WordStats w);
void getStats(WordStats w);
void getWordCount(WordStats w);
void histogram(WordStats w);