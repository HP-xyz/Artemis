/*
 * File:   Artemis_Request_Handler.h
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:12 PM
 */

#ifndef ARTEMIS_REQUEST_HANDLER_H
#define	ARTEMIS_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <map>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include <b64/decode.h>
#include <b64/encode.h>
#include "Artemis_Network_Sender_Connection.h"
#include "Group_Container.h"
#include "Playlist_Container.h"
#include "Group_Playlist_Container.h"
namespace Artemis
{
enum STATUS
{
	NO_RESULT, MULTIPLE_RESULTS, SINGLE_RESULT
};
class Artemis_Request_Handler: private boost::noncopyable
{
public:
	/** Construct Artemis_Request_Handler object */
	explicit Artemis_Request_Handler(
			boost::shared_ptr<Group_Container> p_groups_and_computers,
			boost::shared_ptr<Playlist_Container> p_playlist,
			boost::shared_ptr<Group_Playlist_Container> p_group_playlist);
	~Artemis_Request_Handler();

	/** Handle a request and produce a reply*/
	void handle_request(const std::string&,
			std::string p_remote_address,
			std::map<std::string, std::string>&);

	STATUS result_status;
private:
	/** Result xml document to be returned */
	pugi::xml_document document;
	/** Vector of SQL queries to be executed */
	std::string remote_address;

	void handle_upload(std::string upload_xml, std::string dest_ip,
			std::string dest_port);

	/** Function to parse received XML to find queries to be run */
	void generate_queries(const std::string&);

	void save_uploaded_file(pugi::xml_node tacktech);
	std::string get_binary_file(std::string filename);

	std::map<std::string, std::string> parameters;

	/* Variable for computer names and group names
	 * Note: Format is groups_and_computers[group_index][computer_index]
	 * Note: This can be hostnames or IP addresses */
	boost::shared_ptr<Group_Container> groups_and_computers;

	/* Variable for the playlist */
	boost::shared_ptr<Playlist_Container> playlist;

	/* Variable for the group_playlist container */
	boost::shared_ptr<Group_Playlist_Container> group_playlist;
};
}
#endif	/* ARTEMIS_REQUEST_HANDLER_H */

