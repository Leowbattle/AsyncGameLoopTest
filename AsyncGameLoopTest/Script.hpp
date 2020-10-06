#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <vector>
#include <optional>
#include <experimental/coroutine>

class CancellationToken {
public:
	CancellationToken() : internal(std::make_shared<Internal>()) {}

	void cancel() {
		internal->cancelled = true;
	}

	bool cancelled() {
		return internal->cancelled;
	}

	operator bool() {
		return cancelled();
	}

private:
	struct Internal {
		bool cancelled;
	};

	std::shared_ptr<Internal> internal;
};

template <typename T = void>
class Script {
public:
	Script() {}

	struct promise_type {
		Script get_return_object() {
			ScriptHandle promise = ScriptHandle::from_promise(*this);
			return Script(promise);
		}

		bool initial_suspend() {
			return false;
		}

		bool final_suspend() {
			return true;
		}

		void return_value(T value) {
			result = value;

			for (std::experimental::coroutine_handle<> coro : waiters) {
				coro.resume();
			}
			waiters.clear();
		}

		std::vector<std::experimental::coroutine_handle<>> waiters;
		std::optional<T> result;
	};

	bool await_ready() {
		return handle.done();
	}

	void await_suspend(std::experimental::coroutine_handle<> coro) {
		handle.promise().waiters.push_back(coro);
	}

	T await_resume() {
		return handle.promise().result.value();
	}

private:
	using ScriptHandle = std::experimental::coroutine_handle<promise_type>;

	explicit Script(ScriptHandle handle) : handle(handle) {}

	ScriptHandle handle;
};

template <>
class Script<void> {
public:
	Script() {}

	struct promise_type {
		Script get_return_object() {
			ScriptHandle promise = ScriptHandle::from_promise(*this);
			return Script(promise);
		}

		bool initial_suspend() {
			return false;
		}

		bool final_suspend() {
			return true;
		}

		void return_void() {
			for (std::experimental::coroutine_handle<> coro : waiters) {
				coro.resume();
			}
			waiters.clear();
		}

		std::vector<std::experimental::coroutine_handle<>> waiters;
	};

	bool await_ready() {
		return handle.done();
	}

	void await_suspend(std::experimental::coroutine_handle<> coro) {
		handle.promise().waiters.push_back(coro);
	}

	void await_resume() {}

private:
	using ScriptHandle = std::experimental::coroutine_handle<promise_type>;

	explicit Script(ScriptHandle handle) : handle(handle) {}

	ScriptHandle handle;
};

#endif