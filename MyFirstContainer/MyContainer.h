#pragma once
#include <iostream>

template <typename T> class MyFirstContainerClass
{
private:
	T* m_arr;
	std::size_t m_capacity;
	std::size_t m_current;

public:

	MyFirstContainerClass()
	{
		m_arr = new T[1];
		m_capacity = 1;
		m_current = 0;
	}

	void push_back(T data)
	{
		if (m_current == m_capacity) 
		{
			T* temp = new T[2 * m_capacity];
			for (int i = 0; i < m_capacity; i++) {
				temp[i] = m_arr[i];
			}
			delete[] m_arr;
			m_capacity *= 2;
			m_arr = temp;
		}
		m_arr[m_current] = data;
		m_current++;
	}

	T at(int index)
	{

		// if index is within the range
		if (index < m_current)
			return m_arr[index];
	}

	void pop() { m_current--; }
	int size() { return m_current; }
	int getCapacity() { return m_capacity; }
	void print()
	{
		for (int i = 0; i < m_current; i++) {
			std::cout << m_arr[i] << " ";
		}
		std::cout << std::endl;
	}
};

