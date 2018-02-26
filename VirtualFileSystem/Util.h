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
	"pwd",
	"ls",
	// cd <dir>
	// cd ..
	"mkdir",
	"cat",
	"createTextfile",
	//run <program>
	//start <program>
	//step <program>
	"quit",
};

enum Commands : int{
	PWD = 0,
	LIST = 1,
	MKDIR = 2,
	CAT = 3,
	CREATE_TEXT = 4,
	QUIT = 5
};

#endif