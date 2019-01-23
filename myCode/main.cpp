// GIT-Labor
// main.h

////////////////////////////////////////////////////////////////////////////////

//Own Include Files
#include "CWaypoint.h"
#include "CNavigationSystem.h"

//#define TEST_1
#define TEST_NAVIGATION_SYSTEM


int main (void)
{
#ifdef TEST_1
	CWaypoint amsterdam(52.3680, 4.9036, "amsterdam"),
			  darmstadt(49.8728, 8.6512, "darmstadt"),
			  berlin(52.5166, 13.4050, "berlin"),
			  tokio(35.6895, 139.6917, "tokio");
	CWaypoint newWaypoint;


	/* Test case for all 3 getter functions */
	std::string name = "";
	double latitude = 0.0, longitude = 0.0;

	name 	  = darmstadt.getName();
	latitude  = darmstadt.getLatitude();
	longitude = darmstadt.getLongitude();
	std::cout << "Name: " << name << "Latitude: " << latitude << "Longitude: " << longitude << std::endl;
	/*Result: Name: 0x70fc40 (Size: 32 bytes), Latitude: 0x70fc38 (Size:8 bytes), Longitude: 0x70fc30 (Size: 8 bytes) */

	/* Test case for getAllDataByReference*/
	std::cout << "Name: " << &name << "Latitude: " << &latitude << "Longitude: " << &longitude << std::endl;
	tokio.getAllDataByReference(name, latitude, longitude);
	/*Result: The addresses and values of the local variables and the parameter addresses are the same*/

	/* Test Case to print the coordinates for Berlin in both the formats*/
	berlin.print(DEGREE);
	berlin.print(MMSS);

	/* Test case to calculate distance between Amsterdam and Berlin, Berlin and Tokio*/
	std::cout <<"TC: Distance b/w Amsterdam and Berlin = " << amsterdam.calculateDistance(berlin) << std::endl;
	std::cout <<"TC: Distance b/w Berlin and Tokio = " << berlin.calculateDistance(tokio) << std::endl;
#endif

#ifdef TEST_NAVIGATION_SYSTEM
	/*Implementation of Complete Navigation System*/
	CNavigationSystem obj;
	obj.run();
#endif

	return 0;
}
