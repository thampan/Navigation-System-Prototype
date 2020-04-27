/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CJson.h
* Author          : Jishnu M Thampan
* Description     : class CJson
****************************************************************************/
#ifndef CJSON_H
#define CJSON_H

#include <string>
#include <vector>

#include "CJsonScanner.h"
#include "CPersistentStorage.h"
#include "CPoiDatabase.h"
#include "CWpDatabase.h"
#include "TypeMapping.h"

#define DOUBLE_SPACE "  "      /**< \brief Represents Double Space           */
#define START_OBJECT_TOKEN "{" /**< \brief Represents the start object token   \
                                  */
#define END_OBJECT_TOKEN "}"   /**< \brief Represents end object token       */
#define OBJECT_SEPARATOR_TOKEN                                                 \
  "," /**< \brief Represents Object Separator token */

class CJsonPersistence : public CPersistentStorage {
private:
  typedef std::vector<std::string>
      StringVector; /**< \brief Represents a vector of strings */

  typedef struct {
    std::string
        errorMsg; /**< \brief Describes the error message to be displayed */
    bool isFatal; /**< \brief Describes if the error is fatal or minor */
  } error_data_t; /**< \brief Contains the error data */

  typedef enum {
    IS_WAITING_FOR_FIRST_TOKEN, /**< \brief Represents the
                                   IS_WAITING_FOR_FIRST_TOKEN state */
    IS_WAITING_FOR_DB_NAME, /**< \brief Represents the IS_WAITING_FOR_DB_NAME
                               state */
    IS_WAITING_FOR_DB_NAME_SEPARATOR,           /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_NAME_SEPARATOR state
                                                   */
    IS_WAITING_FOR_DB_ARRAY_BEGIN,              /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_ARRAY_BEGIN state */
    IS_WAITING_FOR_DB_OBJ_BEGIN,                /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_BEGIN state */
    IS_WAITING_FOR_DB_OBJ_ATTR_NAME,            /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_ATTR_NAME state */
    IS_WAITING_FOR_DB_OBJ_ATTR_NAME_SEPARATOR,  /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_ATTR_NAME_SEPARATOR
                                                   state */
    IS_WAITING_FOR_DB_OBJ_ATTR_VALUE,           /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_ATTR_VALUE state
                                                   */
    IS_WAITING_FOR_DB_OBJ_ATTR_VALUE_SEPARATOR, /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_ATTR_VALUE_SEPARATOR
                                                   state */
    IS_WAITING_FOR_DB_OBJ_SEPARATOR,            /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_SEPARATOR state */
    IS_WAITING_FOR_DB_ARRAY_SEPARATOR,          /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_ARRAY_SEPARATOR
                                                   state */
    IS_WAITING_FOR_DB_OBJ_END,                  /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_OBJ_END state */
    IS_WAITING_FOR_DB_ARRAY_END,                /**< \brief Represents the
                                                   IS_WAITING_FOR_DB_ARRAY_END state */
    IS_WAITING_FOR_PARSE_END,                   /**< \brief Represents the
                                                   IS_WAITING_FOR_PARSE_END state */
  } parser_state_t; /**< \brief Represents the states of the parser state
                       machine */

  typedef enum {
    WP_DB,     /**< \brief Represents the Waypoint Database */
    POI_DB,    /**< \brief Represents the POI Database */
    UNKNOWN_DB /**< \brief Represents an Invalid Database */
  } db_type_t; /**< \brief Represents the states of the types of supported
                  databases */

  typedef struct __data {
    double latitude;  /**< \brief Represents the latitude */
    double longitude; /**< \brief Represents the longitude */
    std::string name; /**< \brief Represents the name */
    std::string
        description;     /**< \brief Represents the description of the poi */
    CPOI::t_poi poiType; /**< \brief Represents the type of the poi */
    bool isValid;        /**< \brief Represents if the data is valid */
                         /**
                          * Resets the contents of the data .
                          */
    void reset() {
      latitude = 0;
      longitude = 0;
      poiType = CPOI::UNKNOWN;
      name = DEFAULT_STRING;
      isValid = true;
      description = DEFAULT_STRING;
    }
    /**
     * Constructor which resets the contents of the data .
     */
    __data() { reset(); }
  } data_t; /**< \brief Represents the data contained in the databases */

  typedef struct __stateInfo {
    data_t data; /**< \brief Represents data in the json file*/
    parser_state_t
        currentState; /**< \brief Represents the current state of the state
                         machine*/
    db_type_t dbType; /**< \brief Represents the type of database*/
    std::string
        parameter;  /**< \brief Represents the type of parameter found in the
                       previous iteration*/
    bool dataReady; /**< \brief Set if all the data is extracted and ready*/
                    /**
                     * Constructor which resets the contents of the stateInfo .
                     */
    __stateInfo() {
      currentState = CJsonPersistence::IS_WAITING_FOR_FIRST_TOKEN;
      dbType = CJsonPersistence::UNKNOWN_DB;
      dataReady = false;
    }
  } stateInfo_t;

  std::string m_mediaName; /**< \brief Represents Media Name*/
  std::string
      m_fileName; /**< \brief Represents Waypoint File Name (with path)*/
  StringVector jsonFileContents; /**< \brief Represents a Vector of strings to
                                    hold the json file contents */
  /* Copy constructor and copy assignment operators are
   * not used */
  CJsonPersistence(CJsonPersistence &);
  CJsonPersistence &operator=(CJsonPersistence &);

public:
  CJsonPersistence(){};

  virtual ~CJsonPersistence(){};
  /**
   * Set the name of the media to be used for persistent storage.
   * The exact interpretation of the name depends on the implementation
   * of the component.
   *
   * @param name the media to be used
   */
  void setMediaName(std::string name);
  /**
   * Write the data to the persistent storage.
   *
   * @param waypointDb[IN] the data base with way points
   * @param poiDb[IN] the database with points of interest
   * @return true if the data could be saved successfully
   */
  bool writeData(const CWpDatabase &waypointDb, const CPoiDatabase &poiDb);

  /**
   * Fill the databases with the data from persistent storage. If
   * merge mode is MERGE, the content in the persistent storage
   * will be merged with any content already existing in the data
   * bases. If merge mode is REPLACE, already existing content
   * will be removed before inserting the content from the persistent
   * storage.
   *
   * @param waypointDb[OUT] the the data base with way points
   * @param poiDb[OUT] the database with points of interest
   * @param mode[OUT] the merge mode
   * @return true if the data could be read successfully
   */
  bool readData(CWpDatabase &waypointDb, CPoiDatabase &poiDb, MergeMode mode);

private:
  /**
   * State machine handling the read operation
   *
   * @param CJsonToken* currentToken: [IN] the current Token extracted from the
   * Json file
   * @param stateInfo_t& stateInfo:[OUT]the Reference to the structure
   * containing the attributes of the current state
   * @return None
   */
  void processStateChange(APT::CJsonToken *currentToken,
                          stateInfo_t &stateInfo);
  /**
   * Gets the parameter name and assigns it to out based on the database type
   *
   * @param db_type_t& db: [IN] the Reference to the current database which is
   * extracted from json file
   * @param std::string& in: [IN] the Reference to the current input Token
   * extracted from the Json file
   * @param std::string& out:[OUT]the Reference to the type of the parameter
   * @return true if the database and parameter is a valid one, false otherwise
   */
  bool checkAndExtractParameterName(const db_type_t &db, const std::string &in,
                                    std::string &out);
  /**
   * Checks if the given db is valid or not
   *
   * @param db_type_t& db: [OUT] the Reference to the current database which is
   * extracted from json file
   * @param std::string& dbName: [IN] the Reference to the current database
   * string extracted from the Json file
   * @return true if the database is a valid one, false otherwise
   */
  bool isValidDb(const std::string &dbName, db_type_t &dbType);
  /**
   * Checks if the Parsing of data is completed
   *
   * @param stateInfo_t& stateInfo: [IN] the Reference to the structure
   * containing the attributes of the current state
   * @return true if the data parsing has been completed, false otherwise
   */
  bool isDataParseComplete(const stateInfo_t &stateInfo);
  /**
   * Writes the member data(jsonFileContents) to the Json File
   * @return true if the write is successful, false otherwise
   */
  bool writeToJsonFile();
  /**
   * Converts the data to json compatible format
   *
   * @param std::map<std::string, DataType>& dataMap: [IN] the Reference to the
   * Map containing the Data contents (Wp Data or POI Data)
   * @return None
   */
  template <class DataType>
  void convertToJsonFormat(const std::map<std::string, DataType> &dataMap) {
    size_t count = 1;
    for (typename std::map<std::string, DataType>::const_iterator
             itr = dataMap.begin();
         itr != dataMap.end(); ++itr, ++count) {
      DataType dataType = itr->second;
      formatJsonData(dataType);
      (count == dataMap.size()) ? jsonFileContents.push_back("\t  }")
                                : jsonFileContents.push_back("\t  },");
    }
  }
  /**
   * Formats the data to json compatible format with necessary tabs and quotes
   *
   * @param std::string parameter: [IN] the parameter in the database
   * @param std::string& value: [IN] the value representing the string/number
   * @param bool isNumber: [IN] if the input is a number, quotes will not be
   * applied to it
   * @return string - Modified string
   */
  inline std::string to_json_attributes(std::string parameter,
                                        std::string &value,
                                        bool isNumber = false) {
    std::string result = DOUBLE_TAB_STRING + QUOTE_STRING + parameter +
                         QUOTE_STRING + COLON_STRING;
    if (!isNumber) {
      result += QUOTE_STRING + value + QUOTE_STRING;
    } else {
      result += value;
    }
    return result;
  }
  /**
   * Fills the jsonContents after formatting the data
   *
   * @param Database& databaseObj: [IN] the Reference to the Database object -
   * WaypointDb or POIDb
   * @return None
   */
  template <typename Database> void formatJsonData(Database &databaseObj) {
    CPOI *poiPtr = dynamic_cast<CPOI *>(&databaseObj);
    jsonFileContents.push_back("\t  {");

    double latitude = databaseObj.getLatitude();
    double longitude = databaseObj.getLongitude();

    std::string nameStr = databaseObj.getName();
    std::string jsonName = to_json_attributes("name", nameStr) + COMA_STRING;
    jsonFileContents.push_back(jsonName);

    std::string latitudeStr = type_mapping::to_string(latitude);
    std::string jsonLatitudeStr =
        to_json_attributes("latitude", latitudeStr, true) + COMA_STRING;
    jsonFileContents.push_back(jsonLatitudeStr);

    std::string longitudeStr = type_mapping::to_string(longitude);
    std::string jsonLongitudeStr =
        to_json_attributes("longitude", longitudeStr, true);
    if (poiPtr != NULL)
      jsonLongitudeStr += COMA_STRING;

    jsonFileContents.push_back(jsonLongitudeStr);

    if (poiPtr) {
      std::string poiType;
      /* Convert the POI Type to String to be stored in the POI file */
      if (!type_mapping::convertPoiTypeToString(poiPtr->getType(), poiType)) {
        std::cout << "ERROR!JSON::writeData() failed! Invalid POI Type!"
                  << std::endl;
      }
      std::string descriptionData = poiPtr->getDescription();
      std::string jsonDescription =
          to_json_attributes("description", descriptionData) + COMA_STRING;
      jsonFileContents.push_back(jsonDescription);

      std::string jsonPoi = to_json_attributes("type", poiType);
      jsonFileContents.push_back(jsonPoi);
    }
  }
};

#endif // CJSON_PERSISTENCE_H
