#pragma once

#include "Core/GetInvalid.h"

namespace Renderer
{
	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Generic usable thread pool to avoid recreation of threads each tick
	*
	*  @remarks
	*    The data parallel thread pool is handy for situations were data can be processed in parallel (not task parallel). Example use-cases:
	*    - Frustum culling
	*    - Animation update
	*    - Particles update
	*
	*    Usage example: TODO(co) Streamline by using e.g. a template
	*    // Items which are going to be data-parallel-processed
	*    typedef std::vector<Item> Items;
	*    Items items;
	*
	*    // Worker function
	*    void updateItem(Items::iterator start, Items::iterator end)
	*    {
	*        while (start != end)
	*        {
	*            // ... do work...
	*            ++start;
	*        }
	*    }
	*
	*    // Setup calculation threads
	*    ThreadPool<void>& threadPool = ... get thread pool instance...
	*    size_t itemCount = items.size();
	*    size_t splitCount = 1;	// Package size for each thread to work on (will change when maximum number of threads is reached)
	*    const size_t threadCount = threadPool.getThreadCountAndSplitCount(itemCount, splitCount);
	*    Items::iterator startIterator = items.begin();
	*    Items::iterator endIterator = items.end();
	*    for (size_t threadIndex = 0; threadIndex < threadCount; ++threadIndex)
	*    {
	*        const size_t numberOfItemsToProcess = (threadIndex >= threadCount - 1) ? itemCount : splitCount;	// The last thread has to do all the rest of the remaining work
	*        threadPool.queueTask(std::bind(updateItem, startIterator, startIterator + numberOfItemsToProcess));
	*        itemCount -= splitCount;
	*        startIterator += splitCount;
	*    }
	*
	*    // Wait that all worker threads have done their part of the calculation
	*    threadPool.process();
	*
	*  @note
	*    - Meant for data-parallel use-cases
	*    - Implementation from https://github.com/netromdk/threadpool
	*/
	template <typename RetType> // Return type of tasks
	class ThreadPool final
	{


	public:
		using Task = std::function<RetType()>;
		using Callback = std::function<void()>;


	private:
		using TaskQueue = std::queue<Task>;
		using FutVec = std::vector<std::future<RetType>>;


	public:
		/// Invalid number of threads means to use as many threads as there are hardware threads on the system
		explicit ThreadPool(size_t threads = GetInvalid<size_t>())
		{
			if (IsInvalid(threads))
			{
				threads = std::thread::hardware_concurrency();
			}
			if (threads == 0)
			{
				threads = 1;
			}
			this->threads = threads;
		}

		explicit ThreadPool(const ThreadPool& threadPool) = delete;

		~ThreadPool()
		{
			// If thread is running then wait for it to complete
			if (thread.joinable())
			{
				thread.join();
			}
		}

		const ThreadPool& operator= (const ThreadPool& threadPool) = delete;

		void queueTask(const Task &task)
		{
			tasks.push(task);
		}

		void queueTask(Task &&task)
		{
			tasks.emplace(std::move(task));
		}

		/// Process all enqueued tasks.
		/** If no completion callback is defined then it
		  will block until done, otherwise it returns immediately and the callback
		  will be invoked when done. */
		void process(Callback callback = Callback())
		{
			if (callback)
			{
				thread = std::thread([&]{
					_process();
					callback();
				});
			}
			else
			{
				_process();
			}
		}

		[[nodiscard]] inline size_t getThreadCount() const { return threads; }
		[[nodiscard]] inline FutVec &getFutures() { return futuresDone; }

		[[nodiscard]] size_t getThreadCountAndSplitCount(size_t itemCount, size_t& splitCount) const
		{
			size_t threadCount = static_cast<size_t>(std::ceil(itemCount / static_cast<float>(splitCount)));
			if (threadCount > threads)
			{
				// Clamp thread count to maximum threads
				threadCount = threads;

				// Calculate new package size per thread
				splitCount = itemCount / threads;
			}
			return threadCount;
		}

	private:
		void _process()
		{
			std::lock_guard<std::mutex> lock(processMutex);
			futuresDone.clear();

			while (!tasks.empty())
			{
				const size_t taskAmount = tasks.size();
				const size_t amount = (threads > taskAmount ? taskAmount : threads);

				for (size_t i = 0; i < amount; ++i)
				{
					auto task = tasks.front();
					tasks.pop();

					auto future = std::async(std::launch::async, [=] {
						return task();
						});
					futuresPending.emplace_back(std::move(future));
				}

				for (auto &future : futuresPending)
				{
					future.wait();
					futuresDone.emplace_back(std::move(future));
				}

				futuresPending.clear();
			}
		}

		size_t threads;
		std::thread thread;
		std::mutex processMutex;
		TaskQueue tasks;
		FutVec futuresDone, futuresPending;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
