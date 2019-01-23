#ifndef CTEST_OPERATOR_PLUS_EQUAL_H
#define CTEST_OPERATOR_PLUS_EQUAL_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "../myCode/CRoute.h"

class COperatorPlusEqualTest:public CppUnit::TestCase
{
	private:
		CWpDatabase*  m_pWpDb;
		CPoiDatabase* m_pPoiDb;
		CPOI* m_pPoi;
		CWaypoint* m_pWp;
		CRoute* m_pTestRoute;
	public:
		COperatorPlusEqualTest() : TestCase("Testing Operator+="){}
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
		 * Tests if a Waypoint is added to the route using operator += (Normal case)
		 *
		 * Pre:Conditions:
		 * 1. WP Database needs to be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1
		 * Failure - Otherwise
		 */
		void addWaypointTest()
		{
			const int expectedValue = 1;
			bool wpFound = false;
			/* Pre-conditions */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute->connectToWpDatabase(m_pWpDb);

			/* Test Execution*/
			*m_pTestRoute += m_pWp->getName();

			/* Validation */
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);

			for(std::vector<const CWaypoint*>::const_iterator itr = routeContents.begin(); itr!=routeContents.end(); ++itr)
			{
				if(*itr)
				{
					if( NULL != dynamic_cast<const CWaypoint*>(*itr) &&
							(NULL == dynamic_cast<const CPOI*>(*itr))) //waypoint found
					{
						if((*itr)->getName() ==  m_pWp->getName())  wpFound = true;
					}
				}
			}
			CPPUNIT_ASSERT_MESSAGE("wp is not inserted",  wpFound);
		}
		/**
		 * Tests if a POI is added at the end of the route using operator += (Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI Database needs to be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1
		 * Failure - Otherwise
		 */
		void addPoiAtEndTest()
		{
			const int expectedValue = 1;
			bool poiFound = false;
			/* Pre-conditions */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute->connectToWpDatabase(m_pWpDb);

			/* Test Execution*/
			(*m_pTestRoute) += m_pPoi->getName();

			/* Validation */
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
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
				}
			}
			CPPUNIT_ASSERT_MESSAGE("poi is not inserted", poiFound);
		}
		/**
		 * Tests if a duplicate POI is added to the route using operator +=
		 * POI should be inserted after the first POI(Normal case)
		 *
		 * Pre:Conditions:
		 * 1. POI Database needs to be connected
		 * 2. WP Database needs to be connected
		 *
		 * Validation:
		 * Success - If the size of the resultant route has increased by 1
		 * Failure - Otherwise
		 */
		void addPoiInBetweenTest()
		{
			const int expectedValue = 2;
			CPOI *pPoi  = new CPOI(CPOI::TOURISTIC, "Amsterdam", "Exciting place", 49.8728, 8.6512);
			/* Pre-conditions */
			m_pTestRoute->connectToPoiDatabase(m_pPoiDb);
			m_pTestRoute->connectToWpDatabase(m_pWpDb);
			m_pPoiDb->addPoi(*pPoi);

			/* Test Execution*/
			(*m_pTestRoute) += pPoi->getName();

			/* Validation */
			std::vector<const CWaypoint*>routeContents = m_pTestRoute->getRoute();
			const int actualValue = (int)routeContents.size();
			CPPUNIT_ASSERT_EQUAL(expectedValue, actualValue);
		}
		static CppUnit::TestSuite* suite()
		{
			CppUnit::TestSuite* suite = new CppUnit::TestSuite("Testing operator+=");
			suite->addTest(new CppUnit::TestCaller<COperatorPlusEqualTest>("Add Waypoint using operator= Test",
							   &COperatorPlusEqualTest::addWaypointTest));
			suite->addTest(new CppUnit::TestCaller<COperatorPlusEqualTest>("Add POI at the end using operator= Test",
								&COperatorPlusEqualTest::addPoiAtEndTest));
			suite->addTest(new CppUnit::TestCaller<COperatorPlusEqualTest>("Add POI in between using operator= Test",
								&COperatorPlusEqualTest::addPoiInBetweenTest));
			return suite;
		}
};

#endif // CTEST_OPERATOR_PLUS_EQUAL_H
