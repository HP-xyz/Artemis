/* 
 * File:   Artemis_SQL_Handler.cpp
 * Author: dvorak
 * 
 * Created on September 11, 2011, 7:13 PM
 */

#include "Artemis_SQL_Handler.h"

namespace Artemis
{

//************************************
// Method:    Artemis_SQL_Handler
// FullName:  Artemis::Artemis_SQL_Handler::Artemis_SQL_Handler
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: std::map<std::string
// Parameter: std::string> & parameters
//************************************
Artemis_SQL_Handler::Artemis_SQL_Handler(
		std::map<std::string, std::string>& parameters)
{
	Artemis_SQL_Handler::parameters = parameters;
	Artemis_SQL_Handler::url = new std::string(
			"tcp://" + parameters["database.address"]
					+ parameters["database.part"]);
	Artemis_SQL_Handler::user = new std::string(parameters["database.user"]);
	Artemis_SQL_Handler::password = new std::string(
			parameters["database.password"]);
	Artemis_SQL_Handler::database = new std::string(
			parameters["database.name"]);

	try
	{
		Artemis_SQL_Handler::driver = get_driver_instance();
		Artemis_SQL_Handler::conn = driver->connect(*url, *user, *password);
		conn->setAutoCommit(0);

#ifdef _DEBUG
		std::cout << "\nDatabase connection\'s autocommit mode = "
				<< conn->getAutoCommit() << std::endl;
		conn->setSchema(*database);
		Artemis_SQL_Handler::retrieve_and_print_database_metadata(conn);
#endif // _DEBUG
		Artemis_SQL_Handler::statement = conn->createStatement();
	} catch (sql::SQLException &e)
	{
		//std::cout << "ERROR: SQLException in " << __FILE__;
		//std::cout << " (" << __func__<< ") on line " << __LINE__ << std::endl;
		std::cout << "ERROR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << ")" << std::endl;

		if (e.getErrorCode() == 1047)
		{
			/*
			 Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
			 Message: Unknown command
			 */
			std::cout
					<< "\nYour server does not seem to support Prepared Statements at all. ";
			std::cout << "Perhaps MYSQL < 4.1?" << std::endl;
		}

	} catch (std::runtime_error &e)
	{

		std::cout << "ERROR: runtime_error in " << __FILE__;
		//std::cout << " (" << __func__ << ") on line " << __LINE__ << std::endl;
		std::cout << "ERROR: " << e.what() << std::endl;
	}
}

//************************************
// Method:    Artemis_SQL_Handler
// FullName:  Artemis::Artemis_SQL_Handler::Artemis_SQL_Handler
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: const Artemis_SQL_Handler & orig
//************************************
Artemis_SQL_Handler::Artemis_SQL_Handler(const Artemis_SQL_Handler& orig)
{
}

//************************************
// Method:    ~Artemis_SQL_Handler
// FullName:  Artemis::Artemis_SQL_Handler::~Artemis_SQL_Handler
// Access:    virtual public 
// Returns:   
// Qualifier:
//************************************
Artemis_SQL_Handler::~Artemis_SQL_Handler()
{
#if _DEBUG
	std::cout << "= Artemis SQL Handler" << std::endl
	<< "== Freeing resources" << std::endl;
#endif
	/* Closing the connection */
	conn->close();

	/* Freeing resources */
	if (statement != 0)
		delete statement;
	if (resultset != 0)
		delete resultset;
	if (url != 0)
		delete url;
	if (user != 0)
		delete user;
	if (password != 0)
		delete password;
	if (database != 0)
		delete database;
	if (conn != 0)
		delete conn;
}

//************************************
// Method:    retrieve_and_print_resultset_metadata
// FullName:  Artemis::Artemis_SQL_Handler::retrieve_and_print_resultset_metadata
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: sql::ResultSet * rs
//************************************
void Artemis_SQL_Handler::retrieve_and_print_resultset_metadata(
		sql::ResultSet* rs)
{
	if (rs->rowsCount() == 0)
	{
		throw std::runtime_error(
				"ResultSetMetaData FAILURE - no records in the result set");
	}

	std::cout << "ResultSet Metadata" << std::endl;
	std::cout << "------------------" << std::endl;

	sql::ResultSetMetaData *res_meta = rs->getMetaData();

	int numcols = res_meta->getColumnCount();
	std::cout << "\nNumber of columns in the result set = " << numcols
			<< std::endl << std::endl;

	std::cout.width(20);
	std::cout << "Column Name/Label";
	std::cout.width(20);
	std::cout << "Column Type";
	std::cout.width(20);
	std::cout << "Column Size" << std::endl;

	for (int i = 0; i < numcols; ++i)
	{
		std::cout.width(20);
		std::cout << res_meta->getColumnLabel(i + 1);
		std::cout.width(20);
		std::cout << res_meta->getColumnTypeName(i + 1);
		std::cout.width(20);
		std::cout << res_meta->getColumnDisplaySize(i + 1) << std::endl
				<< std::endl;
	}

	std::cout << "\nColumn \"" << res_meta->getColumnLabel(1);
	std::cout << "\" belongs to the Table: \"" << res_meta->getTableName(1);
	std::cout << "\" which belongs to the Schema: \""
			<< res_meta->getSchemaName(1) << "\"" << std::endl << std::endl;
}

//************************************
// Method:    retrieve_and_print_database_metadata
// FullName:  Artemis::Artemis_SQL_Handler::retrieve_and_print_database_metadata
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: sql::Connection * dbcon
//************************************
void Artemis_SQL_Handler::retrieve_and_print_database_metadata(
		sql::Connection* dbcon)
{
	if (dbcon->isClosed())
	{
		throw std::runtime_error(
				"DatabaseMetaData FAILURE - database connection closed");
	}

	std::cout << "\nDatabase Metadata" << std::endl;
	std::cout << "-----------------" << std::endl;

	std::cout << std::boolalpha;

	/* The following commented statement won't work with Connector/C++ 1.0.5 and later */
	//auto_ptr < DatabaseMetaData > dbcon_meta (dbcon -> getMetaData());
	sql::DatabaseMetaData *dbcon_meta = dbcon->getMetaData();

	std::cout << "Database Product Name: "
			<< dbcon_meta->getDatabaseProductName() << std::endl;
	std::cout << "Database Product Version: "
			<< dbcon_meta->getDatabaseProductVersion() << std::endl;
	std::cout << "Database User Name: " << dbcon_meta->getUserName()
			<< std::endl << std::endl;

	std::cout << "Driver name: " << dbcon_meta->getDriverName() << std::endl;
	std::cout << "Driver version: " << dbcon_meta->getDriverVersion()
			<< std::endl << std::endl;

	std::cout << "Database in Read-Only Mode?: " << dbcon_meta->isReadOnly()
			<< std::endl;
	std::cout << "Supports Transactions?: "
			<< dbcon_meta->supportsTransactions() << std::endl;
	std::cout << "Supports DML Transactions only?: "
			<< dbcon_meta->supportsDataManipulationTransactionsOnly()
			<< std::endl;
	std::cout << "Supports Batch Updates?: "
			<< dbcon_meta->supportsBatchUpdates() << std::endl;
	std::cout << "Supports Outer Joins?: " << dbcon_meta->supportsOuterJoins()
			<< std::endl;
	std::cout << "Supports Multiple Transactions?: "
			<< dbcon_meta->supportsMultipleTransactions() << std::endl;
	std::cout << "Supports Named Parameters?: "
			<< dbcon_meta->supportsNamedParameters() << std::endl;
	std::cout << "Supports Statement Pooling?: "
			<< dbcon_meta->supportsStatementPooling() << std::endl;
	std::cout << "Supports Stored Procedures?: "
			<< dbcon_meta->supportsStoredProcedures() << std::endl;
	std::cout << "Supports Union?: " << dbcon_meta->supportsUnion() << std::endl
			<< std::endl;

	std::cout << "Maximum Connections: " << dbcon_meta->getMaxConnections()
			<< std::endl;
	std::cout << "Maximum Columns per Table: "
			<< dbcon_meta->getMaxColumnsInTable() << std::endl;
	std::cout << "Maximum Columns per Index: "
			<< dbcon_meta->getMaxColumnsInIndex() << std::endl;
	std::cout << "Maximum Row Size per Table: " << dbcon_meta->getMaxRowSize()
			<< " bytes" << std::endl;

	std::cout << "\nDatabase schemas: " << std::endl;

	std::auto_ptr<sql::ResultSet> rs(dbcon_meta->getSchemas());

	std::cout << "\nTotal number of schemas = " << rs->rowsCount() << std::endl;
	std::cout << std::endl;

	int row = 1;

	while (rs->next())
	{
		std::cout << "\t" << row << ". " << rs->getString("TABLE_SCHEM")
				<< std::endl;
		++row;
	} // while

	std::cout << std::endl << std::endl;
}

//************************************
// Method:    run_query
// FullName:  Artemis::Artemis_SQL_Handler::run_query
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const std::string & query
//************************************
void Artemis_SQL_Handler::run_query(const std::string &query)
{
	Artemis_SQL_Handler::resultset =
			Artemis_SQL_Handler::statement->executeQuery(query);
}

//************************************
// Method:    create_resultset_xml
// FullName:  Artemis::Artemis_SQL_Handler::create_resultset_xml
// Access:    public 
// Returns:   std::string
// Qualifier:
//************************************
std::string Artemis_SQL_Handler::create_resultset_xml()
{
#ifdef _DEBUG
	std::cout << "== create_resultset_xml()" << std::endl;
	std::cout << "=========================" << std::endl;
#endif // _DEBUG
	pugi::xml_document document;
	size_t colom_count =
			Artemis_SQL_Handler::resultset->getMetaData()->getColumnCount();

	/* fetch the data : retrieve all the rows in the result set */
	pugi::xml_node root_node = document.append_child("Artemis");
	while (Artemis_SQL_Handler::resultset->next())
	{
		pugi::xml_node row = root_node.append_child("row");
		for (size_t i = 1; i <= colom_count; i++)
		{
			pugi::xml_node node = row.append_child(
					resultset->getMetaData()->getColumnLabel(i).c_str());
			node.append_child(pugi::node_pcdata).set_value(
					resultset->getString(i).c_str());
		}
	} // while
	xml_string_writer writer;
	document.print(writer);

	return writer.result;
}

}