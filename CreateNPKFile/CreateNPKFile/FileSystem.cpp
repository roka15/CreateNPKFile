#include "FileSystem.h"


void roka::file::FileSystem::LoadFile(std::string _path)
{
	std::ifstream is(_path, ios::binary);
	int index = _path.rfind("\\");

	std::string file_name = _path.substr(index + 1);
	/*index = file_name.find(".");
	file_name.erase(index, 4);*/


	if (is)
	{
		is.seekg(0, is.end);
		int length = (int)is.tellg();
		is.seekg(0, is.beg);

		unsigned char* buffer = new unsigned char[length];

		is.read((char*)buffer, length);
		is.close();

		FileInfo* info = new FileInfo(file_name, length, (char*)buffer);

		mAllLength += file_name.length();
		mAllLength += sizeof(int);

		mAllLength += length;
		mAllLength += sizeof(int);

		mFileBuffers.push(info);
	}
}

void roka::file::FileSystem::LoadFiles(std::string _path, std::string _format)
{
	for (auto& file : std::filesystem::recursive_directory_iterator(_path))
	{
		std::string base_path = file.path().parent_path().string();
		std::string file_name = file.path().filename().string();
		std::string cur_path = base_path + "\\" + file_name;
		const std::string ext = file.path().extension().string();

		if (ext.compare(_format))
			continue;
		LoadFile(cur_path);
	}
	return;
}

void roka::file::FileSystem::SaveFile(std::string _path)
{
	std::ofstream fout;

	FileInfo* info = GetLoadFile();

	std::string save_path = _path+ "\\" + info->name;

	fout.open(save_path, std::ios::binary);
	if (fout.is_open())
	{
		int size = info->length;
		fout.write(info->buffer, size);
		fout.close();
	}
	delete info;
	return;
}



void roka::file::FileSystem::SaveFiles(std::string _path)
{
	while (mFileBuffers.empty() == false)
	{
		if (std::filesystem::exists(_path) == false)
		{
			std::filesystem::create_directory(_path);
		}
		SaveFile(_path);
	}
	return;
}

void roka::file::FileSystem::SaveFile(std::string _save_path, const roka::file::FileInfo* _data, std::string name)
{
	std::ofstream fout;

	std::string save_path = _save_path + "\\" + name;

	fout.open(save_path, std::ios::binary);

	if (fout.is_open())
	{
		fout.write(_data->buffer, _data->length);
		fout.close();
	}

	delete _data;
}

void roka::file::FileSystem::SaveFile(std::string _save_path, const FileInfo* _data)
{
	std::ofstream fout;
	std::string save_path= _save_path;
	if (_data->name.size() != 0)
		save_path += ("\\" + _data->name);
	
	fout.open(save_path, std::ios::binary);

	if (fout.is_open())
	{
		fout.write(_data->buffer, _data->length);
		fout.close();
	}

	delete _data;
}

roka::file::FileInfo* roka::file::FileSystem::GetLoadFile()
{
	FileInfo* file = mFileBuffers.front();
	mFileBuffers.pop();

	mAllLength -= file->length;
	mAllLength -= file->name.length();
	mAllLength -= sizeof(int) * 2;

	return file;
}

void roka::file::FileSystem::Release()
{
	while (mFileBuffers.empty() == false)
	{
		FileInfo* file = GetLoadFile();

		delete file;
	}

}


