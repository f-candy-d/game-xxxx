#ifndef DLIB_CC_UTILS_BINARY_FILE_STREAM_H
#define DLIB_CC_UTILS_BINARY_FILE_STREAM_H

#include "../tools/include_lib_index.h"
#include <fstream>
#include <string>
#include <vector>

namespace dlib_cc
{
	namespace utils
	{
		class BinaryFileStream;
	}
}

class dlib_cc::utils::BinaryFileStream : public cocos2d::Ref
{

public:
	static BinaryFileStream* Create();

	template <typename T>
	bool Read(
		const std::string& filename,
		std::vector<T>& buff,
		size_t size,
		size_t ofset = 0,
		std::ios_base::seekdir way = std::ios::beg)
	{
		std::ifstream ifs(filename.c_str(), std::ios::binary);

		if(!ifs)
		{
			std::cout << "error opening file => " << filename << '\n';
			return false;
		}

		if(ofset != 0)
		{
			ifs.seekg(ofset * sizeof(T), way);
		}

		for(size_t i = 0; i < size; ++i)
		{
			T data;
			ifs.read((char*)&data, sizeof(data));
			buff.push_back(data);
		}

		return true;
	}

	template <typename T>
	bool Write(
		const std::string& filename,
		const std::vector<T>& buff,
		bool append = true,
		size_t ofset = 0,
		std::ios_base::seekdir way = std::ios::beg)
	{
		auto mode_flag =
			(append)
			? std::ios::in|std::ios::out|std::ios::binary|std::ios::app
			: std::ios::out|std::ios::binary|std::ios::trunc;

		std::ofstream ofs(filename.c_str(), mode_flag);

		if(!ofs)
		{
			std::cout << "error opening file => " << filename << '\n';
			return false;
		}

		if(append)
		{
			ofs.seekp(ofset * sizeof(T), way);
		}

		for(auto& data : buff)
		{
			ofs.write((char*)&data, sizeof(data));
		}

		return true;
	}

	std::string MakeTmpFile();
	void DeleteTmpFile(const std::string& filename);

protected:
	bool Init();
	BinaryFileStream();
	~BinaryFileStream();

private:
	std::string tmpfile_extension_;
	std::vector<std::string> tmpfile_names_;
};

#endif
