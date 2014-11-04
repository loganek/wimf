/*
 * databuffer.cpp
 *
 *  Created on: Nov 4, 2014
 *      Author: loganek
 */

#include "databuffer.h"

using namespace Wimf;

DataBuffer::data_type& DataBuffer::operator [] (DataBuffer::data_array::size_type index)
{
	return data [index];
}

const DataBuffer::data_type& DataBuffer::operator [] (DataBuffer::data_array::size_type index) const
{
	return data [index];
}

DataBuffer& DataBuffer::operator++ ()
{
	++pointer;
	return *this;
}

DataBuffer DataBuffer::operator++ (int)
{
	auto cpy = *this;
	++pointer;
	return cpy;
}

void DataBuffer::reset ()
{
	pointer = 0;
}

const DataBuffer::data_type* DataBuffer::get_data () const
{
	return data.data ();
}

void DataBuffer::append (data_type c)
{
	operator [] (pointer) = c;
	++*this;
}
