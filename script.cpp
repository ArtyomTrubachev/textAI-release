#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <fstream>

using namespace std;
using namespace cgicc;

const int N = 99999;
const int SHINGLE_LENGTH = 3;
char LatinianLetter[] = "AaBCcEeHKkMmOoPpTXxYy";
char RussianLetter[] = "АаВСсЕеНКкМмОоРрТХхУу";
char word[N];
char charOriginalText[N];
char anotherText[N];
string arrayWordsOriginalText[N];
string arrayWordsAnotherText[N];
string shingleOriginal[N];
string shingleAnother[N];

double anitPlagiarism(string text, string fragment);
bool isSeparator (char c);
void stringToCharsArray(char textArray[], string text);
void toCirilicGetWords (char text[], char Latinian[], char Russian[]);
void toLowerCase (char text[]);
void getWords(char text[], string recordArray[]);
void getShingles(string shingleArray[],string arrayCanonizedWords[], int numberWords);
int countWords(string array[]);
double countplagiat(string arrayShinglesOriginal[], string arrayShinglesAnother[], int numberShinglesOriginal, int numberShinglesAnother);

int getInt(string name);//get length of text putted in the form on your site(this function we use only for example)
string getDB();//get origin text from db.txt (don't modify tis function)

int main()
{
	setlocale(LC_ALL, "Russian");
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";
    
    name = form("name");
    if (!name.empty()) {
    	cout << round(anitPlagiarism(getDB(), name))<< "\n";
    } else {
    	cout << "Text is not provided!\n";
    }	
    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;				
}

string getDB(){
	std::ifstream in("db.txt");

    string dbText = "", s1;
    while (getline(in, s1))
    	dbText += s1;
  
    in.close();
    
    return dbText;
}
//-----------Функция антиплагиаризм-------------------------------------------
double anitPlagiarism(string text, string fragment)
{	
	stringToCharsArray(charOriginalText, text);	
	stringToCharsArray(anotherText, fragment);		
				
	toCirilicGetWords(charOriginalText, LatinianLetter, RussianLetter);
	toCirilicGetWords(anotherText, LatinianLetter, RussianLetter);
	
 	toLowerCase(charOriginalText);	
 	toLowerCase(anotherText);	
 	
	getWords(charOriginalText, arrayWordsOriginalText);
	getWords(anotherText, arrayWordsAnotherText);
	
	getShingles(shingleOriginal, arrayWordsOriginalText, countWords(arrayWordsOriginalText));
	getShingles(shingleAnother, arrayWordsAnotherText, countWords(arrayWordsAnotherText));
	
	return countplagiat(shingleOriginal, shingleAnother, countWords(shingleOriginal), countWords(shingleAnother));
}
//-----------Изменение текста STRING В CHAR-------------------------------------------
void stringToCharsArray(char textArray[], string text)
{
	int i = 0;
	for (i = 0; i < text.length(); i++)
		textArray[i] = text[i];
	textArray[i] = '\0';
}
//-----------Преобразование ВЕРХНЕГО РЕГИСТРА БУКВ В НИЖНИЙ---------------------------
void toLowerCase(char text[])
{
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] >= -64 and text[i] <= -33) 
			text[i] = 32 + text[i]; 
		if (text[i] == -88) 
			text[i] = -72; 		
	} 							
}
//-----------Замена букв из латинского алфавита на русские буквы, если встречаются----
void toCirilicGetWords(char text[], char Latinian[], char Russian[])
{
	for (int i = 0; text[i] != '\0'; i++) 
		for (int j = 0; Latinian[j] != '\0'; j++)
			if (text[i] == Latinian[j])
				text[i] = Russian[j];
}
//-----------Разбивка на слова-------------------------------------------------
void getWords(char text[], string recordArray[])
{
	int index = 0;
	int iw = 0;
	for (int i = 0; text[i] != '\0'; i++) {
		if (!isSeparator(text[i])) {
			word[iw] = text[i];
			iw++;
			if (isSeparator(text[i+1]) or text[i+1] == '\0') {
				word[iw] = '\0';
				iw = 0;
				recordArray[index] = word;
				index++;
			}			
		}	
	}	
}	
//-----------Разделение на слова-------------------------------------------------
bool isSeparator(char c)
{
	char separator[] = " .,:;""''()!&/-";
	for (int i =0; separator[i] != '\0'; i++)
		if (c == separator[i])
			return true;
	return false;
}	
//-----------Разделение на шинглы-------------------------------------------------
void getShingles(string shingleArray[],string arrayCanonizedWords[], int numberWords)
{
	for (int i = 0; i < numberWords - (SHINGLE_LENGTH -1) ; i++) {
		int countWordsInShingle = 0;
		int j = i;
		while (countWordsInShingle < SHINGLE_LENGTH) {
			shingleArray[i] += arrayCanonizedWords[j];
			countWordsInShingle++;
			j++;				
		}
	}
}
//-----------Посчитать кол-во слов в массиве string--------------------------------
int countWords(string array[])
{
	int count = 0;
	for (int i = 0; array[i] != ""; i++) 
		count++;
	return count;
}	
//-----------Расчет плагиата--------------------------------------------------------
double countplagiat(string arrayShinglesOriginal[], string arrayShinglesAnother[], int numberShinglesOriginal, int numberShinglesAnother)
{
	double matchesCount = 0;
	double percentUnic = 0.0;
	for (int i = 0; i < numberShinglesAnother; i++) 
		for (int j = 0; j < numberShinglesOriginal; j++) 	
			if (arrayShinglesAnother[i] == arrayShinglesOriginal[j]) 
				matchesCount++;
	percentUnic = 100 - matchesCount / numberShinglesAnother * 100.0;
	return percentUnic;	
}	

