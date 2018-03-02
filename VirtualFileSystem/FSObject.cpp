/*
 *	Andrew McGuiness
 *	ITEC 371 - Project 2
 *	3/2/2018
*/

#include "FSObject.h"

const std::string& FSObject::getFileName() const
{
	return fileName;
}

void FSObject::setFileName(const std::string& name)
{
	fileName = name;
}
