#ifndef DLIB_CC_UTILS_BINARY_FILE_STREAM_H
#define DLIB_CC_UTILS_BINARY_FILE_STREAM_H

#include "../tools/include_lib_index.h"
#include <fstream>
#include <string>
#include <vector>
#include <type_traits>
#include <unordered_map>

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
	// openmodes
	enum class Mode
	{
		kIn = 1 << 0,
		kOut = 1 << 1,
		kTrunc = 1 << 2,
		kApp = 1 << 3,
		kTmp = 1 << 4
	};
	// seek directions
	enum class SeekDir
	{
		kBeg = 1 << 0,
		kCur = 1 << 1,
		kEnd = 1 << 2
	};

	static BinaryFileStream* Create();

	template <typename T>
	bool Read(
		const std::string& filename,
		std::vector<T>& buff,
		size_t size,
		size_t ofset = 0,
		std::ios_base::seekdir way = std::ios::beg)
	{
		if(!OpenStream(filename, std::ios::out|std::ios::in|std::ios::binary))
		{
			return false;
		}

		auto fs = &opening_streams_[filename];

		if(ofset != 0)
		{
			fs->seekg(ofset * sizeof(T), way);
		}

		for(size_t i = 0; i < size; ++i)
		{
			T data;
			fs->read((char*)&data, sizeof(data));
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
			: std::ios::in|std::ios::out|std::ios::binary|std::ios::trunc;

		if(!OpenStream(filename, mode_flag))
		{
			return false;
		}

		auto fs = &opening_streams_[filename];

		if(append)
		{
			fs->seekp(ofset * sizeof(T), way);
		}

		for(auto& data : buff)
		{
			fs->write((char*)&data, sizeof(data));
		}

		return true;
	}

	std::string MakeTmpFile();
	void DeleteTmpFile(const std::string& filename);
	void Close(const std::string& filename);

protected:
	bool Init();
	BinaryFileStream();
	~BinaryFileStream();

private:
	std::string tmpfile_extension_;
	std::vector<std::string> tmpfile_names_;
	std::unordered_map<std::string, std::fstream> opening_streams_;
	Mode open_mode_;

	bool OpenStream(const std::string& filename, std::ios_base::openmode mode);
};

// AND and OR operators for BinaryFileStream::Mode, BinaryFileStream::SeekDir
namespace dlib_cc
{
	namespace utils
	{
		using enum_mode = BinaryFileStream::Mode;
		using enum_seek_dir = BinaryFileStream::SeekDir;
		using mode_type = std::underlying_type<enum_mode>::type;
		using seek_dir_type = std::underlying_type<enum_seek_dir>::type;

		enum_mode operator|(enum_mode l, enum_mode r);
		enum_mode operator&(enum_mode l, enum_mode r);
		enum_seek_dir operator|(enum_seek_dir l, enum_seek_dir r);
		enum_seek_dir operator&(enum_seek_dir l, enum_seek_dir r);
	}
}

#endif
