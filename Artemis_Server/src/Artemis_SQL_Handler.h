/*
 * File:   Artemis_SQL_Handler.h
 * Author: dvorak
 *
 * Created on September 11, 2011, 7:13 PM
 */

#ifndef ARTEMIS_SQL_HANDLER_H
#define	ARTEMIS_SQL_HANDLER_H
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <pugixml.hpp>
#include <vector>
namespace Artemis
{

class Artemis_SQL_Handler
{
public:
	/** Constructor, creates the connection to the mysql database.
	 ** Receives a parameter map from the calling program*/
	Artemis_SQL_Handler(std::map<std::string, std::string>&);
	Artemis_SQL_Handler(const Artemis_SQL_Handler& orig);
	virtual ~Artemis_SQL_Handler();

	/** Runs the actual query and creates the resultset XML */
	void run_query(const std::string&);
	/** Creates the resultset XML */
	std::string create_resultset_xml();
private:
	/** Retrieve and print to std::cout the resultset metadata */
	void retrieve_and_print_resultset_metadata(sql::ResultSet*);
	/** Retrieve and print to std::cout the database metadata */
	void retrieve_and_print_database_metadata(sql::Connection*);

	struct xml_string_writer: pugi::xml_writer
	{
		std::string result;

		virtual void write(const void* data, size_t size)
		{
			result += std::string(static_cast<const char*>(data), size);
		}
	};

	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *statement;
	sql::ResultSet *resultset;

	std::string *url;
	const std::string *user;
	const std::string *password;
	const std::string *database;

	/** Parameters received in config file, will be initialized by
	 ** constructor */
	std::map<std::string, std::string> parameters;
};
}
#endif	/* ARTEMIS_SQL_HANDLER_H */
