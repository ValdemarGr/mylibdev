#pragma once

#define _DEVMODE 0

#include <vector>
#include <utility>
#include <memory>
#include <functional>

#include <boost\asio.hpp>

#if _DEVMODE
#include "data_wrapper.hpp"
#else
#include <mylibdev\data_wrapper.hpp>
#endif

namespace util
{
	class tcp_server : std::enable_shared_from_this<tcp_server>
	{
	public:
		class client;
	private:
		std::vector<std::shared_ptr<util::tcp_server::client>> _client_vect;

		boost::asio::ip::tcp::acceptor _acceptor;
		boost::asio::ip::tcp::socket _client_sock;
	public:
		tcp_server(boost::asio::io_service& service, unsigned short port);
		~tcp_server(){}

		void accept();

		boost::asio::ip::tcp::socket& _get_sock() { return this->_client_sock; }
		boost::asio::ip::tcp::acceptor& _get_acceptor() { return this->_acceptor; }

		std::vector<std::shared_ptr<util::tcp_server::client>>& get_clients() { return this->_client_vect; }

		std::function<void(std::reference_wrapper<util::tcp_server::client>)> _accept_handle = [](util::tcp_server::client&) {};

		std::function<void(std::reference_wrapper<util::tcp_server::client>)> _read_handle = [](util::tcp_server::client&) {};
		std::function<void(std::reference_wrapper<util::tcp_server::client>)> _write_handle = [](util::tcp_server::client&) {};
	};

	class tcp_server::client
	{
	private:
		boost::asio::ip::tcp::socket _sock;
	public:
		std::size_t _id = 0;
		std::vector<char> _dat_vect;
		std::size_t* _len_ptr;
		std::shared_ptr<util::tcp_server> _parent_ptr;

		client(boost::asio::ip::tcp::socket&, std::shared_ptr<tcp_server>, std::size_t);
		~client(){}

		void read();

		void write(client&);
		void write(std::string&);
		void write(util::data_wrapper&);

		char* get_data();
		std::size_t get_data_length();
		util::data_wrapper get_wrapper();
	};

	inline util::tcp_server::tcp_server(boost::asio::io_service & service, unsigned short port)
		: _acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _client_sock(service)
	{
	}

	inline void util::tcp_server::accept()
	{
		auto handler = [this](boost::system::error_code _ec) {
			if (!_ec)
			{
				auto _client(std::make_shared<util::tcp_server::client>(std::move(this->_client_sock), std::shared_ptr<tcp_server>(this), get_clients().size()));
				get_clients().push_back(_client);

				_accept_handle(*_client);

				accept();
			}
		};

		this->_acceptor.async_accept(this->_client_sock, handler);
	}

	inline util::tcp_server::client::client(boost::asio::ip::tcp::socket& socket, std::shared_ptr<tcp_server> parent_ptr, std::size_t id)
		: _sock(std::move(socket)), _parent_ptr(parent_ptr), _id(id)
	{
		_dat_vect.resize(16);

		_len_ptr = (std::size_t*)_dat_vect.data();
		*_len_ptr = sizeof(std::size_t);

		read();
	}

	inline void util::tcp_server::client::read()
	{
		auto handle = [this](boost::system::error_code _ec, std::size_t _len) {
			if (!_ec)
			{
				auto handler = [this](boost::system::error_code _ec, std::size_t _len) {
					if (!_ec)
					{
						_parent_ptr->_read_handle(*this);

						read();
					}
				};

				_len_ptr = (std::size_t*)_dat_vect.data();

				if (*_len_ptr < _dat_vect.size())
				{
					std::size_t temp_len = *_len_ptr;
					_dat_vect.resize(*_len_ptr + 2);
					_len_ptr = (std::size_t*)_dat_vect.data();
					*_len_ptr = temp_len;
				}


				boost::asio::async_read(_sock, boost::asio::buffer(_dat_vect.data() + sizeof(std::size_t), *_len_ptr - sizeof(std::size_t)), handler);
			}
		};

		boost::asio::async_read(_sock, boost::asio::buffer(_dat_vect.data(), sizeof(std::size_t)), handle);
	}

	inline void util::tcp_server::client::write(client& _client)
	{
		_len_ptr = (std::size_t*)_dat_vect.data();
		if (4 < *_len_ptr)
		{
			auto handler_base = [this](boost::system::error_code _ec, std::size_t _len) {
				if (!_ec)
				{
					_parent_ptr->_write_handle(*this);
				}
			};

			boost::asio::async_write(_sock, boost::asio::buffer(_client._dat_vect.data(), *_len_ptr), handler_base);
		}
	}

	inline void util::tcp_server::client::write(std::string& _str)
	{
		_len_ptr = (std::size_t*)_dat_vect.data();
		if (4 < *_len_ptr)
		{
			char* buf = new char[_str.size() + util::data_wrapper::header_length];
			std::size_t* tmp_len_ptr = (std::size_t*)buf;

			*tmp_len_ptr = _str.size() + sizeof(std::size_t);

			for (size_t i = 0; i < _str.size(); i++)
			{
				*(buf + sizeof(std::size_t) + i) = _str[i];
			}

			auto handler = [this, buf](boost::system::error_code _ec, std::size_t _len) {
				if (!_ec)
				{
					_parent_ptr->_write_handle(*this);

					delete[] buf;
				}
				else 
					delete[] buf;
			};

			boost::asio::async_write(_sock, boost::asio::buffer(buf, *tmp_len_ptr), handler);
		}
	}

	inline void util::tcp_server::client::write(util::data_wrapper& _dat)
	{
		auto handler = [this](boost::system::error_code _e, std::size_t _len) {
			if (!_e)
			{
				_parent_ptr->_write_handle(*this);
			}
		};

		boost::asio::async_write(this->_sock, boost::asio::buffer(_dat.get_buffer(), _dat.get_len()), handler);
	}

	inline char* util::tcp_server::client::get_data()
	{
		return _dat_vect.data() + sizeof(std::size_t);
	}

	inline std::size_t util::tcp_server::client::get_data_length()
	{
		return ((*_len_ptr) - sizeof(std::size_t));
	}

	inline util::data_wrapper util::tcp_server::client::get_wrapper()
	{
		util::data_wrapper return_wrapper;

		for (size_t i = 0; i < *_len_ptr; i++)
			return_wrapper.get_vector()[i] = *(this->_dat_vect.data() + i);
		return_wrapper.update_header();

		return return_wrapper;
	}
}