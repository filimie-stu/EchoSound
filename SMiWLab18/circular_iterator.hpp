#pragma once

// Circular iterator is like a circular buffer.
// Using operator++ when it's pointing to the last element
// of a sequence, loops it back to the first element.
//

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

	// Forwards the inner pointer by one.
	// If pointing to the last element of the sequence,
	// loops back to the initial element.
	//
	circular_iterator& operator++() {
		++current_;
		if (current_ == end_) {
			current_ = begin_;
		}
		return *this;
	}

	// Works like in normal iterators
	//
	value_type& operator*() {
		return *current_;
	}

private:
	using seq_iterator = typename Sequence::iterator;

	seq_iterator begin_;
	seq_iterator end_;
	seq_iterator current_;
};

// Factory method supplied for better template 
// argument deduction.
// 
template<class Sequence>
circular_iterator<Sequence> make_circular_iterator(Sequence& seq) {
	return circular_iterator<Sequence>(seq);
}