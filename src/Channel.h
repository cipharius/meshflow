#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include <condition_variable>
#include <mutex>

template<class T>
class Channel {
  public:
    Channel();
    bool has_changed();
    void reset();
    void push(T value);
    std::shared_ptr<T> pull();
    std::shared_ptr<T> pull_blocking();

  private:
    std::shared_ptr<T> state;
    std::mutex state_mutex;
    std::condition_variable on_update;
    bool state_changed;
};

#endif
