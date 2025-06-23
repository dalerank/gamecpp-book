class thread_pool_t
{
public:
  thread_pool_t(std::size_t n_threads)
  {
    for (std::size_t i = 0; i < n_threads; ++i)
    {
      _threads.push_back(make_thread_handler(_queue));
    }
  }

  ~thread_pool_t()
  {
    // Task = {Execute/Stop, function, args}
    Task const stop_task{TaskType::Stop, {}, {}};
    for (std::size_t i = 0; i < _threads.size(); ++i)
    {
      push(stop_task);
    }
  }

  bool push(Task const& task)
  {
    _queue.push(task);
    return true;
  }
private:
  std::queue<Task> _queue;
  std::vector<std::jthread> _threads;
}
