#pragma once

#include <functional>
#include <boost\asio.hpp>
#include <mylib\data_wrapper.hpp>

namespace util
{
	typedef boost::asio::ip::tcp tcp;

	class tcp_client
	{
	private:
		tcp::socket _sock;
		tcp::resolver _resolv;
	public:
		tcp_client(boost::asio::io_service&, tcp::endpoint&, std::function<void()>);
		tcp_client(boost::asio::io_service&, tcp::endpoint&);
		~tcp_client();

		void send_data(util::data_wrapper&);
		void receive_data(util::data_wrapper&);

		void send_data_async(util::data_wrapper&);
		void receive_data_async(util::data_wrapper&);

		template<typename _LAMBDA>
		void send_data_async(util::data_wrapper&, _LAMBDA&);
		template<typename _LAMBDA>
		void receive_data_async(util::data_wrapper&, _LAMBDA&);

		void send_data_async(util::data_wrapper&, std::function<void()>&);
		void receive_data_async(util::data_wrapper&, std::function<void()>&);
	};

	inline tcp_client::tcp_client(boost::asio::io_service& service_arg, tcp::endpoint& dest_arg, std::function<void()> function_arg)
		: _sock(service_arg), _resolv(service_arg)
	{
		auto handler = [this, function_arg](boost::system::error_code _e, boost::asio::ip::tcp::resolver::iterator it) {
			if (!_e)
			{
				function_arg();
			}
		};

		boost::asio::async_connect(this->_sock, this->_resolv.resolve(dest_arg), handler);
	}

	inline tcp_client::tcp_client(boost::asio::io_service& service_arg, tcp::endpoint& dest_arg)
		: _sock(service_arg), _resolv(service_arg)
	{
		boost::asio::connect(this->_sock, this->_resolv.resolve(dest_arg));
	}

	inline tcp_client::~tcp_client()
	{
	}

	inline void tcp_client::send_data(util::data_wrapper& pack_arg)
	{
		boost::asio::write(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), pack_arg.get_len()));
	}

	inline void tcp_client::receive_data(util::data_wrapper& pack_arg)
	{
		boost::asio::read(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), util::data_wrapper::header_length));

		pack_arg.update_header();

		boost::asio::read(this->_sock, boost::asio::buffer(pack_arg.get_buffer() + util::data_wrapper::header_length, pack_arg.get_len() - util::data_wrapper::header_length));
	}

	inline void tcp_client::send_data_async(util::data_wrapper& pack_arg)
	{
		auto handler = [this](boost::system::error_code _e, std::size_t _len) {
			if (!_e)
			{

			}
		};

		boost::asio::async_write(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), pack_arg.get_len()), handler);
	}

	inline void tcp_client::receive_data_async(util::data_wrapper& pack_arg)
	{
		auto handler = [this](boost::system::error_code _e, std::size_t _len) {
			if (!_e)
			{

			}
		};

		boost::asio::async_read(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), util::data_wrapper::header_length), handler);

		pack_arg.update_header();

		boost::asio::async_read(this->_sock, boost::asio::buffer(pack_arg.get_buffer() + util::data_wrapper::header_length, pack_arg.get_len() - util::data_wrapper::header_length), handler);
	}

	template<typename _LAMBDA>
	inline void tcp_client::send_data_async(util::data_wrapper& pack_arg, _LAMBDA& handler)
	{
		boost::asio::async_write(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), pack_arg.get_len()), handler);
	}

	template<typename _LAMBDA>
	inline void tcp_client::receive_data_async(util::data_wrapper& pack_arg, _LAMBDA& handler)
	{
		boost::asio::async_read(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), util::data_wrapper::header_length), handler);

		pack_arg.update_header();

		boost::asio::async_read(this->_sock, boost::asio::buffer(pack_arg.get_buffer() + util::data_wrapper::header_length, pack_arg.get_len() - util::data_wrapper::header_length), handler);
	}

	inline void tcp_client::send_data_async(util::data_wrapper& pack_arg, std::function<void()>& handler)
	{
		auto handle = [&,this](boost::system::error_code _e, std::size_t _len) {
			if (!_e)
			{
				handler();
			}
		};

		boost::asio::async_write(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), pack_arg.get_len()), handle);
	}

	inline void tcp_client::receive_data_async(util::data_wrapper& pack_arg, std::function<void()>& handler)
	{
		auto handle_empty = [&, this](boost::system::error_code _e, std::size_t _len) {
			if (!_e)
			{
			}
		};

		auto handle = [&, this](boost::system::error_code _e, std::size_t _len) {
			if (!_e)
			{
				handler();
			}
		};

		boost::asio::async_read(this->_sock, boost::asio::buffer(pack_arg.get_buffer(), util::data_wrapper::header_length), handle_empty);

		pack_arg.update_header();

		boost::asio::async_read(this->_sock, boost::asio::buffer(pack_arg.get_buffer() + util::data_wrapper::header_length, pack_arg.get_len() - util::data_wrapper::header_length), handle);
	}
}