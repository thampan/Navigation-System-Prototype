/***************************************************************************
* ============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CWAYPOINT.H
* Author          : Jishnu M Thampan
* Description     : class CWaypoint
* 					CWaypoint represents a Waypoint which
*consist of
* 					latitude and longitude and a name. This
*class provides
* 					methods to print the Waypoints in different
*formats,
* 					and methods to calculate distance between
*two waypoints.
****************************************************************************/

#ifndef CWAYPOINT_H
#define CWAYPOINT_H

// System Include Files
#include <iostream>

//#define SHOWADRESS 		/**< \brief To show the object addresses
//(verbose mode) */
#define DEGREE 1 /**< \brief To print the Coordinates in DEGREE format */
#define MMSS 2   /**< \brief To print the Coordinates in MMSS format */
#define DEFAULT_STRING "default" /**< \brief Represents an empty string */

#define MIN_LATITUDE -90 /**< \brief Defines the minimum valid latitude */
#define MAX_LATITUDE 90  /**< \brief Defines the maximum valid latitude  */

#define MIN_LONGITUDE -180 /**< \brief Defines the minimum valid longitude  */
#define MAX_LONGITUDE 180  /**< \brief Defines the maximum valid longitude  */

/** CWaypoint class represents a Waypoint in a Navigation System
 *  which consist of latitude and longitude and a name. This class
 *  provides methods to print the Waypoints in different formats,
 *  and methods to calculate distance between two Waypoints.
 */
class CWaypoint {
protected:
  /* Protected Member variables */

  double m_longitude; /**< \brief Contains the Longitude of the Waypoint */
  double m_latitude;  /**< \brief Contains the Latitude of the Waypoint */
  std::string m_name; /**< \brief Contains the name of the Waypoint */

public:
  /* Public Member Functions */
  virtual ~CWaypoint(){};
  /**
   * Parameterized constructor to CWaypoint class
   * Calls the set member function to set the provided values
   * In case of invalid input, default values are set
   * @param double latitude  - Latitude value of the CWaypoint object, default:
   * 0.0   [IN]
   * @param double latitude  - Longitude value of the CWaypoint object, default:
   * 0.0  [IN]
   * @param std::string name - Name of the CWaypoint object, default:
   * EMPTY_STRING    [IN]
   */
  CWaypoint(double latitude = 0, double longitude = 0,
            std::string name = DEFAULT_STRING);
  /**
   * Sets the attributes of the CWaypoint class
   * Sets the object attributes to provided values if they fall within the valid
   * range
   * Sets the object attributes to default values in case they don't fall within
   * the valid range
   * @param  std::string name - Name of the CWaypoint object
   * [IN]
   * @param  double latitude  - Latitude value of the CWaypoint object  [IN]
   * @param  double latitude  - Longitude value of the CWaypoint object [IN]
   * @return None
   */
  bool set(std::string name, double latitude, double longitude);
  /**
   * Gets the  data of all attributes of the CWaypoint class by reference
   * @param  std::string& name - Name of the CWaypoint object
   * [OUT]
   * @param  double& latitude  - Latitude value of the CWaypoint object  [OUT]
   * @param  double& latitude  - Longitude value of the CWaypoint object [OUT]
   * @return None
   */
  void getAllDataByReference(std::string &name, double &latitude,
                             double &longitude) const;
  /**
   * Get the attribute 'name' of CWaypoint object
   * @param  None
   * @return name
   */
  std::string getName() const;
  /**
   * Get the attribute 'latitude' of CWaypoint object
   * @param  None
   * @return latitude
   */
  double getLatitude() const;
  /**
   * Get the attribute 'longitude' of CWaypoint object
   * @param  None
   * @return longitude
   */
  double getLongitude() const;
  /**
   * Calculate distance between two Waypoints
   * 1. Convert the Waypoint attribute data to radian format
   * 2. Apply the formula to get the distance
   * @param  const CWaypoint& - Reference to the CWaypoint object [IN]
   * @return calculated distance
   */
  double calculateDistance(const CWaypoint &wp);
  /**
   * Print the CWaypoint object in the specified format
   * @param  int format - The format in which the CWaypoint object is to be
   * printed - DEGREE or MMSS format [IN]
   * @return None
   */
  virtual void print(int format);
  /**
   * Overloaded operator<< for Waypoint class. Prints all Waypoint specific data
   * to the given ostream object
   * @param  std::ostream& stream [OUT] - The ostream object where the Waypoint
   * data is to be pushed into
   * @param  const CWaypoint& wp [IN] - The Waypoint object whose data is to be
   * printed
   * @return None
   */
  friend std::ostream &operator<<(std::ostream &stream, CWaypoint &wp);
  /**
   * Transform Latitude to [deg mm ss] format
   * @param  int& deg - Reference to deg [OUT]
   * @param  int& mm  - Reference to mm  [OUT]
   * @param  int& ss  - Reference to ss  [OUT]
   * @return None
   */
  void transformLatitude2degmmss(int &deg, int &mm, double &ss) const;
  /**
   * Transform Longitude to [deg mm ss] format
   * @param  int& deg - Reference to deg [OUT]
   * @param  int& mm  - Reference to mm  [OUT]
   * @param  int& ss  - Reference to ss  [OUT]
   * @return None
   */
  void transformLongitude2degmmss(int &deg, int &mm, double &ss) const;
};
/********************
**  CLASS END
*********************/
#endif /* CWAYPOINT_H */
