/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CCSV.cpp
* Author          : Jishnu M Thampan
* Description     : class CJson
****************************************************************************/
#include "CJsonPersistence.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "TypeMapping.h"


#define EMPTY_STRING ""						/**< \brief Represents an EMPTY STRING */
#define JSON_BEGIN_STRING "{"				/**< \brief Represents an BEGIN STRING */
#define JSON_END_STRING "}"					/**< \brief Represents an END STRING */
#define WAYPOINTS_ARRAY_NAME "waypoints"    /**< \brief Represents the name of WP DB */
#define POI_ARRAY_NAME "pois" 				/**< \brief Represents the name of POI DB */

#define FILE_LOCATION "myCode/"		   		/**< \brief Represents the CSV/JSON File location */
#define FILE_EXTENSION ".json"				/**< \brief Represents the extension of the json file */

#define ERR_IGNORE (false)   /**< \brief Represents the case when a json parsing error is ignored */
#define ERR_CRITICAL (true)  /**< \brief Represents the case when a json parsing error is critical */


/**
 * Set the name of the media to be used for persistent storage.
 * The exact interpretation of the name depends on the implementation
 * of the component.
 *
 * @param name the media to be used
 */
void CJsonPersistence::setMediaName(std::string name)
{
	this->m_mediaName = name;
	/* Set the file name as per the requirement <medianame>-<type>.txt */
	m_fileName  = FILE_LOCATION + m_mediaName + FILE_EXTENSION;
}
/**
 * Gets the parameter name and assigns it to out based on the database type
 *
 * @param db_type_t& db: [IN] the Reference to the current database which is extracted from json file
 * @param std::string& in: [IN] the Reference to the current input Token extracted from the Json file
 * @param std::string& out:[OUT]the Reference to the type of the parameter
 * @return true if the database and parameter is a valid one, false otherwise
 */
bool CJsonPersistence::checkAndExtractParameterName(const db_type_t& db, const std::string& in, std::string& out)
{
	bool isFound 					  = false;
	unsigned int arraySize 			  = 0;
	const std::string* attributePtr   = NULL;
	const std::string wpAttributes[]  = {"name", "latitude", "longitude"};
	const std::string poiAttributes[] = {"name", "latitude", "longitude", "description", "type"};
	if(WP_DB ==  db)
	{
		attributePtr = wpAttributes;
		arraySize    = sizeof(wpAttributes)/sizeof(wpAttributes[0]);
	}
	else if(POI_DB == db)
	{
		attributePtr = poiAttributes;
		arraySize    = sizeof(poiAttributes)/sizeof(poiAttributes[0]);
	}
	else
	{
		return isFound;
	}
	for (unsigned int i = 0; i < arraySize; i++)
	{
		if (attributePtr[i] == in)
		{
			out     = attributePtr[i];
			isFound = true;
			break;
		}
	}
	return isFound;
}
/**
 * Checks if the given db is valid or not
 *
 * @param db_type_t& db: [OUT] the Reference to the current database which is extracted from json file
 * @param std::string& dbName: [IN] the Reference to the current database string extracted from the Json file
 * @return true if the database is a valid one, false otherwise
 */
bool CJsonPersistence::isValidDb(const std::string& dbName, db_type_t& dbType)
{
	bool isValid = false;
	if (dbName == WAYPOINTS_ARRAY_NAME)
	{
		dbType 	= CJsonPersistence::WP_DB;
		isValid = true;
	}
	else if (dbName == POI_ARRAY_NAME)
	{
		dbType  = CJsonPersistence::POI_DB;
		isValid = true;
	}
	else
	{
		dbType  = CJsonPersistence::UNKNOWN_DB;
	}
	return isValid;
}
/**
 * Checks if the Parsing of data is completed
 *
 * @param stateInfo_t& stateInfo: [IN] the Reference to the structure containing the attributes of the current state
 * @return true if the data parsing has been completed, false otherwise
 */
bool CJsonPersistence::isDataParseComplete(const stateInfo_t& stateInfo)
{
	/* Data Parsing is considered completed only if all the attributes of wp db or poi db
	 * are read successfully
	 */
	if (stateInfo.dbType == WP_DB)
	{
		return (stateInfo.data.latitude != 0 && stateInfo.data.longitude != 0
				&& stateInfo.data.name != DEFAULT_STRING);
	}
	else if (stateInfo.dbType == POI_DB)
	{
		return (stateInfo.data.latitude != 0 && stateInfo.data.longitude != 0
				&& stateInfo.data.name != DEFAULT_STRING
				&& stateInfo.data.description != DEFAULT_STRING
				&& stateInfo.data.poiType != CPOI::UNKNOWN);
	}
	return false;
}
/**
 * State machine handling the read operation
 *
 * @param CJsonToken* currentToken: [IN] the current Token extracted from the Json file
 * @param stateInfo_t& stateInfo:[OUT] the Reference to the structure containing the attributes of the current state
 * @return None
 */
void CJsonPersistence::processStateChange(APT::CJsonToken* currentToken, stateInfo_t& stateInfo)
{
	/* Get the stateInfo struct attributes by reference */

	parser_state_t& currentState  = stateInfo.currentState;
	db_type_t& dbType			  = stateInfo.dbType;
	std::string& parameter		  = stateInfo.parameter;

	double& latitude  			  = stateInfo.data.latitude;
	double& longitude 			  = stateInfo.data.longitude;
	std::string& name 			  = stateInfo.data.name;
	std::string& description      = stateInfo.data.description;
	CPOI::t_poi& poiType 	      = stateInfo.data.poiType;

	/* ERROR Handling Criteria is defined as follows:
	 * =>In case there is an error in the standard Json format, then the parsing is aborted
	 * =>In case there is issue with the contents, that particular set alone is ignored and
	 *   the parsing is resumed
	 */
	switch (currentState)
	{
		case CJsonPersistence::IS_WAITING_FOR_FIRST_TOKEN:
		{
			if (APT::CJsonToken::BEGIN_OBJECT == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_NAME;
			}
			else
			{
				error_data_t error_data = {"INVALID_BEGIN_OBJECT_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_NAME:
		{
			bool isError = true;
			currentState = CJsonPersistence::IS_WAITING_FOR_DB_NAME_SEPARATOR;
			if (APT::CJsonToken::STRING == currentToken->getType())
			{
				APT::CJsonStringToken* sToken = (APT::CJsonStringToken*) currentToken;
				isError = !isValidDb(sToken->getValue(), dbType); /*Validate the Database */
			}
			if(isError)
			{
				error_data_t error_data = {"INVALID_DB_NAME_TOKEN", ERR_IGNORE}; /* Ignore the error since it is the problem with this db */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_NAME_SEPARATOR:
		{
			if (APT::CJsonToken::NAME_SEPARATOR == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_ARRAY_BEGIN;
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_NAME_SEPARATOR_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_ARRAY_BEGIN:
		{
			if (APT::CJsonToken::BEGIN_ARRAY == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_BEGIN;
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_ARRAY_BEGIN_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_OBJ_BEGIN:
		{
			if (APT::CJsonToken::BEGIN_OBJECT == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_NAME;
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_OBJ_BEGIN_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_NAME:
		{
			currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_NAME_SEPARATOR;
			if (APT::CJsonToken::STRING == currentToken->getType())
			{
				APT::CJsonStringToken* sToken = (APT::CJsonStringToken*) currentToken;
				if(!checkAndExtractParameterName(dbType, sToken->getValue(), parameter)) /* Checks if the parameter/db is valid*/
				{
					if(dbType == UNKNOWN_DB) /* Invalidate the data in case of unknown db */
					{
						stateInfo.data.isValid = false;
					}
					error_data_t error_data = {"INVALID_DB_OBJ_ATTR_NAME_TOKEN", ERR_IGNORE};
					throw error_data;
				}
				else if(isDataParseComplete(stateInfo))
				{
					error_data_t error_data = {"OBJECT NOT TERMINATED", ERR_CRITICAL};
					throw error_data;
				}
			}
			else
			{
				if(isDataParseComplete(stateInfo))
				{
					error_data_t error_data = {"Database attribute count exceeded", ERR_CRITICAL};
					throw error_data;
				}
				error_data_t error_data = {"INVALID_DB_OBJ_ATTR_NAME_TOKEN", ERR_IGNORE};
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_NAME_SEPARATOR:
		{
			if (APT::CJsonToken::NAME_SEPARATOR == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_VALUE;
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_OBJ_ATTR_NAME_SEPARATOR_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_VALUE:
		{
			if (APT::CJsonToken::STRING == currentToken->getType())
			{
				APT::CJsonStringToken* sToken = (APT::CJsonStringToken*) currentToken;
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_VALUE_SEPARATOR;
				if (parameter == "name")
				{
					if(name == DEFAULT_STRING) /* Name is checked if it is written already, so that repeated redundant fields can be detected*/
					{
						name = sToken->getValue();
						if(!type_mapping::validateStringData(name)) /* Validate the string data */
						{
							error_data_t error_data = {"INVALID_NAME", ERR_IGNORE};
							throw error_data;
						}
					}
					else
					{
						error_data_t error_data = {"Repeated 'name' Entry detected", ERR_IGNORE};
						throw error_data;
					}
				}
				else if (parameter == "description")
				{
					if(description == DEFAULT_STRING) /* Description is checked if it is written already, so that repeated redundant fields can be detected*/
					{
						description = sToken->getValue();
						if(!type_mapping::validateStringData(description)) /* Validate the string data */
						{
							error_data_t error_data = {"INVALID_DESCRIPTION", ERR_IGNORE};
							throw error_data;
						}
					}
					else
					{
						error_data_t error_data = {"Repeated description field detected", ERR_IGNORE};
						throw error_data;
					}
				}
				else if (parameter == "type")
				{
					if(poiType == CPOI::UNKNOWN) /* POI is checked if it is written already, so that repeated redundant fields can be detected*/
					{
						std::string str_poiType = sToken->getValue();
						if(!type_mapping::convertStringToPoiType(str_poiType, poiType)) /* Converts the string to POI Type */
						{
							error_data_t error_data = {"INVALID_POI_TYPE", ERR_IGNORE};
							throw error_data;
						}
					}
					else
					{
						error_data_t error_data = {"Repeated type field detected", ERR_IGNORE};
						throw error_data;
					}
				}
				else
				{
					error_data_t error_data = {"Error", ERR_IGNORE};
					throw error_data;
				}
			}
			if (APT::CJsonToken::NUMBER == currentToken->getType())
			{
				APT::CJsonNumberToken* nToken = (APT::CJsonNumberToken*) currentToken;
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_VALUE_SEPARATOR;
				if (parameter == "latitude")
				{
					if(!latitude) /* Latitude is checked if it is written already, so that repeated redundant fields can be detected*/
					{
						latitude = nToken->getValue();
						if(!type_mapping::validateLatitude(latitude)) /* Validate the latitude */
						{
							error_data_t error_data = {"INVALID_LATITUDE", ERR_IGNORE};
							throw error_data;
						}
					}
					else
					{
						error_data_t error_data = {"Repeated latitude field detectced", ERR_IGNORE};
						throw error_data;
					}
				}
				else if (parameter == "longitude")
				{
					if(!longitude) /* Longitude is checked if it is written already, so that repeated redundant fields can be detected*/
					{
						longitude = nToken->getValue();
						if(!type_mapping::validateLongitude(longitude)) /* Validate the longitude */
						{
							error_data_t error_data = {"INVALID_LONGITUDE", ERR_IGNORE};
							throw error_data;
						}
					}
					else
					{
						error_data_t error_data = {"Repeated Longitude field detected", ERR_IGNORE};
						throw error_data;
					}
				}
				else
				{
					error_data_t error_data = {"Error", ERR_IGNORE};
					throw error_data;
				}
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_VALUE_SEPARATOR:
		{
			if (APT::CJsonToken::VALUE_SEPARATOR == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_ATTR_NAME;
			}
			else if (APT::CJsonToken::END_OBJECT == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_SEPARATOR;
				if(isDataParseComplete(stateInfo)) /* Checks if the one set of data is extracted successfully */
				{
					stateInfo.dataReady = true; // flag is set so that this set of data can be written to the db
				}
				else
				{
					error_data_t error_data = {"Incorrect Number of attributes", ERR_IGNORE};
					throw error_data;
				}
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_OBJ_ATTR_VALUE_SEPARATOR_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_OBJ_SEPARATOR:
		{
			stateInfo.data.reset();/* Reset the data after parsing one set */
			if (APT::CJsonToken::VALUE_SEPARATOR == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_OBJ_BEGIN;
			}
			else if (APT::CJsonToken::END_ARRAY == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_ARRAY_SEPARATOR;
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_OBJ_SEPARATOR_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_DB_ARRAY_SEPARATOR:
		{
			if (APT::CJsonToken::VALUE_SEPARATOR == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_DB_NAME;
			}
			else if (APT::CJsonToken::END_OBJECT == currentToken->getType())
			{
				currentState = CJsonPersistence::IS_WAITING_FOR_PARSE_END;
			}
			else
			{
				error_data_t error_data = {"INVALID_DB_ARRAY_SEPARATOR_TOKEN", ERR_CRITICAL}; /* Parsing is aborted since the format is not correct */
				throw error_data;
			}
			break;
		}
		case CJsonPersistence::IS_WAITING_FOR_PARSE_END:
		{
			std::cout << "Parsing completed" << std::endl;
			break;
		}
		default:
		{
			std::cout <<"FATAL ERROR occured!! System Restart needed!";
		}
	}
}
/**
 * Fill the databases with the data from persistent storage. If
 * merge mode is MERGE, the content in the persistent storage
 * will be merged with any content already existing in the data
 * bases. If merge mode is REPLACE, already existing content
 * will be removed before inserting the content from the persistent
 * storage.
 * Note: The current implementation is such that, in case the json parser detects an
 *       error at line number 'n', the valid db objects upto n-1 will be added to the database.
 *       The db objects after 'n' will not be added. There is a possibility of an inconsistent data
 *       in the database, the solution is to store the the db objects locally in a vector/array
 *       and after the successful parse of the json file, add them to the corresponding dbs.
 *
 * @param waypointDb[OUT] the the data base with way points
 * @param poiDb[OUT] the database with points of interest
 * @param mode[IN] the merge mode
 * @return true if the data could be read successfully
 */
bool CJsonPersistence::readData (CWpDatabase& waypointDb, CPoiDatabase& poiDb, MergeMode mode)
{
	bool retVal = true;
	std::fstream jsonFile(m_fileName.c_str(), std::ios::in);
	if(!jsonFile.is_open())
	{
		return false;
	}
	switch(mode)
	{
		case MERGE:
		{
			//no change
			break;
		}
		case REPLACE:
		{
			waypointDb.clearWpDatabase();
			poiDb.clearPoiDatabase();
			break;
		}
		default:
		{
			std::cout << "Invalid Mode Specified" << std::endl;
			return false;
		}
	}
	APT::CJsonScanner scannerObj(jsonFile);
	stateInfo_t stateInfo;
	APT::CJsonToken* currentToken = NULL;
	try
	{
		while(NULL != (currentToken = scannerObj.nextToken())) // Get the token 1 by 1
		{
			try
			{
				processStateChange(currentToken, stateInfo); // Parse the Token through the state machine and take appropriate actions
				if(stateInfo.dataReady) //checks if 1 set of data is extracted successfully
				{
					stateInfo.dataReady = false; // reset the data ready flag
					if(stateInfo.data.isValid) //  Check if the data is valid, if valid, add to database
					switch(stateInfo.dbType)
					{
						case CJsonPersistence::WP_DB:
						{
							CWaypoint wpObj(stateInfo.data.latitude, stateInfo.data.longitude, stateInfo.data.name);
							waypointDb.addWaypoint(wpObj);
							break;
						}
						case CJsonPersistence::POI_DB:
						{
							CPOI poiObj(stateInfo.data.poiType, stateInfo.data.name, stateInfo.data.description, stateInfo.data.latitude, stateInfo.data.longitude);
							poiDb.addPoi(poiObj);
							break;
						}
						default:
						{
							CJsonPersistence::error_data_t errorData;
							errorData.errorMsg   = "INVALID_DB_OBJ_ATTR_NAME";
							throw errorData;
						}
					}
					stateInfo.data.reset(); // resets the data to parse the next set
				}
			}
			catch(CJsonPersistence::error_data_t& errorData) // Exception handling for exceptions from the state machine
			{
				if(stateInfo.data.isValid) // to stop reporting more errors if 1 type of error is detected
				{
					std::cout << "Exception occurred! Exception Cause:" << errorData.errorMsg << " at Line:" << scannerObj.scannedLine()<< std::endl;
					if(errorData.isFatal) // Check if the parsing needs to be continued
					{
						std::cout <<"Aborted Json parsing due to incorrect Json Format, Please correct and try again!" << std::endl;
						retVal = false;
						break;
					}
					stateInfo.data.isValid = false; // This is set so that this invalid data is not written to the database
				}
			}
			catch(...) // default exception handler
			{
				throw; // delegate exception handling to the central default exception handler
			}
		}
	}
	catch(std::string& illegalChar) //Exception handling for exceptions from the scanner class
	{
		std::cout<<"Exception occurred!:Illegal character: " << illegalChar << " in Json file at line number :" << scannerObj.scannedLine() << std::endl;
		std::cout <<"Aborted Json parsing due to incorrect Json Format, Please correct and try again!" << std::endl;
	}
	catch(...) // central default exception handler
	{
		std::cout << "An unknown exception has occurred!" << std::endl;
		std::cout <<"Please check the json file and try again!" << std::endl;
	}
	return retVal;
}
/**
 * Writes the member data(jsonFileContents) to the Json File
 * @return true if the write is successful, false otherwise
 */
bool CJsonPersistence::writeToJsonFile()
{
	bool retVal = true;
	std::fstream jsonFile(m_fileName.c_str(), std::ios::out);
	if(jsonFile.is_open())
	{
		if(!jsonFileContents.empty())
		{
			jsonFile << JSON_BEGIN_STRING << std::endl;
		}
		for(StringVector::const_iterator itr = jsonFileContents.begin(); itr != jsonFileContents.end(); ++itr)
		{
			jsonFile << (*itr) << std::endl;
		}
		if(!jsonFileContents.empty())
		{
			jsonFile << JSON_END_STRING;
		}
	}
	else
	{
		retVal = false;
		std::cout << "ERROR!CJsonPersistence::writeData() failed: Unable to open " << jsonFile << std::endl;
	}
	return retVal;
}
/**
 * Write the data to the persistent storage.
 *
 * @param waypointDb[IN] the data base with way points
 * @param poiDb[IN] the database with points of interest
 * @return true if the data could be saved successfully
 */
bool CJsonPersistence::writeData (const CWpDatabase& waypointDb, const CPoiDatabase& poiDb)
{
	bool retVal = true;

	CPoiDatabase::POIDatabaseMap_t     poiDbMap;
	CWpDatabase::WaypointDatabaseMap_t wpDbMap;

	/* Get Waypoint Database contents from the map and write to Json File */
	waypointDb.getWpDatabase(wpDbMap);
	jsonFileContents.push_back("\t\"waypoints\": [");
	convertToJsonFormat(wpDbMap);
	jsonFileContents.push_back("\t],");

	/* Get Waypoint POI contents from the map and write to Json File */
	poiDb.getPoiDatabase(poiDbMap);
	jsonFileContents.push_back("\t\"pois\": [");
	convertToJsonFormat(poiDbMap);
	jsonFileContents.push_back("\t]");

	retVal = writeToJsonFile();

	return retVal;
}
