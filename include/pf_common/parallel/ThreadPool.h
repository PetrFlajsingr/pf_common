//
// Created by petr on 12/26/19.
//

#ifndef UTILITIES_THREADPOOL_H
#define UTILITIES_THREADPOOL_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <future>
#include <pf_common/exceptions/StackTraceException.h>
#include <pf_common/parallel/SafeQueue.h>
#include <ranges>
#include <thread>

namespace pf {
namespace details {
struct Task {
  void operator()() { execute(); }
  virtual void execute() = 0;

  virtual ~Task() = default;
};

template<typename T>
struct PTask : public Task {
  explicit PTask(std::packaged_task<T()> &&task) : task(std::move(task)) {}
  void execute() override { task(); }
  std::packaged_task<T()> task;
};
}// namespace details

enum class ThreadPoolState { Run, Stop, FinishAndStop };

class ThreadPool {
 public:
  inline explicit ThreadPool(std::size_t threadCount) {
    threads.resize(threadCount);
    std::ranges::generate(threads.begin(), threads.end(), [this] { return std::thread{[this] { threadLoop(); }}; });
  }
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = default;
  ThreadPool &operator=(ThreadPool &&) = default;

  auto enqueue(std::invocable auto &&callable) {
    if (state != ThreadPoolState::Run) { throw InvalidArgumentException{"Threadpool is not in Run state."}; }
    using result_type = std::invoke_result_t<decltype(callable)>;
    auto task = std::packaged_task<result_type()>(std::forward<decltype(callable)>(callable));
    auto future = task.get_future();
    queue.enqueue(std::make_unique<details::PTask<result_type>>(std::move(task)));
    return future;
  }

  inline void finishAndStop() { state = ThreadPoolState::FinishAndStop; }
  inline void cancelAndStop() {
    state = ThreadPoolState::Stop;
    queue.shutdown();
  }

  inline ~ThreadPool() {
    if (state == ThreadPoolState::Run) { state = ThreadPoolState::FinishAndStop; }
    queue.shutdown();
    if (state == ThreadPoolState::FinishAndStop) {
      while (!queue.isEmpty()) {};
    }
    for (auto &thread : threads) { thread.join(); }
  }

 private:
  std::vector<std::thread> threads;
  SafeQueue<std::unique_ptr<details::Task>> queue;
  ThreadPoolState state = ThreadPoolState::Run;

  inline void threadLoop() {
    while (true) {
      auto task = queue.dequeue();
      if (task.has_value()) {
        auto callable = std::move(task.value());
        (*callable)();
      } else {
        return;
      }
    }
  }
};
}// namespace pf
#endif// UTILITIES_THREADPOOL_H
