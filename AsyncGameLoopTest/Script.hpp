#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <experimental/coroutine>

class Script {
public:
	Script(const Script&) = delete;

	~Script();

	struct promise_type {
		Script get_return_object();

		bool initial_suspend();
		bool final_suspend();

		void return_void();
	};

private:
	using ScriptHandle = std::experimental::coroutine_handle<promise_type>;

	explicit Script(ScriptHandle handle);

	ScriptHandle handle;
};

#endif