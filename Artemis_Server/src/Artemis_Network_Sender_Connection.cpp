#include "Artemis_Network_Sender_Connection.h"
namespace Artemis
{
Artemis_Network_Sender_Connection::Artemis_Network_Sender_Connection(
		boost::asio::io_service &io_service,
		std::map<std::string, std::string>& parameters, std::string _xml_string) :
		strand(io_service), m_socket(io_service)
{
	xml_string = _xml_string;
	sent_buffer_count = 0;
	start_index = 0;
	bytes_sent = 0;
}

Artemis_Network_Sender_Connection::~Artemis_Network_Sender_Connection()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender_Connection::"
			<< "~Artemis_Network_Sender_Connection()" << std::endl;
#endif // _DEBUG
}
/** @brief start_write
 * Starts a async write connection to the target system
 */
void Artemis_Network_Sender_Connection::start_write()
{
#ifdef _DEBUG
	std::cout << " =Artemis_Network_Sender_Connection::start_write()"
			<< std::endl;
#endif //_DEBUG
	m_socket.get_io_service().post(
			boost::bind(&Artemis_Network_Sender_Connection::do_write, this));
	//m_socket.get_io_service().run();
}
void Artemis_Network_Sender_Connection::do_write()
{
#ifdef _DEBUG
	std::cout << " =Artemis_Network_Sender_Connection::do_write()" << std::endl;
	std::cout << " - Writing size: " << xml_string.size() << std::endl;
#endif //_DEBUG
	boost::asio::async_write(m_socket,
			boost::asio::buffer(xml_string.c_str(), xml_string.size()),
			boost::bind(&Artemis_Network_Sender_Connection::handle_write, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}
void Artemis_Network_Sender_Connection::handle_write(
		const boost::system::error_code& error, std::size_t bytes_transferred)
{
#ifdef _DEBUG
	std::cout << " == Handle Write" << std::endl << " ==============="
			<< std::endl;
	std::cout << " - Bytes_Transferred: " << bytes_transferred << std::endl;
#endif
	if (error)
	{
		std::cerr << " - Error encountered: " << error.message() << std::endl;
	}
#ifdef _DEBUG
	std::cout << " - Closing" << std::endl;
#endif // _DEBUG
	//Initiate graceful connection closure.
	boost::system::error_code ignored_ec;
	Artemis_Network_Sender_Connection::m_socket.shutdown(
			boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

	// No new asynchronous operations are started. This means that all shared_ptr
	// references to the connection object will disappear and the object will be
	// destroyed automatically after this handler returns. The connection class's
	// destructor closes the socket.
}
void Artemis_Network_Sender_Connection::connect(std::string dest_ip,
		std::string dest_port)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender_Connection::connect()" << std::endl;
	std::cout << " - Creating resolver" << std::endl;
#endif // _DEBUG
	boost::asio::ip::tcp::resolver resolver(m_socket.get_io_service());
#ifdef _DEBUG
	std::cout << " - Creating query" << std::endl;
#endif //_DEBUG
	boost::asio::ip::tcp::resolver::query query(dest_ip, dest_port);
#ifdef _DEBUG
	std::cout << " - Creating endpoint_iterator" << std::endl;
#endif //_DEBUG
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
			resolver.resolve(query);
#ifdef _DEBUG
	std::cout << " - Calling async_connect" << std::endl;
#endif //_DEBUG
	m_socket.get_io_service().post(
			boost::bind(&Artemis_Network_Sender_Connection::do_connect, this,
					endpoint_iterator));
	//m_socket.get_io_service().run();
}
void Artemis_Network_Sender_Connection::do_connect(
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender_Connection::do_connect()"
			<< std::endl;
#endif // _DEBUG
	boost::asio::async_connect(m_socket, endpoint_iterator,
			boost::bind(&Artemis_Network_Sender_Connection::handle_connect,
					this, boost::asio::placeholders::error));
}
void Artemis_Network_Sender_Connection::handle_connect(
		const boost::system::error_code &error)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender_Connection::handle_connect()"
			<< std::endl;
#endif // _DEBUG
	if (!error)
	{
#ifdef _DEBUG
		std::cout << " - Connected without error" << std::endl;
#endif //_DEBUG
	}
	else
	{
		std::cerr << "Error: " << error.message() << std::endl;
	}
}
void Artemis_Network_Sender_Connection::do_close()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender_Connection::do_close()" << std::endl;
#endif // _DEBUG
	m_socket.close();
}
}
