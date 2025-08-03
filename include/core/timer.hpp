#pragma once 

#include <chrono>


class Timer
{
public:

	Timer()
		: m_this_created(std::chrono::high_resolution_clock::now())
	{
	}
	~Timer()
	{

	}

	//return time in seconds
	double getTimeFromCreation() const
	{
		return std::chrono::duration<double>{std::chrono::high_resolution_clock::now() - m_this_created}.count();
	}

private:

	std::chrono::time_point<std::chrono::high_resolution_clock> m_this_created;

};