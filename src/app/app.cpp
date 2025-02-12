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
  Tracer tracer(__func__);
  while (!done) {
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
  Tracer tracer(__func__);
  while (!done) {
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
  Tracer tracer(__func__);
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

  locale::global(locale{"en_US.UTF-8"});
  cout << format("Total prime count: {:L}\n", totalPrimeCount.load());
  return 0;
}
