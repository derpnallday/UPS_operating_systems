#include "stats.h"

WordStats setHisto(WordStats w){
	int i = 0;

	while(i<ALPHABET_SIZE){
		w.histo[i]=0;
		i++;
	}
	return w;
}


int wordCount(char str[]){
	int c = 0;
	char * pch;
	pch = strtok(str," ");
	
	//loop through tokens
	while (pch != NULL){	
		int symbol = 1;
		int i =0;
		//check for special char
		while(pch[i]){
			//check if char symbol outside latin char range
			if(pch[i] < 65 || pch[i] > 122){
				symbol--;
				break;
			}
			i++;
		}
		//if there was a symbol in word then not word
		if(symbol){
			c++;
		}
		pch = strtok(NULL, " ");
	}
	return c;
}


WordStats update(char str[], WordStats w) {

	int i = 0;
	int vowel = 0;
	int space = 0;
	int index = 0;

	//get word count
	w.wordCount += wordCount(str);


	//loop through all chars
	while (str[i]){

		//convort char to int
		int x = (int) str[i];

		//printf("loop %d with char %c\n with asci %d\n",i, str[i], x);

		//convort upper to lower case
		if (x >= 65 && x <= 90){
			x += 32;
		}

		//increment histogram counters
		if (x >= 97 && x <= 122){
			//index for histogram
			index = x - 97;

			//update histo index
			w.histo[index]++;
			//printf("increment index %d", index);
		}


		//check for vowels asci code
		if(x == 97 || x == 101 || x == 105 || x == 111 || x == 117){
			vowel++;
		} 

    	//check for asci space
		if(x == 32)
			space++;

    	//increment counter
		i++;
	}
    str[i] = '\0';    //null-terminate buf

    //vowel count is vowels tracked
    w.vowelCount += vowel;
    //consonant is total characters - spaces and vowels
    w.consonantCount += i - vowel - space;
    //word count function

    return w;



}



void getStats(WordStats w){
	int total = w.vowelCount + w.consonantCount;

	float vow = (w.vowelCount*1.0) / total;
	float con = (w.consonantCount*1.0) / total;

	printf("Vowels = %d (%f%%), Consonants = %d (%f%%), Total= %d\n\n", w.vowelCount, vow, w.consonantCount, con, total);
}

void getWordCount(WordStats w){
	printf("%d\n\n",w.wordCount);
}



void histogram(WordStats w){
	int height = 0;
	int i=0;


	while(i<ALPHABET_SIZE){
		if(height< w.histo[i]){
			height = w.histo[i];
		}
		i++;
	}

	int curHeight = height;

	while(curHeight != 0){
		char str[ALPHABET_SIZE];

		//position in string
		int c = 0;

		while(c<ALPHABET_SIZE){
			//if freq >= height add *
			if(curHeight <= w.histo[c]){
				str[c] = '*';
			}else{
				str[c] = ' ';
			}
			c++;
		}
		str[c] = '\0';
		printf("%s\n",str);
		curHeight--;
	}

	printf ("abcdefghijklmnopqrstuvwxyz\n");

	i=0;
	char freq[ALPHABET_SIZE];

	while(i<ALPHABET_SIZE){
		freq[i] = w.histo[i]+ '0';
		i++;
	}
    freq[i] = '\0';

	printf("%s\n\n",freq);

}








/*
17 Enter 1 to print vowel and consonant frequency.
 18 Enter 2 to print word count.
 19 Enter 3 to print histogram.
 20 Enter 4 to return to inputting more strings.
 21 Enter 5 to quit.
 */