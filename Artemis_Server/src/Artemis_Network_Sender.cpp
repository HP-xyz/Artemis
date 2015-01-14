#include "Artemis_Network_Sender.h"
namespace Artemis
{
Artemis_Network_Sender::Artemis_Network_Sender(size_t p_thread_pool_size,
		boost::asio::io_service *_io_service) :
		signals(*_io_service)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::Artemis_Network_Sender()"
			<< std::endl;
#endif // _DEBUG
	thread_pool_size = p_thread_pool_size;
	m_io_service = _io_service;
	signals.add(SIGINT);
	signals.add(SIGTERM);
#if defined(SIGQUIT)
	signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals.async_wait(boost::bind(&Artemis_Network_Sender::handle_stop, this));
}

void Artemis_Network_Sender::set_parameters(
		std::map<std::string, std::string> &p_parameters)
{
	parameters = parameters;
}

void Artemis_Network_Sender::connect(std::string dest_ip,
		std::string xml_string)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::connect()" << std::endl;
	std::cout << " - Creating resolver" << std::endl;
#endif // _DEBUG
	boost::asio::ip::tcp::resolver resolver(*m_io_service);
#ifdef _DEBUG
	std::cout << " - Creating query" << std::endl;
#endif //_DEBUG
	boost::asio::ip::tcp::resolver::query query(dest_ip, "9001");
#ifdef _DEBUG
	std::cout << " - Creating endpoint_iterator" << std::endl;
#endif //_DEBUG
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
			resolver.resolve(query);
#ifdef _DEBUG
	std::cout << " - Resetting new connection" << std::endl;
#endif //_DEBUG
	new_connection.reset(
			new Artemis_Network_Sender_Connection(*m_io_service, parameters));
#ifdef _DEBUG
	std::cout << " - Calling async_connect" << std::endl;
#endif //_DEBUG
	boost::asio::async_connect(new_connection->socket(), endpoint_iterator,
			boost::bind(&Artemis_Network_Sender::handle_connect, this,
					boost::asio::placeholders::error, xml_string));
}

Artemis_Network_Sender::~Artemis_Network_Sender(void)
{
}

void Artemis_Network_Sender::run()
{
	std::cout << " =Artemis_Network_Sender::run()" << std::endl
			<< " ======================" << std::endl;
	std::cout << " - Tread pool size: " << thread_pool_size << std::endl;
	//Create a pool of threads to run all of the io_services
	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < Artemis_Network_Sender::thread_pool_size; ++i)
	{
		boost::shared_ptr<boost::thread> thread(
				new boost::thread(
						boost::bind(&boost::asio::io_service::run,
								m_io_service)));
		threads.push_back(thread);
	}

	std::cout << " - Created threads" << std::endl;

	// Wait for all threads in pool to exit
	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();

	std::cout << " - Joined Threads" << std::endl;
}

void Artemis_Network_Sender::do_write(std::string xml_string)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::do_write()" << std::endl;
#endif // _DEBUG
	//boost::asio::async_write(m_socket,
	//                       boost::asio::buffer(xml_string, xml_string.size()),
	//                     boost::bind(&Artemis_Network_Sender::handle_write, this,
	//                               boost::asio::placeholders::error));
}

void Artemis_Network_Sender::handle_write(
		const boost::system::error_code& error)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::handle_write()" << std::endl;
#endif // _DEBUG
	if (!error)
	{
#ifdef _DEBUG
		std::cout << " - Sent successfully" << std::endl;
#endif // _DEBUG
		do_close();
	}
	else
	{
		std::cerr << "Error: " << error.message() << std::endl;
		do_close();
	}
}

void Artemis_Network_Sender::do_close()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::do_close()" << std::endl;
#endif // _DEBUG
//    m_socket.close();
}

void Artemis_Network_Sender::write(const std::string xml_string)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::write()" << std::endl;
	std::cout << " - xml_String = " << xml_string << std::endl;
#endif // _DEBUG
	m_io_service->post(
			boost::bind(&Artemis_Network_Sender::do_write, this, xml_string));
}

void Artemis_Network_Sender::close()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::close()" << std::endl;
#endif // _DEBUG
	m_io_service->post(boost::bind(&Artemis_Network_Sender::do_close, this));
}

void Artemis_Network_Sender::handle_connect(
		const boost::system::error_code &error, std::string xml_string)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::handle_connect()" << std::endl;
#endif // _DEBUG
	if (!error)
	{
		new_connection->start_write(xml_string);
	}
	else
	{
		std::cerr << "Error: " << error.message() << std::endl;
	}
}
void Artemis_Network_Sender::handle_stop()
{
	m_io_service->stop();
}
}

