/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CPOIDATABASE.CPP
 * Author          : Jishnu M Thampan
 * Description     : class CPOI
 * 				     This class maintains the database which consists
 *of
 *					 CPOI objects.
 ****************************************************************************/
// Own Include Files
#include <nsp/CPoiDatabase.h>

// Method Implementations

CPoiDatabase::CPoiDatabase() {}
/**
* Add a POI to the POIDatabase
* If the POI table is not full, adds the POI with the given attributes to the
* PoiDatabase
* If a POI with the name already exists, display an info message
* @param CPOI const& poi - the POI object to be inserted [IN]
* @return None
*/
void CPoiDatabase::addPoi(CPOI const &poi) {
  CDatabase::addData(poi.getName(), poi);
}
/**
 * Get Pointer to the POI object in the POI Database
 * Note: If no match is found NULL is returned
 * @param std::string name - Name of the POI object which is to be searched in
 * the POI Database [IN]
 * @return CPOI* 		   - If present, returns the pointer to the CPOI
 * object in the database, else NULL
 */
CPOI *CPoiDatabase::getPointerToPoi(std::string name) {
  CPOI *pPoi = CDatabase::getPointerToData(name);
  return pPoi;
}
/**
 * Get the container data from POI Database
 * @param POIDatabaseMap_t& poidDbMap [OUT] - The output data structure where
 * the internal container is copied
 * @return None
 */
void CPoiDatabase::getPoiDatabase(POIDatabaseMap_t &poidDbMap) const {
  (CDatabase::getDatabaseContent(poidDbMap));
}
/**
 * Clears the contents of POI Database
 * @param None
 * @return None
 */
void CPoiDatabase::clearPoiDatabase(void) {
  /* Clear the contents of the Database */
  CDatabase::clearDatabase();
}
/**
 * Prints the content of POI Database
 * @param None
 * @return None
 */
void CPoiDatabase::print(void) { CDatabase::printDatabase(); }
