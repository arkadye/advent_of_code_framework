#pragma once

namespace utils
{

	template <typename LockType>
	class shared_lock_guard
	{
		LockType& lock;
	public:
		explicit shared_lock_guard(LockType& init)
			: lock{ init }
		{
			lock.lock_shared();
		}
		~shared_lock_guard() noexcept
		{
			lock.unlock_shared();
		}
	};

}