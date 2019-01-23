/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CNAVIGATIONSYSTEM.H
* Author          : Jishnu M Thampan
* Description     : class CNavigationSystem
* 					This class is the controller class for the Navigation
* 					System. It is responsible of creating a POI database,
* 					creating a route which may consist of Waypoints and
* 					Routes.It takes input from GPS Sensor and calculates
* 					distance to the nearest POI.
* 					CNavigationSystem consists of 3 main modules
* 					1. GPS Sensor module
* 					2. Route module
* 					3. POI Database module
****************************************************************************/

#ifndef CNAVIGATIONSYSTEM_H
#define CNAVIGATIONSYSTEM_H

#include "CCSV.h"
#include "CGPSSensor.h"
#include "CJsonPersistence.h"
#include "CRoute.h"
#include "CPoiDatabase.h"
#include "CWpDatabase.h"
/**
 * This class is the controller class for the Navigation
 * System. It is responsible of creating a POI database,
 * creating a route which may consist of Waypoints and Routes.
 * CNavigationSystem consists of 3 main modules
 * 1. GPS Sensor module
 * 2. Route module
 * 3. POI Database module
 */
class CNavigationSystem
{
private:
 /* Private Member variables */

    CGPSSensor m_GPSSensor;     /**< \brief GPS Sensor object */
    CRoute m_route; 		    /**< \brief Route object */
    CPoiDatabase m_PoiDatabase; /**< \brief CPoiDatabase object */
    CWpDatabase m_WpDatabase; /**< \brief CWpDatabase object */
	CCSV csv;
	CJsonPersistence cj;
 /* Private Member Functions */

    /**
     * Gets the current position from the GPS Sensor.
     * If it is valid, it prints the distance to the nearest POI,
     * else displays an error message
     * @param  None
     * @return None
     */
    void printDistanceCurPosNextPoi(void);
	/**
	 * Prints the route which includes the Waypoints and Point of interests
	 * @param  None
	 * @return None
	 */
    void printRoute(void);
	/**
	 * Adds Waypoints and point of interests to the route
	 * @param  None
	 * @return None
	 */
    void enterRoute(void);

 /* Test Cases section */

    /**
	 * Test case to add Waypoints more than the specified limit.
	 * Expected Result: Addition of the waypoints failes due to size constraints
	 * and prints an error message
	 * @param  None
	 * @return None
	 */
    void TC_addMoreWaypointsThanSpecified();
    /**
	 * Test case to add an Invalid POI ( POI not in POI Database) to the Route
	 * Expected Result: Addition fails and prints an error message
	 * @param  None
	 * @return None
	 */
	void TC_addInvalidPoi();
    /**
	 * Test case to add all the POIs in the POI Database to the route
	 * Expected Result: All POIs would be successfully added, provided the size
	 * is huge enough to accommodate all.
	 * @param  None
	 * @return None
	 */
	void TC_addAllPoiToRoute();
    /**
	 * Test case to add more POIs than the specified limit
	 * Expected Result: Addition fails and prints an error message
	 * @param  None
	 * @return None
	 */
	void TC_addMorePoiThanSpecifiedToRoute();
    /**
	 * Test case to add more POIs than the specified limit to the POI Database.
	 * Expected Result: Addition fails and prints an error
	 * @param  None
	 * @return None
	 */
	void TC_poiDatabaseOverflow();
    /**
	 * Test case to add no POIs at all to the POI Database and then try to
	 * add one to the route
	 * Expected Result: No crashes or impacts to the waypoint part.
	 * @param  None
	 * @return None
	 */
	void TC_poiDatabaseUnderflow();
    /**
	 * Test case to set an invalid database pointer to the route and
	 * then try to add/print
	 * Expected Result: Unable to set the Database connection. Route addition fails
	 * @param  None
	 * @return None
	 */
	void TC_invalidDatabasePointer();
    /**
	 * Test case to get invalid data from GPS Sensor and try to print the data
	 * and see if the default values are set correctly
	 * Expected Result: Default values are set correctly.
	 * @param  None
	 * @return None
	 */
	void TC_getInvalidDataFromGPSSensor();
    /**
	 * Test case to add an Invalid Poi to the Poi database and then try to
	 * access it elsewhere.
	 * Expected Result: Addition fails and error message is printed
	 * @param  None
	 * @return None
	 */
	void TC_addInvalidPoiToPoiDatabase();

	void TC_operatorPlusOverload();

	void TC_operatorEqualOverload();

	void TC_operatorPlusEqualOverload();

	void TC_routeCopyConstructor();

	void TC_createDatabases();

	void TC_writeToFile();

	void TC_readFromFile();

	void TC_readMergeMode();

	void TC_readReplaceMode();

public:

    /* Public Member Functions */

	/**
	 * Constructor of CNavigationSystem class
	 */
    CNavigationSystem();
    /**
     * Run method starts the Navigation System
     * Performs route creation/addition
     * Invokes print of the created routed
     * Invokes the method which prints the distance to next POI from current position
     * @param None
     * @return None
     */
    void run(void);

};
/********************
**  CLASS END
*********************/
#endif /* CNAVIGATIONSYSTEM_H */
