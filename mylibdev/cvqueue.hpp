#pragma once
#include <condition_variable>
#include <mutex>
#include <atomic>


namespace util
{
	class queued_condition_variable : public std::condition_variable
	{
	private:
		std::mutex _mtx;
		std::atomic<std::size_t> _notifies = 0;
		std::condition_variable _cv;
	public:
		void queued_wait(std::unique_lock<std::mutex>& _Lck)
		{
			{
				//Ensure that the queued notify isn't called
				std::lock_guard<std::mutex> lk(_mtx);
				//If there are queued notifies
				if (_notifies > 0)
				{
					//Consume the notify by not waiting
					_notifies--;
					return;
				}
			}
			//If the function did not return, we wait
			this->wait(_Lck);
			//We've been woken, notify consumed
			_notifies--;
		}

		void queued_notify_one() 
		{
			{
				//Lock, no waiting so no deadlock
				std::lock_guard<std::mutex> lk(_mtx);
				//Increment notify count
				_notifies++;
				//Long story short, if the notifier before was 0, that could indicate that a thread was waiting
				//The standard notify is not queued so the wait above will exit and the notifies will be decremented there if needed
				//If the queued_wait method does not wait the next queued_wait will simply just skip the next wait
				if ((_notifies - 1) == 0)
					this->notify_one();
			}
		}

		void set_notifications(std::size_t _nt)
		{
			//Atomic variable
			_notifies = _nt;
		}

		std::size_t get_notifications()
		{
			return _notifies.load();
		}
	};
}