/*
 *	Andrew McGuiness
 * ITEC 371 - Project 1
 * 2/8/2018
*/

#include "ProgramFile.h"
#include <iostream>
#include <string>

std::shared_ptr<ProgramFile> ProgramFile::makeProgramFile( const std::string& name )
{
	// Check the name of the file
	std::string n = name;
	if( !validName(&n) )
	{
		std::cout << "Invalid program name!" << std::endl;
	}

	int c;
	int m;
	
	// Error correction for the values
	while( true ){
		std::cout << "Enter CPU requirements>";
		std::cin >> c;
		
		if( std::cin.fail() ){
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Invalid Input\n";
		}
		else break;
	}
	
	while( true ){
		std::cout << "Enter memory requirements>";
		std::cin >> m;
		
		if( std::cin.fail() ){
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Invalid Input\n";
		}
		else break;
	}

	// Ensure that all the garbage in the buffer is clear
	std::cin.clear();
	std::cin.ignore(10000, '\n');

	// Send back out new Prog
	return std::make_shared<ProgramFile>(n, c, m);
}

ProgramFile::ProgramFile( const std::string& name, int cpuReq, int memReq)
{
	fileName = name;

	cpuRequirements = cpuReq;
	memoryRequirements = memReq;
}

bool ProgramFile::validName( std::string* name)
{
	// The system will accept just a name as the name, or if there is an extension

	// Strip the extension
	auto back = name->end();
	if( *--back == 'p' && *--back == '.')
	{
		name->erase(back, name->end());
	}

	// Check the length
	if( name->length() > 8)
	{
		std::cout << "File names cannot exceed 8 characters.\n";
		return false;
	}
		
	// Verify all the characters are valid
	auto iter = name->begin();
	for(;iter != name->end(); ++iter)
	{
		char c = *iter;
		if( !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||((c >= '0' && c <= '9')) ) )
		{
			std::cout << "File names must be alpha-numeric.\n";
			return false;
		}
	}

	return true;
}


void ProgramFile::printData(int tabs)
{
	// Make us some sweet sweet tabs
	std::string t;
	for( int i = 0; i < tabs; i++)
		t += "\t";

	std::cout << t << fileName << ".p" << std::endl;
	std::cout << t << "\t" << "CPU Requirement: " << cpuRequirements << std::endl;
	std::cout << t << "\t" << "Mem Requirement: " << memoryRequirements << std::endl;
}


void ProgramFile::writeToFile(std::ofstream& stream)
{
	stream << fileName;
	for( int i = fileName.length(); i < 8; i++)
	{
		stream << '\0';
	}
	stream << ".p";
	stream << '\0';

	// Ugly, but it writes the ints to the file as char arrays
	stream.write((char*)&cpuRequirements, sizeof(cpuRequirements));
	stream.write((char*)&memoryRequirements, sizeof(memoryRequirements));
}


std::shared_ptr<ProgramFile> ProgramFile::inflateProgramFile(std::string& name, std::ifstream& stream)
{
	std::string n = name;
	validName(&n);

	int c;
	int m;
	
	// Again, ugly, but it reads the ints back from char arrays
	stream.read((char*)&c, sizeof(c));
	stream.read((char*)&m, sizeof(m));

	return std::make_shared<ProgramFile>(n, c, m);
}
