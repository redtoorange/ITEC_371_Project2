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
void createDir()
{
	cout << "Enter directory name>";
	string fileName;
	getline(cin, fileName);

	auto d = Directory::CreateDirectory(fileName, currentDirectory );

	// Swap currentDirectory only if d was created
	if( d )
	{
		currentDirectory->addObject( d );
		currentDirectory = d.get();
	}
}

/*!
*   \brief Attempt to create a new file inside currentDirectory.  There needs to be a touch of
*   parsing done here to direct control flow. 
*/
void createFile()
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
*   \brief Close currentDirectory, which for the purposes of the assignment, will seal it completely.  
*   If currentDirectory is root.d, then an error will be printed.  I decided that only Quit should close root.
*/
void closeDir()
{
	if( currentDirectory == rootPointer )
		cout << "Currently at root.  Enter Quit to close the FileSystem." << endl;
	else
		currentDirectory = currentDirectory->getParent();
}

/*!
*   \brief Parse a Command and redirect control flow.  This is just for a bit of abstraction.
*/
void executeCommand( Commands command )
{
	switch( command )
	{
	case C_DIR:
		createDir();
		break;

	case C_FILE:
		createFile();
		break;

	case E_DIR:
		closeDir();
		break;

	case QUIT:
		running = false;
		break;
	case LIST:
		currentDirectory->printData(0);
		break;
	case PWD:
		cout << "current directory is " << *currentDirectory << endl; 
		break;
	}
}

/*!
*   \brief The application has been opened in Create ("-c") mode.  This basically will allow the user to build up the
*   filesystem from root.d up.  There is no limit to nesting or complexity.
*/
void createMode( const string& filename )
{
	shared_ptr<Directory> root = Directory::CreateDirectory("root", nullptr, true);
	currentDirectory = root.get();
	rootPointer = root.get();

    // Happy Greeting
	cout << "Welcome to RUFS creator. Enter a command\n";
	cout << "CreateDir, CreateFile, EndDir or Quit\n";
	
    // Loop until Quit was entered
    while( running )
	{
		cout << "Command>";
		std::string input;
		getline(cin, input);
		bool handled = false;

		for( int i = 0; i < 6 && !handled; i++)
		{
			if( strcmp(input.c_str(), cmd[i].c_str()) == 0)
			{
                // Fire the input into the control branching
				executeCommand(static_cast<Commands>(i));
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


/*!
*   \brief The program was launched in read ("-r") mode.  The mode does more than it seems.  To 
*   satisfy the project, we could have just decoded it into a string.  Instead we take the flatfile
*   and re-inflate it to the original data structure.
*/
void readMode( const string& filename )
{
	shared_ptr<Directory> root = nullptr;

	ifstream inFile{ filename, ios::in | ios::binary};

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
		auto iter = parsed.end();
		char ext = *--iter;

        // We found a directory
		if( ext == 'd' && *--iter == '.')
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
		else if( ext == 't' && *--iter == '.')
			currentDirectory->addObject( TextFile::inflateTextFile(parsed, inFile));

        // We found a program
		else if( ext == 'p' && *--iter == '.')
			currentDirectory->addObject( ProgramFile::inflateProgramFile(parsed, inFile));
	
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

    // The assignment was to just print stuff, but realistically the filesystem is ready to be written to again
	root->printData(0);
}




int main( int argc, char* argv[])
{
    // Verify we have enough arguments
	if( argc < 3)
	{
		cout << "Missing arguments.  -c filename or -r filename are required." << endl;
		return 0;
	}
	
	// Parse the flag into either creation or reader mode
	if( strcmp( argv[1], "-c") == 0)
		createMode( argv[2] );

	else if( strcmp( argv[1], "-r") == 0)
		readMode( argv[2] );

	// Unsupported argument
	else
		cout << "Unknown argument. -c filename or -r filename are required." << endl;

	return 0;
}
