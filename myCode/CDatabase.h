/***************************************************************************
*============= Copyright by Darmstadt University of Applied Sciences =======
****************************************************************************
* Filename        : CDatabase.H
* Author          : Jishnu M Thampan
* Description     : class CDatabase
*					This class maintains the database which consists of
*					CPOI/CWaypoint objects.
*
****************************************************************************/

#ifndef CDATABASE_H
#define CDATABASE_H

#include <map>
#include <string>
#include <algorithm>

template <class KeyValue, class DataType>
class CDatabase
{
	private:

		typedef std::map<KeyValue, DataType> DatabaseMap_t;

		DatabaseMap_t m_databaseMap; /**< \brief Describes the Database object */

	public:
	    /**
		 * Adds the requested data to the map
		 * @param const KeyValue& key [IN] - Key value of the Template Map
		 * @param const DataType& data [IN]- The Actual Data to be stored in the map
		 * @return None
		 */
		void addData(const KeyValue& key, const DataType& data)
		{
			if (m_databaseMap.size() < m_databaseMap.max_size())
			{
				/* If there is space available then insert it into the database */
				std::pair<typename DatabaseMap_t::iterator, bool> result = m_databaseMap.insert(std::make_pair(key, data));
				if(!(result.second))
				{
					std::cout <<"CWpDatabase::addWaypoint: Data already present in the Waypoint Database" << std::endl;
				}
			}
			else
			{
				std::cout << "ERROR: CWpDatabase::addWaypoint() failed: No space available!!" << std::endl;
			}
		}
	    /**
		 * Gets the Pointer to the  Template object in the map
		 * @param const KeyValue key [IN] - Key value of the Template Map
		 * @return DataType* - Pointer to the data if found, otherwise null
		 */
	    DataType* getPointerToData(const KeyValue key)
	    {
	    	DataType* pWp = NULL;
	    	typename DatabaseMap_t::iterator itr = m_databaseMap.find(key);
	    	if(itr != m_databaseMap.end())
	    	{
	    		pWp = &(itr->second);
	    	}
	    	else
	    	{
	    		std::cout << "ERROR!CWpDatabase::getPointerToWaypoint!" << key <<" not found"<< std::endl;
	    	}
	    	return pWp;
	    }
	    /**
		 * Get the Pointer to the container of Database
		 * @param  DatabaseMap_t& dataBaseMap [OUT] -  Gets the internal map contents by reference
		 * @return None
		 */
	    void getDatabaseContent(DatabaseMap_t& dataBaseMap)const
	    {
	    	dataBaseMap = m_databaseMap;
	    }
	    /**
		 * Clears the contents of Database
		 * @param None
		 * @return None
		 */
	    void clearDatabase(void)
	    {
	    	m_databaseMap.clear();
	    }
	    /**
		 * Prints the content of Database
		 * @param None
		 * @return None
		 */
	    void printDatabase(void)
	    {
	    	for(typename DatabaseMap_t::iterator itr = m_databaseMap.begin(); itr != m_databaseMap.end(); ++itr)
	    	{
	    		std::cout << (itr->second) << std::endl;
	    	}
	    }
};

#endif
