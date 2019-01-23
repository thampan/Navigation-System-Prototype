/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CAttributeExtractor.cpp
* Author          : Jishnu M Thampan
* Description     : class CAttributeExtractor
****************************************************************************/
#include <algorithm> //std::count
#include <iostream>
#include "CAttributeExtractor.h"

CAttributeExtractor::CAttributeExtractor()
{
	max_attributes = 0;
}
/**
 * Extracts the attributes from the string text and inserts the attributes to the string vector
 * @param const std::string& line [IN] - The text to be extracted
 * @param std::vector<std::string>& attributeVec [OUT] - vector of strings which holds the result after extraction
 * @return RC_t - the error code after extraction
 */
CAttributeExtractor::RC_t CAttributeExtractor::extractAttributes(const std::string& line, std::vector<std::string>& attributeVec)
{
	unsigned int numberOfAttributes = 0;
	/* numberOfAttributes = sum of all possible delimiters */
	for (unsigned int i = 0; i < getDelimiterCount(); i++)
	{
		numberOfAttributes += std::count(line.begin(), line.end(), m_deliemiterVec[i]);
	}
	/* Check if the configured max attributes and the number of attributes in the text matches */
	if (max_attributes != (numberOfAttributes + 1))
	{
		return CAttributeExtractor::RC_INCORRECT_NUMBER_OF_ATTRIBUTES;
	}
	size_t parseStartPosition = 0, parseEndPosition = line.length();
	/* Extract attribute by attribute and store it in the string vector */
	for (unsigned int i = 0; (i <= numberOfAttributes) && (parseStartPosition <= parseEndPosition); i++)
	{
		std::string textToBeProcessed = line.substr(parseStartPosition, parseEndPosition); /* Text which represents the remaining atrributes to be extracted */
		size_t delimiter_pos          = getClosestDelimiterPos(textToBeProcessed); /* Get the closes delimiter pos in the text */
		std::string attr 		      = line.substr(parseStartPosition, delimiter_pos); /* Get the first attribute */
		parseStartPosition           += delimiter_pos + 1; /* Move the parsePosition to the next attribute */
		attributeVec.push_back(attr); /* Store the extracted attribute */
	}
	/* Return success if the output is not empty, else return error */
	return (attributeVec.size() ? CAttributeExtractor::RC_SUCCESS : RC_UNKNOWN_ERROR);
}
/**
 * Gets the closes Delimiter Positon within the input text
 * @param  const std::string& line [IN] - Input text
 * @return size_t - Position of the closes delimiter
 */
size_t CAttributeExtractor::getClosestDelimiterPos(const std::string& line)
{
	unsigned int index = 0;
	size_t pos = line.find(m_deliemiterVec.at(index));
	/* Get the closest delimiter in te text*/
	for (unsigned int i = 1; i < getDelimiterCount(); i++)
	{
		size_t new_pos = line.find(m_deliemiterVec.at(i));
		if (pos >= new_pos)
		{
			pos = new_pos;
		}
	}
	return pos;
}
/**
 * Sets the Max Attributes
 * @param unsigned int maxAttributes [IN] - The maximum attributes supported in the text
 * @return None
*/
void CAttributeExtractor::setMaxAttributes(const unsigned int maxAttributes)
{
	max_attributes = maxAttributes;
}
/**
 * Sets the Delimiters
 * @param std::vector<char>& delimiterVec [IN] - The list of delimiters
 * @return None
 */
void CAttributeExtractor::setDelimiters(const std::vector<char>& delimiterVec)
{
	m_deliemiterVec = delimiterVec;
}
/**
 * Gets the delimiter count
 * @param None
 * @return unsigned int - Count of the number of delimiters
 */
unsigned int CAttributeExtractor::getDelimiterCount(void) const
{
	return m_deliemiterVec.size();
}
/**
 * Prints the content of WP Database
 * @param RC_t& ec[IN] - Reference to the error code to be converted to string
 * @return string  - The string representation of the error code
 */
std::string CAttributeExtractor::getErrorType(const RC_t& ec)
{
	std::string errorType;
	switch(ec)
	{
		case RC_INCORRECT_NUMBER_OF_ATTRIBUTES:
		{
			errorType = "INCORRECT_NUMBER_OF_ATTRIBUTES";
			break;
		}
		case RC_UNKNOWN_ERROR:
		{
			errorType = "UNKNOWN_ERROR";
			break;
		}
		default:
		{
			errorType = "Unidentifed Error";
			break;
		}
	}
	return errorType;
}
