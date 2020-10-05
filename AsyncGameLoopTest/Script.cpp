#include "Script.hpp"

Script::~Script() {
	if (handle) {
		handle.destroy();
	}
}

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
	
}