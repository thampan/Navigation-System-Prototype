/***************************************************************************
 * ============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CGPSSENSOR.CPP
 * Author          : Jishnu M Thampan
 * Description     : class CGPSSensor
 * CGPSSensor class is responsible for fetching the coordinates
 * from the GPS module (keyboard in this case).
 * This class creates a CWaypoint object out of these coordinates
 * and return this object when invoked.
 ****************************************************************************/
// System include files
#include <limits>

// Own Include Files
#include <nsp/CGPSSensor.h>
#include <nsp/CWaypoint.h>

// Method Implementations

/**
 * Constructor of CGPSSensor class
 */
CGPSSensor::CGPSSensor() {}
/**
 * Get the current position from the user
 * In case of invalid position from the user,
 * set the name attribute to EMPTY_STRING which
 * indicates that the inpur parsing has been failed
 * @return CWaypoint CWaypoint object which holds the current position
 */
CWaypoint CGPSSensor::getCurrentPosition(void) {
  double latitude = 0.0, longitude = 0.0;
  std::string name = "The Current Position";

  std::cout << "GPS Sensor" << std::endl;
  std::cout << "   Enter latitude" << std::endl;
  std::cin >> latitude;

  if (std::cin.fail()) /* In case the input validation fails */
  {
    std::cin.clear(); /* Clear the error state of the buffer */
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); /* Ignore the rest of the line */
    std::cout << "ERROR!CGPSSensor::getCurrentPosition(): Invalid Latitude! "
                 "Hence Defaulting to 0"
              << std::endl;
    name = DEFAULT_STRING; /* Set the name to EMPTY_STRING which indicates the
                              position is invalid */
  }
  std::cout << "   Enter longitude" << std::endl;
  std::cin >> longitude;

  if (std::cin.fail()) /* In case the input validation fails */
  {
    std::cin.clear(); /* Clear the error state of the buffer */
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); /* Ignore the rest of the line */
    std::cout << "ERROR!CGPSSensor::getCurrentPosition(): Invalid Longitude! "
                 "Hence Defaulting to 0"
              << std::endl;
    name = DEFAULT_STRING; /* Set the name to EMPTY_STRING which indicates the
                              position is invalid */
  }
  return CWaypoint(
      latitude, longitude,
      name); /*Return CWaypoint object after setting the attributes */
}
