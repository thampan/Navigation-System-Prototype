/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CPOIDATABASE.H
* Author          : Jishnu M Thampan
* Description     : class CPoiDatabase
*					This class maintains the database which consists of
*					CPOI objects.
*
****************************************************************************/

#ifndef CPOIDATABASE_H
#define CPOIDATABASE_H

#include <map>
//Own Include Files
#include "CPOI.h"
#include "CDatabase.h"


/**
 * CPoiDatabase class maintains the database which consists of
 * CPOI objects.
 */
class CPoiDatabase:public CDatabase<std::string, CPOI>
{

public:
	typedef std::map<std::string, CPOI> POIDatabaseMap_t;
    /**
     * Constructor of CPoiDatabase class
     * Initializes the Number of POIs to MIN_POI_DATABASE and
     * clears the POI Database
     */
    CPoiDatabase();
    /**
    * Add a POI to the POIDatabase
    * If the POI table is not full, adds the POI with the given attributes to the PoiDatabase
    * Else Display an error.
    * @param t_poi type 			- type of the POI object		[IN]
    * @param std::string name 		- Name of the POI object		[IN]
    * @param std::string description- Description of the POI object [IN]
    * @param double latitude 		- Latitude of the POI object	[IN]
    * @param double longitude 		- Longitude of the POI object	[IN]
    * @return None
    */
    void addPoi(CPOI const& poi);
    /**
     * Get Pointer to the POI object in the POI Database
     * Note: In case of multiple POI objects with the same name, the first one is returned.
     *       If no match is found NULL is returned
     * @param std::string name - Name of the POI object which is to be searched in the POI Database [IN]
     * @return CPOI* 		   - If present, returns the pointer to the CPOI object in the database, else NULL
     */
    CPOI* getPointerToPoi(std::string name);
    /**
     * Get the container data from POI Database
     * @param POIDatabaseMap_t& poidDbMap [OUT] - The output data structure where the internal container is copied
     * @return None
     */
    void getPoiDatabase(POIDatabaseMap_t& poidDbMap)const;
    /**
	 * Clears the contents of POI Database
	 * @param None
	 * @return None
	 */
    void clearPoiDatabase(void);
    /**
	 * Prints the content of POI Database
	 * @param None
	 * @return None
	 */
    void print(void);
};
/********************
**  CLASS END
*********************/
#endif /* CPOIDATABASE_H */
