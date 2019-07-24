/***************************************************************************
****************************************************************************
* Filename        : CCSV.H
* Author          : Jishnu M Thampan
* Description     : class CCSV
****************************************************************************/
#ifndef CCSV_H
#define CCSV_H

#include <string>
#include <vector>

#include "CWpDatabase.h"
#include "CPoiDatabase.h"
#include "CAttributeExtractor.h"
#include "CPersistentStorage.h"

class CCSV:public CPersistentStorage
{
	private:
		std::string m_mediaName;	/**< \brief Represents Media Name*/
		std::string m_wpFileName; 	/**< \brief Represents Waypoint File Name (with path)*/
		std::string m_poiFileName;	/**< \brief Represents POI File Name (with path)*/
		CAttributeExtractor m_attributeExtractor; /**< \brief Represents the Utility class which performs attribute extraction*/

		/* Copy constructor and copy assignment operators are
		 * not used */
		CCSV(CCSV&);
		CCSV& operator=(CCSV&);

public:
		CCSV();
		virtual ~CCSV(){};
		typedef std::vector<std::string> StringVector;
		typedef enum
		{
			WP_FILE = 0,
			POI_FILE
		}fileType_t;
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
		 * @param waypointDb the data base with way points
		 * @param poiDb the database with points of interest
		 * @return true if the data could be saved successfully
		 */
		bool writeData (const CWpDatabase& waypointDb, const CPoiDatabase& poiDb);

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
		bool readData (CWpDatabase& waypointDb, CPoiDatabase& poiDb, MergeMode mode);

private:
		/**
		 * Reads the CSV file and copies the contents to the string vector
		 * @param std::string& fileName 		  - Name of the file to be read	[IN]
		 * @param StringVector& fileContents 	  - The contents of the file	[OUT]
		 * @return true if read is successful, false otherwise
		 */
		bool readCSVFile(const std::string& fileName, StringVector& fileContents);
		/**
		 * Parses the CSV file contents, Validates and Stores the contents to the Databases
		 * @param const fileType_t fileType [IN]		     - The File type- POI or Waypoint
		 * @param const CCSV::StringVector& fileContents [IN]- The contents of the file
		 * @param CPoiDatabase& poiDb [OUT]- The POI Database obj where the extracted contents are written into
		 * @param CWpDatabase &wpDb   [OUT]- The Waypoint Database obj where the extracted contents are written into
		 * @return void	- Errors are displayed as when encountered, not reported to the caller.
		 */
		bool parseCSVFileContents(const fileType_t& fileType, const CCSV::StringVector& fileContents, CPoiDatabase& poiDb, CWpDatabase &wpDb);
		/**
		 * Writes the Waypoint Database contents into the CSV Waypoint File
		 * @param const CWpDatabase& waypointDb [IN] - The reference to the Waypoint Database
		 * @return true if the write is successful, false otherwise
		 */
		bool writeToCSVWaypointFile(const CWpDatabase& waypointDb);
		/**
		 * Writes the POI Database contents into the CSV POI File
		 * @param const CPoiDatabase& poiDb [IN] - The reference to the POI Database
		 * @return true if the write is successful, false otherwise
		 */
		bool writeToCSVPoiFile(const CPoiDatabase& poiDb);
		/**
		 * Validates and if valid stores the Waypoint in the Waypoint Database
		 * @param const StringVector& attributeVec [IN] - The string vector consisting of the extracted attributes
		 * @param CPoiDatabase& poiDb [OUT]	- Waypoint Database where the validated content is to be stored
		 * @return true if validation is successful, false otherwise
		 */
		bool validateAndStoreWaypoint(const StringVector& attributeVec, CWpDatabase& wpDb);
		/**
		 * Validates and if valid stores the POI in the POI Database
		 * @param const StringVector& attributeVec [IN] - The string vector consisting of the extracted attributes
		 * @param CPoiDatabase& poiDb [OUT]	- POI Database where the validated content is to be stored
		 * @return true if validation is successful, false otherwise
		 */
		bool validateAndStorePoi(const StringVector& attributeVec, CPoiDatabase& poiDb);
		/**
		 * Initializes the Max attributes for the attribute extractor depending upon the file type
		 * @param const fileType_t& fileType [IN] - The file type - Waypoint file or POI file
		 * @return true if initialization is successful, false otherwise
		 */
		bool initMaxAttributes(const fileType_t& fileType);
};

#endif //CCSV_H
