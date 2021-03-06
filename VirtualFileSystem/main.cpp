/*
 *	Andrew McGuiness
 *	ITEC 371 - Project 2
 *	3/2/2018
*/

#include <iostream>
#include <fstream>

#include "Util.h"
#include "Directory.h"
#include "TextFile.h"

using  std::cout;
using  std::cin;
using  std::endl;
using  std::string;

//! Should the Application be running?
bool running = true;

//! Holds a pointer to the current directory
Directory* currentDirectory;

//! Holds a pointer to the root for comparison during runtime
Directory* rootPointer;

/*!
* \brief Attempt to create a new directory.  If the directory is made successfully, 
*   then it will be placed inside the current directory, then the currentDirectory
*   pointer will be set to point at the new directory.
*/
void createDir( const std::string& dirName)
{
	const auto dir = Directory::CreateDirectory(dirName, currentDirectory );

	// Swap currentDirectory only if d was created
	if( dir )
	{
		currentDirectory->addObject( dir );
	}
}

/*!
*   \brief Attempt to create a new file inside currentDirectory.  There needs to be a touch of
*   parsing done here to direct control flow. 
*/
void createTextFile( )
{
	cout << "Enter filename>";
		
	string fileName;
	getline(cin, fileName);

	if( fileName.empty() )
	{
		cout << "Filename cannot be empty." << endl;
		return;
	}


    // Parse the last two chars of the filename
	int len = fileName.length();
	if( fileName.substr(len-2) == ".t")
	{
		const auto file = TextFile::makeTextFile(fileName);
		if( file )
		currentDirectory->addObject( file );
	}
	else
	{
		cout << "TextFiles must end in .t" << endl;
	} 
}

/*!
 *	\brief Attempt to change to the directory to the requested directory name.
 */
void tryToChangeDir( const std::string& dirName )
{
	if( Directory* dir = currentDirectory->getDirectory(dirName) )
	{
		currentDirectory = dir;
	}
	else if(dirName == ".." && rootPointer == currentDirectory)
	{
		return;
	}
	else
	{
		// The dir wasn't found, let the user know
		cout << "Could not change Directory to <" << dirName << ">" << std::endl;
	}
}

/**!
 *	\brief Search for a TextFile and print it's contents if it was found.
 */
void printTextFile(const string& fileName)
{
	// Try to get the file
	TextFile* text = currentDirectory->getTextfile( fileName );

	// Print the file if it exists
	if( text )
		cout << "Text file contents:\n" << text->getContents() << endl;
	else
		cout << "Could not read from <" << fileName << ">" << endl;
}

/**!
 *	\brief Handle complex commands that take an argument
 */
bool handleCompound(Commands command, const std::string& input )
{
	// Get the length of the command + 1 for a space
	const auto len = cmd[command].length()+1;
	bool valid = input.length() > len && input[len-1] == ' ';
	bool handled = true;

	if( valid )
	{
		switch( command )
		{
			case MKDIR:
				createDir( input.substr(len));
				break;

			case CAT:
				printTextFile(input.substr(len));
				break;

			case RUN:
				cout << "Running " << input.substr(len) << endl;
				break;

			case START:
				cout << "Starting " << input.substr(len) << endl;
				break;

			case STEP:
				cout << "Stepping " << input.substr(len) << endl;
				break;

			case CD:
				tryToChangeDir( input.substr(3) );
				break;

			default:
				handled = false;
		}
	}
	else
	{
		cout << "Error: Malformed input, <command filename/directory> is required format." << std::endl;
	}

	return handled;
}

/**!
 *	\brief Handle simple commands that do not take any arguments.
 */
bool handleSimple(Commands command, const std::string& input )
{
	bool handled = true;

	switch( command )
	{
		case CREATE_TEXT:
			if( equalIC(input, "createtextfile"))
				createTextFile();
			else
				cout << "Error: Malformed input, createTextFile takes no arguments." << std::endl;
			break;

		case LIST:
			if( equalIC(input, "ls"))
				currentDirectory->printData(0);
			else
				cout << "Error: Malformed input, <ls> is required format." << std::endl;
			break;

		case PWD:
			if( equalIC(input, "pwd"))
				cout << "Current directory is " << *currentDirectory << endl; 
			else
				cout << "Error: Malformed input, <pwd> is required format." << std::endl;	
			break;

		case QUIT:
			running = false;
			break;

		default:
			handled = false;
	}

	return handled;
}

/*!
*   \brief Parse a Command and redirect control flow.  This is just for a bit of abstraction.  
*/
bool executeCommand( Commands command, const std::string& input )
{
	switch(command)
	{
		case MKDIR: case CAT: case RUN: case START: case STEP: case CD:
			return handleCompound(command, input);
		case CREATE_TEXT: case LIST: case PWD: case QUIT:
			return handleSimple(command, input);
	}

	return false;
}

/*!
*   \brief The program should look for and inflate the FS that is stored on disk. This processes rebuilds the FS
*   structure based on the requirements.
*/
std::shared_ptr<Directory>  readInFile( const string& filename )
{
	std::shared_ptr<Directory> root = nullptr;

	std::ifstream inFile{ filename, std::ios::in | std::ios::binary};

    // 11 char token to pull from the file.  All files/directories are identified by 11 chars, 8 for name,
    //  a period, a 1 char extension and then a null character
	char token[11];

    // Once we will out the token, we have to toss out the null chars to parse it easier
	std::string parsed;
	
	while( running == true)
	{
        // Grab a token
		inFile.read( token, sizeof(char)*11 );
		parsed.clear();

		// Strip the null chars
		for( int i = 0; i < 11; i++)
		{
			if( token[i] != '\0')
				parsed += token[i];
		}

        // Prep an iterator
		auto extension = parsed.substr(parsed.size()-2);

        // We found a directory
		if( extension == ".d" )
		{
            // Special case for root.d
			if( !currentDirectory )
			{
				root = Directory::CreateDirectory( parsed, nullptr, true);
				currentDirectory = root.get();
				rootPointer = root.get();
			}
			else
			{
                // Try to inflate the flat directory back into the linked structure
				auto d = Directory::CreateDirectory(parsed, currentDirectory );

				// Swap currentDirectory only if d was created
				if( d )
				{
					currentDirectory->addObject( d );
					currentDirectory = d.get();
				}
            }

            // We get the number of files under the directory, but it's not used
			int data;
			inFile.read((char*)&data, sizeof(data));
		}

        // We found a textfile
		else if( extension == ".t" )
			currentDirectory->addObject( TextFile::inflateTextFile(parsed, inFile));

        // REMOVED: We found a program
		//else if( extension == ".p")
		//	currentDirectory->addObject( ProgramFile::inflateProgramFile(parsed, inFile));
	
        // found an endX token, close the current directory
        else if( parsed.find_first_of("end" + currentDirectory->getFileName()) == 0)
		{
			if( currentDirectory == rootPointer )
				running = false;
			else
				currentDirectory = currentDirectory->getParent();
		}
	}

	inFile.close();

	return root;
}

/*!
*   \brief Enter into the main command loop for the application.  RUFS will continue to listen for input until the quit
*   command is entered.
*/
void commandLoop( const string& filename, std::shared_ptr<Directory> rootPtr )
{
	running = true;
	std::shared_ptr<Directory> root = rootPtr;

	if( !root )
	{
		root = Directory::CreateDirectory("root", nullptr, true);
	}

	currentDirectory = root.get();
	rootPointer = root.get();
	
    // Loop until Quit was entered
    while( running )
	{
		// Read in command
		cout << "EnterCommand>";
		std::string input;
		getline(cin, input);
		bool handled = false;

		
		if( input.empty() ) continue;

		// Step through known commands
		for( int i = 0; i <= QUIT && !handled; i++)
		{
			int len = cmd[i].length();

			if( equalIC(input.substr(0, len), cmd[i]) )
			{
                // Fire the input into the control branching
				handled = executeCommand( static_cast<Commands>(i), input );
			}
		}

		if( !handled ) cout << "Unknown Command...\n";
	}

    // User has finished, compress everything down and write it out to a file
	std::ofstream outfile{filename, std::ios::out | std::ios::binary};

    // Starting from the root node, recursively write each piece into the file
	root->writeToFile(outfile);

    // Jobs done!
	outfile.close();
}


int main( int argc, char* argv[])
{
    // Verify we have enough arguments
	if( argc < 2)
	{
		cout << "Missing arguments, a filename is required." << endl;
		return -1;
	}
	
	std::fstream file(argv[1], std::ios::in | std::ios::binary);
	if( file.good() )
	{
		file.close();
		commandLoop( argv[1], readInFile(argv[1]));
	}
	else
	{
		file.close();
		commandLoop( argv[1], nullptr );
	}
	

	return 0;
}
