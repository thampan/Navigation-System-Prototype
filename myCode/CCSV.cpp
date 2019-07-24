/***************************************************************************
****************************************************************************
* Filename        : CCSV.cpp
* Author          : Jishnu M Thampan
* Description     : class CCSV
****************************************************************************/
#include <iostream>
#include <fstream>

#include "TypeMapping.h"
#include "CCSV.h"

#define EMPTY_STRING ""
#define SPACE_STRING " "			/**< \brief Represents Space String */
#define COMA_STRING ","				/**< \brief Represents Coma String */
#define SEMI_COLON_STRING ";"	    /**< \brief Represents Semi Colon String */
#define NEW_LINE_STRING "\n"		/**< \brief Represents New Line String */
#define SEMI_COLON_CHAR ';'			/**< \brief Represents Semi Colon Character */
#define COMA_CHAR ','				/**< \brief Represents Coma Character */
#define DELIMITER SEMI_COLON_STRING

#define FILE_LOCATION "myCode/"		   /**< \brief Represents the CSV File location */
#define WP_FILE_EXTENSION "-wp.txt"	   /**< \brief Represents the extension for Waypoint File */
#define POI_FILE_EXTENSION "-poi.txt"  /**< \brief Represents the extension for POI File */

CCSV::CCSV()
{
	/* Initialize the delimiters of the CSV file */
	std::vector<char> delimiters;
	delimiters.push_back(SEMI_COLON_CHAR);
	delimiters.push_back(COMA_CHAR);
	m_attributeExtractor.setDelimiters(delimiters);
}
/**
 * Set the name of the media to be used for persistent storage.
 * The exact interpretation of the name depends on the implementation
 * of the component.
 *
 * @param name the media to be used
 */
void CCSV::setMediaName(std::string name)
{
	this->m_mediaName = name;
	/* Set the file name as per the requirement <medianame>-<type>.txt */
	m_wpFileName  = FILE_LOCATION + m_mediaName + WP_FILE_EXTENSION;
	m_poiFileName = FILE_LOCATION + m_mediaName + POI_FILE_EXTENSION;
}
/**
 * Fill the databases with the data from persistent storage. If
 * merge mode is MERGE, the content in the persistent storage
 * will be merged with any content already existing in the data
 * bases. If merge mode is REPLACE, already existing content
 * will be removed before inserting the content from the persistent
 * storage.
 *
 * @param waypointDb the the data base with way points
 * @param poiDb the database with points of interest
 * @param mode the merge mode
 * @return true if the data could be read successfully
 */
bool CCSV::readData (CWpDatabase& waypointDb, CPoiDatabase& poiDb, MergeMode mode)
{
	bool retVal = true;
	switch(mode)
	{
		case MERGE:
		{
			CCSV::StringVector wpFileContents, poiFileContents;

			if (!(readCSVFile(m_poiFileName, poiFileContents) && /* Read and parse from POI File is Not Successful */
						parseCSVFileContents(POI_FILE, poiFileContents, poiDb, waypointDb)))
				retVal = false;

			if (!(readCSVFile(m_wpFileName, wpFileContents) && /* Read and parse from Waypoint File is Not Successful */
						parseCSVFileContents(WP_FILE, wpFileContents, poiDb, waypointDb)))
				retVal = false;

			break;
		}
		case REPLACE:
		{
			CCSV::StringVector wpFileContents, poiFileContents;
			if(readCSVFile(m_poiFileName, poiFileContents)) /* Read from POI File is Successful */
			{
				poiDb.clearPoiDatabase(); /* Clear POI Database */
				if(!parseCSVFileContents(POI_FILE, poiFileContents, poiDb, waypointDb)) /* Parse the POI file is unsuccessful */
					retVal = false;
			}
			else
				retVal = false;

			if(readCSVFile(m_wpFileName, wpFileContents)) /* Read from Waypoint File is Successful */
			{
				waypointDb.clearWpDatabase(); /* Clear Waypoint Database */
				if(!parseCSVFileContents(WP_FILE, wpFileContents, poiDb, waypointDb)) /* Parse the Waypoint file is unsuccessful */
					retVal = false;
			}
			else
				retVal = false;

			break;
		}
		default:
		{
			retVal = false;
			std::cout << "ERROR! CCSV::readData() failed! Invalid Merge Mode!" << std::endl;
		}
	}
	return retVal;
}
/**
 * Writes the Waypoint Database contents into the CSV Waypoint File
 * @param const CWpDatabase& waypointDb [IN] - The reference to the Waypoint Database
 * @return true if the write is successful, false otherwise
 */
bool CCSV::writeToCSVWaypointFile(const CWpDatabase& waypointDb)
{
	std::fstream wpFile (m_wpFileName.c_str(), std::ios::out);
	bool retVal = true;
	if(wpFile.is_open())
	{
		/* Get the Pointer to the Database Container */
		CWpDatabase::WaypointDatabaseMap_t db;
		waypointDb.getWpDatabase(db);

		/* Iterate through the container and write the contents to the file */
		for(CWpDatabase::WaypointDatabaseMap_t::const_iterator itr = db.begin(); itr != db.end(); ++itr)
		{
			wpFile << (itr->first) << DELIMITER << (itr->second).getLatitude()
					<< DELIMITER << (itr->second).getLongitude() << std::endl;
		}
		wpFile.close();
	}
	else
	{
		retVal = false;
		std::cout << "ERROR!CCSV::writeData() failed: Unable to open " << wpFile << std::endl;
	}
	return retVal;
}
/**
 * Writes the POI Database contents into the CSV POI File
 * @param const CPoiDatabase& poiDb [IN] - The reference to the POI Database
 * @return true if the write is successful, false otherwise
 */
bool CCSV::writeToCSVPoiFile(const CPoiDatabase& poiDb)
{
	bool retVal = true;

	std::fstream poiFile(m_poiFileName.c_str(), std::ios::out);
	if(poiFile.is_open())
	{
		/* Get the Pointer to the Database Container */
		CPoiDatabase::POIDatabaseMap_t db;
		poiDb.getPoiDatabase(db);
		/* Iterate through the container and write the contents to the file */
		for(CPoiDatabase::POIDatabaseMap_t::const_iterator itr = db.begin(); itr != db.end(); ++itr)
		{
			std::string poiType;
			/* Convert the POI Type to String to be stored in the POI file */
			if(type_mapping::convertPoiTypeToString((itr->second).getType(), poiType))
			{
				poiFile << poiType << DELIMITER <<  (itr->first) << DELIMITER
						<< (itr->second.getDescription()) << DELIMITER
						<<(itr->second).getLatitude() << DELIMITER << (itr->second).getLongitude() << std::endl;
			}
			else
			{
				std::cout << "ERROR!CCSV::writeData() failed! Invalid POI Type!" << std::endl;
			}
	     }
		 poiFile.close();
	}
	else
	{
		retVal = false;
		std::cout << "ERROR!CCSV::writeData() failed: Unable to open " << poiFile << std::endl;
	}
	return retVal;
}
/**
 * Write the data to the persistent storage.
 *
 * @param waypointDb the data base with way points
 * @param poiDb the database with points of interest
 * @return true if the data could be saved successfully
 */
bool CCSV::writeData (const CWpDatabase& waypointDb, const CPoiDatabase& poiDb)
{
	bool retVal = true;
	/* Write data to Waypoint file */
	if(!writeToCSVWaypointFile(waypointDb))
	{
		retVal = false;
	}
	/* Write data to POI file */
	if(!writeToCSVPoiFile(poiDb))
	{
		retVal = false;
	}
	return retVal;
}
/**
 * Reads the CSV file and copies the contents to the string vector
 * @param std::string& fileName 		  - Name of the file to be read	[IN]
 * @param StringVector& fileContents 	  - The contents of the file	[OUT]
 * @return true if read is successful, false otherwise
 */
bool CCSV::readCSVFile(const std::string& fileName, StringVector& fileContents)
{
	bool retVal = true;
	std::fstream poiFile(fileName.c_str(), std::ios::in);
	if(poiFile.is_open())
	{
		std::string line;
		/* Fetch the contents line by line and store it into fileContents*/
		while (getline(poiFile, line))
		{
			fileContents.push_back(line);
		/* No further processing of the text is done here in order to release the resource quickly */
		}
		poiFile.close();
	}
	else
	{
		retVal = false;
		std::cout<<"ERROR!CCSV::readCSVFile() failed! Unable to open " <<  fileName << std::endl;
	}
	return retVal;
}
/**
 * Initializes the Max attributes for the attribute extractor depending upon the file type
 * @param const fileType_t& fileType [IN] - The file type - Waypoint file or POI file
 * @return true if initialization is successful, false otherwise
 */
bool CCSV::initMaxAttributes(const fileType_t& fileType)
{
	bool retVal = true;

	if(WP_FILE == fileType)
		m_attributeExtractor.setMaxAttributes(MAX_WAYPOINT_ATTRIBUTES);

	else if(POI_FILE == fileType)
		m_attributeExtractor.setMaxAttributes(MAX_POI_ATTRIBUTES);

	else
		retVal = false;

	return retVal;
}
/**
 * Parses the CSV file contents, Validates and Stores the contents to the Databases
 * @param const fileType_t fileType [IN]		     - The File type- POI or Waypoint
 * @param const CCSV::StringVector& fileContents [IN]- The contents of the file
 * @param CPoiDatabase& poiDb [OUT]- The POI Database obj where the extracted contents are written into
 * @param CWpDatabase &wpDb   [OUT]- The Waypoint Database obj where the extracted contents are written into
 * @return void	- Errors are displayed as when encountered, not reported to the caller.
 */
bool CCSV::parseCSVFileContents(const fileType_t& fileType, const CCSV::StringVector& fileContents, CPoiDatabase& poiDb, CWpDatabase &wpDb)
{
	/* Initialize the Attribute Extractor (Utility class) with the file specific delimiters and attribute no */
	if(!initMaxAttributes(fileType))
	{
		std::cout << "ERROR!  CCSV::parseCSVFileContents()!Invalid file provided" << std::endl;
		return false;
	}
	unsigned int lineNumber = 1;
	/* Iterate through the file contents */
	for(CCSV::StringVector::const_iterator itr = fileContents.begin(); itr != fileContents.end(); ++itr, ++lineNumber)
	{
		std::string line = *itr;
		if ((SPACE_STRING == line) || (NEW_LINE_STRING == line) || (EMPTY_STRING == line))
			continue; //process text delimit

		CCSV::StringVector attributeVec;
		/* Extract the attributes and store it in attributeVec */
		CAttributeExtractor::RC_t errorType = m_attributeExtractor.extractAttributes(line, attributeVec);

		if(CAttributeExtractor::RC_SUCCESS != errorType) /* Display Err if extraction fails */
		{
			std::cout << "ERROR!CCSV::parseCSVFile() failed!ErrorType:" << m_attributeExtractor.getErrorType(errorType) << " line_no: "<< lineNumber << std::endl;
			continue;
		}
		/* Validate and Store the extracted data */
		bool isValid = ((WP_FILE == fileType) ? validateAndStoreWaypoint(attributeVec, wpDb) : validateAndStorePoi(attributeVec, poiDb));
		/* If not valid print the line number and continue */
		if (!isValid)
		{
			std::cout << " line_no: " << lineNumber << std::endl;
		}
	}
	return true;
}
/**
 * Validates and if valid stores the POI in the POI Database
 * @param const StringVector& attributeVec [IN] - The string vector consisting of the extracted attributes
 * @param CPoiDatabase& poiDb [OUT]	- POI Database where the validated content is to be stored
 * @return true if validation is successful, false otherwise
 */
bool CCSV::validateAndStorePoi(const StringVector& attributeVec, CPoiDatabase& poiDb)
{
	bool retVal = true;
	CPOI newPoi; /* POI object to be added to the database */

	/* Perform Validation of the extracted attributes */
	type_mapping::errorCode_t ec = type_mapping::convertStringToCPoiType(attributeVec, newPoi);
	if(type_mapping::RC_SUCCESS == ec)
	{
		/* After successful validation store the attributes to the database */
		poiDb.addPoi(newPoi);
	}
	else
	{
		retVal = false;
		std::cout << "ERROR!CCSV::parseCSVPoiFile() failed! ErrorType:" << type_mapping::getErrorType(ec);
	}
	return retVal;
}
/**
 * Validates and if valid stores the Waypoint in the Waypoint Database
 * @param const StringVector& attributeVec [IN] - The string vector consisting of the extracted attributes
 * @param CPoiDatabase& poiDb [OUT]	- Waypoint Database where the validated content is to be stored
 * @return true if validation is successful, false otherwise
 */
bool CCSV::validateAndStoreWaypoint(const StringVector& attributeVec, CWpDatabase& wpDb)
{
	bool retVal = true;
	CWaypoint wp; /* Waypoint to be added in the database */

	/* Perform Validation of the extracted attributes */
	type_mapping::errorCode_t ec = type_mapping::convertStringToCWaypointType(attributeVec, wp);
	if(type_mapping::RC_SUCCESS == ec)
	{
		/* After successful validation store the attributes to the database */
		wpDb.addWaypoint(wp);
	}
	else
	{
		retVal = false;
		std::cout << "ERROR!CCSV::parseCSVWaypointFile() failed!ErrorType:" << type_mapping::getErrorType(ec);
	}
	return retVal;
}
