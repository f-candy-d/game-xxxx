#ifndef DLIB_BINARY_FILE_STREAM_BINARY_FILE_STREAM_IMPL_H
#define DLIB_BINARY_FILE_STREAM_BINARY_FILE_STREAM_IMPL_H

#include "binary_file_stream.h"
#include <cstdio>

/**
 * implementation file for bfstram class
 */

/**
 * public
 */
template <typename T>
dlib::bfstream<T>::bfstream()
:is_open_(false)
{}

template <typename T>
dlib::bfstream<T>::bfstream
(const std::string& filename, const dlib::ios_f::open_mode openmode)
:filename_(filename)
,is_open_(false)
,openmode_(openmode)
{
	stream_.open(filename.c_str(), to_ios_base_openmode(openmode) | std::ios::binary);
	if(stream_)
	{
		is_open_ = true;
	}
}

template<typename T>
dlib::bfstream<T>::~bfstream()
{
	if(is_open_)
	{
		stream_.close();
	}

	// delete the file if 'tmp-mode' is enable
	if(openmode_ & dlib::ios_f::tmp)
	{
		std::remove(filename_.c_str());
	}
}

template<typename T>
void dlib::bfstream<T>::open
(const std::string& filename, const dlib::ios_f::open_mode openmode)
{
	filename_ = filename;
	openmode_ = openmode;
	stream_.open(filename.c_str(), to_ios_base_openmode(openmode) | std::ios::binary);

	if(stream_)
	{
		is_open_ = true;
	}
}

template<typename T>
void dlib::bfstream<T>::write_from_buff(const std::vector<T>& buff)
{
	assert(openmode_ & dlib::ios_f::out);
	assert(is_open_);

	for(auto& data : buff)
	{
		stream_.write((char*)&data, sizeof(T));
	}
}

template<typename T>
void dlib::bfstream<T>::read_into_buff(std::vector<T>& buff, size_t size)
{
	assert(openmode_ & dlib::ios_f::in);
	assert(is_open_);

	for(size_t i = 0; i < size; ++i)
	{
		T data;
		stream_.read((char*)&data, sizeof(T));
		buff.push_back(data);
	}
}

template<typename T>
void dlib::bfstream<T>::write_one(const T data)
{
	assert(openmode_ & dlib::ios_f::out);
	assert(is_open_);

	stream_.write((char*)&data, sizeof(T));
}

template<typename T>
T dlib::bfstream<T>::read_one()
{
	assert(openmode_ & dlib::ios_f::in);
	assert(is_open_);

	T data;
	stream_.read((char*)&data, sizeof(T));

	return data;
}

template<typename T>
void dlib::bfstream<T>::seekg(const size_t ofset, const dlib::ios_f::seek_dir way)
{
	assert(openmode_ & dlib::ios_f::in);
	assert(is_open_);

	stream_.seekg(ofset * sizeof(T), to_ios_base_seekdir(way));
}

template<typename T>
void dlib::bfstream<T>::seekp(const size_t ofset, const dlib::ios_f::seek_dir way)
{
	assert(openmode_ & dlib::ios_f::out);
	assert(is_open_);

	stream_.seekp(ofset * sizeof(T), to_ios_base_seekdir(way));
}

/**
 * private
 */
template<typename T>
std::ios_base::openmode dlib::bfstream<T>::to_ios_base_openmode
(dlib::ios_f::open_mode flg)
{
	std::ios_base::openmode flag;

	if(flg & dlib::ios_f::in)
		flag |= std::ios::in;
	if(flg & dlib::ios_f::out)
		flag |= std::ios::out;
	if(flg & dlib::ios_f::trunc)
		flag |= std::ios::trunc;
	if(flg & dlib::ios_f::app)
		flag |= std::ios::app;
	if(flg & dlib::ios_f::ate)
		flag |= std::ios::ate;

	return flag;
}

template<typename T>
std::ios_base::seekdir dlib::bfstream<T>::to_ios_base_seekdir
(dlib::ios_f::seek_dir flg)
{
	std::ios_base::seekdir flag;

	if(flg & dlib::ios_f::beg)
		flag = std::ios::beg;
	if(flg & dlib::ios_f::cur)
		flag = std::ios::cur;
	if(flg & dlib::ios_f::end)
		flag = std::ios::end;

	return flag;
}

#endif
