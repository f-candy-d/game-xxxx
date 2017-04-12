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

void BinaryFileStream::Close(const std::string& filename)
{
	auto itr = opening_streams_.find(filename);
	if(itr != opening_streams_.end() && opening_streams_[itr->first].is_open())
	{
		opening_streams_[itr->first].close();
		opening_streams_.erase(itr);
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
	// close all opening file streams
	for(auto itr = opening_streams_.begin(); itr != opening_streams_.end(); ++itr)
	{
		itr->second.close();
	}

	// delete all tmp files
	for(auto& tmpfile_name : tmpfile_names_)
	{
		DeleteTmpFile(tmpfile_name);
	}
}

bool BinaryFileStream::OpenStream(const std::string &filename, std::ios_base::openmode mode)
{
	auto itr = opening_streams_.find(filename);
	if(itr != opening_streams_.end())
	{
		// a file stream for the file does already exist
		return true;
	}

	opening_streams_[filename].open(filename.c_str(), mode);
	if(!opening_streams_[filename])
	{
		std::cout << "BinaryFileStream :: error openinig file => " << filename << '\n';
		opening_streams_.erase(filename);
		return false;
	}
	else
	{
		return true;
	}
}
