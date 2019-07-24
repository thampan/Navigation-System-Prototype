/***************************************************************************
****************************************************************************
* Filename        : CWAYPOINT.CPP
* Author          : Jishnu M Thampan
* Description     : Implementation of class CWaypoint
* 					CWaypoint represents a Waypoint which consist of
* 					latitude and longitude and a name. This class provides
* 					methods to print the Waypoints in different formats,
* 					and methods to calculate distance between two Waypoints.
****************************************************************************/
//System Include Files
#include <stdlib.h>
#include <math.h>

//Own Include Files
#include "CWaypoint.h"

#define EARTH_RADIUS_LENGTH 6378.17 				/**< \brief Defines the Radius of Earth in kilometers*/
#define PI 3.14159265 					  			/**< \brief Defines the constant PI */
#define CONVERT_DEGREE_TO_RADIAN(x) (x * PI/180) 	/**< \brief Defines the conversion from degree to radian*/
#define TIME_UNIT 60								/**< \brief Defines the time unit  */

//Method Implementations

/**
 * Parameterized constructor to CWaypoint class
 * Calls the set member function to set the provided values
 * In case of invalid input, default values are set
 * @param double latitude  - Latitude value of the CWaypoint object, default: 0.0   [IN]
 * @param double latitude  - Longitude value of the CWaypoint object, default: 0.0  [IN]
 * @param std::string name - Name of the CWaypoint object, default: EMPTY_STRING    [IN]
 */
CWaypoint::CWaypoint(double latitude, double longitude, std::string name)
: m_longitude(longitude),
  m_latitude(latitude),
  m_name(name)
{
	this->set(name, latitude, longitude);
#ifdef SHOWADRESS
	std::cout << "CWaypoint::constructor()" << std::endl;
	std::cout << "CWaypoint::CWaypoint:Object address:" << this << std::endl;
	std::cout << "CWaypoint::CWaypoint:Object attributes:" << std::endl;
	std::cout << "CWaypoint::CWaypoint:name: "      << this->m_name      << ", addr:" << &this->m_name      << std::endl;
	std::cout << "CWaypoint::CWaypoint:latitude: "  << this->m_latitude  << ", addr:" << &this->m_latitude  << std::endl;
	std::cout << "CWaypoint::CWaypoint:longitude: " << this->m_longitude << ", addr:" << &this->m_longitude <<std::endl;
#endif
}
/**
 * Sets the attributes of the CWaypoint class
 * Sets the object attributes to provided values if they fall within the valid range
 * Sets the object attributes to default values in case they don't fall within the valid range
 * @param std::string name - Name of the CWaypoint object 			 [IN]
 * @param double latitude  - Latitude value of the CWaypoint object  [IN]
 * @param double latitude  - Longitude value of the CWaypoint object [IN]
 * @return None
 */
bool CWaypoint::set(std::string name, double latitude, double longitude)
{
	bool isValidWaypoint = false;
	do
	{
		if(((MIN_LATITUDE <= latitude) && (MAX_LATITUDE >= latitude)) &&
				((MIN_LONGITUDE <= longitude) && (MAX_LONGITUDE >= longitude))) /* Checks if the latitude and longitude
																			   fall within the valid range*/
		{
			isValidWaypoint   = true;
		}
		else
		{
			std::cout<<"ERROR!:set()failed: Invalid Latitude/Longitude Parameters!" << std::endl;
			isValidWaypoint = false;
			break;
		}
		if(name.find_first_not_of(' ') != std::string::npos)
		{
			isValidWaypoint = true;
		}
		else
		{
			std::cout<<"--ERROR!:set()failed: Invalid Name!" << std::endl;
			isValidWaypoint = false;
			break;
		}
	}while(false);

	if(isValidWaypoint)
	{
		this->m_latitude  = latitude;
		this->m_longitude = longitude;
		m_name 			  = name;
	}
	else /* If the provided values does not fall in the valid range, set the object attributes to default values */
	{
		this->m_latitude  = 0;
		this->m_longitude = 0;
		this->m_name      = DEFAULT_STRING;
		std::cout << "WARNING! CWaypoint::set(): Invalid values provided, so default values in action!" << std::endl;
	}
	return isValidWaypoint;
}
/**
 * Get the attribute 'name' of CWaypoint object
 * @param None
 * @return name
 */
std::string CWaypoint::getName()const
{
	return m_name;
}
/**
 * Get the attribute 'latitude' of CWaypoint object
 * @param None
 * @return latitude
 */
double CWaypoint::getLatitude()const
{
	return m_latitude;
}
/**
 * Get the attribute 'longitude' of CWaypoint object
 * @param None
 * @return longitude
 */
double CWaypoint::getLongitude()const
{
	return m_longitude;
}
/**
 * Gets the  data of all attributes of the CWaypoint class by reference
 * @param std::string& name - Name of the CWaypoint object			  [OUT]
 * @param double& latitude  - Latitude value of the CWaypoint object  [OUT]
 * @param double& latitude  - Longitude value of the CWaypoint object [OUT]
 * @return None
 */
void CWaypoint::getAllDataByReference(std::string& name, double& latitude, double& longitude) const
{
	name      = this->getName(); /* Gets the attribute 'name' of the object and assigns that */
	latitude  = this->getLatitude(); /* Gets the attribute 'latitude' of the object and assigns that */
	longitude = this->getLongitude(); /* Gets the attribute 'longitude' of the object and assigns that */
}
/**
 * Calculate distance between two Waypoints
 * 1. Convert the Waypoint attribute data to radian format
 * 2. Apply the formula to get the distance
 * @param const CWaypoint& - Reference to the CWaypoint object [IN]
 * @return calculated distance
 */
double CWaypoint::calculateDistance(const CWaypoint& wp)
{
	double latitude_1  = CONVERT_DEGREE_TO_RADIAN(this->getLatitude()); /* Convert the first latitude from degree to radian format*/
	double longitude_1 = CONVERT_DEGREE_TO_RADIAN(this->getLongitude()); /* Convert the first longitude from degree to radian format*/

	double latitude_2  = CONVERT_DEGREE_TO_RADIAN(wp.m_latitude); /* Convert the second latitude from degree to radian format*/
	double longitude_2 = CONVERT_DEGREE_TO_RADIAN(wp.m_longitude); /* Convert the second longitude from degree to radian format*/

	double distance    = EARTH_RADIUS_LENGTH * acos(sin(latitude_1)  * sin(latitude_2) + cos(latitude_1) * cos(latitude_2) *
							      cos((longitude_2 - longitude_1))); /* Calculate distance between two Waypoints using the formula */

	return distance; /* Return the calculated distance */
}
/**
 * Print the CWaypoint object in the specified format
 * @param int format - The format in which the CWaypoint object is to be printed - DEGREE or MMSS format [IN]
 * @return None
 */
void CWaypoint::print(int format)
{
	std::cout << (*this) << std::endl;
}
/**
 * Transform Latitude to [deg mm ss] format
 * @param int& deg - Reference to degree  [OUT]
 * @param int& mm  - Reference to minute  [OUT]
 * @param int& ss  - Reference to seconds [OUT]
 * @return None
 */
void CWaypoint::transformLongitude2degmmss(int& deg, int& mm, double& ss)const
{
	deg 			= floor(this->getLongitude()); /* Get the integer portion in the longitude*/
	double decimal 	= this->getLongitude() - deg; /* Get the decimal part in the longitude */

	double minutes  = (decimal * TIME_UNIT); /* To get the Minute count, multiply the decimal portion by 60*/
	mm 				= floor(minutes); /* The integer portion of the result would hold the minutes data */

	double seconds  = minutes - mm; /* Get the decimal portion in the minutes data*/
	ss				= (seconds * TIME_UNIT); /* To get the second count, again multiply the decimal portion by 60*/
}
/**
 * Transform Longitude to [deg mm ss] format
 * @param int& deg - Reference to degree  [OUT]
 * @param int& mm  - Reference to minute  [OUT]
 * @param int& ss  - Reference to seconds [OUT]
 * @return None
 */
void CWaypoint::transformLatitude2degmmss(int& deg, int& mm, double& ss)const
{
	deg 		   = floor(this->getLatitude()); /* Get the integer portion in the latitude*/
	double decimal = this->getLatitude() - deg; /* Get the decimal part in the latitude */

	double minutes = (decimal * TIME_UNIT); /* To get the Minute count, multiply the decimal portion by 60*/
	mm 			   = floor(minutes); /* The integer portion of the result would hold the minutes data */

	double seconds = minutes - mm; /* Get the decimal portion in the minutes data*/
	ss 			   = (seconds * TIME_UNIT); /* To get the second count, again multiply the decimal portion by 60*/
}
/**
 * Overloaded operator<< for Waypoint class. Prints all Waypoint specific data to the given ostream object
 * @param  std::ostream& stream [OUT] - The ostream object where the Waypoint data is to be pushed into
 * @param  const CWaypoint& wp [IN] - The Waypoint object whose data is to be printed
 * @return None
 */
std::ostream& operator<<(std::ostream& stream, CWaypoint& wp)
{

	int lat_deg = 0, lat_mm = 0, lon_deg = 0, lon_mm = 0;
	double lat_ss = 0.0, lon_ss = 0.0;

	wp.transformLongitude2degmmss(lon_deg, lon_mm, lon_ss); /* Transforms the longitude to DEGSS format*/
	wp.transformLatitude2degmmss(lat_deg, lat_mm, lat_ss); /* Transforms the latitude to DEGSS format*/

	stream << wp.getName() << " on latitude = " << lat_deg << " deg "
			<< lat_mm << " min " << lat_ss << " s" << " and longitude = "
			<< lon_deg << "deg " << lon_mm << " min " << lon_ss << " s" << std::endl;

	return stream;
}
