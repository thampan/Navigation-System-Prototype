/***************************************************************************
* ============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : Typemapping.cpp
* Author          : Jishnu M Thampan
* Description     :
****************************************************************************/
#include <sstream>

#include <nsp/TypeMapping.h>

#define POI_TYPE_POS (0) /**< \brief Represents the position of POI Type */
#define POI_NAME_POS (1) /**< \brief Represents the position of POI Name */
#define POI_DESCRIPTION_POS                                                    \
  (2) /**< \brief Represents the position of POI Description */
#define POI_LATITUDE_POS                                                       \
  (3) /**< \brief Represents the position of POI Latitude */
#define POI_LONGITUDE_POS                                                      \
  (4) /**< \brief Represents the position of POI Longitude */

#define WAYPOINT_NAME_POS                                                      \
  (0) /**< \brief Represents the position of Waypoint name */
#define WAYPOINT_LATITUDE_POS                                                  \
  (1) /**< \brief Represents the position of Waypoint latitude */
#define WAYPOINT_LONGITUDE_POS                                                 \
  (2) /**< \brief Represents the position of Waypoint longitude */

/**
 * NameSpace type_mapping stores all the relevant methods of converting from one
 * type to another.
 * There is no storage involved and hence all are simple C-like utility
 * functions. Additionally
 * it performs all necessary validations while converting from one type to
 * another.
 * Also reports the relevant errors which might arise during the type
 * conversions.
 */
namespace type_mapping {

/**
 * Validates the POI Type
 * @param  const CPOI::t_poi& poiType[IN] - t_poi type
 * @return True if Valid t_poi Type, else False
 */
static bool validatePoiType(const CPOI::t_poi &poiType);

bool validateLatitude(double &latitude) {
  if ((MIN_LATITUDE <= latitude) &&
      (MAX_LATITUDE >=
       latitude)) /* Checks if the Latitude is within the allowed range */
  {
    return true;
  }
  return false;
}
bool validateLongitude(double &longitude) {
  if ((MIN_LONGITUDE <= longitude) &&
      (MAX_LONGITUDE >=
       longitude)) /* Checks if the Longitude is within the allowed range */
  {
    return true;
  }
  return false;
}
/**
 * Converts the string to latitude type. Performs the necessary validations for
 * the same.
 * @param const std::string& attr [IN] - Input string to be converted
 * @param double& latitude [OUT] - Converted data
 * @return true if conversion is successful, else false
 */
bool convertStringToLatitude(const std::string &attr, double &latitude) {
  if (convertStringToDouble(attr, latitude)) {
    return validateLatitude(latitude);
  }
  return false;
}
/**
 * Converts the string to longitude type. Performs the necessary validations for
 * the same.
 * @param const std::string& attr [IN] - Input string to be converted
 * @param double& longitude [OUT] - Converted data
 * @return true if conversion is successful, else false
 */
bool convertStringToLongitude(const std::string &attr, double &longitude) {
  if (convertStringToDouble(attr, longitude)) {
    return validateLongitude(longitude);
  }
  return false;
}
/**
 * Validates the String data
 * @param const std::string& attr - the string data to be validated
 * @return True if Valid string data, else False
 */
bool validateStringData(const std::string &attr) {
  if (attr.find_first_not_of(' ') != std::string::npos) {
    return true;
  }
  return false;
}
/**
 * Validates the POI Type
 * @param  const CPOI::t_poi& poiType - t_poi type
 * @return True if Valid t_poi Type, else False
 */
bool validatePoiType(const CPOI::t_poi &poiType) {
  bool retVal = false;
  if ((poiType < CPOI::UNKNOWN) && (poiType >= 0)) {
    retVal = true;
  }
  return retVal;
}
/**
 * Converts the string to double type. Performs the necessary validations for
 * the same.
 * @param const std::string& attr [IN] - Input string to be converted
 * @param double& val [OUT] - Converted data
 * @return true if conversion is successful, else false
 */
bool convertStringToDouble(const std::string &attr, double &val) {
  std::stringstream ss(attr);
  ss >> val;
  if (ss.fail()) {
    return false;
  }
  unsigned int dotCount = 0, i = 0;
  for (; i < attr.length() && (dotCount <= 1); i++) {
    char character = attr[i];
    /* Validation-1: Allow '-' character only in the beginning to indicate a
     * negative number */
    if (character == '-' && i == 0)
      continue;

    /* Validation-2: Allow only numericals between 0-9 */
    else if (character >= '0' && character <= '9')
      continue;

    /* Validation-3: Allow decimal point only once */
    else if (character == '.')
      dotCount++;

    /* If any of the above validations are not passed, stop checking further */
    else
      break;
  }
  /* Iterator has reached the end of the string => all characters have been
   * validated and fine,
   * Else validation has failed at some point */
  return (i == attr.length());
}
/**
 * Converts the string to CPOI type. Performs the necessary validations for the
 * same.
 * @param const std::vector<std::string>& poiAttributeVec [IN] - Input string
 * vector consisting of the CPOI attributes to be converted
 * @param CPOI &poi [OUT] - Converted data which of CPOI type
 * @return errorCode_t    - Error codes based on the conversion result
 */
errorCode_t
convertStringToCPoiType(const std::vector<std::string> &poiAttributeVec,
                        CPOI &poi) {
  errorCode_t retVal = RC_SUCCESS;
  double latitude = 0, longitude = 0;
  std::string name, description;
  do {
    /* Validation-1: Check if there is an attribute mismatch */
    if (poiAttributeVec.size() != MAX_POI_ATTRIBUTES) {
      retVal = RC_INCORRECT_NUMBER_OF_ATTRIBUTES;
      break;
    }
    CPOI::t_poi poiType;
    /* Validation-2: Check if there is a valid POI type */
    if (!convertStringToPoiType(poiAttributeVec.at(POI_TYPE_POS), poiType)) {
      retVal = RC_INVALID_POI_TYPE;
      break;
    }
    /* Validation-3: Check if there is a valid latitude */
    if (!convertStringToLatitude(poiAttributeVec.at(POI_LATITUDE_POS),
                                 latitude)) {
      retVal = RC_INVALID_LATITUDE;
      break;
    }
    /* Validation-4: Check if there is a valid longitude */
    if (!convertStringToLongitude(poiAttributeVec.at(POI_LONGITUDE_POS),
                                  longitude)) {
      retVal = RC_INVALID_LONGITUDE;
      break;
    }
    /* Validation-5: Check if there is a valid name */
    if (!validateStringData((name = poiAttributeVec.at(POI_NAME_POS)))) {
      retVal = RC_INVALID_NAME;
      break;
    }
    /* Validation-6: Check if there is a valid description */
    if (!validateStringData(
            (description = poiAttributeVec.at(POI_DESCRIPTION_POS)))) {
      retVal = RC_INVALID_DESCRIPTION;
      break;
    }
    /* Validation-7: Check if the POI itself is valid */
    if (!(poi.set(poiType, name, description, latitude, longitude))) {
      retVal = RC_INVALID_POI;
      break;
    }
  } while (false);

  return retVal;
}
/**
 * Converts the string to CWaypoint type. Performs the necessary validations for
 * the same.
 * @param const std::vector<std::string>&wpAttributeVec [IN] - Input string
 * vector consisting of the Waypoint attributes to be converted
 * @param double& val [OUT] - Converted data which is of Waypoint type
 * @return errorCode_t      - Error codes based on the conversion result
 */
errorCode_t
convertStringToCWaypointType(const std::vector<std::string> &wpAttributeVec,
                             CWaypoint &wp) {
  errorCode_t retVal = RC_SUCCESS;
  double latitude = 0, longitude = 0;
  std::string name, description;
  do {
    /* Validation-1: Check if there is an attribute mismatch */
    if (wpAttributeVec.size() != MAX_WAYPOINT_ATTRIBUTES) {
      retVal = RC_INCORRECT_NUMBER_OF_ATTRIBUTES;
      break;
    }
    /* Validation-2: Check if there is a valid latitude */
    if (!convertStringToLatitude(wpAttributeVec.at(WAYPOINT_LATITUDE_POS),
                                 latitude)) {
      retVal = RC_INVALID_LATITUDE;
      break;
    }
    /* Validation-3: Check if there is a valid longitude */
    if (!convertStringToLongitude(wpAttributeVec.at(WAYPOINT_LONGITUDE_POS),
                                  longitude)) {
      retVal = RC_INVALID_LONGITUDE;
      break;
    }
    /* Validation-4: Check if there is a valid name */
    if (!validateStringData((name = wpAttributeVec.at(WAYPOINT_NAME_POS)))) {
      retVal = RC_INVALID_NAME;
      break;
    }
    /* Validation-5: Finally, Check if the Waypoint itself is valid */
    if (!wp.set(name, latitude, longitude)) {
      retVal = RC_INVALID_WAYPOINT;
      break;
    }
  } while (false);

  return retVal;
}
/**
 * Converts the POI type to String. Performs the necessary validations for the
 * same.
 * @param const CPOI::t_poi& poiType [IN] - POI type data to be converted
 * @param std::string& stringType [OUT] - Converted data of string type
 * @return true if conversion is successful, else false
 */
bool convertPoiTypeToString(const CPOI::t_poi &poiType,
                            std::string &stringType) {
  bool retVal = true;
  switch (poiType) {
  case CPOI::RESTAURANT: {
    stringType = "RESTAURANT";
    break;
  }
  case CPOI::TOURISTIC: {
    stringType = "TOURISTIC";
    break;
  }
  case CPOI::GASSTATION: {
    stringType = "GASSTATION";
    break;
  }
  case CPOI::UNIVERSITY: {
    stringType = "UNIVERSITY";
    break;
  }
  default: {
    stringType = "UNKNOWN";
    retVal = false;
  }
  }
  return retVal;
}
/**
 * Converts the string to POI type. Performs the necessary validations for the
 * same.
 * @param const std::string& stringType [IN] - Input string to be converted
 * @param CPOI::t_poi& poiType [OUT] - Converted data
 * @return true if conversion is successful, else false
 */
bool convertStringToPoiType(const std::string &stringType,
                            CPOI::t_poi &poiType) {
  bool retVal = false;
  if ("RESTAURANT" == stringType) {
    poiType = CPOI::RESTAURANT;
  } else if ("TOURISTIC" == stringType) {
    poiType = CPOI::TOURISTIC;
  } else if ("GASSTATION" == stringType) {
    poiType = CPOI::GASSTATION;
  } else if ("UNIVERSITY" == stringType) {
    poiType = CPOI::UNIVERSITY;
  } else {
    poiType = CPOI::UNKNOWN;
  }
  if (validatePoiType(poiType)) {
    retVal = true;
  }
  return retVal;
}
/**
 * Gets the Error Type in String representation
 * @param  const errorCode_t& ec [IN] - The errorCode to be converted
 * @return string - the equivalent string represenation of the error code
 */
std::string getErrorType(const errorCode_t &ec) {
  std::string errorType = "Unknown";
  switch (ec) {
  case RC_INVALID_LATITUDE: {
    errorType = "INVALID_LATITUDE";
    break;
  }
  case RC_INVALID_LONGITUDE: {
    errorType = "INVALID_LONGITUDE";
    break;
  }
  case RC_INVALID_POI_TYPE: {
    errorType = "INVALID_POI_TYPE";
    break;
  }
  case RC_INVALID_WAYPOINT: {
    errorType = "INVALID_WAYPOINT";
    break;
  }
  case RC_INVALID_POI: {
    errorType = "INVALID_POI";
    break;
  }
  case RC_INCORRECT_NUMBER_OF_ATTRIBUTES: {
    errorType = "INCORRECT_NUMBER_OF_ATTRIBUTES";
    break;
  }
  case RC_INVALID_NAME: {
    errorType = "INVALID_NAME";
    break;
  }
  case RC_INVALID_DESCRIPTION: {
    errorType = "INVALID_DESCRIPTION";
    break;
  }
  default: { errorType = "Unknown"; }
  }
  return errorType;
}

} // namespace type_mapping
