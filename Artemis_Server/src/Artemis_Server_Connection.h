/*
 * File:   Artemis_Server_Connection.h
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:30 PM
 */

#ifndef ARTEMIS_SERVER_CONNECTION_H
#define	ARTEMIS_SERVER_CONNECTION_H
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/logic/tribool.hpp>
#include "boost/tuple/tuple.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/shared_array.hpp>
#include <pugixml.hpp>
#include <b64/encode.h>
#include <exception>
#include <fstream>
#include <vector>
#include <utility>
#include "Artemis_Request_Handler.h"
#include "Group_Container.h"
#include "Playlist_Container.h"
#include "Group_Playlist_Container.h"

namespace Artemis
{

class Artemis_Server_Connection: public boost::enable_shared_from_this<
		Artemis_Server_Connection>, private boost::noncopyable

{
public:

	/** Constructs a connection withe the given io_service */
	explicit
	Artemis_Server_Connection(boost::asio::io_service& io_service,
			std::map<std::string, std::string>&,
			boost::shared_ptr<Group_Container> p_groups_and_computers,
			boost::shared_ptr<Playlist_Container> p_playlist,
			boost::shared_ptr<Group_Playlist_Container> p_group_playlist);

	/** Get the socket associated with the connection*/
	boost::asio::ip::tcp::socket&
	socket();
	/** Start the first asynchronous operation for the connection */
	void
	start();
private:
	/** Handle completion of a read operation */
	void
	handle_read(const boost::system::error_code& error,
			std::size_t bytes_transferred);
	/** Handle completion of a write operation */
	void
	handle_write(const boost::system::error_code& error,
			std::vector<std::vector<char> >&, std::size_t);
	void
	handle_write(const boost::system::error_code& error,
			std::size_t bytes_transferred);
	/** Strand to ensure the connections handlers are not called concurrently */
	boost::asio::io_service::strand strand;
	/** Socket for the connection */
	boost::asio::ip::tcp::socket m_socket;
	/** Buffer for incoming data */
	boost::asio::streambuf buffer;

	std::map<std::string, std::string> parms;

	long long received_size;

	/* Variable for computer names and group names
	 * Note: Format is groups_and_computers[group_index][computer_index]
	 * Note: This can be hostnames or IP addresses */
	boost::shared_ptr<Group_Container> groups_and_computers;

	/* Variable for the playlist */
	boost::shared_ptr<Playlist_Container> playlist;

	/* Variable for the group_playlist container */
	boost::shared_ptr<Group_Playlist_Container> group_playlist;

	std::string
	get_binary_file(std::string filename);
};
typedef boost::shared_ptr<Artemis_Server_Connection> Artemis_Server_Connection_ptr;
}
#endif	/* ARTEMIS_SERVER_CONNECTION_H */

