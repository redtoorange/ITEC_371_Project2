/*
 *	Andrew McGuiness
 *	ITEC 371 - Project 2
 *	3/2/2018
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

//! Contains the string literal version of all commands
const std::string cmd[] = {
	"pwd",
	"ls",
	"mkdir",
	"cat",
	"createTextfile",
	"run",
	"start",
	"step",
	"cd",
	"quit",
};

//! Contains the enumerated version of all commands.
enum Commands : int{
	PWD = 0,
	LIST = 1,
	MKDIR = 2,
	CAT = 3,
	CREATE_TEXT = 4,
	RUN = 5,
	START = 6,
	STEP = 7,
	CD = 8,
	QUIT = 9
};

//! Check two string to see if they are equal, ignoring case.
bool equalIC( const std::string& a, const std::string& b);

#endif