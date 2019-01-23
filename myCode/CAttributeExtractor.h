/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CAttributeExtractor.H
* Author          : Jishnu M Thampan
* Description     : class CAttributeExtractor
****************************************************************************/
#ifndef CATTRIBUTE_EXTRACTOR_H
#define CATTRIBUTE_EXTRACTOR_H

#include <string>
#include <vector>

/**
 * CAttributeExtractor is a utility class which
 * extracts the contents from a file based on the
 * configured delimit characters and the maximum
 * attributes allowed per line
 */
class CAttributeExtractor
{

private:
	/* Copy constructor and copy assignment operators are
	 * not available since this is simply a utility class
	 * and hence their use is not desired.
	 */
	CAttributeExtractor(CAttributeExtractor&);
	CAttributeExtractor& operator=(CAttributeExtractor&);

	std::vector<char> m_deliemiterVec;	/**< \brief Represents the container that holds the delimiters */

	unsigned int max_attributes;	/**< \brief Represents the max number of attributes in the text */

    /**
	 * Gets the closes Delimiter Positon within the input text
	 * @param  const std::string& line [IN] - Input text
	 * @return size_t - Position of the closest delimiter
	 */
	size_t getClosestDelimiterPos(const std::string& line);
    /**
	 * Gets the delimiter count
	 * @param None
	 * @return unsigned int - Count of the number of delimiters
	 */
	unsigned int getDelimiterCount(void)const;

public:
	typedef enum
	{
		RC_SUCCESS,
		RC_INCORRECT_NUMBER_OF_ATTRIBUTES,
		RC_UNKNOWN_ERROR
	}RC_t;
	CAttributeExtractor();
	~CAttributeExtractor()
	{
		m_deliemiterVec.clear();
		max_attributes = 0;
	}
	/**
	 * Sets the Max Attributes
	 * @param unsigned int maxAttributes [IN] - The maximum attributes supported in the text
	 * @return None
	*/
	void setMaxAttributes(const unsigned int maxAttributes);
    /**
	 * Sets the Delimiters
	 * @param std::vector<char>& delimiterVec [IN] - The list of delimiters
	 * @return None
	 */
	void setDelimiters(const std::vector<char>& delimiterVec);
    /**
	 * Extracts the attributes from the string text
	 * @param const std::string& line [IN] - The text to be extracted
	 * @param std::vector<std::string>& attributeVec [OUT] - vector of strings which holds the result after extraction
	 * @return RC_t - the error code after extraction
	 */
	RC_t extractAttributes(const std::string& line, std::vector<std::string>& attributeVec);
    /**
	 * Prints the content of WP Database
	 * @param RC_t& ec[IN] - Reference to the error code to be converted to string
	 * @return string  - The string representation of the error code
	 */
	std::string getErrorType(const RC_t& ec);
};

#endif
