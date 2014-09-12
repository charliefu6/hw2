#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

struct classroom
{
	std::string name;
	double work;
	double learning;
};

void isNumeric(std::string num) //checks each character of the string to see if it is numeric.
{
	unsigned int i;
	for(i=0; i < num.size(); i++)
	{
		if( num[i] < '0' || num[i] > '9')
		{
			std::cout << "Error- some data collected is non-numeric." << std::endl;
			exit (EXIT_FAILURE);
		}
	}
}

void isDouble(std::string num) //checks the string to see if it is a double.
{
	unsigned int i;
	bool dotCounter = false;
	for(i=0; i < num.size(); i++)
	{
		if( (num[i] < '0' || num[i] > '9') && num[i] != '.') // checks to make sure the string contains only numbers or periods.
		{
//			std::cout << num[i] << std::endl;
			std::cout << "Error- some data collected is non-numeric." << std::endl;
			exit (EXIT_FAILURE);
		}
		else if (num[i] == '.') // if the string encounters a . , set dotcounter to true. if another . is encountered, throw an error.
		{
			if (dotCounter == true)
			{
//				std::cout << num[i] << std::endl;
				std::cout << "Error- some data collected is non-numeric." << std::endl;
				exit (EXIT_FAILURE);
			}
			dotCounter=true;
		}
	}
}

double atod(std::string num) //converts a string to a double and returns the double
{
	int counter=-1;
	unsigned int i=0;
	double number=0;
	while(num[i] != '.' && i < num.size())
	{
		counter++;
		i++;
	}
	for(i=0; i<num.size(); i++)
	{
		if(num[i] != '.')
		{
			
			number += (num[i]-48)*pow(10,counter);
			counter--; 
//			std::cout << number << std::endl;
		}
	}
//	std::cout << number << std::endl;
	return number;
}

void checkLineNumber(int currentLine,int expectedLineTotal) //checks that the programs isn't reading more lines than specified
{
	if ( (currentLine) >= expectedLineTotal)
	{
		std::cout << "Error - More lines being read than specified in file" << std::endl;
		exit (EXIT_FAILURE);
	}
}

int getNumber(std::string line) //checks that the string is a positive integer and then returns it as an int
{
	isNumeric(line);
	return atoi(line.c_str());
}

double getDouble(std::string line) // same as previous but as a double.
{
	isDouble(line);
	return atod(line);
}

void getLineCount(std::string line, int *lineCount, double* workLim, classroom ** classArray) //reads all of the data in the first line and allocates it appropriately.
{
	std::istringstream iss;
	std::string numGetter;
	iss.str(line);
	iss >> numGetter;
	if(numGetter.empty()) //checks that the first line of data isn't empty
	{
		std::cout << "Error - First line of data is empty." << std::endl;
		exit(EXIT_FAILURE);
	} //takes the first data entry and uses assigns it to linecount. then dynamically allocates memory to an array of structs.
	*lineCount=getNumber(numGetter);
//	std::cout << *lineCount << std::endl;
	*classArray = new classroom [*lineCount];
	numGetter.erase(numGetter.begin(), numGetter.end());
	iss >> numGetter;
	if(numGetter.empty()) //checks that there are two pieces of data in the first line
	{
		std::cout << "Error - Not enough data in first line" << std::endl;
		exit(EXIT_FAILURE);
	}//the second input is assigned to your work limit.
	*workLim=getDouble(numGetter);
//	std::cout << *workLim << std::endl;
	numGetter.erase(numGetter.begin(), numGetter.end());
	iss >> numGetter;
	if (!(numGetter.empty()) ) //checks that the first line of data doesn't contain extra data.
	{
		std::cout << "Error - Too many arguments in first line." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void fillClassData(std::string line, classroom *classArray, int index) //uses a for loop to parse the information in the text file and allocate it appropriately.
{
	std::istringstream iss;
	iss.str(line);
	for(unsigned int i = 0; i < 4; i++)
	{
		std::string dGetter;
		iss >> dGetter;
		if(dGetter.empty() && i != 3)
		{
			std::cout << "Error - not enough data in lines." << std::endl;
			exit(EXIT_FAILURE);
		}	
		if( i == 0)
		{
			classArray[index].name = dGetter;
//			std::cout<< classArray[index].name << " ";
		}
		else if (i == 1)
		{
			classArray[index].work = getDouble(dGetter);
//			std::cout<< classArray[index].work << " ";
		}
		else if (i == 2)
		{
			classArray[index].learning = getDouble(dGetter);
//			std::cout<< classArray[index].learning << std::endl;
		}
		else if(!(dGetter.empty()) )
		{
			std::cout << "Error - Too many arguments in data." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

double auxCapLearning(classroom* classArray, int index, double t_Work, double workLimit, int numOfClasses)
{
	if(index == -1)
	{
//		std::cout << "entered encapsulated call, will now initiate recursive function call." << std::endl;
	}
	else t_Work += classArray[index].work;
	if (t_Work > workLimit) //base case if workLimit is exceeded
	{
		return 0;
	}
	double tempMaxLearning = 0;
	for(int i = index+1; i<numOfClasses; i++)
	{
		double nextLearning = auxCapLearning(classArray, i, t_Work, workLimit, numOfClasses);
		if(nextLearning > tempMaxLearning)
		{
			tempMaxLearning = nextLearning;
		}
//		std::cout << "New maxLearning is " << tempMaxLearning << std::endl;
	}
	return tempMaxLearning + classArray[index].learning; /*if index exceeds numberOfClasses the loop immediately exits and returns the last value.
								* otherwise, recursively returns the combination of the most recently added classes.
								*/
}

double maximizeLearning(classroom *classArray, double workLimit, int numOfClasses) //encapsulates the recursive function to make sure it doesn't get called with strange values.
{
//	std::cout << "entered the main call, proceeding to encapsulated call." << std::endl;
	return auxCapLearning(classArray, -1, 0,  workLimit, numOfClasses);
}	

int main (int argc, char *argv[])
{
	if (argc != 2) //checks the arguments in command line
	{
		std::cout << "Error - Incorrect number of arguments. When running this program, enter './<name of executable>' followed by only one text file. Eg. somefile.txt." << std::endl;
		exit (EXIT_FAILURE);
	}
	else
	{
		std::ifstream text (argv[1]);
		if ( !text.is_open() ) //checks that the file was successfully opened/exists.
		{
			std::cout << "Error - Could not open file" << std::endl;
			exit (EXIT_FAILURE);
		}
		else
		{
			std::string line;
			int lineCount = 0;
			int lineNumber = 0;
			double workLimit = 0;
			classroom *classArray;
			bool numRead = false;
			while ( getline( text, line ) )
			{
				if (numRead == false) //tells program to parse the first line differently than the other
				{
					getLineCount(line, &lineCount, &workLimit, &classArray);
					numRead = true; //tells program that the first line of text has been read
					getline(text, line);
				} //reads the rest of the text file appropriately
				checkLineNumber( lineNumber, lineCount );
				fillClassData(line, classArray, lineNumber);
				lineNumber++;
				//std::cout << lineNumber << std::endl;
			}
			if (lineNumber != lineCount)
			{
				std::cout << "Error- Lines read is less than lines specified." << std::endl;
				exit(EXIT_FAILURE);
			}
//			std::cout << "sample potential learning = " << classArray[1].learning << std::endl;
			double maxLearning = 0;
			maxLearning = maximizeLearning(classArray, workLimit, lineCount);
			std::cout << "Maximum learning: " << maxLearning << std::endl;
		delete []classArray;
		}
	}
	return 0;
}
