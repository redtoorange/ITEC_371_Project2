/*
 *	Andrew McGuiness
 * ITEC 371 - Project 1
 * 2/8/2018
*/

#ifndef PROGRAM_FILE_H
#define PROGRAM_FILE_H

#include "File.h"
#include <memory>

/*!
*	\brief A special type of File that is designed to hold 
*	metadata about a program.
*/
class ProgramFile : public File
{
public:
	//! Create a new ProgramFile given a filename, user will be prompted for input
	static std::shared_ptr<ProgramFile> makeProgramFile( const std::string& name );
	
	//! Inflate a stored ProgramFile back into a full file
	static std::shared_ptr<ProgramFile> inflateProgramFile( std::string& name, std::ifstream& stream );

	//! Ensure that the supplied name is valid
	static bool validName( std::string* name);

	//! Should NOT be used outside of the class
	ProgramFile(const std::string& name, int cpuReq, int memReq);

	//! Formatted data printer
	void printData(int tabs) override;
	
	//! Write the program file out to a stream
	void writeToFile(std::ofstream& stream) override;
	
private:
	int cpuRequirements;
	int memoryRequirements;
};

#endif