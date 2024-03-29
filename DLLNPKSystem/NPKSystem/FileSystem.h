#pragma once

#include <queue>
#include <map>
#include <filesystem>
#include <fstream>


#ifdef CREATEDLL_EXPORTS
#define MYDLL_DECLSPEC __declspec(dllexport)
#else
#define MYDLL_DECLSPEC __declspec(dllimport)
#endif

namespace roka::file
{

	MYDLL_DECLSPEC std::wstring s2ws(const std::string& str);
	MYDLL_DECLSPEC std::string ws2s(const std::wstring& wstr);
	//1. 이미지 바이너리 읽고 쓰기 부터 테스트
	//2. 이미지 버퍼 앞에 text 정보 쓰기
	//3.filebuffers 내용들 합쳐서 bin 파일로 저장.
	struct MYDLL_DECLSPEC FileInfo
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
	struct MYDLL_DECLSPEC PackInfo
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
	
	enum class MYDLL_DECLSPEC DataType
	{
		FILE,
		PACK,
		CSV,
	};
	class MYDLL_DECLSPEC FileSystem
	{
	public:
		FileSystem() : mAllLength(0) {}
		~FileSystem() { Release(); }
		void LoadFile(std::string _path);
		void LoadFiles(std::string _path, std::string _format);
		void SaveFile(std::string _path);
		void SaveFiles(std::string _path);
		void SaveFile(std::string _save_path,const FileInfo* _data, std::string name);
		void SaveFile(std::string _save_path, const FileInfo* _data);
		FileInfo* GetLoadFile();
		virtual void Release();
	protected:
		std::queue<FileInfo*> mFileBuffers;
		int mAllLength;
	};


	class MYDLL_DECLSPEC NPKSystem :public FileSystem
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

