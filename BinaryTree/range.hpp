#pragma once
#ifndef RANGE_HPP
#define RANGE_HPP

#include<type_traits>
#include<iterator>

namespace my
{
	template<class T, typename = std::enable_if_t<std::is_integral_v<T>>>
	class range
	{
	public:
		explicit range(T start, T stop, T step)
			: first_{ start }, last_{ stop }, step_{ step }
		{ check_range(first_, last_, step_); }

		explicit range(T stop, T step)
			: first_{ T{0} }, last_{ stop }, step_{ step }
		{ check_range(first_, last_, step_); }

		explicit range(T stop)
			: first_{ T{0} }, last_{ stop }, step_{ T{1} }
		{ check_range(first_, last_, step_); }

		class range_iterator : public std::iterator<std::input_iterator_tag, T>
		{
			friend class range<T>;

		public:
			range_iterator(const range_iterator& other)
				: current_{ other.current_ }, step_{ other.step_ }{}
			range_iterator operator++() { current_ += step_; return *this; }
			range_iterator operator++(int) { auto tmp = *this; this->operator++(); return tmp; }
			bool operator==(const range_iterator& other) const { return this->current_ == other.current_; }
			bool operator!=(const range_iterator& other) const { return this->current_ != other.current_; }
			T operator*() { return current_; }

		private:
			range_iterator(T curr, T step) : current_{ curr }, step_{ step } {}
			T current_;
			T step_;
		};

		using iterator = range_iterator;

		iterator begin() { return iterator(first_, step_); }
		iterator end() { return iterator(last_, step_); }

	private:
		range(const range&) = delete;
		range& operator=(const range&) = delete;
		range(range&&) = delete;
		range& operator=(range&&) = delete;

		void check_range(T start_, T stop_, T step_)
		{
			if (start_ > stop_ && step_ > static_cast<T>(0) ||
				start_ < stop_ && step_ < static_cast<T>(0))
			{
				throw std::runtime_error{ "range is invalid!" };
			}
		}

		T first_;
		T last_;
		T step_;
	};
}

#endif // RANGE_HPP