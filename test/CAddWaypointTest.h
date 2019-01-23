#ifndef CADD_WAYPOINT_H
#define CADD_WAYPOINT_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class CAddWaypointTest:public CppUnit::TestCase
{
	private:
		CWpDatabase* m_wpDb;
		CWaypoint* m_wp;
		CRoute* m_pTestRoute;
	public:
		CAddWaypointTest() : TestCase("Testing Add Waypoint"){}
		/*
		 * Function which would allocate the memory needed and the values
		 * for the tests running in this suite
		 */
		void setUp()
		{
			m_wpDb  	 = new CWpDatabase();
			m_wp    	 = new CWaypoint(32.3680, 4.9036, "Amsterdam");
			m_pTestRoute = new CRoute();
			m_wpDb->addWaypoint(*m_wp);
		}
		/*
		 * Function which would release the memory allocated
		 * the tests running in this suite
		 */
		void tearDown()
		{
			if(m_wpDb) delete m_wpDb;
			if(m_wp)   delete m_wp;
		}
		/**
		 * Tests if a Waypoint is not added to the route when db is not connected (Error case)
		 *
		 * Pre:Conditions:
		 * 1. WP Databases should not be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1
		 * Failure - Otherwise
		 */
		void addWaypointTestWithDisconnectedDb()
		{
			const int expectedValue = 0; /* Error case, no route should not be added*/

			/* Test Execution*/
			m_pTestRoute->addWaypoint(m_wp->getName());

			/* Validation*/
			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("addWaypointTestWithDisconnectedDb failed", expectedValue, actualValue);
		}
		/**
		 * Tests if a Waypoint is added to the route when db is connected (Normal case)
		 *
		 * Pre:Conditions:
		 * 1. WP Databases should  be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1 and inserted waypoint is present in the route
		 * Failure - Otherwise
		 */
		void addWaypointTestWithConnectedDb()
		{
			int expectedValue = 1; //accounting for 1 waypoint and 0 poi
			bool wpFound 	  = false;

			/* Pre-conditions */
			/* Connect to a valid db and test the add function */
			m_pTestRoute->connectToWpDatabase(m_wpDb);

			/* Test Execution*/
			m_pTestRoute->addWaypoint(m_wp->getName());

			/* Validation*/
			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL_MESSAGE("addWaypointTestWithDisconnectedDb failed", expectedValue, actualValue);

			for(std::vector<const CWaypoint*>::const_iterator itr = routeContents.begin(); itr!=routeContents.end(); ++itr)
			{
				if(*itr)
				{
					if( NULL != dynamic_cast<const CWaypoint*>(*itr) &&
							(NULL == dynamic_cast<const CPOI*>(*itr))) //waypoint found
					{
						if((*itr)->getName() ==  m_wp->getName())  wpFound = true;
					}
				}
			}
			CPPUNIT_ASSERT_MESSAGE("wp is not inserted",  wpFound);
		}
		/**
		 * Tests if a Waypoint is not added to the route when input wp is not in db(Error case)
		 *
		 * Pre:Conditions:
		 * 1. WP Databases should be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1
		 * Failure - Otherwise
		 */
		void addWaypointNotInDb()
		{
			int expectedValue = 0; //accounting for 0 waypoint and 0 poi
			/* Create a Wp object but dont add it to the db */
			CWaypoint* wpObj = new CWaypoint(50.1, 25.5, "Berlin");

			/* Pre-conditions */
			/* Connect to a valid db and test the add function */
			m_pTestRoute->connectToWpDatabase(m_wpDb);

			/* Test Execution*/
			m_pTestRoute->addWaypoint(wpObj->getName());

			/* Validation*/
			/* Get the Route contents after the test*/
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			int actualValue = (int)routeContents.size();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("addWaypointNotInDb failed", expectedValue, actualValue);

			if(wpObj) delete wpObj;
		}
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("AddWaypoint() Tests");
			suite->addTest( new CppUnit::TestCaller<CAddWaypointTest>("addWaypointTestWithDisconnectedDb",
							&CAddWaypointTest::addWaypointTestWithDisconnectedDb));
			suite->addTest( new CppUnit::TestCaller<CAddWaypointTest>("addWaypointTestWithConnectedDb",
							&CAddWaypointTest::addWaypointTestWithConnectedDb));
			suite->addTest( new CppUnit::TestCaller<CAddWaypointTest>("addWaypointNotInDb",
								&CAddWaypointTest::addWaypointNotInDb));
			return suite;
		}
};

#endif //CADD_WAYPOINT_H
