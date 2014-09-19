#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <ctime>

void isNumeric(std::string num) //Checks to see if string input is numeric
{
	unsigned int i;
	for(i=0; i < num.size(); i++)
	{
		if( num[i] < '0' || num[i] > '9')
		{
			std::cout << "Error- some data collected is non-numeric." << std::endl;
			exit (EXIT_FAILURE);
		}
		/* checks each character in the string.
		* if the character is non-numeric, output error message and exit program.
		*/
	}
}

void isInteger(std::string num) //Checks to see if string input is an integer (has a negative sign in front)
{
	if (num[0] == '-')
	{
		std::string numCopy=num;
		numCopy.erase(numCopy.begin());
		isNumeric(numCopy);
		//if the first character is '-', put the rest of the string into a copy and check if it's numeric (call isNumeric with the rest of the string)
	}
	else
	{
		isNumeric(num);
		//if the first character isn't '-', call isNumeric with the given string
	}
}

int getNumber(std::string line) //checks if the string passed is an integer and then converts it into an integer before returning it.
{
	isInteger(line);
	return atoi(line.c_str());
}		

void checkLineNumber(int currentLine,int expectedLineTotal) //checks to see if the program is reading more lines than specified at runtime. If so, throws an error and exits program
{
	if ( (currentLine) >= expectedLineTotal)
	{
		std::cout << "Error - More lines being read than specified in file" << std::endl;
		exit (EXIT_FAILURE);
	}
}

void getLineCount(std::string line, int *lineCount, int ** numberArray) //reads the first line of the text file to find how many lines are in the file.
{
	std::istringstream iss;
	std::string numGetter;
	iss.str(line);
	iss >> numGetter;
	if(numGetter.empty()) //checks to make sure that the first line of the text file actually has data. if not, throws an error
	{
		std::cout << "Error - First line of data is empty." << std::endl;
		exit(EXIT_FAILURE);
	} //otherwise, extract the number of lines and uses it to dynamically allocate memory into an array of numbers.
	*lineCount= getNumber(numGetter);
	//std::cout << *lineCount << std::endl;
	*numberArray = new int [*lineCount];
	numGetter.erase(numGetter.begin(), numGetter.end());
	iss >> numGetter;
	if (!(numGetter.empty()) ) //checks to make sure there isn't extraneous data in the first line of the text file. throws an error if there is an issue.
	{
		std::cout << "Error - Too many arguments in first line." << std::endl;
		exit(EXIT_FAILURE);
	}
}

int iterativeAdd(int* numberArray, int size) //uses a for loop to iteratively increment or decrement i to the specified value and return it.
{
	int sum=0;
	int i = 0;
	while(i < size)
	{
		sum += numberArray[i];
		i++;
	}
	return sum;
}

int recursiveAdd(int* numberArray, int size) //uses a recursive call to increment or decrement i to the specified value and return it.
{
	if( size != -1)
		return numberArray[size] + recursiveAdd(numberArray, size-1);
	else
		return 0;
}

int main (int argc, char *argv[])
{
	if (argc != 2) //checks number of command-line arguments
	{
		std::cout << "Error - Incorrect number of arguments. When running this program, enter './<name of executable>' followed by only one text file. Eg. somefile.txt." << std::endl;
		exit (EXIT_FAILURE);
	}
	else
	{
		std::ifstream text (argv[1]);
		if ( !text.is_open() ) //checks if the text file was successfully opened/exists.
		{
			std::cout << "Error - Could not open file" << std::endl;
			exit (EXIT_FAILURE);
		}
		else
		{
			std::string line;
			int lineCount = 0;
			int lineNumber = 0;
			int * numberArray;
			bool numRead = false;
			while ( getline( text, line ) )
			{
				if (numRead == false) //tells the program to read the parse the first line of data differently than the other.
				{
					getLineCount(line, &lineCount, &numberArray); //dynamically allocates the number of integers into an array.
					numRead = true; // tells the program that the first line has been read, and that we don't need to execute it anymore.
					getline( text, line );
				}
				//reads the rest of the lines and puts those numbers into the array.
				checkLineNumber( lineNumber, lineCount );
				numberArray[lineNumber] = getNumber( line );
				lineNumber++;

			}
			if(lineNumber != lineCount)
			{
				std::cout << "Finished reading lines. Number of lines read does not match number of lines specified. Continuing program and outputting word count in reverse." << std::endl;
			}
/*
			for (lineNumber=lineNumber-1; lineNumber >= 0; lineNumber--)
			{
				std::cout << numberArray[lineNumber] << std::endl;
				if (lineNumber != 0)
				{
					std::cout << std::endl;
				}
			}
*/
		int iterativeSum = 0, recursiveSum = 0;
		clock_t start, finish;
		double iterativeDuration, recursiveDuration;
		int i;
		int k;
		start = clock();
		for (k=0; k < 200000000; k++) //iteratively adds the numbers in the array k times and calculates the time taken
		{
			iterativeSum=iterativeAdd(numberArray, lineNumber);
		}
		finish = clock();
		iterativeDuration = (double) (finish-start);
		iterativeDuration /= CLOCKS_PER_SEC;
		std::cout << "Iterative sum is " << iterativeSum << std::endl;
		std::cout << "Elapsed seconds: " << iterativeDuration << std::endl;
		start = clock();
		for (k = 0; k < 200000000; k++) //recursively adds the numbers in the array k times and calculates the time taken.
		{
			recursiveSum=recursiveAdd(numberArray, lineNumber-1);
		}
		finish = clock();
		std::cout << "recursive sum is " << recursiveSum << std::endl;
		if (recursiveSum != iterativeSum) //throws an error if the iterative and recursive sums are not equal.
		{
			std::cout << "iterative and recursive sums not equal." << std::endl;
		}
		recursiveDuration = (double) (finish-start);
		recursiveDuration /= CLOCKS_PER_SEC;
		std::cout << "Elapsed seconds: " << recursiveDuration << std::endl;
		//std::cout << "Recursive duration is " << recursiveDuration-iterativeDuration << " seconds slower than the iterative duration." << std::endl <<
		//"Iterative addition is " << recursiveDuration/iterativeDuration*100 << '%' << " faster." << std::endl;

		delete []numberArray;
		}
	}
	return 0;
}
