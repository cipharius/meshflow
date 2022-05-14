#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include <condition_variable>
#include <mutex>

template<class T>
class Channel {
  public:
    Channel() : state_changed(false) {}

    constexpr bool has_changed() {
      return state_changed;
    }

    void reset() {
      std::unique_lock<std::mutex> lock(state_mutex);
      state.reset();
      state_changed = true;
      on_update.notify_all();
    }

    void push(std::shared_ptr<T> value) {
      {
        std::unique_lock<std::mutex> lock(state_mutex);
        if (state == value) return;
        if (state && value && *state == *value) return;
        state = value;
      };

      state_changed = true;
      on_update.notify_all();
    }

    std::shared_ptr<T> pull() {
      std::unique_lock<std::mutex> lock(state_mutex);
      state_changed = false;
      return state;
    }

    std::shared_ptr<T> pull_blocking() {
      std::unique_lock<std::mutex> lock(state_mutex);
      on_update.wait(lock);
      state_changed = false;
      return state;
    }

  private:
    std::shared_ptr<T> state;
    std::mutex state_mutex;
    std::condition_variable on_update;
    bool state_changed;
};

#endif
