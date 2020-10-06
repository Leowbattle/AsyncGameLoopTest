#include "Script.hpp"

Script::Script(ScriptHandle handle) : handle(handle) {}

Script Script::promise_type::get_return_object() {
	ScriptHandle promise = ScriptHandle::from_promise(*this);
	return Script(promise);
}

bool Script::promise_type::initial_suspend() {
	return false;
}

bool Script::promise_type::final_suspend() {
	return false;
}

void Script::promise_type::return_void() {
	for (std::experimental::coroutine_handle<> coro : waiters) {
		coro.resume();
	}
	waiters.clear();
}

bool Script::await_ready() {
	return handle.done();
}

void Script::await_suspend(std::experimental::coroutine_handle<> coro) {
	handle.promise().waiters.push_back(coro);
}

void Script::await_resume() {

}