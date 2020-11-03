#pragma once
#include <iostream>
#include <algorithm>
#include <typeinfo>

template <typename T> class MyFirstContainerClass
{
private:
	T* m_buffer;
	std::size_t m_capacity;
	std::size_t m_length;
	bool m_loop = { false };
	bool m_wall = { false };
	T m_wallIndex;

	struct Deleter
	{
		void operator()(T* m_buffer) const
		{
			::operator delete(m_buffer);
		}
	};

public:

	MyFirstContainerClass(int capacity = 10)
		: m_capacity(capacity), m_length(0),
		m_buffer (static_cast<T*>(::operator new(sizeof(T)*capacity)))
	{}
	MyFirstContainerClass(int capacity, bool wall, bool loop)
		:MyFirstContainerClass(10), m_wall(wall), m_loop(loop) 
	{
		if (m_wall == true) m_loop = false;
	}

	void setLoop() {
		m_loop = true; m_wall = false;
	}
	void setWall(T const& value) {
		m_loop = false; m_wall = true;
		m_wallIndex = value;
	}
	void setWallIndex(T const& value) { m_wallIndex = value; }
	int getWallIndex() { return m_wallIndex; }

	~MyFirstContainerClass()
	{
		std::unique_ptr<T, Deleter> deleter(m_buffer, Deleter());
		clearElements();
	}
	MyFirstContainerClass(MyFirstContainerClass const& copy)
		:m_capacity(copy.m_length), m_length(0),
		m_buffer(static_cast<T*>(::operator new(sizeof(T)* m_capacity)))
	{
		try
		{
			for (std::size_t loop = 0; loop < copy.m_length; ++loop)
			{
				push_back(copy.m_buffer[loop]);
			}
		}
		catch (...)
		{
			std::unique_ptr<T, Deleter> deleter(m_buffer, Deleter());
			clearElements();
			throw;
		}
	}
	MyFirstContainerClass& operator=(MyFirstContainerClass const& copy)
	{
		copyAssing(copy);
		return *this;
	}
	MyFirstContainerClass(MyFirstContainerClass&& move) noexcept
		: m_capacity(0), m_length(0), m_buffer(nullptr)
	{
		move.swap(*this);
	}
	MyFirstContainerClass& operator=(MyFirstContainerClass&& move) noexcept
	{
		move.swap(*this);
		return *this;
	}
	void swap(MyFirstContainerClass& other) noexcept
	{
		using std::swap;
		swap(m_capacity, other.m_capacity);
		swap(m_length, other.m_length);
		swap(m_buffer, other.m_buffer);
	}
	void push_back(T const& value)
	{
		resizeIfRequire();
		pushBackInternal(value);
	}
	void pop_back()
	{
		--m_length;
		m_buffer[m_length].~T();
	}
	void reserve(std::size_t capacityUpperBound)
	{
		if (capacityUpperBound > m_capacity)
		{
			reserveCapacity(capacityUpperBound);
		}
	}
	T at(const int index)
	{
		//std::cout << index << "\n";
		if (m_wall)
		{
			if (index < 0 || index > m_length - 1) return m_wallIndex;
		}
		else if (m_loop)
		{
			if (index < 0)
			{
				int correctIndex = m_length + index;
				return m_buffer[correctIndex];
			}
			if (index == m_length) return m_buffer[0];
			if (index > m_length)
			{
				int correctIndex = index - m_length;
				return m_buffer[correctIndex];
			}
		}
		return m_buffer[index];

	}
	T& operator[] (const std::size_t index)
	{
		return m_buffer[index];
	}
	std::size_t size()
	{
		return m_length;
	}
private:
	void resizeIfRequire()
	{
		if (m_length == m_capacity)
		{
			std::size_t     newCapacity = std::max(2.0, m_capacity * 1.5);
			reserveCapacity(newCapacity);
		}
	}
	void reserveCapacity(std::size_t newCapacity)
	{
		MyFirstContainerClass<T>  tmpBuffer(newCapacity);

		simpleCopy<T>(tmpBuffer);

		tmpBuffer.swap(*this);
	}
	void pushBackInternal(T const& value)
	{
		new (m_buffer + m_length) T(value);
		++m_length;
	}
	void moveBackInternal(T&& value)
	{
		new (m_buffer + m_length) T(std::move(value));
		++m_length;
	}
	template<typename X>
	typename std::enable_if<std::is_nothrow_move_constructible<X>::value == false>::type
		simpleCopy(MyFirstContainerClass<T>& dst)
	{
		std::for_each(m_buffer, m_buffer + m_length,
			[&dst](T const& v) {dst.pushBackInternal(v); }
		);
	}

	template<typename X>
	typename std::enable_if<std::is_nothrow_move_constructible<X>::value == true>::type
		simpleCopy(MyFirstContainerClass<T>& dst)
	{
		std::for_each(m_buffer, m_buffer + m_length,
			[&dst](T& v) {dst.moveBackInternal(std::move(v)); }
		);
	}
	void clearElements()
	{
		for (std::size_t loop = 0; loop < m_length; ++loop)
		{
			m_buffer[m_length - 1 - loop].~T();
		}
	}

	template<typename X>
	typename std::enable_if<(std::is_nothrow_copy_constructible<X>::value
		&& std::is_nothrow_destructible<X>::value) == true>::type
		copyAssign(MyFirstContainerClass<X>& copy)
	{
		if (this == &copy)
		{
			return;
		}

		if (m_capacity <= copy.m_length)
		{
			clearElements();
			m_length = 0;
			for (int loop = 0; loop < copy.m_length; ++loop)
			{
				pushBackInternal(copy[loop]);
			}
		}
		else
		{
			// Copy and Swap idiom
			MyFirstContainerClass<T>  tmp(copy);
			tmp.swap(*this);
		}
	}

	void copyAssign(MyFirstContainerClass& copy)
	{
		// Copy and Swap idiom
		MyFirstContainerClass<T>  tmp(copy);
		tmp.swap(*this);
	}
};

