#ifndef CADD_POI_H
#define CADD_POI_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class CAddPoiTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute;
	public:
		CAddPoiTest() : TestCase("Testing Add Poi") {
		}

		/*
		 * Function which would allocate the memory needed and the values
		 * for the tests running in this suite
		 */
		void setUp()
		{
			m_pWpDb  = new CWpDatabase();
			m_pPoiDb = new CPoiDatabase();
			m_pPoi   = new CPOI(CPOI::RESTAURANT, "Mensa HDA", "the best Mensa", 49.8728, 8.6512);
			m_pWp    = new CWaypoint(32.3680, 4.9036, "Amsterdam");
			m_pTestRoute = new CRoute();

			m_pPoiDb->addPoi(*m_pPoi);
			m_pWpDb->addWaypoint(*m_pWp);
		}
		/*
		 * Function which would release the memory allocated
		 * the tests running in this suite
		 */
		void tearDown()
		{
			if(m_pWpDb)  	 delete m_pWpDb;
			if(m_pPoiDb) 	 delete m_pPoiDb;
			if(m_pPoi)   	 delete m_pPoi;
			if(m_pWp)    	 delete m_pWp;
			if(m_pTestRoute) delete m_pTestRoute;
		}

		/**
		 * Tests if a POI is added to the route (Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected
		 * 2. A waypoint has to be present in the route
		 * 3. The poi has to be added after this waypoint
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased and if the inserted contents are present in the route
		 * Failure - Otherwise
		 */
		void addPoiWithConnectedDbTest()
		{
			const int expectedValue = 2; //accounting for 1 waypoint and 1 poi
			bool wpFound = false, poiFound = false;

			/* Pre-conditions */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb); /* Connect Route to the POI Database from where POIs can be fetched */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			m_pTestRoute->addWaypoint(m_pWp->getName());

			/* Test Execution*/
			m_pTestRoute->addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Validation */

			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL_MESSAGE("addPoiWithConnectedDbTest failed: route size mismatch", expectedValue, actualValue);

			for(std::vector<const CWaypoint*>::const_iterator itr = routeContents.begin(); itr!=routeContents.end(); ++itr)
			{
				if(*itr)
				{
					if(NULL != dynamic_cast<const CPOI*>(*itr)) //poi found
					{
						if((*itr)->getName() == m_pPoi->getName()) poiFound = true;
					}
					else if( NULL != dynamic_cast<const CWaypoint*>(*itr)) //waypoint found
					{
						if((*itr)->getName() ==  m_pWp->getName())  wpFound = true;
					}
				}
			}
			CPPUNIT_ASSERT_MESSAGE("wp is not inserted",  wpFound);
			CPPUNIT_ASSERT_MESSAGE("poi is not inserted", poiFound);
		}
		/**
		 * Tests if a POI is added to the route when the db is disconnected (Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases are not connected

		 * Validation:
		 * Success - If the size of the resultant route has increased
		 * Failure - Otherwise
		 */
		void addPoiWithDisconnectedDbTest()
		{
			const int expectedValue = 0; //accounting for 0 wp/poi

			/* Pre-conditions*/
			 // --

			/* Test Execution*/
			m_pTestRoute->addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Validation*/

			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("Entry added even if db is not connected", expectedValue, actualValue);
		}
		/**
		 * Tests if a POI is added to the route when a POI name not in POI Db is passed (Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases are connected
		 * 2. A waypoint has to be present in the route
		 * Validation:
		 * Success - If the size of the resultant route has increased
		 * Failure - Otherwise
		 */
		void addPoiWithInvalidPoiName()
		{
			const int expectedValue = 1; //accounting for 1 waypoint

			/* Pre-conditions */
			/* Connect the Database Object with the Route */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb); /* Connect Route to the POI Database from where POIs can be fetched */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			m_pTestRoute->addWaypoint(m_pWp->getName());

			/* Test Execution*/

			/* Create a POI which is not in the database */
			CPOI* testPoi = new CPOI(CPOI::RESTAURANT, "Test", "the best Mensa", 49.8728, 8.6512);
			/* Testing the Add POI*/
			m_pTestRoute->addPoi(testPoi->getName(), m_pWp->getName());

			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("addPoiWithConnectedDbTest failed", expectedValue, actualValue);

			if(testPoi) delete testPoi;
		}
		/**
		 * Tests if a POI is added to the route when an Invalid Waypoint(wp not present in route) is passed (Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases are connected

		 * Validation:
		 * Success - If the size of the resultant route has increased
		 * Failure - Otherwise
		 */
		void addPoiWithInvalidWaypointName()
		{
			const int expectedValue = 0; //accounting for 0 waypoint and 0 poi

			/* Pre-conditions */

			/* Connect the Database Object with the Route */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb); /* Connect Route to the POI Database from where POIs can be fetched */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			/* Do not add the wp to the route */

			/* Test Execution*/
			m_pTestRoute->addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Validation*/

			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("addPoiWithConnectedDbTest failed", expectedValue, actualValue);

		}
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Add POI Test");
			suite->addTest(new CppUnit::TestCaller<CAddPoiTest>("Add Poi with connected db Test",
							&CAddPoiTest::addPoiWithConnectedDbTest));
			suite->addTest(new CppUnit::TestCaller<CAddPoiTest>("Add Poi with disconnected db Test",
							&CAddPoiTest::addPoiWithDisconnectedDbTest));
			suite->addTest(new CppUnit::TestCaller<CAddPoiTest>("Add Poi with a POI not in POI Database",
							&CAddPoiTest::addPoiWithInvalidPoiName));
			suite->addTest(new CppUnit::TestCaller<CAddPoiTest>("Add Poi with a Waypoint which is not in Route",
							&CAddPoiTest::addPoiWithInvalidWaypointName));

			return suite;
		}
};
#endif
