#ifndef PARALOOPER_HPP_
#define PARALOOPER_HPP_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace utl {

  class ParaLooper {
    public:

      ParaLooper(const std::size_t maxWorker = std::thread::hardware_concurrency())
        : mMaxThreads(maxWorker)
        , mTotalTasks(0)
        , mRunning(false)
        , mPaused(false)
        , mWaiting(false)
        , mTaskAvailableCv{}
        , mTaskDoneCv{}
        , mTasksMutex {}
      {
        createThreads();
      }

      ~ParaLooper() {
        waitTasks();
        destroyThreads();
      }

      void execute(std::function<void(std::size_t, std::size_t)> task, const std::size_t jobCounts = 0, const bool awaitTasks = true) {
        const std::size_t jobs = (jobCounts == 0 || jobCounts > mMaxThreads) ? mMaxThreads : jobCounts;
        for (std::size_t id = 0; id < jobs; ++id) {
          addTask(task, id, jobs);
        }

        if(awaitTasks) {
          waitTasks();
        }
      }

      void waitTasks() {
        mWaiting = true;
        std::unique_lock<std::mutex> tasks_lock(mTasksMutex);
        mTaskDoneCv.wait(tasks_lock, [this] {
          return (mTotalTasks == (mPaused ? mTasks.size() : 0));
        });
        mWaiting = false;
      }

      const std::size_t getAwaitingTasks() const {
        const std::scoped_lock lock(mTasksMutex);
        return mTasks.size();
      }

      std::size_t getRunningTasks() const {
        const std::scoped_lock lock(mTasksMutex);
        return mTotalTasks - mTasks.size();
      }

      const size_t getTotalTasks() const {
        return mTotalTasks;
      }

      const bool isRunning() {
        return mRunning;
      }

      const bool isPaused() {
        return mPaused;
      }

      void pause() {
        mPaused = true;
      }

      void resume() {
        mPaused = false;
      }

    private:
      std::size_t mMaxThreads;
      std::atomic<size_t> mTotalTasks;
      std::atomic<bool> mRunning;
      std::atomic<bool> mPaused;
      std::atomic<bool> mWaiting;
      std::condition_variable mTaskAvailableCv;
      std::condition_variable mTaskDoneCv;
      mutable std::mutex mTasksMutex;
      std::vector<std::thread> mThreads;
      std::queue<std::function<void()>> mTasks;


      void createThreads() {
        mRunning = true;
        for (std::size_t i = 0; i < mMaxThreads; ++i) {
          mThreads.emplace_back(std::thread(&ParaLooper::worker, this));
        }
      }

      void destroyThreads() {
        mRunning = false;
        mTaskAvailableCv.notify_all();
        for (std::size_t i = 0; i < mMaxThreads; ++i) {
          mThreads[i].join();
        }
      }

      template<typename F, typename ... A>
        void addTask(const F& task, const A& ... args) {
          {
            const std::scoped_lock lock(mTasksMutex);
            if constexpr (sizeof...(args) == 0)
              mTasks.push(std::function<void()>(task));
            else
              mTasks.push(std::function<void()>([task, args...] {
                task(args...);
              }));
          }
          ++mTotalTasks;
          mTaskAvailableCv.notify_one();
        }

      void worker() {
        while (mRunning) {
          std::function<void()> task;
          std::unique_lock<std::mutex> lock(mTasksMutex);
          mTaskAvailableCv.wait(lock, [&] {
            return !mTasks.empty() || !mRunning;
          });
          if (mRunning && !mPaused) {
            task = std::move(mTasks.front());
            mTasks.pop();
            lock.unlock();
            task();
            --mTotalTasks;
            if (mWaiting)
              mTaskDoneCv.notify_one();
          }
        }
      }
  };

}



#endif /* PARALOOPER_HPP_ */
