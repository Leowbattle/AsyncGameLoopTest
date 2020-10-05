#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>
#include <optional>
#include <experimental/coroutine>

template <typename T = void>
class Event {
public:
	Event() : internal(std::make_shared<EventInternal>()) {
		
	}

	void setResult(T value) {
		std::shared_ptr<EventInternal> old = internal;
		internal = std::make_shared<EventInternal>();
		old->setResult(value);
	}

	bool await_ready() {
		return internal->result.has_value() && !internal->used;
	}

	void await_suspend(std::experimental::coroutine_handle<> coro) {
		internal->waiters.push_back(coro);
	}

	T await_resume() {
		return internal->result.value();
	}

private:
	class EventInternal {
		friend class Event;

		bool used = false;
		std::optional<T> result;
		std::vector<std::experimental::coroutine_handle<>> waiters;

		void setResult(T value) {
			used = true;
			result = value;

			for (std::experimental::coroutine_handle<> waiter : waiters) {
				waiter.resume();
			}
		}
	};

	std::shared_ptr<EventInternal> internal;
};

template <>
class Event<void> {
public:
	Event() : internal(std::make_shared<EventInternal>()) {

	}

	void setResult() {
		std::shared_ptr<EventInternal> old = internal;
		internal = std::make_shared<EventInternal>();
		//printf("allocate ");
		old->setResult();
	}

	bool await_ready() {
		return internal->ready && !internal->used;
	}

	void await_suspend(std::experimental::coroutine_handle<> coro) {
		internal->waiters.push_back(coro);
	}

	void await_resume() {
		
	}

private:
	class EventInternal {
		friend class Event;

		bool used = false;
		bool ready = false;
		std::vector<std::experimental::coroutine_handle<>> waiters;

		void setResult() {
			used = true;
			ready = true;

			for (std::experimental::coroutine_handle<> waiter : waiters) {
				waiter.resume();
			}
		}
	};

	std::shared_ptr<EventInternal> internal;
};

#endif