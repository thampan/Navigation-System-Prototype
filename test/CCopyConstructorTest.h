#ifndef CTEST_COPY_CONSTRUCTOR_H
#define CTEST_COPY_CONSTRUCTOR_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class CCopyConstructorTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute;
	public:
		CCopyConstructorTest() : TestCase("Testing Copy Constructor of CRoute"){}
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
		 * Tests if a route is copied when the databases are not connected (Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases should not to be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has NOT increased by 1
		 * Failure - Otherwise
		 */
		void copyConstructorTestWithInvalidDb()
		{
			const int expectedValue = 0; //0 wp and 0 poi

			/*Pre-conditions*/
			m_pTestRoute->connectToWpDatabase(NULL);
			m_pTestRoute->connectToPoiDatabase(NULL);

			/* Test Execution*/
			CRoute newRoute(*m_pTestRoute);
			/* Validation */
			std::vector<const CWaypoint*>routeContents = newRoute.getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);
		}
		/**
		 * Tests if a route is copied when the source route is empty(Error case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases should be connected
		 * 2. Source route should be empty
		 *
		 * Validation:
		 * Success - If the size of the resultant route has NOT increased by 1
		 * Failure - Otherwise
		 */
		void copyConstructorTestWithEmptyRoute()
		{
			const int expectedValue = 0; //0 wp and 0 poi

			/* Pre-conditions */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb);

			/* Test Execution*/
			CRoute newRoute(*m_pTestRoute);

			/* Validation */
			std::vector<const CWaypoint*>routeContents = newRoute.getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);
		}
		/**
		 * Tests if a route is copied when the source route is non-empty(Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI and WP Databases should be connected
		 * 2. Source route should not be empty
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1
		 * Failure - Otherwise
		 */
		void copyConstructorTestWithNonEmptyRoute()
		{
			const int expectedValue = 2; //1 wp and 1 poi
			bool wpFound = false, poiFound = false;
			/* Pre-conditions */
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute->addWaypoint(m_pWp->getName());
			m_pTestRoute->addPoi(m_pPoi->getName(), m_pWp->getName());

			/* Test Execution*/
			CRoute newRoute(*m_pTestRoute);

			/* Validation */
			std::vector<const CWaypoint*>routeContents = newRoute.getRoute();
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
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Testing Copy Constructor of CRoute");
			suite->addTest(
					new CppUnit::TestCaller<CCopyConstructorTest>(
							"TC: Copy Constructor Test with Invalid DB",
							&CCopyConstructorTest::copyConstructorTestWithEmptyRoute));
			suite->addTest(
					new CppUnit::TestCaller<CCopyConstructorTest>(
							"TC: Copy Constructor Test with Empty Route ",
							&CCopyConstructorTest::copyConstructorTestWithInvalidDb));
			suite->addTest(
					new CppUnit::TestCaller<CCopyConstructorTest>(
							"TC: Copy Constructor Test with Non Empty Route",
							&CCopyConstructorTest::copyConstructorTestWithNonEmptyRoute));

			return suite;
		}

};

#endif // CTEST_COPY_CONSTRUCTOR_H
