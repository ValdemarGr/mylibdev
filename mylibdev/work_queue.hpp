#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <mutex>

namespace util
{
	template<typename _T>
	class work_queue
	{
	private:
		std::shared_ptr<std::vector<std::shared_ptr<_T>>> _ptr;
		std::mutex _mtx;
	public:
		work_queue();
		~work_queue();

		std::shared_ptr<std::vector<std::shared_ptr<_T>>> extract();
		void insert(std::shared_ptr<_T>);
	};

	template<typename _T>
	inline work_queue<_T>::work_queue()
		: _ptr(std::make_shared<std::vector<std::shared_ptr<_T>>>())
	{
	}

	template<typename _T>
	inline work_queue<_T>::~work_queue()
	{
	}

	template<typename _T>
	inline std::shared_ptr<std::vector<std::shared_ptr<_T>>> work_queue<_T>::extract()
	{
		std::lock_guard<std::mutex> grd(_mtx);
		std::shared_ptr<std::vector<std::shared_ptr<_T>>> temp(_ptr);

		_ptr = std::make_shared<std::vector<std::shared_ptr<_T>>>();

		return temp;
	}

	template<typename _T>
	inline void work_queue<_T>::insert(std::shared_ptr<_T> insertion_ptr)
	{
		std::lock_guard<std::mutex> grd(_mtx);
		_ptr->push_back(insertion_ptr);
	}
}