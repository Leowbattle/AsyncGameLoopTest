#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <vector>
#include <experimental/coroutine>

class Script {
public:
	Script(const Script&) = delete;

	struct promise_type {
		Script get_return_object();

		bool initial_suspend();
		bool final_suspend();

		void return_void();

		std::vector<std::experimental::coroutine_handle<>> waiters;
	};

	bool await_ready();
	void await_suspend(std::experimental::coroutine_handle<> coro);
	void await_resume();

private:
	using ScriptHandle = std::experimental::coroutine_handle<promise_type>;

	explicit Script(ScriptHandle handle);

	ScriptHandle handle;
};

#endif