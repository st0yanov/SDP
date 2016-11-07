#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define FILENAME "input.txt"

/*
 * This function is used to get the number of words contained in the file.
 */
int countWords() {
  ifstream file(FILENAME);
  int counter = 0;

  if(file.is_open()) {
    string line;

    while(getline(file, line)) counter++;

    file.close();
  } else {
    cout<<"Unable to open file \""<<FILENAME<<"\"."<<endl;
  }

  return counter;
}

/*
 * This function is used to read the words and save them in a referenced string variable.
 */
void readWords(string *words) {
  ifstream file(FILENAME);

  if(file.is_open()) {
    string line;
    int i = 0;

    while(getline(file, line)) {
      words[i] = line;
      i++;
    }

    file.close();
  } else {
    cout<<"Unable to open file \""<<FILENAME<<"\"."<<endl;
  }
}

/*
 * This function is used to convert all chars of a string to lowercase.
 */
void toLower(string &word) {
  string lower_alphabet = "abcdefghijklmnopqrstuvwxyz";
  string higher_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  size_t word_length = word.length();
  size_t alphabet_length = higher_alphabet.length();

  /*
   * The idea here is to search for uppercase letters by looping through the
   * higher alphabet string. If we find a match we can break the current
   * iteration of the inner (alphabet) loop.
   */
  for(int i=0; i<word_length; i++) {
    for(int p=0; p<alphabet_length; p++) {
      if(word[i] == higher_alphabet[p]) {
        word[i] = lower_alphabet[p];
        break;
      }
    }
  }
}

/*
 * This function is used to sort all characters of a string in alphabetical order.
 */
void sortWordChars(string &word) {
  size_t word_length = word.length();
  char tmp;

  /*
   * Since the characters can be represented by an integer and also their
   * numeric representation is higher for each consequent alphabet character
   * we can easily iterate through the chars and sort them alphabetically.
   */
  for(int i=0; i<word_length-1; i++) {
    for(int k=i+1; k<word_length; k++) {
      int char_diff = word[i] - word[k];
      if(char_diff > 0) {
        tmp = word[i];
        word[i] = word[k];
        word[k] = tmp;
      }
    }
  }
}

/*
 * This function is used to prepare the words - first make them lowercase and
 * after that sort them using the helper functions.
 */
void prepareWords(string *words, int words_size) {
  for(int i=0; i<words_size; i++) {
    toLower(words[i]);
    sortWordChars(words[i]);
  }
}

int main() {
  int anagram_counter = 0; // We'll count the anagrams here...
  int remove_counter = 0;  // ...and keep an eye on the already counted ones.

  int words_size = countWords();
  string words[words_size];

  readWords(words);
  prepareWords(words, words_size);

  /*
   * We want to compare all the words with the others below the current word.
   */
  for(int i=0; i<words_size-1; i++) {
    // For each word we assign a flag to determine whether the word was already counted as anagram.
    bool counted = false;

    /*
     * Then we can simply iterate through the next words and compare them to the
     * current one. Since all of them are sorted alphabetically we can easily
     * determine that they are anagrams. We also assign the counted flag when
     * we find an anagram. If the flag is assign and we continue to find
     * anagrams, we just increment a separate counter which we will subtract.
     */
    for(int k=i+1; k<words_size; k++) {
      if(words[i] == words[k]) {
        if(counted) {
          remove_counter++;
        } else {
          anagram_counter++;
          counted = true;
        }
      }
    }
  }

  // Finally we can show the result of unique anagrams.
  cout<<anagram_counter-remove_counter<<endl;

  return 0;
}
