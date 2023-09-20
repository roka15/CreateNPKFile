#pragma once
//#include <iostream>
#include <queue>
#include <map>
#include <filesystem>
#include <fstream>

namespace roka::file
{
	//1. �̹��� ���̳ʸ� �а� ���� ���� �׽�Ʈ
	//2. �̹��� ���� �տ� text ���� ����
	//3.filebuffers ����� ���ļ� bin ���Ϸ� ����.

	struct FileInfo
	{
		FileInfo() :length(0), buffer(nullptr) {};
		FileInfo(const FileInfo& ref)
		{
			name = ref.name;
			parent_path = ref.parent_path;
			length = ref.length;
			buffer = new char[length];
			memcpy(buffer, ref.buffer, length);
		}
		FileInfo(std::string _name, size_t _len, char* _buffer) :name(_name), length(_len), buffer(_buffer) {};
		~FileInfo() { delete [] buffer; }
		std::string name;
		std::string parent_path;
		size_t length;
		char* buffer;
	};

	struct CSVInfo
	{
		std::string name;
		std::vector<std::pair<int, int>> canvas;
		std::vector<std::pair<int, int>> pos;
		std::vector<std::pair<int, int>> size;
	};
	struct PackInfo
	{
		PackInfo() {}
		PackInfo(const PackInfo& ref)
		{
			name = ref.name;
			std::vector<FileInfo*> vec = ref.binbuf;
			for (int i = 0; i < vec.size(); i++)
			{
				FileInfo* file = new FileInfo(*(vec[i]));
				binbuf.push_back(file);
			}
		}
		~PackInfo()
		{
			for (FileInfo*& file : binbuf)
			{
				if (file != nullptr)
				{
					delete file;
					file = nullptr;
				}
			}
		}
		std::string name;
		std::vector<FileInfo*> binbuf;
	};
	enum class DataType
	{
		FILE,
		PACK,
		CSV,
	};
	class FileSystem
	{
	public:
		FileSystem() :mAllLength(0) {};
		~FileSystem() { Release(); }
		void LoadFile(std::string _path);
		void LoadFiles(std::string _path, std::string _format);
		void SaveFile(std::string _path);
		void SaveFiles(std::string _path);
		void SaveFile(std::string _save_path, const FileInfo* _data, std::string name);
		void SaveFile(std::string _save_path, const FileInfo* _data);
		FileInfo* GetLoadFile();

		virtual void Release();
	protected:
		std::queue<FileInfo*> mFileBuffers;
		int mAllLength;
	};

}

