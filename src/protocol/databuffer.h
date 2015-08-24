/*
 * databuffer.h
 *
 *  Created on: Nov 4, 2014
 *      Author: loganek
 */

#ifndef DATABUFFER_H_
#define DATABUFFER_H_

#include <array>

namespace Wimf {

class DataBuffer
{
public:
	typedef std::array<unsigned char, 128> data_array;
	typedef data_array::value_type data_type;

private:
	data_array data;
	data_array::size_type pointer = 0;

public:
	virtual ~DataBuffer () {}

	data_type& operator[] (data_array::size_type index);
	const data_type& operator[] (data_array::size_type index) const;

	DataBuffer& operator++ ();
	DataBuffer operator++ (int);

	void reset ();
	const data_type* get_data () const;
	void append (data_type c);

	data_array::size_type get_pointer () const { return pointer; }
	data_array::size_type get_size () const { return data.size (); }

	data_array::const_reverse_iterator rbegin ()  const { return data.rbegin (); }
	data_array::const_reverse_iterator rend () const { return data.rend (); }

	data_array::const_iterator begin ()  const { return data.begin (); }
	data_array::const_iterator end () const { return data.end (); }
};

}

#endif /* DATABUFFER_H_ */
