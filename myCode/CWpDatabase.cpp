/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CWpDatabase.CPP
 * Author          : Jishnu M Thampan
 * Description     : class CWpDatabase
 * 				     This class maintains the database which consists of
 *					 CWaypoint objects.
 ****************************************************************************/
//Own Include Files
#include "CWpDatabase.h"

//Method Implementations
CWpDatabase::CWpDatabase()
{
}
/**
 * Gets the Pointer to the Waypoint object in the map
 * @param const CWaypoint& wp [IN] - Name (Key value) of the Waypoint Map
 * @return None
 */
CWaypoint* CWpDatabase::getPointerToWaypoint(std::string name)
{
	CWaypoint* pWp = CDatabase::getPointerToData(name);
	return pWp;
}
/**
 * Prints the content of WP Database
 * @param None
 * @return None
 */
void CWpDatabase::print(void)
{
	CDatabase::printDatabase();
}
/**
 * Adds a Waypoint to the Waypoint Database
 * @param const CWaypoint& wp[IN] - Reference to the Waypoint object to be added
 * @return None
 */
void CWpDatabase::addWaypoint(const CWaypoint& wp)
{
	CDatabase::addData(wp.getName(), wp);
}
/**
 * Clears the contents of WP Database
 * @param None
 * @return None
 */
void CWpDatabase::clearWpDatabase(void)
{
	/* Clear the contents of the Database */
	CDatabase::clearDatabase();
}
/**
  * Get the container data from WP Database
  * @param WaypointDatabaseMap_t& wpDbMap [OUT] - The output data structure where the internal container is copied
  * @return None
  */
 void CWpDatabase::getWpDatabase(WaypointDatabaseMap_t& wpDbMap)const
 {
	 (CDatabase::getDatabaseContent(wpDbMap));
 }

