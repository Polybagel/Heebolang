#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

int bufferIndex = 0;
int memory[256];
int loadedData = 0;

std::string parsePath(std::string c, int startingIndex)
{
    std::string result = "";

    for (int i = startingIndex+1; i < c.length(); i++)
    {
        char currentChar = c[i];

        if (currentChar == ']')
        {
            return result;
        }
        else
        {
            result += currentChar;
        }
    }

    return result;
}

int ReadFileToCell(std::string path)
{
    std::ifstream myfile;
    myfile.open(path);
    std::string line;
    int dataRead = 0;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            dataRead = std::stoi(line);
            memory[bufferIndex] = dataRead;
        }
        myfile.close();
    }
    else
    {
        std::cout << "Error reading file!" << std::endl;
    }
    return dataRead;
}
void WriteCellToFile(std::string path)
{
    std::ofstream myfile(path);
    if (myfile.is_open())
    {
        myfile << memory[bufferIndex];
        myfile.close();
    }
}

void interprete(std::string c)
{
    int loopBegin = 0; //the start of the current loop

    for (int i = 0; i < c.length(); i++)
    {
        char command = c[i]; //current character being interpreted

        //variables for storing file operations and where they are located in the code
        int fileOperationStartIndex = 0;
        std::string filePath;
        bool parsingPath = true;

        switch (command)
        {
            case ',':
                if (!parsingPath) { bufferIndex--; }
                break;
            case '.':
                if (!parsingPath) { bufferIndex++; }
                break;
            case ':':
                if (!parsingPath) { memory[bufferIndex]--; }
                break;
            case ';':
                if (!parsingPath) { memory[bufferIndex]++; }
                break;
            case '(':
                loopBegin = i;
                break;
            case ')':
                if (memory[bufferIndex] != 0) { i = loopBegin; } //loop back to the beginning if the conditional is false
                break;
            case '`':
                std::cout << memory[bufferIndex] << std::endl; //print the integer value
                break;
            case '-':
                std::cout << char(memory[bufferIndex]) << std::endl; //print the corresponding ASCII character
                break;
            case 'R':
                parsingPath = true; //start parsing the path
                fileOperationStartIndex = i + 1; //set the index of the path, making sure to remove the [ character

                filePath = parsePath(c, fileOperationStartIndex); //parse the path from the code

                ReadFileToCell(filePath); //read the content of the file into memory

                fileOperationStartIndex = 0; //reset the index of the path
                break;
            case 'W':
                fileOperationStartIndex = i + 1;

                filePath = parsePath(c, fileOperationStartIndex);

                WriteCellToFile(filePath);

                fileOperationStartIndex = 0;
                break;
            case ']':
                parsingPath = false; //if we've hit the end of the path container, now we can read the rest of the code
                break;
        }
    }
}

void printHelp()
{
    std::cout << "Welcome to Heebolang! Created by Polybagel" << std::endl;

    std::cout << std::endl;

    std::cout << "Shift bit register left: ," << std::endl;
    std::cout << "Shift bit register right: ." << std::endl;

    std::cout << std::endl;

    std::cout << "decrement cell: :" << std::endl;
    std::cout << "increment cell: ;" << std::endl;

    std::cout << std::endl;

    std::cout << "loop: ()" << std::endl;

    std::cout << std::endl;

    std::cout << "output current cell data(as integer): `" << std::endl;
    std::cout << "output current cell data(as ascii) : -" << std::endl;

    std::cout << std::endl;

    std::cout << "Load data from file into current cell: R[FILE PATH]" << std::endl;
    std::cout << "Write current cell data to file: W[FILE PATH]" << std::endl;

    std::cout << std::endl;
}

int main()
{
    std::string code; //where the written code is stored

    printHelp(); //print the help screen at the start of the program

    while (1) //infinite loop obviously lmao
    {
        std::cout << ": ";
        std::cin >> code;

        interprete(code); //run the interpreter and also reset the code

        code = "";
    }
}