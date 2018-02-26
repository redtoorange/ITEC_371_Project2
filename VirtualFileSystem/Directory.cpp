/*
 *	Andrew McGuiness
 * ITEC 371 - Project 1
 * 2/8/2018
*/

#include "Directory.h"
#include <iostream>
#include <string>
#include "TextFile.h"

void Directory::addObject( std::shared_ptr<FSObject> obj)
{
	objCount++;
	objects.emplace_back(obj);
}

std::shared_ptr<Directory> Directory::CreateDirectory( std::string name, Directory* parent, bool root)
{
	// Check the filename
	std::string n = name;
	if( !validName( &n ) )
	{
		std::cout << "Invalid directory name!" << std::endl;
		return nullptr;
	}

	// Toss back out new Dir
	return std::make_shared<Directory>(n, parent, root);
}

bool Directory::validName( std::string* name)
{
	// The system will accept just a name as the name, or if there is an extension

	// Strip the extension
	auto back = name->end();
	if( *--back == 'd' && *--back == '.')
	{
		name->erase(back, name->end());
	}

	// Check the length
	if( name->length() > 8)
	{
		std::cout << "Directory names cannot exceed 8 characters.\n";
		return false;
	}
		
	// Verify all the characters are valid
	auto iter = name->begin();
	for(;iter != name->end(); ++iter)
	{
		char c = *iter;
		if( !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) )
		{
			std::cout << "Directory names must contain only alphabetic characters.\n";
			return false;
		}
	}

	return true;
}

Directory::Directory(const std::string& name, Directory* parent, bool root)
{
	isRoot = root;
	fileName = name;
	this->parent = parent;
}

Directory* Directory::getParent()
{
	return parent;
}

void Directory::printData(int tabs)
{
	// Print the Dir based on the project spec, special
	//	Case for the root folder handled.
//	if( isRoot )
//	{
//		std::cout << fileName << ".d:" << std::endl;
//		for( auto& e : objects)
//		{
//			e->printData(tabs);
//		}
//	}
//	else
//	{
//		// Make some tabs for proper formatting
//		std::string t;
//		for( int i = 0; i < tabs; i++)
//			t += "\t";
//
//		
//		std::cout << t << "Beginning listing for " << fileName << std::endl;
//		
//		// Recursively print kids
//		for( auto& e : objects)
//		{
//			e->printData(tabs + 1);
//		}
//		
//		std::cout <<t <<  "Ending listing for " << fileName << std::endl;
//	}

		
	std::cout << "Directory Name: " << fileName << std::endl;
		
	// Recursively print kids
	for( auto& e : objects)
	{
		auto* d = dynamic_cast<Directory*>(e.get());
		if( d )
		{
			std::cout << "Directory Name: " << d->getFileName() << std::endl;
		}
		else
		{
			e->printData(0);
		}
	}
}


Directory* Directory::getDirectory(const std::string& name)
{
	if( name == "..")
	{
		return parent;
	}
		

	for( auto& e : objects)
	{
		Directory* d = dynamic_cast<Directory*>(e.get());
		if( d && d->getFileName() == name )
		{
			return d;
		}
	}

	return nullptr;
}


TextFile* Directory::getTextfile(const std::string& name)
{
	for( auto& e : objects)
	{
		TextFile* t = dynamic_cast<TextFile*>(e.get());
		if( t && (t->getFileName() == name || name == t->getFileName() + ".t" ) )
		{
			return t;
		}
	}

	return nullptr;
}

void Directory::writeToFile(std::ofstream& stream)
{
	stream << fileName;

	// Name checks out, fill with null's and reattach extension
	for( int i = fileName.length(); i < 8; i++)
	{
		stream << '\0';
	}

	stream << ".d";
	stream << '\0';
	stream.write((char*)&objCount, sizeof(objCount));
	
	// Recursively write all kids
	for( auto& e : objects)
	{
		e->writeToFile(stream);
	}

	// Write the Dir Closer
	stream << "end" + fileName;
	for( int i = fileName.length(); i < 8; i++)
	{
		stream << '\0';
	}
}
