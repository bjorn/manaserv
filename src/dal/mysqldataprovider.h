/*
 *  The Mana World Server
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World  is free software; you can redistribute  it and/or modify it
 *  under the terms of the GNU General  Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or any later version.
 *
 *  The Mana  World is  distributed in  the hope  that it  will be  useful, but
 *  WITHOUT ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should  have received a  copy of the  GNU General Public  License along
 *  with The Mana  World; if not, write to the  Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  $Id$
 */


#ifndef _TMWSERV_MYSQL_DATA_PROVIDER_H_
#define _TMWSERV_MYSQL_DATA_PROVIDER_H_


#include <string>

#include <mysql/mysql.h>

#include "dataprovider.h"


namespace tmwserv
{
namespace dal
{


/**
 * A MySQL Data Provider.
 */
class MySqlDataProvider: public DataProvider
{
    public:
        /**
         * Constructor.
         */
        MySqlDataProvider(void)
            throw();


        /**
         * Destructor.
         */
        ~MySqlDataProvider(void)
            throw();


        /**
         * Get the name of the database backend.
         *
         * @return the database backend name.
         */
        DbBackends
        getDbBackend(void) const
            throw();


        /**
         * Create a connection to the database.
         *
         * @param dbName the database name.
         * @param userName the user name.
         * @param password the user password.
         *
         * @exception DbConnectionFailure if unsuccessful connection.
         */
        void
        connect(const std::string& dbName,
                const std::string& userName,
                const std::string& password);


        /**
         * Execute a SQL query.
         *
         * @param sql the SQL query.
         * @param refresh if true, refresh the cache (default = false).
         *
         * @return a recordset.
         *
         * @exception DbSqlQueryExecFailure if unsuccessful execution.
         * @exception std::runtime_error if trying to query a closed database.
         */
        const RecordSet&
        execSql(const std::string& sql,
                const bool refresh = false);


        /**
         * Close the connection to the database.
         *
         * @exception DbDisconnectionFailure if unsuccessful disconnection.
         */
        void
        disconnect(void);


    private:
        MYSQL* mDb; /**< the handle to the database connection */
};


} // namespace dal
} // namespace tmwserv


#endif // _TMWSERV_MYSQL_DATA_PROVIDER_H_