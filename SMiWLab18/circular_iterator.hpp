#pragma once

template<class Sequence>
class circular_iterator {
public:
	using value_type = typename Sequence::value_type;

	circular_iterator(Sequence& seq) :
		begin_(seq.begin()),
		end_(seq.end()),
		current_(seq.begin())
	{
	}

	circular_iterator& operator++() {
		++current_;
		if (current_ == end_) {
			current_ = begin_;
		}
		return *this;
	}

	value_type& operator*() {
		return *current_;
	}

private:
	using seq_iterator = typename Sequence::iterator;

	seq_iterator begin_;
	seq_iterator end_;
	seq_iterator current_;
};

template<class Sequence>
circular_iterator<Sequence> make_circular_iterator(Sequence& seq) {
	return circular_iterator<Sequence>(seq);
}