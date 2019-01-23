/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CPOI.CPP
 * Author          : Jishnu M Thampan
 * Description     : class CPOI
 *  				 This class represents Point of Interests which may
 * 					 optionally be present in a route. A CPOI object is
 * 					 contained in the CPOIDatabase class. The CPOI class
 * 					 is a CWaypoint class (inherited) and has description
 * 					 of the POI as the extra attribute from the CWaypoint
 * 					 class. Different kinds of POIs are possible depending
 * 					 on the type and other attributes.
 ****************************************************************************/
#include <string>
//Own Include Files
#include "CPOI.h"

#define SPACE_CHAR " " /**< \brief Represents Space character */
//Method Implementations

/**
 * Constructor of CPOI class
 * Initializes all the data members with the values provided or with the default values
 * @param t_poi type 			  - type of the POI object			[IN] Default: UNKNOWN
 * @param std::string name 		  - Name of the POI object			[IN] Default: EMPTY_STRING
 * @param std::string description - Description of the POI object	[IN] Default: EMPTY_STRING
 * @param double latitude 	 	  - Latitude of the POI object		[IN] Default: 0.0
 * @param double longitude 		  - Longitude of the POI object		[IN] Default: 0.0
 */
CPOI::CPOI(t_poi type, std::string name, std::string description, double latitude,
		double longitude)
: CWaypoint(latitude, longitude, name), /* Calls the Waypoint (Base class) constructor to set the class attributes */
  m_type(type), /* Initialize m_type object with the default type or the provided type */
  m_description(description)  /* Initialize m_description object with the default type or the provided type */
{
}
/**
 * Sets all of the POI attributes
 * @param t_poi type 			  - type of the POI object			[IN]
 * @param std::string name 		  - Name of the POI object			[IN]
 * @param std::string description - Description of the POI object	[IN]
 * @param double latitude 	 	  - Latitude of the POI object		[IN]
 * @param double longitude 		  - Longitude of the POI object		[IN]
 * @return true if the set is succeful, else false
 */
bool CPOI::set(CPOI::t_poi type, std::string name, std::string description, double latitude, double longitude)
{
	bool isValidPoi = false;
	do
	{
		if((type < CPOI::UNKNOWN) && (type >= 0))
		{
			isValidPoi = true;
		}
		else
		{
			isValidPoi = false;
			std::cout << "ERROR!Invalid Poi Type!" << std::endl;
			break;
		}
		if(description.find_first_not_of(' ') != std::string::npos)
		{
			isValidPoi = true;
		}
		else
		{
			isValidPoi = false;
			std::cout << "--ERROR!Invalid Poi Description!" << std::endl;
			break;
		}
		if((isValidPoi = CWaypoint::set(name, latitude, longitude)))
		{
			m_type 		  = type;
			m_description = description;
		}
		else
		{
			m_type 		  = CPOI::UNKNOWN;
			m_description = DEFAULT_STRING;
		}

	}while(false);

	return isValidPoi;
}
/**
 * Gets the  data of all attributes of the CPOI class by reference
 * @param std::string& name 		  - Name of the CPOI object				[OUT]
 * @param std::string& description 	  - Description of the CPOI object		[OUT]
 * @param double& latitude  		  - Latitude value of the CPOI object	[OUT]
 * @param double& latitude  		  - Longitude value of the CPOI object	[OUT]
 * @return None
 */
void CPOI::getAllDataByReference(std::string& name, double& latitude, double& longitude,
		t_poi& type, std::string& description) const
{
	CWaypoint::getAllDataByReference(name, latitude, longitude); /* Get the Waypoint Data from the Waypoint (Base) Class */
	type		= this->m_type;
	description = this->m_description;
}
/**
 * Prints all of the POI attributes
 * @param  None
 * @return None
 */
void CPOI::print(void)
{
	std::cout << *this << "\n";
}
/**
 * Overloaded operator<< for POI class. Prints all POI specific data to the given ostream object
 * @param  std::ostream& stream [OUT] - The ostream object where the POI data is to be pushed into
 * @param  const CPOI& poi [IN] - The POI object whose data is to be printed
 * @return None
 */
std::ostream& operator<<(std::ostream& stream, const CPOI& poi)
{
	std::string name = DEFAULT_STRING,  description = DEFAULT_STRING;
		double latitude  = 0.0, longitude = 0.0;
		CPOI::t_poi type       = CPOI::UNKNOWN;

		poi.getAllDataByReference(name, latitude, longitude, type, description); /*Populate the local variables with the object data */
		/*Print the POI type based on the type which was assigned by the user */
		std::string poiType = "of Type ";
		stream  << "Point of interest" << std::endl;
		stream  << "=================" << std::endl;
		switch(type)
		{
			case CPOI::RESTAURANT:
			{
				poiType += "Restaurant:";
				break;
			}
			case CPOI::TOURISTIC:
			{
				poiType += "Touristic:";
				break;
			}
			case CPOI::GASSTATION:
			{
				poiType += "Gasstation:";
				break;
			}
			case CPOI::UNIVERSITY:
			{
				poiType += "University:";
				break;
			}
			default:
			{
				poiType += "Unknown:";
				stream << ", Unknown type!: " << description << std::endl;
			}
		}
		stream << poiType << SPACE_CHAR << description << std::endl;
		int lat_deg   = 0, lat_mm = 0, lon_deg = 0, lon_mm = 0;
		double lat_ss = 0, lon_ss = 0;

		poi.transformLatitude2degmmss(lat_deg, lat_mm, lat_ss);
		poi.transformLatitude2degmmss(lon_deg, lon_mm, lon_ss);

		stream << name << " on latitude = " << lat_deg << " deg "
					<< lat_mm << " min " << lat_ss << " s" << " and longitude = "
					<< lon_deg << "deg " << lon_mm << " min " << lon_ss << " s" << std::endl;
		/*stream << name << SPACE_CHAR << "on latitude:" << latitude
				  << SPACE_CHAR << "and longitude:" << longitude << std::endl;*/
	return stream;
}
/**
 * Gets the description field of the POI object
 * @return string - the poi description
 */
std::string CPOI::getDescription()const
{
	return m_description;
}
/**
 * Gets the type field of the POI object
 * @return string - the poi type
 */
CPOI::t_poi CPOI::getType()const
{
	return m_type;
}
