export module bm.taskqueue;

import std;

namespace bm::core
{

export class TaskQueue
{
public:

	using Task = std::move_only_function<void()>;

public:

	void push(Task&& task)
	{
		m_tasks.push(std::move(task));
	}

	void execute()
	{
		while (not m_tasks.empty())
		{
			m_tasks.front()();
			m_tasks.pop();
		}
	}

private:

	std::queue<Task> m_tasks;

};

}