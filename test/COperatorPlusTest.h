#ifndef CTEST_OPERATOR_PLUS_H
#define CTEST_OPERATOR_PLUS_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class COperatorPlusTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute;
	public:
		COperatorPlusTest() : TestCase("Testing Operator+"){}
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
		 * Tests if two valid routes can be added (Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected for both the routes
		 * 2. Route should not be empty
		 * 3. A waypoint has to be present in the route
		 * 4. The poi has to be added after this waypoint
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased to 3
		 * Failure - Otherwise
		 */
		void addTwoValidRoutes()
		{
			const int expectedValue = 3;
			/* Pre-conditions */
			CRoute route1;
			route1.connectToPoiDatabase(m_pPoiDb);
			route1.connectToWpDatabase(m_pWpDb);
			route1.addWaypoint(m_pWp->getName());
			CRoute route2;
			route2.connectToPoiDatabase(m_pPoiDb);
			route2.connectToWpDatabase(m_pWpDb);

			route2.addWaypoint(m_pWp->getName());
			route2.addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Test Execution*/
			CRoute sum = route1 + route2;

			/* Validation */
			std::vector<const CWaypoint*>routeContents = sum.getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);
		}
		/**
		 * Tests if two invalid routes can be added (error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases should not be connected for both the routes
		 * 2. Routes may be empty
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased to 3
		 * Failure - Otherwise
		 */
		void addTwoInvalidRoutes()
		{
			const int expectedValue = 0;
			CRoute route1, route2;

			/* Test Execution*/
			CRoute sum = route1 + route2;

			/* Validation */
			std::vector<const CWaypoint*>routeContents = sum.getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);
		}
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Testing Operator+");
			suite->addTest(new CppUnit::TestCaller<COperatorPlusTest>("Add Two Valid Routes Test",
						&COperatorPlusTest::addTwoValidRoutes));
			suite->addTest(new CppUnit::TestCaller<COperatorPlusTest>("Add Two Invalid Routes Test",
						&COperatorPlusTest::addTwoInvalidRoutes));

			return suite;
		}
};

#endif // CTEST_OPERATOR_PLUS_H
