/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CROUTE.H
 * Author          : Jishnu M Thampan
 * Description     : class CRoute
 * 					 This class represents a route in a Navigation System.
 * 					 The Waypoints in the route can be added to this class
 * 					 and is stored here. This class stores the pointers to
 * 					 the POI data in the CPOIdatabase. The POIs to the route
 * 					 can also be added to this class and when doing so the
 * 					 corresponding pointers from the CPoiDatabase class are
 * 					 fetched and stored in this class. Also the route data
 * 					 which consists of Waypoints and POIs can be printed here.
 * 					 Additionally this class provides the method to calculate
 * 					 the distance to the nearest POI from a particular location.
 ****************************************************************************/

#ifndef CROUTE_H
#define CROUTE_H

#include <list>
#include <vector>

//Own Include Files
#include "CPoiDatabase.h"
#include "CWpDatabase.h"

class CWaypoint; /**< \brief Forward declaration of class CWaypoint*/

/**
 * This class represents a route in a Navigation System.
 * The Waypoints in the route can be added to this class.
 * This class stores the pointers to the POI data in the
 * CPOIdatabase. The POIs to the route can also be added
 * to this class and when doing so the corresponding
 * pointers from the CPoiDatabase class are fetched and stored
 * in this class. Also the route data which consists of Waypoints
 * and POIs can be printed here. Additionally this class provides
 * the method to calculate the distance to the nearest POI from
 * a particular location.
 */
class CRoute
{

private:

	typedef std::list<CWaypoint*> CRouteList;

	CPoiDatabase* m_pPoiDatabase; /**< \brief Represents the poi database connection */
	CWpDatabase* m_pWpDatabase;	  /**< \brief Represents the waypoint database connection */
	CRouteList m_routeContainer;  /**< \brief Represents the container to store the route */
	unsigned int m_waypointCount; /**< \brief Represents the waypoint count in the route */
	unsigned int m_poiCount;	  /**< \brief Represents the poi count in the route */

public:
	 /* Public Member Functions */

    /**
    * Parameterized constructor to CRoute class - Sets the values in case of valid input, else takes the default values
    * Allocates the dynamic Memory needed to store the Waypoints and Pointers to the POI Database
    * @param unsigned int maxWp   - Maximum Waypoint			[IN] Default Value: 0
    * @param unsigned int maxPoi  - Maximum Point of Interest	[IN] Default Value: 0
    */
	CRoute();
    /**
    * Copy constructor of CRoute class
    * Allocates the dynamic Memory needed to store the Waypoints and Pointers to the POI Database
    * and performs a deep copy of the object.
    * @param const CRoute& - Reference to the CRoute object to be assigned [IN]
    * @return (*this) 	   - The current object under consideration
    */
	CRoute(const CRoute& origin);
    /**
    * Destructor of CRoute class
    * Deallocates all the dynamic memory allocated for Waypoint objects and for the Pointer to Database objects
    */
	~CRoute();
    /**
    * Connects the m_pPoiDatabase object to the CPOIDatabase
    * @param CPoiDatabase* pPoiDB - Pointer to the CPOIDatabase object [IN]
    * @return None
    */
	void connectToPoiDatabase(CPoiDatabase* pPoiDB);
    /**
    * Connects the m_wpDatabase object to the CWpDatabase
    * @param CWpDatabase* pWpDB - Pointer to the CWpDatabase object [IN]
    * @return None
    */
	void connectToWpDatabase(CWpDatabase* pWpDB);
    /**
    * Searches for the Waypoint  with name namePoi in CWpDatabase and adds
    * to the Route. Does not add if the same is not found in the database
    * @param std::string namePoi - Name of the Waypoint object to be added to the route [IN]
    * @return None
    */
	void addWaypoint(std::string name);
    /**
    * Searches for the POI with name namePoi in CPOIDatabase and adds
    * it after the waypoint with name 'afterWp' in the Route. Does not
    * add if the same is not found in the database
    * @param std::string namePoi - Name of the POI object to be added to the route [IN]
    * @param std::string afterWp - Name of the Waypoint object after which the POI is to be added [IN].
    * @return None
    */
	void addPoi(std::string namePoi, std::string afterWp);

	/**
	 * Gets the distance to the nearest POI (from the current location) by traversing through the POIs of the current route
	 * @param const CWaypoint& wp - Reference to the Waypoint object [IN]
	 * @param CPOI& poi   		  - Reference to the POI object		 [OUT]
	 * @return distance  		  - Distance to the nearest POI
	 */
	double getDistanceNextPoi(const CWaypoint& wp, CPOI& poi);
	/**
	 * Prints the Waypoints(either in DEGREE or MMSS format) and POIs in the current route
	 * @param None
	 * @return None
	 */
	void print(void);
	/**
	 * Overloaded operator+ - Adds 2 routes and returns the result
	 * @param  const CRoute& route [IN] - The second route to be added
	 * @return CRoute - The new CRoute which is the sum of the two input routes
	 */
	CRoute operator+(const CRoute& route);
	/**
	 * Overloaded operator+=
	 * 1. Finds if the Waypoint 'name' is found in the Waypoint Database
	 *    If Present adds it to the end of the current route
	 * 2. Checks if the POI 'name' is found in the database
	 *    If Present adds it to the end of the current route
	 * @param  const CRoute& route [IN] - The name of the POI/Waypoint to be added
	 * @return CRoute& - The Reference to the modified route
	 */
	CRoute& operator+=(const std::string name);
	/**
	 * Overloaded operator - Assigns the new route to the current route
	 * @param  const CRoute& rop[IN] - The Reference to the new Route
	 * @return CRoute& - The Reference to the modified route
	 */
	CRoute& operator=(const CRoute& rop);

	const std::vector<const CWaypoint*> getRoute();

};
/********************
 **  CLASS END
 *********************/
#endif /* CROUTE_H */
