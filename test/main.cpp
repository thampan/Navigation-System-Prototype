#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "CAddPoiTest.h"
#include "CAddWaypointTest.h"
#include "CCopyConstructorTest.h"
#include "COperatorPlusTest.h"
#include "COperatorEqualsTest.h"
#include "COperatorPlusEqualTest.h"
#include "CPrintTest.h"
#include "CGetDistanceNextPoiTest.h"

using namespace CppUnit;

int main()
{
	TextUi::TestRunner runner;

	runner.addTest( CAddWaypointTest::suite() );
	runner.addTest( CAddPoiTest::suite() );
	runner.addTest( CCopyConstructorTest::suite() );
	runner.addTest( COperatorPlusTest::suite() );
	runner.addTest( COperatorEqualsTest::suite() );
	runner.addTest( COperatorPlusEqualTest::suite() );
	runner.addTest( CPrintTest::suite() );
	runner.addTest( CGetDistanceNextPoiTest::suite() );
	runner.run();

	return 0;
}
