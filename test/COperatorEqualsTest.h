#ifndef CTEST_OPERATOR_EQUALS_H
#define CTEST_OPERATOR_EQUALS_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class COperatorEqualsTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute1;
		CRoute* m_pTestRoute2;
	public:
		COperatorEqualsTest() : TestCase("Testing Operator="){}
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
			m_pTestRoute1 = new CRoute();
			m_pTestRoute2 = new CRoute();

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
			if(m_pTestRoute1) delete m_pTestRoute1;
			if(m_pTestRoute2) delete m_pTestRoute2;
		}
		/**
		 * Tests if self assignment is possible to the route (Error case)
		 *
		 * Pre:Conditions: -
		 *
		 * Validation:
		 * Success - If the size of the resultant route has NOT increased
		 * Failure - Otherwise
		 */
		void selfAssignmentTest()
		{
			const int expectedValue = 0;

			/* Test Execution*/
			*m_pTestRoute1 = *m_pTestRoute1;

			/* Validation */
			std::vector<const CWaypoint*>routeContents = m_pTestRoute1->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);

		}
		/**
		 * Tests if route assignment is possible if the databases are not connected (Error case)
		 *
		 * Pre:Conditions: Database of either of the routes should not be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has NOT increased
		 * Failure - Otherwise
		 */
		void DbInvalidConnectionTest()
		{
			const int expectedValue = 0;

			/* Test Execution*/
			*m_pTestRoute1 = *m_pTestRoute2;

			/* Validation */
			std::vector<const CWaypoint*>routeContents = m_pTestRoute1->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);
		}
		/**
		 * Tests if an empty route can be assigned using operator= (Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has NOT increased
		 * Failure - Otherwise
		 */
		void emptyRouteTest()
		{
			const int expectedValue = 0;
			CRoute *route1 = new CRoute();
			CRoute *route2 = new CRoute();
			/* Pre-conditions */
			route2->connectToPoiDatabase(m_pPoiDb);
			route2->connectToWpDatabase(m_pWpDb);

			/* Test Execution*/
			*route2 = *route1;

			/* Validation */
			std::vector<const CWaypoint*>routeContents = route2->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);

			if(route1) delete route1;
			if(route2) delete route2;
		}
		/**
		 * Tests if one route can be assigned to another using operator=(Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected
		 * 2. A waypoint has to be present in the route
		 * 3. The poi has to be added after this waypoint
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased
		 * Failure - Otherwise
		 */
		void validComparisonTest()
		{
			const int expectedValue = 2;
			bool wpFound = false, poiFound = false;
			/* Pre-conditions */
			m_pTestRoute2->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute2->connectToWpDatabase(m_pWpDb);
			m_pTestRoute1->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute1->connectToWpDatabase(m_pWpDb);

			m_pTestRoute2->addWaypoint(m_pWp->getName());
			m_pTestRoute2->addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Test Execution*/
			*m_pTestRoute1 = *m_pTestRoute2;

			/* Validation */
			std::vector<const CWaypoint*>routeContents = m_pTestRoute1->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);

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
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Testing assignment operator");
			suite->addTest(new CppUnit::TestCaller<COperatorEqualsTest>("Testing self assignment",
								&COperatorEqualsTest::selfAssignmentTest));
			suite->addTest(new CppUnit::TestCaller<COperatorEqualsTest>("Testing assignment operator with invalid db connection",
								&COperatorEqualsTest::DbInvalidConnectionTest));
			suite->addTest(new CppUnit::TestCaller<COperatorEqualsTest>("Testing assignment operator with empty Route",
								&COperatorEqualsTest::emptyRouteTest));
			suite->addTest(new CppUnit::TestCaller<COperatorEqualsTest>("Testing assignment operator with valid routes",
								&COperatorEqualsTest::validComparisonTest));

			return suite;
		}
};

#endif // CTEST_OPERATOR_EQUALS_H
