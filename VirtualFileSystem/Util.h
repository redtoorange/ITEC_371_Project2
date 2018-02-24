/*
 *	Andrew McGuiness
 * ITEC 371 - Project 1
 * 2/8/2018
*/

#ifndef UTIL_H
#define UTIL_H

#include <string>
/*!
*	\brief Contains some generic useful stuff that will be
*	referenced throughout the program.
*/

const std::string PROG_EXTENSION = ".p";
const std::string TEXT_EXTENSION = ".t";

std::string cmd[] = {
	"CreateDir", 
	"CreateFile", 
	"EndDir", 
	"Quit",
	"ls",
	"pwd"
};

enum Commands : int{
	C_DIR = 0,
	C_FILE = 1,
	E_DIR = 2,
	QUIT = 3,
	LIST = 4,
	PWD = 5
};

#endif