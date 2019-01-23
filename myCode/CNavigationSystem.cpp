/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CNAVIGATIONSYSTEM.CPP
 * Author          : Jishnu M Thampan
 * Description     : class CNavigationSystem
 *  				 This class is the controller class for the Navigation
 * 					 System. It is responsible of creating a POI database,
 * 					 creating a route which may consist of Waypoints and
 * 					 Routes. It takes input from GPS Sensor and calculates
 * 					 distance to the nearest POI.
 * 					 CNavigationSystem consists of 3 main modules
 * 					 1. GPS Sensor module
 * 					 2. Route module
 * 					 3. POI Database module
 * 					 4. Waypoint database module
 ****************************************************************************/

//Own Include Files
#include "CNavigationSystem.h"

/**
 * Constructor of CGPSSensor class
 */
CNavigationSystem::CNavigationSystem()
: m_route() /* Initialize m_route object */
{
	/* Init PoiDatabase */
	/* Add all the Waypoints */
	CWaypoint wpTable[]  = { CWaypoint(52.3680, 4.9036, "Frankfurt"),
					  	  	 CWaypoint(49.8728, 8.6512, "Darmstadt"),
							 CWaypoint(52.5166, 13.4050, "Berlin"),
							 CWaypoint(49.8673, 8.6382,  "H_da"),
							 CWaypoint(32.3680, 4.9036, "Amsterdam"),
							 CWaypoint(48.7823,  9.1770200, "Stuttgart"),
							 CWaypoint(49.4076,  8.69079, "Heidelberg"),
							 CWaypoint(49.49671, 8.47955, "Mannheim")
						   };/* Table which stores the Waypoint data which is to be added to the Route */

	/* Iterate through the Waypoint table and add each of the Waypoint to the Route */
	for(unsigned int i = 0; i < sizeof(wpTable)/sizeof(CWaypoint); i++)
	{
		m_WpDatabase.addWaypoint(wpTable[i]);
	}
	/* Create a table which stores the POI data which is to be added to the POI Database */
    CPOI poiDbTable[] = { CPOI(CPOI::RESTAURANT, "Mensa HDA", "the best Mensa", 49.8728, 8.6512),
						  CPOI(CPOI::TOURISTIC, "Frankfurt", "Rhine River", 48.8738, 6.6712),
						  CPOI(CPOI::RESTAURANT, "Sitte", "more expensive but good", 49.6118, 8.4578),
						  CPOI(CPOI::GASSTATION, "Darmstadt", "more expensive but good", 49.8728, 8.2512),
						  CPOI(CPOI::UNIVERSITY, "TU Darmstadt", "Research oriented", 49.8889, 8.6121),
						  CPOI(CPOI::TOURISTIC, "Eberstadt", "Frankenstein Castle!!", 49.2233, 10.4312),
						  CPOI(CPOI::GASSTATION, "Total Tankstelle", "Best gasstation in Berlin", 52.5200, 8.7766),
						  CPOI(CPOI::RESTAURANT, "Radieschen", "Vegan Restaurant", 49.0011, 8.9876),
						  CPOI(CPOI::TOURISTIC, "Frankfurt Cathedral", " Roman Catholic Church", 48.9988, 6.2123)
						};

    /* Iterate through the POI table and add each of the POI to the POI database */
	for(unsigned int i = 0; i < sizeof(poiDbTable)/sizeof(CPOI); i++)
	{
		m_PoiDatabase.addPoi(poiDbTable[i]);
	}
	/* Connect the Database Object with the Route */
	m_route.connectToPoiDatabase(&m_PoiDatabase); /* Connect Route to the POI Database from where POIs can be fetched */
	m_route.connectToWpDatabase(&m_WpDatabase);
	csv.setMediaName("FileIO");
	cj.setMediaName("CJson");
}
void CNavigationSystem::TC_operatorPlusOverload()
{
	/*Testing + operator */
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;

	CWaypoint wp(49.8673, 8.6382,  "H_da");
	m_WpDatabase.addWaypoint(wp);

	CRoute a, b, c;

	b = m_route;
	a.connectToPoiDatabase(&m_PoiDatabase);
	a.connectToWpDatabase(&m_WpDatabase);
	a.addWaypoint("H_da");
	a.addPoi("Darmstadt", "H_da");

	std::cout << "=========Before adding============" << std::endl;
	std::cout <<"Route-1-------" << std::endl;
	a.print();
	std::cout <<"Route-2-------" << std::endl;
	b.print();
	std::cout <<"Sum-------" << std::endl;
	c.print();
	std::cout << "=========After adding=============" << std::endl;
	c = a + b;
	std::cout <<"Route-1-------" << std::endl;
	a.print();
	std::cout <<"Route-2-------" << std::endl;
	b.print();
	std::cout <<"Sum-------" << std::endl;
	c.print();
}
void CNavigationSystem::TC_operatorEqualOverload()
{
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;
	/* assmt opr test */
	CRoute cr;
	cr = (m_route);
	std::cout << "==========Old Route===========" << std::endl;
	m_route.print();
	std::cout << "==========New Route=========" << std::endl;
	cr.print();
}
void CNavigationSystem::TC_operatorPlusEqualOverload()
{
	//4 cases
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;
	/* testing += operator */
	//CWaypoint wp(49.8673, 8.6382,  "H_da");
	//m_WpDatabase.addWaypoint(wp);
	std::cout << "==========Before adding===========" << std::endl;
	m_route.print();
	std::cout << "==========After adding============" << std::endl;
	//m_route += "H_da";
	//m_route += "Sitte";
	m_route += "Frankfurt";
	m_route.print();
}
void CNavigationSystem::TC_routeCopyConstructor()
{
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;
	/*copy constr test*/
	CRoute cr(m_route);
	std::cout << "==========Old Route============" << std::endl;
	m_route.print();
	std::cout << "==========New Route============" << std::endl;
	cr.print();
}
void CNavigationSystem::TC_createDatabases()
{
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;
}
void CNavigationSystem::TC_writeToFile()
{
	/* testing write/read data*/
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;
	if(cj.writeData(m_WpDatabase, m_PoiDatabase))
	{
		std::cout <<"####### Write to file is successful #######" << std::endl;
	}
	else
	{
		std::cout <<"####### Write to file is unsuccessful #######" << std::endl;
	}
}
void CNavigationSystem::TC_readMergeMode()
{
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;

	std::cout << "==============BEFORE READ========================" << std::endl;
	std::cout <<"POI DB:" << std::endl;
	CPoiDatabase poiDb;
	poiDb.print();

	std::cout <<"\nWP DB:" << std::endl;
	CWpDatabase wpDb;
	wpDb.print();

	csv.readData(wpDb, poiDb, CCSV::MERGE);
	std::cout << "==============AFTER READ========================" << std::endl;

	std::cout <<"POI DB:" << std::endl;
	poiDb.print();

	std::cout <<"\nWP DB:" << std::endl;
	wpDb.print();
}
void CNavigationSystem::TC_readReplaceMode()
{
#if 0
	std::cout << "============================================================" << std::endl;
	std::cout << "TestCase Executed: " << __FUNCTION__ << std::endl;
	std::cout << "============================================================" << std::endl;

	std::cout << "==============BEFORE READ========================" << std::endl;
	std::cout <<"POI DB:" << std::endl;
	m_PoiDatabase.print();
	std::cout <<"\nWP DB:" << std::endl;
	m_WpDatabase.print();
#endif

	CWpDatabase wpDb; CPoiDatabase poiDb;
	std::cout << "==============AFTER READ========================" << std::endl;
	cj.readData(wpDb,poiDb, CCSV::REPLACE);
#if 1
	std::cout <<"POI DB:" << std::endl;
	poiDb.print();
	std::cout <<"\nWP DB:" << std::endl;
	wpDb.print();
#endif
}
/**
 * Run method starts the Navigation System
 * Performs route creation/addition
 * Invokes print method of the created route
 * Invokes the method which prints the distance to next POI from current position
 * @param  None
 * @return None
 */
void CNavigationSystem::run(void)
{
	//enterRoute();
	//printRoute();
	//TC_operatorPlusOverload();
	//TC_operatorPlusEqualOverload();
	//TC_operatorEqualOverload();
	//TC_routeCopyConstructor();
	//TC_createDatabases();
	//TC_writeToFile();
	//TC_readMergeMode();
	TC_readReplaceMode();

#if 0
	TC_addAllPoiToRoute();
	TC_addMorePoiThanSpecifiedToRoute();
	TC_addInvalidPoi();

	TC_poiDatabaseOverflow();
	TC_poiDatabaseUnderflow();
	TC_invalidDatabasePointer();
	TC_addInvalidPoiToPoiDatabase();
#endif

	//printDistanceCurPosNextPoi();
	//TC_getInvalidDataFromGPSSensor();
}
/**
 * Adds Waypoints and point of interests to the route
 * @param  None
 * @return None
 */
void CNavigationSystem::enterRoute(void)
{
	/* Add all the Waypoints */
	CWaypoint wpTable[]  = { CWaypoint(52.3680, 4.9036, "Frankfurt"),
					  	  	 	   CWaypoint(49.8728, 8.6512, "Darmstadt"),
								   CWaypoint(52.5166, 13.4050, "Stuttgart")
								  //CWaypoint(49.8673, 8.6382,  "H_da")
						   	   	 };/* Table which stores the Waypoint data which is to be added to the Route */

	/* Iterate through the Waypoint table and add each of the Waypoint to the Route */
	for(unsigned int i = 0; i < sizeof(wpTable)/sizeof(CWaypoint); i++)
	{
		//m_WpDatabase.addWaypoint(wpTable[i]);
		m_route.addWaypoint(wpTable[i].getName());
	}
	/* Test Case */
	//TC_addMoreWaypointsThanSpecified();

	/* Add some Pois to the route */

	/* Create a table which stores the POI data which is to be added to the Route */
	const std::string poiTable[] = { "Mensa HDA",
									 "Sitte",
									 /*"Frankfurt"*/
									 "Darmstadt"
									 /*"Eberstadt",
									 "Total Tankstelle",
									 "Radieschen",*/
								   };

	/* Iterate through the POI table and add each of the POI to the Route */
	for(unsigned int i = 0; i < sizeof(poiTable)/sizeof(std::string); i++)
	{
		m_route.addPoi(poiTable[i], wpTable[i].getName());
	}
}
/**
 * Prints the route which includes the Waypoints and point of interests
 * @param None
 * @return None
 */
void CNavigationSystem::printRoute(void)
{
	m_route.print(); /* Print the current route */
}
/**
 * Gets the current position from the GPS Sensor.
 * If it is valid, it prints the distance to the nearest POI,
 * else displays an error message
 * @param  None
 * @return None
 */
void CNavigationSystem::printDistanceCurPosNextPoi(void)
{
	 CWaypoint wp = m_GPSSensor.getCurrentPosition(); /*Get current position from the GPS Sensor */
	 if(DEFAULT_STRING != wp.getName()) /*Check if the current position is valid */
	{
		CPOI cpoiObj; /* POI object to hold the POI data which would be returned
						 after searching for the nearest POI in the database */

		double distanceToNextPoi = m_route.getDistanceNextPoi(wp, cpoiObj); /*Fetches the distance to the nearest POI */

		if(0 <= distanceToNextPoi) /*Checks if the calculated distance is valid */
		{
			std::cout <<"Distance to the next POI = " << distanceToNextPoi << std::endl;
			cpoiObj.print(); /* Prints the attributes of the obtained nearby POI object */
		}
		else /* Print error if the calculated distance is invalid */
		{
			std::cout <<"No nearby POIs available!" << std::endl;
		}
	}
	else /* Print error if the input from the GPS Sensor is wrong */
	{
		std::cout <<"ERROR::CNavigationSystem::printDistanceCurPosNextPoi(): Wrong input from GPS Sensor" << std::endl;
	}
}

/* ----------------------------Test Cases section -------------------------------------------*/

/**
 * Test case to add Waypoints more than the specified limit.
 * Expected Result: Addition of the waypoints fails due to size constraints
 * and prints an error message
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_addMoreWaypointsThanSpecified()
{
	const CWaypoint wpTable[]  = { CWaypoint(48.7823,  9.1770200, "Stuttgart"),
					  	  	 	   CWaypoint(49.4076,  8.69079, "Heidelberg"),
								   CWaypoint(49.49671, 8.47955, "Mannheim"),
								   CWaypoint(49.8673,  8.6382,  "H_da")
						   	   	 };/* Table which stores the Waypoint data which is to be added to the Route */

	/* Iterate through the Waypoint table and add each of the Waypoint to the Route */
	for(unsigned int i = 0; i < sizeof(wpTable)/sizeof(CWaypoint); i++)
	{
		//m_route.addWayPoint(wpTable[i]);
	}
}
/**
 * Test case to add all the POIs in the POI Database to the route
 * Expected Result: All POIs would be successfully added, provided the size
 * is huge enough to accommodate all.
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_addAllPoiToRoute()
{
	//m_route.addPoi("TU Darmstadt");
}
/**
 * Test case to add more POIs than the specified limit
 * Expected Result: Addition fails and prints an error message
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_addMorePoiThanSpecifiedToRoute()
{
	//m_route.addPoi("Frankfurt Cathedral");
}
/**
 * Test case to add an Invalid POI ( POI not in POI Database) to the Route
 * Expected Result: Addition fails and prints an error message
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_addInvalidPoi()
{
	/* Create a table which stores the POI data which is to be added to the Route */
	const std::string poiTable[] = { "Something" };

	/* Iterate through the POI table and add each of the POI to the Route */
	for(unsigned int i = 0; i < sizeof(poiTable)/sizeof(std::string); i++)
	{
		//m_route.addPoi(poiTable[i]);
	}
}
/**
 * Test case to add more POIs than the specified limit to the POI Database.
 * Expected Result: Addition fails and prints an error
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_poiDatabaseOverflow()
{
	 CPOI poiDbTable[] = { CPOI(CPOI::RESTAURANT, "Test1", "Overflow-test-data", 49.8728, 8.6512),
						   CPOI(CPOI::TOURISTIC, "Test2", "Overflow-test-data2", 48.8738, 6.6712),
					     };

	    /* Iterate through the POI table and add each of the POI to the POI database */
		for(unsigned int i = 0; i < sizeof(poiDbTable)/sizeof(CPOI); i++)
		{
			m_PoiDatabase.addPoi(poiDbTable[i]);
		}
}
/**
 * Test case to add none of the POIs at all to the POI Database and then try to
 * add one to the route
 * Expected Result: No crashes or impacts to the waypoint part. System is still stable
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_poiDatabaseUnderflow()
{
	CPoiDatabase tempPoiDb;
	CRoute tempRoute; // create just 1 poi and 0 waypoints
	tempRoute.connectToPoiDatabase(&tempPoiDb);
	/* Create a table which stores the POI data which is to be added to the Route */
	const std::string poiTable[] = { "Frankfurt Cathedral" };

	/* Iterate through the POI table and add each of the POI to the Route */
	for(unsigned int i = 0; i < sizeof(poiTable)/sizeof(std::string); i++)
	{
		//tempRoute.addPoi(poiTable[i]);
	}
	tempRoute.print();
}
/**
 * Test case to set an invalid database pointer to the route and
 * then try to add/print
 * Expected Result: Unable to set the Database connection. Route addition fails
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_invalidDatabasePointer()
{
	m_route.connectToPoiDatabase(NULL);
	/* Create a table which stores the POI data which is to be added to the Route */
	const std::string poiTable[] = { "Frankfurt Cathedral" };

	/* Iterate through the POI table and add each of the POI to the Route */
	for (unsigned int i = 0; i < sizeof(poiTable) / sizeof(std::string); i++) {
		//m_route.addPoi(poiTable[i]);
	}
	m_route.print();
	m_route.connectToPoiDatabase(&m_PoiDatabase); /* Reset the route connection */
}
/**
 * Test case to add an Invalid Poi to the Poi database and then try to
 * access it elsewhere.
 * Expected Result: Addition fails and error message is printed
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_addInvalidPoiToPoiDatabase()
{
	CPOI testPoi(CPOI::TOURISTIC, "Virtual place", "unknown data", -200, 200);
	m_PoiDatabase.addPoi(testPoi);
	//m_route.addPoi("Virtual place");
	m_route.print();
}
/**
 * Test case to get invalid data from GPS Sensor and try to print the data
 * and see if the default values are set correctly
 * Expected Result: Default values are set correctly.
 * @param  None
 * @return None
 */
void CNavigationSystem::TC_getInvalidDataFromGPSSensor()
{
	 CWaypoint wp = m_GPSSensor.getCurrentPosition();
	 wp.print(DEGREE);
}
