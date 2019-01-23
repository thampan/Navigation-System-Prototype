/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CWpDATABASE.H
* Author          : Jishnu M Thampan
* Description     : class CWpDatabase
*					This class maintains the database which consists of
*					CWp objects.
*
****************************************************************************/

#ifndef CWpDATABASE_H
#define CWpDATABASE_H

#include <map>

//Own Include Files
#include "CWaypoint.h"
#include "CDatabase.h"

class CWpDatabase:public CDatabase<std::string,CWaypoint>
{

public:
	typedef std::map<std::string, CWaypoint> WaypointDatabaseMap_t;
    /**
	 * Constructor of CWpDatabase class
	 */
    CWpDatabase();
    /**
	 * Adds a Waypoint to the Waypoint Database
	 * @param const CWaypoint& wp [IN] - Reference to the Waypoint object to be added
	 * @return None
	 */
    void addWaypoint(const CWaypoint& wp);
    /**
	 * Gets the Pointer to the Waypoint object in the map
	 * @param const CWaypoint& wp [IN] - Name (Key value) of the Waypoint Map
	 * @return None
	 */
    CWaypoint* getPointerToWaypoint(const std::string name);
    /**
	 * Clears the contents of WP Database
	 * @param None
	 * @return None
	 */
    void clearWpDatabase(void);
    /**
	 * Prints the content of WP Database
	 * @param None
	 * @return None
	 */
    void print(void);
    /**
     * Get the container data from POI Database
     * @param POIDatabaseMap_t& poidDbMap [OUT] - The output data structure where the internal container is copied
     * @return None
     */
    void getWpDatabase(WaypointDatabaseMap_t& wpDbMap)const;
};
/********************
**  CLASS END
*********************/
#endif /* CWpDATABASE_H */
