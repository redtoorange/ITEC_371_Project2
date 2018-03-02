/*
 *	Andrew McGuiness
 *	ITEC 371 - Project 2
 *	3/2/2018
*/

#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include "File.h"
#include <string>
#include <memory>
/*!
*	\brief A special type of File that is designed to hold 
*	a single string which is the "file contents"
*/
class TextFile : public File
{
public:
	//! Create a new TextFile given a name
	static std::shared_ptr<TextFile> makeTextFile( const std::string& name );
	
	//! Inflate a TextFile from a flat file
	static std::shared_ptr<TextFile> inflateTextFile(  std::string& name, std::ifstream& stream);
	
	//! Check to see if the name follows the rules for a TextFile
	static bool validName( std::string* name);

	//! Constructor that should NOT be used outside the class
	TextFile(const std::string& name, const std::string& contents);

	//! Formatted printer for the file
	void printData(int tabs) override;
	
	//! Write the data into a a stream
	void writeToFile(std::ofstream& stream) override;

	//! Get the file's contents
	const std::string& getContents() const;
	
private:
	std::string fileContents;
};

#endif