#pragma once
#include "FileSystem.h"
#define CANVAS_SAME 1
#define CANVAS_DIFF 0

namespace roka::file
{
	class NPKSystem:public FileSystem
	{
	public:
		NPKSystem() :mCsvLine(0), mCSVBuffers(nullptr) {};
		virtual ~NPKSystem() { Release(); }
		void SavePacks(std::string _save_path, std::map<std::string, PackInfo*> _pack);
		void SaveCsvs(std::string _save_path, std::map<std::string, CSVInfo*> _csv);
		const FileInfo* CreateImagePackage(std::string _read_path, std::string _road_format);
		size_t ReadImagePackage(const char* _buf, std::map<std::string, CSVInfo*>& _csvmap, std::map<std::string, PackInfo*>& _packmap);
		void CreateNPK(std::string _image_path, std::string _txt_path, std::string _format, std::string _save_path);
		void ReadNPK(std::string _path, std::map<std::string, CSVInfo*>& _csvmap, std::map<std::string, PackInfo*>& _packmap);
		void OpenCSV(std::string _path);
		FileInfo* CreateCSVLineBuffer();
		size_t ReadCSVLine(const char* buf, std::map<std::string, CSVInfo*>& _csvmap, std::string& _out_str);
		virtual void Release()override;
		void Clear();
	private:
		std::queue<FileInfo*> mImagePackBuffers;
		FileInfo* mCSVBuffers;
		int mCsvLine;
	};

}
