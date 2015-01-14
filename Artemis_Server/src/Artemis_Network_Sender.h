#ifndef ARTEMIS_NETWORK_SENDER_H
#define ARTEMIS_NETWORK_SENDER_H

#include "Artemis_Network_Sender_Connection.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>
namespace Artemis
{

class Artemis_Network_Sender: boost::noncopyable
{
public:
	/** Starts a connection */
	Artemis_Network_Sender(size_t p_thread_pool_size, boost::asio::io_service*);

	~Artemis_Network_Sender(void);

	/** Begins the write operation of the xml_string provided */
	void write(std::string xml_string);
	void connect(std::string dest_ip, std::string xml_string);
	void set_parameters(std::map<std::string, std::string> &p_parameters);
	/** Closes the socket */
	void close();

	void run();
	boost::asio::io_service *m_io_service;
private:
	std::map<std::string, std::string> parameters;
	/** The number of threads that will call io_service::run()*/
	std::size_t thread_pool_size;
	/** The signal_set is used to register process termination notifications */
	boost::asio::signal_set signals;

	Artemis_Network_Sender_Connection_ptr new_connection;

	void handle_stop();
	void handle_connect(const boost::system::error_code&, std::string);
	void do_write(std::string xml_string);
	void handle_write(const boost::system::error_code& error);
	void do_close();
};
}
#endif // ARTEMIS_NETWORK_SENDER_H
