#ifndef DLIB_BINARY_FILE_STREAM_BINARY_FILE_STREAM_H
#define DLIB_BINARY_FILE_STREAM_BINARY_FILE_STREAM_H

#include <fstream>
#include <string>
#include <vector>

namespace dlib
{
	template<typename T> class bfstream;

	namespace ios_f
	{
		using open_mode = int;
		using seek_dir = int;

		// i/o stream's flags
		// openmodes
		constexpr open_mode in = 1 << 0;
		constexpr open_mode out = 1 << 1;
		constexpr open_mode trunc = 1 << 2;
		constexpr open_mode app = 1 << 3;
		constexpr open_mode ate = 1 << 4;
		constexpr open_mode tmp = 1 << 5;

		// seek directions
		constexpr seek_dir beg = 1 << 0;
		constexpr seek_dir cur = 1 << 1;
		constexpr seek_dir end = 1 << 2;
	}
}

template<typename T> class dlib::bfstream
{
public:
	bfstream();
	bfstream(const std::string& filename, const ios_f::open_mode openmode);
	~bfstream();
	void open(const std::string& filename, const ios_f::open_mode openmode);
	void write_from_buff(const std::vector<T>& buff);
	void read_into_buff(std::vector<T>& buff, size_t size);
	void seekg(const size_t ofset, const ios_f::seek_dir way);
	void seekp(const size_t ofset, const ios_f::seek_dir way);
	bool is_open() const { return is_open_; }

private:
	std::string filename_;
	bool is_open_;
	ios_f::open_mode openmode_;
	std::fstream stream_;

	std::ios_base::openmode to_ios_base_openmode(ios_f::open_mode flg);
	std::ios_base::seekdir to_ios_base_seekdir(ios_f::seek_dir flg);
};

// include the implementation file here
#include "binary_file_stream_impl.h"

#endif
