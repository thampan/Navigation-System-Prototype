/***************************************************************************
****************************************************************************
* Filename        : CGPSSENSOR.H
* Author          : Jishnu M Thampan
* Description     : class CGPSSensor
* 					CGPSSensor class is responsible for
*fetching the coordinates
* 					from the GPS module (keyboard in this
*case). This class creates
* 					a CWaypoint object out of these coordinates
*and return this
* 					object when invoked.
****************************************************************************/

#ifndef CGPSSENSOR_H
#define CGPSSENSOR_H

#include "CWaypoint.h"

/**
 * class CGPSSensor is responsible for fetching the coordinates
 * from the GPS module (keyboard in this case). This class creates
 * a CWaypoint object out of these coordinates and return this object when
 * invoked.
 */
class CGPSSensor {

public:
  /* Public Member Functions */

  /**
       * Constructor of CGPSSensor class
       */
  CGPSSensor();
  /**
   * Get the current position from the user
   * @return CWaypoint - CWaypoint object
   */
  CWaypoint getCurrentPosition(void);
};
/********************
**  CLASS END
*********************/
#endif /* CGPSSENSOR_H */
