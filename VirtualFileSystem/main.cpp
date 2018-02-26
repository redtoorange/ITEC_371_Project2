/*
 *	Andrew McGuiness
 * ITEC 371 - Project 2
 * 2/24/2018
*/

#include <iostream>
#include <string.h>
#include <fstream>

#include "Util.h"
#include "Directory.h"
#include "ProgramFile.h"
#include "TextFile.h"

using namespace std;

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
	auto d = Directory::CreateDirectory(dirName, currentDirectory );

	// Swap currentDirectory only if d was created
	if( d )
	{
		currentDirectory->addObject( d );
	}
}

/*!
*   \brief Attempt to create a new file inside currentDirectory.  There needs to be a touch of
*   parsing done here to direct control flow. 
*/
void createFile( )
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
	auto iter = fileName.end();
    --iter; // Move the pointer off of the null at the end

	if( *iter == 't' && *(--iter) == '.')
    {
        auto t = TextFile::makeTextFile(fileName);
        if( t )
		    currentDirectory->addObject( t );
	}
    else if(*iter == 'p' && *(--iter) == '.')
	{
        auto p = ProgramFile::makeProgramFile(fileName);
        if( p )
            currentDirectory->addObject( p );
    }
    else
	    cout << "Files must end in .t or .p" << endl;
}

/*!
*   \brief Parse a Command and redirect control flow.  This is just for a bit of abstraction.
*/
void executeCommand( Commands command, const std::string& input )
{
	//cout << "Handling <" << input << ">" << endl;
	switch( command )
	{
	case MKDIR:
		{
			int len = cmd[command].length()+1;
			if( input.length() > len)
			{
				createDir( input.substr(len));
			}
			else
			{
				cout << "Error: Malformed input, filename required." << std::endl;
			}
		}
		break;

	case CAT:
		{
			int len = cmd[command].length()+1;
			if( input.length() > len)
			{
				string fileName = input.substr(len);

				TextFile* text = currentDirectory->getTextfile( fileName );
				if( text )
				{
					cout << "Text file contents:" << endl;
					cout << text->getContents() << endl;;
				}
				else
				{
					cout << "Could not read from <" << fileName << ">" << endl;
				}
			}
			else
			{
				cout << "Error: Malformed input, filename required." << std::endl;
			}
		}
		break;

	case CREATE_TEXT:
		{
			createFile();
		}
		break;

	case LIST:
		currentDirectory->printData(0);
		break;

	case PWD:
		cout << "Current directory is " << *currentDirectory << endl; 
		break;

	case QUIT:
		running = false;
		break;
	}
}

void tryToChangeDir( const std::string& dirName )
{
	Directory* dir = currentDirectory->getDirectory(dirName);

	if( dir )
	{
		currentDirectory = dir;
		//cout << "Changed Directory." << std::endl;
	}
	else
	{
		if( dirName == ".." && rootPointer == currentDirectory)
		{
			cout << "Cannot change to the parent of root." << std::endl;
		}
		else
		{
			cout << "Could not change Directory to <" << dirName << ">" << std::endl;
		}
	}
}

/*!
*   \brief The application has been opened in Create ("-c") mode.  This basically will allow the user to build up the
*   filesystem from root.d up.  There is no limit to nesting or complexity.
*/
void commandLoop( const string& filename )
{
	shared_ptr<Directory> root = Directory::CreateDirectory("root", nullptr, true);
	currentDirectory = root.get();
	rootPointer = root.get();
	
    // Loop until Quit was entered
    while( running )
	{
		cout << "EnterCommand>";
		std::string input;
		getline(cin, input);
		bool handled = false;

		if( input.substr(0, 3) == "cd ")
		{
			tryToChangeDir( input.substr(3));
			handled = true;
		}

		for( int i = 0; i < 6 && !handled; i++)
		{
			int len = cmd[i].length();

			if( input.substr(0, len) == cmd[i] )
			{
                // Fire the input into the control branching
				executeCommand( static_cast<Commands>(i), input );
				handled = true;
			}
		}
			

		if( !handled ) cout << "Unknown Command...\n";
	}

    // User has finished, compress everything down and write it out to a file
	ofstream outfile{filename, ios::out | ios::binary};

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
		return 0;
	}
	
	commandLoop( argv[1] );

	return 0;
}
