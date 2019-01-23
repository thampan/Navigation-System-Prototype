/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CPOI.H
* Author          : Jishnu M Thampan
* Description     : class CPOI
* 					This class represents Point of Interests which may
* 					optionally be present in a route. A CPOI object is
* 					contained in the CPOIDatabase class. The CPOI class
* 					is a CWaypoint class (inherited) and has description
* 					of the POI as the extra attribute from the CWaypoint
* 					class. Different kinds of POIs are possible depending
* 					on the type and other attributes.
****************************************************************************/

#ifndef CPOI_H
#define CPOI_H

//Own Include Files
#include "CWaypoint.h"

/**
 * CPOI class represents Point of Interests which may
 * optionally be present in a route. A CPOI object is
 * contained in the CPOIDatabase class. The CPOI class
 * is a CWaypoint class (inherited) and has description
 * of the POI as the extra attribute from the CWaypoint
 * class. Different kinds of POIs are possible depending
 * on the type and other attributes.
 */
class CPOI: public CWaypoint /* CPOI is a CWaypoint */
{
public:
	virtual ~CPOI(){};
	/**
	 * \brief POI types to indicate what kind of POI it is.
	 */
	typedef enum {
		RESTAURANT, /**< POI is of RESTAURANT type */
		TOURISTIC,  /**< POI is of TOURISTIC type */
		GASSTATION, /**< POI is of GASSTATION type */
		UNIVERSITY, /**< POI is of UNIVERSITY type */
		UNKNOWN 	/**< POI is of UNKNOWN type */
	} t_poi;

    /* Public Member Functions */

    /**
	 * Constructor of CPOI class
	 * Initializes all the data members with the values provided or with the default values
	 * @param t_poi type 			  - type of the POI object			[IN] Default: UNKNOWN
	 * @param std::string name 		  - Name of the POI object			[IN] Default: EMPTY_STRING
	 * @param std::string description - Description of the POI object	[IN] Default: EMPTY_STRING
	 * @param double latitude 	 	  - Latitude of the POI object		[IN] Default: 0.0
	 * @param double longitude 		  - Longitude of the POI object		[IN] Default: 0.0
	 */
    CPOI(t_poi type = UNKNOWN, std::string name = DEFAULT_STRING, std::string description = DEFAULT_STRING,
    		double latitude = 0.0, double longitude = 0.0);
    /**
	 * Sets all of the POI attributes
	 * @param t_poi type 			  - type of the POI object			[IN]
	 * @param std::string name 		  - Name of the POI object			[IN]
	 * @param std::string description - Description of the POI object	[IN]
	 * @param double latitude 	 	  - Latitude of the POI object		[IN]
	 * @param double longitude 		  - Longitude of the POI object		[IN]
	 * @return true if the set is succeful, else false
	 */
    bool set(CPOI::t_poi type, std::string name, std::string description, double latitude, double longitude);
    /**
	 * Prints all of the POI attributes
	 * @param None
	 * @return None
	 */
    virtual void print(void);
	/**
	 * Gets the  data of all attributes of the CPOI class by reference
	 * @param std::string& name 		  - Name of the CPOI object				[OUT]
	 * @param std::string& description 	  - Description of the CPOI object		[OUT]
	 * @param double& latitude  		  - Latitude value of the CPOI object	[OUT]
	 * @param double& latitude  		  - Longitude value of the CPOI object	[OUT]
	 * @return None
	 */
    void getAllDataByReference(std::string& name, double& latitude, double& longitude, t_poi& type, std::string& description)const;
    /**
     * Gets the description field of the POI object
     * @return string - the poi description
     */
    std::string getDescription(void)const;
    /**
     * Gets the type field of the POI object
     * @return string - the poi type
     */
    t_poi getType(void)const;
    /**
     * Overloaded operator<< for POI class. Prints all POI specific data to the given ostream object
     * @param  std::ostream& stream [OUT] - The ostream object where the POI data is to be pushed into
     * @param  const CPOI& poi [IN] - The POI object whose data is to be printed
     * @return None
     */
    friend std::ostream& operator<<(std::ostream& stream, const CPOI& poi);

private:
	 /* Private Member variables */

    t_poi m_type; 				/**< \brief Indicates the type of POI */
    std::string m_description;  /**< \brief Describes the POI */
};
/********************
**  CLASS END
*********************/
#endif /* CPOI_H */
