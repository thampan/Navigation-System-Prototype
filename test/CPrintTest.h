#ifndef CTEST_PRINT_H
#define CTEST_PRINT_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class CPrintTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute;
	public:
		CPrintTest() : TestCase("Testing Print Method in CRoute"){}
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
		 * Function which would allocate the memory needed and the values
		 * for the tests running in this suite
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
		 * Tests the print functionality in CRoute(Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases needs to be connected
		 * 2. A waypoint has to be present in the route
		 * 3. The poi has to be added after this waypoint
		 *
		 * Validation:
		 * Success - If the Function is called successfully without giving any exceptions
		 * Failure - Otherwise
		 */
		void printTestWithValidRoute()
		{
			/* Pre-conditions */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute->connectToWpDatabase(m_pWpDb);

			m_pTestRoute->addWaypoint(m_pWp->getName());
		    m_pTestRoute->addPoi(m_pPoi->getName(), m_pWp->getName());

		    /* Test Execution*/
		    m_pTestRoute->print();
		}
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Test Print method in CRoute");
			suite->addTest(new CppUnit::TestCaller<CPrintTest>("Testing Print Method in the normal case",
								&CPrintTest::printTestWithValidRoute));
			return suite;
		}
};

#endif // CTEST_PRINT_H
