#include "libapp/libapp.h"
#include "libtracer/trace_points.h"
#include <atomic>
#include <condition_variable>
#include <format>
#include <iostream>
#include <locale>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;
using namespace std::chrono;

mutex mtx;
queue<Page> workQueue;
condition_variable cv;
atomic<bool> done{false};
atomic<int> totalPrimeCount{0};

void producer() {
  while (!done) {
    lttng_ust_tracepoint(demo_app, function_call_tracepoint, __func__);
    Page page = createNumbers();
    {
      lock_guard<mutex> lock(mtx);
      if (workQueue.size() >= 10) {
        workQueue.pop();
      }
      workQueue.push(page);
    }
    cv.notify_one();
  }
}

void consumer() {
  while (!done) {
    lttng_ust_tracepoint(demo_app, function_call_tracepoint, __func__);
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return !workQueue.empty() || done; });
    if (!workQueue.empty()) {
      Page page = workQueue.front();
      workQueue.pop();
      lock.unlock();
      totalPrimeCount += countPrimes(page);
    }
  }
}

int main() {
  thread producerThread(producer);
  vector<thread> consumerThreads;
  for (int i{}; i < 4; i += 1) {
    consumerThreads.emplace_back(consumer);
  }

  this_thread::sleep_for(10s);
  done = true;
  cv.notify_all();

  producerThread.join();
  for (auto &t : consumerThreads) {
    t.join();
  }

  lttng_ust_tracepoint(demo_app, performance_tracepoint, totalPrimeCount);
  return 0;
}
