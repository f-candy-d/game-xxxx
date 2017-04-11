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
		class FileStream;
	}
}

class dlib_cc::utils::FileStream : public cocos2d::Ref
{

public:
	// compile-time constants
	static constexpr int kModeOverwrite = 1 << 0;
	static constexpr int kModeAppend = 1 << 1;
	static constexpr int kModeBinary = 1 << 2;
	static constexpr int kModeText = 1 << 3;

	FileStream* Create();

	template <typename T>
	bool ReadBySize(
		std::string filename,
		std::vector<T>& buff,
		int mode = kModeOverwrite | kModeText,
		std::streamoff ofset = 0,
		std::ios_base::seekdir way = std::ios::beg)
	{
		std::ifstream ifs(filename.c_str(), MakeModeFlag(mode));

		if(!ifs)
		{
			std::cout << "error opening file => " << filename << '\n';
			return false;
		}



		return true;
	}

private:
	std::ios_base::open_mode MakeModeFlag(int mode);
};

#endif
