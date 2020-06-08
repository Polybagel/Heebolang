#include <iostream>
#include <string>
#include <stdlib.h>

int bufferIndex;
int memory[256];

char commands[8] = { ',','.',':',';','(',')','`','-' };

void interprete(std::string c)
{
    int loopBegin = 0; //the start of the current loop

    for (int i = 0; i < c.length(); i++)
    {
        char command = c[i]; //current character being interpreted

        switch (command)
        {
        case ',':
            bufferIndex--;
            break;
        case '.':
            bufferIndex++;
            break;
        case ':':
            memory[bufferIndex]--;
            break;
        case ';':
            memory[bufferIndex]++;
            break;
        case '(':
            loopBegin = i;
            break;
        case ')':
            if (memory[bufferIndex] != 0)
            {
                i = loopBegin;
            }
            break;
        case '`':
            std::cout << memory[bufferIndex] << std::endl;
            break;
        case '-':
            std::cout << char(memory[bufferIndex]) << std::endl;
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

    std::cout << "decrement cell: :" << std::endl;
    std::cout << "increment cell: ;" << std::endl;

    std::cout << "loop: ()" << std::endl;

    std::cout << "output current cell data(as integer): `" << std::endl;
    std::cout << "output current cell data(as ascii) : -" << std::endl;

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