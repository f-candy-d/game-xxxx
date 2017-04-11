#include "binary_file_stream.h"
#include <algorithm>
#include <cstdio>

/**
 * dlib_cc::utils::BinaryFileStream class
 */
using namespace dlib_cc::utils;
using namespace cocos2d;

/**
 * public
 */
BinaryFileStream* BinaryFileStream::Create()
{
	auto ret = new BinaryFileStream();
	if(ret->Init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

std::string BinaryFileStream::MakeTmpFile()
{
	std::string filename("binaryfilestreamtmp" + std::to_string(tmpfile_names_.size()) + "." + tmpfile_extension_);
	tmpfile_names_.push_back(filename);
	std::ofstream ofs(filename.c_str());

	if(!ofs)
		return std::move(std::string(""));
	else
		return std::move(filename);
}

void BinaryFileStream::DeleteTmpFile(const std::string &filename)
{
	// std::remove(filename.c_str());
	if(!std::remove(filename.c_str()))
	{
		std::cout << "BinaryFileStream :: file successfully deleted! => " << filename << '\n';
	}
	else
	{
		std::cout << "BinaryFileStream :: error deleting file... => " << filename << '\n';
	}
}

/**
 * protected
 */
bool BinaryFileStream::Init()
{
	return true;
}

BinaryFileStream::BinaryFileStream()
:tmpfile_extension_("dat")
{}

BinaryFileStream::~BinaryFileStream()
{
	// delete all tmp files
	for(auto& tmpfile_name : tmpfile_names_)
	{
		DeleteTmpFile(tmpfile_name);
	}
}
