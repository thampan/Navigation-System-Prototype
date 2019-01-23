/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CROUTE.CPP
 * Author          : Jishnu M Thampan
 * Description     : class CRoute
 * 					 This class represents a route in a Navigation System.
 * 					 The Waypoints in the route are stored can be added to
 * 					 this class and is stored here. This class stores the
 * 					 pointers to the POI and Waypoint data in the CPOIdatabase. The POIs
 * 					 to the route can also be added to this class and when
 * 					 doing so the corresponding pointers from the CPoiDatabase
 * 					 class are fetched and stored in this class.
 * 					 Also the route data which consists of Waypoints and POIs
 * 					 can be printed here. Additionally this class provides the
 * 					 method to calculate the distance to the nearest POI from
 * 					 a particular location.
 ****************************************************************************/

//Own Include Files
#include "CRoute.h"

/**
 * Parameterized constructor to CRoute class - Sets the values in case of valid input, else takes the default values
 * @param unsigned int maxWp   - Maximum Waypoint			[IN]
 * @param unsigned int maxPoi  - Maximum Point of Interest	[IN]
 */
CRoute::CRoute()
/*Initialise all class members to default values */
{
	m_waypointCount = 0;
	m_poiCount		= 0;
	m_pPoiDatabase  =  NULL;
	m_pWpDatabase   =  NULL;
}
/**
* Copy constructor of CRoute class
* Allocates the dynamic Memory needed to store the Waypoints and Pointers to the POI Database
* and performs a deep copy of the object.
* @param const CRoute& - Reference to the CRoute object to be assigned [IN]
* @return (*this) 	   - The current object under consideration
*/
CRoute::CRoute(CRoute const& origin)
{
	/* Check if the route is connected to a valid database*/
	if((!(origin.m_pPoiDatabase)) || (!(origin.m_pWpDatabase)))
	{
		/* Display a Warning */
		std::cout << "Warning!Database is not connected!" << std::endl;
		/* We allow the route copy even if it is connected to invalid database as long as there is
		 * a valid route present */
	}
	if(!origin.m_routeContainer.empty())
	{
		/*Copy the database pointers */
		m_pPoiDatabase = origin.m_pPoiDatabase;
		m_pWpDatabase = origin.m_pWpDatabase;
		/* Copy the Counts*/
		m_waypointCount = origin.m_waypointCount;
		m_poiCount = origin.m_poiCount;
		/* Clear and copy the list */
		m_routeContainer.clear();
		m_routeContainer = origin.m_routeContainer; //overloading assignment of the iterator
	}
	else
	{
		std::cout << "ERROR!CRoute():Copy Failed since the route is empty!" << std::endl;
	}
}
/**
* Destructor of CRoute class
* Deallocates all the dynamic memory allocated for Waypoint objects and for the Pointer to Database objects
*/
CRoute::~CRoute()
{
	m_routeContainer.clear();
}
/**
* Connects the m_wpDatabase object to the CWpDatabase
* @param CWpDatabase* pWpDB - Pointer to the CWpDatabase object [IN]
* @return None
*/
void CRoute::connectToWpDatabase(CWpDatabase* pWpDB)
{
	if(NULL != pWpDB) /*Checks of the Database pointer is valid */
	{
		m_pWpDatabase = pWpDB; /* Connects the m_pPoiDatabase with the POI Database */
	}
	else /*Prints error in case of invalid Database pointer */
	{
		std::cout << "ERROR: CRoute::connectToPoiDatabase() failed! Invalid Database Pointer!!" << std::endl;
	}
}
/**
* Connects the m_pPoiDatabase object to the CPOIDatabase
* @param CPoiDatabase* pPoiDB - Pointer to the CPOIDatabase object [IN]
* @return None
*/
void CRoute::connectToPoiDatabase(CPoiDatabase* pPoiDB)
{
	if(NULL != pPoiDB) /*Checks of the Database pointer is valid */
	{
		m_pPoiDatabase = pPoiDB; /* Connects the m_pPoiDatabase with the POI Database */
	}
	else /*Prints error in case of invalid Database pointer */
	{
		std::cout << "ERROR: CRoute::connectToPoiDatabase() failed! Invalid Database Pointer!!" << std::endl;
	}
}
/**
* Searches for the Waypoint  with name namePoi in CWpDatabase and adds
* to the Route. Does not add if the same is not found in the database
* @param std::string namePoi - Name of the Waypoint object to be added to the route [IN]
* @return None
*/
void CRoute::addWaypoint(std::string name)
{
	if (m_pWpDatabase)
	{
		CWaypoint* pWp = m_pWpDatabase->getPointerToWaypoint(name);
		if (NULL != pWp) /* If Valid Waypoint pointer, add it to the database */
		{
			m_routeContainer.push_back(pWp);
			m_waypointCount++;
		}
		else
		{
			std::cout
					<< "ERROR!:CRoute::addPoi failed!: Waypoint not found in Database"
					<< std::endl;
		}
	}
	else
	{
		std::cout
				<< "ERROR!:CRoute::addPoi failed!: Waypoint Database not connected!"
				<< std::endl;
	}
}
/**
* Searches for the POI with name namePoi in CPOIDatabase and adds
* it after the waypoint with name 'afterWp' in the Route. Does not
* add if the same is not found in the database
* @param std::string namePoi - Name of the POI object to be added to the route [IN]
* @param std::string afterWp - Name of the Waypoint object after which the POI is to be added.
* @return None
*/
void CRoute::addPoi(std::string namePoi, std::string afterWp)
{
	/* Check if the POI is connected */
	if(!m_pPoiDatabase)
	{
		std::cout << "ERROR!:CRoute::addPoi failed!: POI Database not connected!" << std::endl;
		return;
	}
	/* Start route addition from here on */

	CPOI* pPoi = m_pPoiDatabase->getPointerToPoi(namePoi);
	/* Display Error and return if POI is not found in POI Database */
	if(!pPoi)
	{
		std::cout << "ERROR!:CRoute::addPoi failed!: POI not found in POIDatabase!" << std::endl;
		return;
	}
	bool isWaypointInserted = false; /* To indicate if the POI is successfully inserted */
	for(std::list<CWaypoint*>::reverse_iterator itr = m_routeContainer.rbegin(); itr != m_routeContainer.rend(); ++itr)
	{
		if(NULL != *itr)
		{
			CWaypoint* pWp = dynamic_cast<CWaypoint*>(*itr); /* Check if the List object type is of Waypoint */

			/* Checking if the list object is not a POI, but a Waypoint
			 * This is a redundant check in this case (when the list has objects of only 2 types),
			 * but double confirms it is Waypoint alone, also adds security if the list is extended to further types in future*/
			if((NULL != pWp) &&
					(NULL == (dynamic_cast<CPOI*>(*itr))))
			{
				/* 1. Check if the Given Name matches with the Waypoint Name
				 * 2. If not found, add the POI to the end */
				if((afterWp == (pWp->getName())))
				{
					/* Insert the POI after this Waypoint */
					m_routeContainer.insert(itr.base(), pPoi);
					m_poiCount++;
					isWaypointInserted = true;
					break;
				}
			}
		}
	}
	/* Display Error if the POI insertion was not successful */
	if(!isWaypointInserted)
	{
		std::cout << "ERROR!:CRoute::addPoi failed!:" << afterWp << " NOT found in the current Route!" << std::endl;
	}
}
/**
 * Overloaded operator+ - Adds 2 routes and returns the result
 * @param  const CRoute& route [IN] - The second route to be added
 * @return CRoute - The new CRoute which is the sum of the two input routes
 */
CRoute CRoute::operator+(const CRoute& route)
{
	/* Check if the databases are the same and is valid */
	if( (route.m_pPoiDatabase == this->m_pPoiDatabase) && (route.m_pWpDatabase == this->m_pWpDatabase)
				&& (NULL != route.m_pPoiDatabase) && (NULL != route.m_pWpDatabase))
		{
			CRoute sum; /* To Store the result */

			/* Insert the first route to the resultant route */
			sum.m_routeContainer.insert(sum.m_routeContainer.end(), this->m_routeContainer.begin(), this->m_routeContainer.end());
			/* Now insert the second route to the resultant route */
			sum.m_routeContainer.insert(sum.m_routeContainer.end(), route.m_routeContainer.begin(), route.m_routeContainer.end());

			sum.m_waypointCount = m_waypointCount + route.m_waypointCount; /* Sum up the Waypoint counts */
			sum.m_poiCount		= m_poiCount	  + route.m_poiCount;	   /* Sum up the POI counts */

			/* Initialize the new route with the database pointers */
			sum.m_pPoiDatabase = route.m_pPoiDatabase;
			sum.m_pWpDatabase  = route.m_pWpDatabase;

			return sum;
		}
	else
	{
		std::cout << "CRoute::ERROR!Route cannot be added since they do not point to same databases" << std::endl;
		return CRoute(); //return dummy route
	}
}
/**
 * Overloaded operator+=
 * 1. Finds if the Waypoint 'name' is found in the Waypoint Database
 *    If Present adds it to the end of the current route
 * 2. Checks if the POI 'name' is found in the database
 *    If Present adds it to the end of the current route
 * @param  const CRoute& route [IN] - The name of the POI/Waypoint to be added
 * @return CRoute& - The Reference to the modified route
 */
CRoute& CRoute::operator+=(const std::string name)
{
	bool isWaypointAddedToRoute = false; /* Indicates if Waypoint Addition is successful */
	if(m_pWpDatabase)
	{
		CWaypoint * pWp = this->m_pWpDatabase->getPointerToWaypoint(name); /* Get the Waypoint pointer from Database */
		if(pWp)
		{
			this->addWaypoint(name);	/* Add the Waypoint to the route */
			isWaypointAddedToRoute = true;
		}
	}
	if(m_pPoiDatabase)
	{
		CPOI* pPoi = this->m_pPoiDatabase->getPointerToPoi(name); /* Get the POI pointer from Database */
		if(pPoi)
		{
			/* If Waypoint with 'name' is added before, add the POI after the Waypoint*/
			if(isWaypointAddedToRoute)
			{
				this->addPoi(name, name);
			}
			/* Else Add the POI to the END */
			else
			{
				m_routeContainer.push_back(pPoi);
				m_poiCount++;
			}
		}
	}
	return (*this);
}

/**
 * Prints the Waypoints(either in DEGREE or MMSS format) and POIs in the current route
 * @param  None
 * @return None
 */
void CRoute::print(void)
{
	std::cout <<"============================================================================================="<< std::endl;
	std::cout << "Our Route has " << m_waypointCount << " Waypoints and " << m_poiCount << " Points of Interest\n" << std::endl;
	std::cout <<"============================================================================================="<< std::endl;
	/* Iterate through the Route, identify the type of the object and call the respective print routines */
	for(std::list<CWaypoint*>::iterator itr = m_routeContainer.begin(); itr != m_routeContainer.end(); itr++)
	{
		/* Answer: Native Overload : cout << (**i) << endl
		 * Results in calling the Waypoint class overloaded << operator alone due to early binding
		 * So we need to bind it to object it is currently pointing into using dynamic cast
		 */
		if(NULL != (*itr))
		{
			CPOI * pPoi = dynamic_cast<CPOI*>(*itr);
			if (NULL != pPoi) /* POI Detected */
			{
				std::cout << "POI:";
				std::cout << (*pPoi) << std::endl;
			}
			else /* Waypoint Detected */
			{
				std::cout << "Waypoint:";
				std::cout << (*(CWaypoint*)(*itr)) << std::endl;
			}
		}
		std::cout << "-----------------------------------------------------------------" << std::endl;
	}
	std::cout <<"============================================================================================="<< std::endl;
}
/**
 * Gets the distance to the nearest POI (from the current location) by traversing through the POIs of the current route
 * @param const CWaypoint& wp - Reference to the Waypoint object [IN]
 * @param CPOI& poi   		  - Reference to the POI object		 [OUT]
 * @return distance  		  - Distance to the nearest POI
 */
double CRoute::getDistanceNextPoi(const CWaypoint& wp, CPOI& poi)
{
	if(m_routeContainer.empty()) /* Checks if the POI Database is empty */
	{
		std::cout << "ERROR: CRoute::getDistanceNextPoi() failed! Route is empty! " << std::endl;
		return -1; /* Returns Invalid distance in case if the POI database is empty */
	}
	double smallestDistance = -1;
	for(CRouteList::iterator itr = m_routeContainer.begin(); itr != m_routeContainer.end(); ++itr)
	{
		if(NULL != (*itr))
		{
			if(dynamic_cast<CPOI*>(*itr))
			{
				smallestDistance = (*itr)->calculateDistance(wp); /* Set the smallest distance to the distance between the first element and the current location*/
				break;
			}
		}
	}
	CRouteList::iterator index; /* Stores the index where the nearest POI is located */

	/*Iterate though the pointers list that point to the data in the POI database */
	for(CRouteList::iterator itr = m_routeContainer.begin(); itr != m_routeContainer.end(); ++itr)
	{
		if(NULL != (*itr))
		{
			if(dynamic_cast<CPOI*>(*itr))
			{
				double currentDistance  = (*itr)->calculateDistance(wp); /*Current distance holds the distance to the POI at the current index*/
				if( smallestDistance >= currentDistance) /* Checks if the distance calculated is the smallest that is calculated so far*/
				{
					/* 1. Update the smallestDistance with the currentDistance in case the
					 * 	  distance to the POI in the current iteration is closer
					 * 2. Store the index of the pointer that points to the POI in the POI Database*/
					smallestDistance = currentDistance;
					index = itr;
				}
			}
		}
	}
	CPOI *nearestPoi = (CPOI *)(*index);
	if(nearestPoi)
		poi = (*nearestPoi); /* Update the POI object with the POI which is the nearest from the current position */

    return smallestDistance; /* Return the distance to the nearest POI */
}
CRoute& CRoute::operator=(const CRoute& rop)
{
	/* Check for self assignments */
	if(this == &rop)
	{
		return *this;
	}
	/* Check if the route is connected to a valid database*/
	if((!(rop.m_pPoiDatabase)) || (!(rop.m_pWpDatabase)))
	{
		/* Display a Warning */
		std::cout << "Warning!Database is not connected!" << std::endl;
		/* We allow the route copy even if it is connected to invalid database as long as there is
		 * a valid route present */
	}
	/*Check if the route is empty */
	if(!rop.m_routeContainer.empty())
	{
		/* Copy the Counts*/
		m_waypointCount = rop.m_waypointCount;
		m_poiCount = rop.m_poiCount;

		/*Copy the database pointers */
		m_pPoiDatabase = rop.m_pPoiDatabase;
		m_pWpDatabase = rop.m_pWpDatabase;

		/* Clear and copy the list */
		m_routeContainer.clear();
		m_routeContainer = rop.m_routeContainer; //overloading assignment of the iterator
	}
	else
	{
		std::cout << "ERROR!CRoute():Copy Assignment failed since the route is empty!" << std::endl;
	}
	return (*this);
}
const std::vector<const CWaypoint*> CRoute::getRoute()
{
	std::vector<const CWaypoint*> routeVec;
	for(CRouteList::iterator itr = m_routeContainer.begin(); itr != m_routeContainer.end(); ++itr)
	{
		routeVec.push_back(*itr);
	}
	return routeVec;
}
