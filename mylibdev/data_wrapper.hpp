#pragma once


#include <iostream>
#include <vector>
#include <cstring>

namespace util
{
	///Packet data wrapper
	/*
	 * Leftmost and/or first will be the first element, in both extraction and insertion.
	 *
	 * @example:
	 *
	 * Input:
	 *	Chain calling: 
	 *		packet << first << second << n;
	 * 
	 *	Multiline calling:
	 *		packet << first;
	 *		packet << second
	 *		packet << n;
	 * 
	 * Output:
	 *	Chain calling:
	 *		packet >> n >> second >> first;
	 *	
	 *	Multiline calling:
	 *		packet >> n;
	 *		packet >> second;
	 *		packet >> first;
	*/
	class data_wrapper
	{
	private:
		std::size_t _current_element = header_length;
		std::vector<char> _data;

	public:
		enum
		{
			//Header contains the following:
			/// Amount of bytes of the data,
			/// Amount of different items
			header_length = sizeof(std::size_t),
			start_size = 32
		};

		data_wrapper();
		~data_wrapper();

		void update_header();

		void reset_iterator();

		char* get_buffer();
		std::vector<char>& get_vector();

		std::size_t get_len();

		template<typename _T>
		data_wrapper& operator<<(const _T&);

		template<typename _T>
		data_wrapper& operator>>(_T&);
	};

	template<typename _T>
	inline data_wrapper & data_wrapper::operator<<(const _T& type_arg)
	{
		if (this->_data.size() < this->_current_element + sizeof(_T))
		{
			std::size_t diff = this->_current_element + sizeof(_T);
			this->_data.resize(this->_data.size() + diff + 16);
		}
		else
		{
			//Here the dest is the data buffer, the source is the type and the size is the size of the type
			std::memcpy(this->_data.data() + this->_current_element, &type_arg, sizeof(_T));
			//Move the "data iterator" forward so the next data can be insereted
			this->_current_element += sizeof(_T);

			//Update header
			std::memcpy(this->_data.data(), &this->_current_element, sizeof(std::size_t));

#ifdef _DEBUG_DATA_WRAPPER
			std::cout << "Len in var: " << this->_current_element << std::endl;
			std::cout << "Len in buf: " << *((std::size_t*)this->_data.data()) << std::endl;
#endif // _DEBUG_DATA_WRAPPER
		}

		return *this;
	}

	template<typename _T>
	inline data_wrapper & data_wrapper::operator>>(_T& type_arg)
	{
		if (header_length <= this->_current_element - sizeof(_T))
		{
			//Move the "data iterator" back so the earlier data can be extracted
			this->_current_element -= sizeof(_T);
			//Here the dest is the provided argument, the source is the data buffer and the size is the size of the type
			std::memcpy(&type_arg, this->_data.data() + this->_current_element, sizeof(_T));

			//Update header
			std::memcpy(this->_data.data(), &this->_current_element, sizeof(std::size_t));

#ifdef _DEBUG_DATA_WRAPPER
			std::cout << "Len in var: " << this->_current_element << std::endl;
			std::cout << "Len in buf: " << *((std::size_t*)this->_data.data()) << std::endl;
#endif // _DEBUG_DATA_WRAPPER
		}

		return *this;
	}

	inline void data_wrapper::update_header()
	{
#ifdef _DEBUG_DATA_WRAPPER
		std::cout << "Head: " << this->_current_element << std::endl;
#endif // _DEBUG_DATA_WRAPPER

		std::memcpy(&this->_current_element, this->_data.data(), sizeof(std::size_t));

		if (this->_data.size() < this->_current_element)
		{
			this->_data.resize(this->_data.size() + this->_current_element + 16);
		}

#ifdef _DEBUG_DATA_WRAPPER
		std::cout << "New head: " << this->_current_element << std::endl;
#endif // _DEBUG_DATA_WRAPPER

	}

	inline void data_wrapper::reset_iterator()
	{
		this->_current_element = header_length;
		std::memcpy(this->_data.data(), &this->_current_element, sizeof(std::size_t));
	}

	inline data_wrapper::data_wrapper()
	{
		this->_data.resize(start_size);
	}

	inline data_wrapper::~data_wrapper()
	{
	}

	inline char* data_wrapper::get_buffer()
	{
		return this->_data.data();
	}

	inline std::vector<char>& data_wrapper::get_vector()
	{
		return this->_data;
	}

	inline std::size_t data_wrapper::get_len()
	{
		return this->_current_element;
	}
}
