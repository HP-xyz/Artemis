#ifndef ARTEMIS_NETWORK_SENDER_CONNECTION_H
#define ARTEMIS_NETWORK_SENDER_CONNECTION_H
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
#include <pugixml.hpp>
#include <exception>
#include <vector>
#include <list>
#include <iostream>

namespace Artemis
{
class Artemis_Network_Sender_Connection: public boost::enable_shared_from_this<
		Artemis_Network_Sender_Connection>, private boost::noncopyable

{
public:
	explicit Artemis_Network_Sender_Connection(
			boost::asio::io_service &io_service,
			std::map<std::string, std::string>&, std::string _xml_string);
	virtual ~Artemis_Network_Sender_Connection();

	void connect(std::string dest_ip, std::string dest_port);
	void start_write();
	void do_close();
private:
	/** Handle completion of a write operation */
	void handle_write(const boost::system::error_code& error, std::size_t);

	/** Strand to ensure the connections handlers are not called concurrently */
	boost::asio::io_service::strand strand;
	/** Socket for the connection */
	boost::asio::ip::tcp::socket m_socket;

	std::map<std::string, std::string> parms;
	std::string xml_string;
	unsigned long long sent_buffer_count;
	unsigned long long start_index;
	unsigned long long bytes_sent;
	void do_write();
	void do_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code &error);
};
typedef boost::shared_ptr<Artemis_Network_Sender_Connection> Artemis_Network_Sender_Connection_ptr;
}
#endif // ARTEMIS_NETWORK_SENDER_CONNECTION_H
