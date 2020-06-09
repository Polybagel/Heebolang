#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <windows.h>

int bufferIndex = 0;
int memory[256];
int loadedData = 0;

const int canvasHeight = 15;
const int canvasWidth = 30;

int pixels[canvasWidth][canvasHeight];

unsigned char b0 = ' ';
unsigned char b1 = unsigned char(176);
unsigned char b2 = unsigned char(177);
unsigned char b3 = unsigned char(178);
unsigned char b4 = unsigned char(219);

unsigned char brightnesses[5] = {b0,b1,b2,b3,b4};

struct coords
{
    int x;
    int y;
};

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

unsigned char getColor(int b)
{
    return brightnesses[b];
}

void RenderCanvas()
{
    for (int y = 0; y < canvasHeight; y++)
    {
        for (int x = 0; x < canvasWidth; x++)
        {
            std::cout << getColor(pixels[x][y]);
        }
        std::cout << std::endl;
    }
}

void PaintPixel(int x, int y, int b)
{
    if (x > canvasWidth) return; //is the pixel out of bounds? if so, ignore it.
    if (x < 0) return;
    if (y > canvasHeight) return;
    if (y < 0) return;

    //if not, set the pixel.
    pixels[x][y] = b;
}

coords ParsePixelCoords(std::string c, int ind)
{
    coords result;
    result.x = 0;
    result.y = 0;

    std::string xStr = "";
    std::string yStr = "";

    int progress = 0;

    for (int i = ind; i < c.length(); i++)
    {
        char currentChar = c[i];
        if (currentChar == '|') { progress++; }

        if (currentChar != '{' && currentChar != '}' && currentChar != '|')
        {
            if (currentChar != 'i')
            {
                if (progress == 0) { xStr += currentChar; }
                if (progress == 1) { yStr += currentChar; }
            }
            else
            {
                if (progress == 0) { 
                    result.x = memory[bufferIndex]-1; 
                }
                if (progress == 1) { 
                    result.y = memory[bufferIndex]-1; 
                }
            }
        }

        if (currentChar == '}')
        {
            return result;
        }

        if (xStr.length() > 0 && yStr.length() > 0)
        {
            result.x = std::stoi(xStr);
            result.y = std::stoi(yStr);
        }
    }
    return result;
}

void interprete(std::string c)
{
    int loopBegin = 0; //the start of the current loop

    //variables for storing file operations and where they are located in the code
    int fileOperationStartIndex = 0;
    std::string filePath;
    bool parsingPath = false;

    //variables for keeping track of graphics operations
    int paintPixelStartIndex = 0;
    bool parsingCoords = false;

    for (int i = 0; i < c.length(); i++)
    {
        char command = c[i]; //current character being interpreted

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
            case '@':
                system("cls");
                RenderCanvas();
                break;
            case '&':
                for (int x = 0; x < canvasWidth; x++)
                {
                    for (int y = 0; y < canvasHeight; y++)
                    {
                        pixels[x][y] = 0;
                    }
                }
                break;
            case 'P':
                paintPixelStartIndex = i + 1;
                parsingCoords = true;
                coords paintCoords = ParsePixelCoords(c, paintPixelStartIndex);
                PaintPixel(paintCoords.x, paintCoords.y, 1);
                parsingCoords = false;
                break;
            case '=':
                std::string inp = " ";
                std::cin >> inp;

                memory[bufferIndex] = static_cast<unsigned char>(inp[0]);
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

    std::cout << "Decrement cell: :" << std::endl;
    std::cout << "Increment cell: ;" << std::endl;

    std::cout << std::endl;

    std::cout << "Loop: ()" << std::endl;

    std::cout << std::endl;

    std::cout << "Output current cell data(as integer): `" << std::endl;
    std::cout << "Output current cell data(as ascii) : -" << std::endl;
    std::cout << "Take single key stroke and store its ASCII value into current cell: =" << std::endl;

    std::cout << std::endl;

    std::cout << "Load data from file into current cell: R[FILE PATH]" << std::endl;
    std::cout << "Write current cell data to file: W[FILE PATH]" << std::endl;

    std::cout << std::endl;

    std::cout << "Clear Canvas: &" << std::endl;
    std::cout << "Render Canvas: @" << std::endl;
    std::cout << "Paint Pixel: P{X|Y}" << std::endl;

    std::cout << std::endl;
}

int main()
{
    SetConsoleTitle(TEXT("Heebolang Interpreter")); //Set console title


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