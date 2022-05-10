#include "Channel.h"

template<class T>
Channel<T>::Channel() : state_changed(false) {
}

template<class T>
bool Channel<T>::has_changed() {
  return state_changed;
}

template<class T>
void Channel<T>::reset() {
  std::unique_lock<std::mutex> lock(state_mutex);
  state.reset();
  state_changed = true;
  on_update.notify_all();
}

template<class T>
void Channel<T>::push(T value) {
  {
    std::unique_lock<std::mutex> lock(state_mutex);

    if (auto cur_value = state)
      if (*cur_value == value)
        return;

    state = std::make_shared<T>(std::move(value));
  };

  state_changed = true;
  on_update.notify_all();
}

template<class T>
std::shared_ptr<T> Channel<T>::pull() {
  std::unique_lock<std::mutex> lock(state_mutex);
  state_changed = false;
  return state;
}

template<class T>
std::shared_ptr<T> Channel<T>::pull_blocking() {
  std::unique_lock<std::mutex> lock(state_mutex);
  on_update.wait(lock);
  state_changed = false;
  return state;
}
