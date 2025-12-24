// ChessEngineCore.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
	std::cout << "This is a simple C++ program to demonstrate the structure of a C++ application.\n";

	uint8_t black_pawn = 8;
	uint8_t black_knight = 9;
	uint8_t black_bishop = 10;
	uint8_t white_bishop = 2;
	std::cout << static_cast<int>(black_pawn & 8);
	std::cout << static_cast<int>(black_knight & 8);
	std::cout << static_cast<int>(black_bishop & 8);
	std::cout << static_cast<int>(white_bishop & 8);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
