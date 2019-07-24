/***************************************************************************
****************************************************************************
* Filename        : Typemapping.H
* Author          : Jishnu M Thampan
* Description     : Namespace type_mapping
****************************************************************************/
#ifndef TYPE_MAPPING_H
#define TYPE_MAPPING_H

#include <string>
#include <vector>
#include <sstream>

#include "CPOI.h"
#include "CWaypoint.h"

#define MAX_POI_ATTRIBUTES (5)		/**< \brief Represents Max number of POI Attributes */
#define MAX_WAYPOINT_ATTRIBUTES (3)	/**< \brief Represents Max number of Waypoint Attributes */

const std::string TAB_STRING 			= "\t";
const std::string DOUBLE_TAB_STRING 	= "\t\t";
const std::string COLON_STRING 			= ":";
const std::string QUOTE_STRING 			= "\"";
const std::string COMA_STRING  			= ",";

/**
 * NameSpace type_mapping stores all the relevant methods of
 * converting from one type to another. There is no storage involved and hence all
 * are simple C-like utility functions. Additionally it performs all necessary
 * validations while converting from one type to another.
 * Also reports the relevant errors which might arise during
 * the type conversions.
 */
namespace type_mapping
{

	typedef enum
	{
		RC_SUCCESS = 0, 						/**< \brief Represents No Error*/
		RC_INVALID_LATITUDE,					/**< \brief Represents Error of type Invalid latitude */
		RC_INVALID_LONGITUDE, 					/**< \brief Represents Error of type Invalid longitude */
		RC_INVALID_NAME,						/**< \brief Represents Error of type Invalid name */
		RC_INVALID_DESCRIPTION, 				/**< \brief Represents Error of type Invalid description */
		RC_INVALID_POI_TYPE,					/**< \brief Represents Error of type Invalid POI type */
		RC_INVALID_WAYPOINT,					/**< \brief Represents Error of type Invalid Waypoint */
		RC_INCORRECT_NUMBER_OF_ATTRIBUTES,		/**< \brief Represents Error of type Invalid number of attributes */
		RC_INVALID_POI							/**< \brief Represents Error of type Invalid POI */
	}errorCode_t;

	/**
	 * Converts the string to longitude type. Performs the necessary validations for the same.
	 * @param const std::string& attr [IN] - Input string to be converted
	 * @param double& longitude [OUT] - Converted data
	 * @return true if conversion is successful, else false
	 */
	bool convertStringToLongitude(const std::string& attr, double& longitude);
	/**
	 * Converts the string to latitude type. Performs the necessary validations for the same.
	 * @param const std::string& attr [IN] - Input string to be converted
	 * @param double& latitude [OUT] - Converted data
	 * @return true if conversion is successful, else false
	 */
	bool convertStringToLatitude(const std::string& attr, double& latitude);
	/**
	 * Converts the string to double type. Performs the necessary validations for the same.
	 * @param const std::string& attr [IN] - Input string to be converted
	 * @param double& val [OUT] - Converted data
	 * @return true if conversion is successful, else false
	 */
	bool convertStringToDouble(const std::string& attr, double& val);
	/**
	 * Converts the string to POI type. Performs the necessary validations for the same.
	 * @param const std::string& stringType [IN] - Input string to be converted
	 * @param CPOI::t_poi& poiType [OUT] - Converted data
	 * @return true if conversion is successful, else false
	 */
	 bool convertStringToPoiType(const std::string& stringType, CPOI::t_poi& poiType);

	/**
	 * Converts the string to CPOI type. Performs the necessary validations for the same.
	 * @param const std::vector<std::string>& poiAttributeVec [IN] - Input string vector consisting of the CPOI attributes to be converted
	 * @param CPOI &poi [OUT] - Converted data which of CPOI type
	 * @return errorCode_t    - Error codes based on the conversion result
	 */
	errorCode_t convertStringToCPoiType(const std::vector<std::string>& poiAttributeVec, CPOI &poi);
	/**
	 * Gets the Error Type in String representation
	 * @param  const errorCode_t& ec [IN] - The errorCode to be converted
	 * @return string - the equivalent string represenation of the error code
	 */
	std::string getErrorType(const errorCode_t& ec);
	/**
	 * Converts the string to CWaypoint type. Performs the necessary validations for the same.
	 * @param const std::vector<std::string>&wpAttributeVec [IN] - Input string vector consisting of the Waypoint attributes to be converted
	 * @param double& val [OUT] - Converted data which is of Waypoint type
	 * @return errorCode_t      - Error codes based on the conversion result
	 */
	errorCode_t convertStringToCWaypointType(const std::vector<std::string>&wpAttributeVec, CWaypoint &wp);
	/**
	 * Converts the POI type to String. Performs the necessary validations for the same.
	 * @param const CPOI::t_poi& poiType [IN] - POI type data to be converted
	 * @param std::string& stringType [OUT] - Converted data of string type
	 * @return true if conversion is successful, else false
	 */
	bool convertPoiTypeToString(const CPOI::t_poi& poiType, std::string& stringType);

	/**
	 * Validates the String data
	 * @param const std::string& attr[IN] - the string data to be validated
	 * @return True if Valid string data, else False
	 */
	bool validateStringData(const std::string& attr);

	bool validateLatitude(double&);
	bool validateLongitude(double&);

	template<typename T>
	std::string to_string(T& data)
	{
		std::stringstream ss;
		ss << data;
		std::string result = ss.str();
		return result;
	}

}// namespace type_mapping

#endif // TYPE_MAPPING_H
