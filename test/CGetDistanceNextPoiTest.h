#ifndef CTEST_GET_DISTANCE_NEXT_POI_H
#define CTEST_GET_DISTANCE_NEXT_POI_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class CGetDistanceNextPoiTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute;
	public:
		CGetDistanceNextPoiTest() : TestCase("Testing GetDistanceNextPoi"){}
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
		 * Tests if the distance calculated is invalid in case if an empty route is present(Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected
		 * 2. Route has to be empty
		 *
		 * Validation:
		 * Success - if the distance return is -1 (invalid)
		 * Failure - Otherwise
		 */
		void routeEmptyTest()
		{
			const int expectedValue = -1; //Invalid distance

			/* Pre-conditions */
			/* Connect the Database Object with the Route */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb); /* Connect Route to the POI Database from where POIs can be fetched */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);

			/* Test Execution*/
			const int actualValue  = m_pTestRoute->getDistanceNextPoi(*m_pWp, *m_pPoi);

			/* Validation */
			CPPUNIT_ASSERT_EQUAL_MESSAGE("routeEmptyTest failed", expectedValue, actualValue);
		}
		/**
		 * Tests the distance calculated functionality from the current position to nearest poi (Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected
		 * 2. A waypoint has to be present in the route
		 * 3. The poi has to be added after this waypoint
		 *
		 * Validation:
		 * Success - If the returned distance equals 1973(pre-calculated value using distance formula)
		 * Failure - Otherwise
		 */
		void getDistanceTest()
		{
			const int expectedValue = 1973; //pre-calculated distance between the wp and poi

			/* Pre-conditions */
			/* Connect the Database Object with the Route */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb); /* Connect Route to the POI Database from where POIs can be fetched */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			/* Add the wp and poi to the route */
			m_pTestRoute->addWaypoint(m_pWp->getName());
			m_pTestRoute->addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Test Execution*/
			int actualValue  = m_pTestRoute->getDistanceNextPoi(*m_pWp, *m_pPoi);

			/* Validation */
			CPPUNIT_ASSERT_EQUAL_MESSAGE("getDistanceTest failed", expectedValue, actualValue);
		}
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Load Tests");
			suite->addTest(new CppUnit::TestCaller<CGetDistanceNextPoiTest>("Get Distance with Empty Route Test",
						&CGetDistanceNextPoiTest::routeEmptyTest));
			suite->addTest(new CppUnit::TestCaller<CGetDistanceNextPoiTest>("Get Distance with Valid Route Test",
						&CGetDistanceNextPoiTest::getDistanceTest));

			return suite;
		}
};

#endif // CTEST_GET_DISTANCE_NEXT_POI_H
