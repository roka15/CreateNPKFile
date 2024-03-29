#include "NPKSystem.h"

namespace roka::file
{
	
	void roka::file::NPKSystem::SavePacks(std::string _save_path, std::map<std::string, PackInfo*> _pack)
	{
		for (auto& pack : _pack)
		{
			std::string result_path = _save_path;
			result_path += "\\";
			result_path += pack.first;
			if (std::filesystem::exists(result_path) == false)
			{
				std::filesystem::create_directory(result_path);
			}
			PackInfo* info = pack.second;
			for (int i = 0; i < info->binbuf.size(); i++)
			{
				FileInfo* file = info->binbuf[i];
				SaveFile(result_path, file);
			}
		}
	}

	void NPKSystem::SaveCsvs(std::string _save_path, std::map<std::string, CSVInfo*> _csv)
	{
		std::string result_path = _save_path;
		FileInfo* file = new FileInfo();
		std::string txt;
		char* num2str = new char[255];
		for (auto& csv : _csv)
		{
			 int size = csv.second->canvas.size();
			 bool sameFlag = true;
			 for (int i = 1; i < size; i++)
			 {
				 if (csv.second->canvas[0].first != csv.second->canvas[i].first
					 && csv.second->canvas[0].second != csv.second->canvas[i].second)
					 sameFlag = false;
			 }
			 txt+=csv.first;
			 txt += '\'';
		
			 sprintf(num2str, "%d", size);
			 txt += num2str;
			 memset(num2str,'\0', 255);
			 txt += '\'';
			 sprintf(num2str, "%d",(int)sameFlag);
			 txt += num2str;
			 memset(num2str, '\0', 255);
			 txt += '\'';
			 if (sameFlag == true)
			 {
				 sprintf(num2str, "%d", csv.second->canvas[0].first);
				 txt += num2str;
				 memset(num2str, '\0', 255);
				 txt += '\'';
				 sprintf(num2str, "%d", csv.second->canvas[0].second);
				 txt += num2str;
				 memset(num2str, '\0', 255); 
				 txt += '\'';
			 }
			 for (int i = 0; i < size; i++)
			 {
				 if (sameFlag == false)
				 {
					 sprintf(num2str, "%d", csv.second->canvas[i].first);
					 txt += num2str;
					 memset(num2str, '\0', 255);
					 txt += '\'';
					 sprintf(num2str, "%d", csv.second->canvas[0].second);
					 txt += num2str;
					 memset(num2str, '\0', 255);
					 txt += '\'';
				 }
				 sprintf(num2str, "%d", csv.second->pos[i].first);
				 txt += num2str;
				 memset(num2str, '\0', 255);
				 txt += '\'';
				 sprintf(num2str, "%d", csv.second->pos[i].second);
				 txt += num2str;
				 memset(num2str, '\0', 255);
				 txt += '\'';
				 sprintf(num2str, "%d", csv.second->size[i].first);
				 txt += num2str;
				 memset(num2str, '\0', 255);
				 txt += '\'';
				 sprintf(num2str, "%d", csv.second->size[i].second);
				 txt += num2str;
				 memset(num2str, '\0', 255);
				 if(i!=size-1)
					 txt += '\'';
			 }
			 txt += '\n';
		}
		delete []num2str;
		file->buffer = new char[txt.size()+1];
		memset(file->buffer, '\0', txt.size());
		strcpy(file->buffer, txt.c_str());
		file->length = txt.size();
		SaveFile(result_path, file);
		delete file;
	}

	const roka::file::FileInfo* roka::file::NPKSystem::CreateImagePackage(std::string _read_path, std::string _road_format)
	{
		LoadFiles(_read_path, _road_format);

		std::string folder_name = std::filesystem::path(_read_path).filename().string();

		FileInfo* csv_line = CreateCSVLineBuffer();
		int csv_size = csv_line->length;

		int size = mAllLength + (sizeof(int) * 3) + csv_size;
		int image_cnt = mFileBuffers.size();

		if (image_cnt == 0)
			return nullptr;

		char* npk_buffer = new char[size];
		memset(npk_buffer, '\0', size);
		memcpy(npk_buffer, &size, sizeof(int));
		char* point = npk_buffer + sizeof(int);

		memcpy(point, csv_line->buffer, csv_size);
		point += csv_size;

		delete csv_line;

		memcpy(point, &image_cnt, sizeof(int));
		point += sizeof(int);



		while (mFileBuffers.empty() == false)
		{
			FileInfo* file = GetLoadFile();

			std::string filename = file->name;
			int length = file->length;
			const char* image_bin = file->buffer;

			int filename_size = filename.length();
			memcpy(point, &filename_size, sizeof(int));
			point += sizeof(int);
			memcpy(point, filename.c_str(), filename_size);
			point += filename_size;
			memcpy(point, &length, sizeof(int));
			point += sizeof(int);
			memcpy(point, image_bin, length);
			point += length;

			delete file;
		}
		FileInfo* info = new FileInfo("", size, npk_buffer);
		mAllLength = 0;
		return info;
	}

	size_t roka::file::NPKSystem::ReadImagePackage(const char* _buf, std::map<std::string, CSVInfo*>& _csvmap, std::map<std::string, PackInfo*>& _packmap)
	{
		const char* buf = _buf;
		const char* point = buf;
		char* filename = nullptr;
		int filename_size;
		int filesize = 0;
		int image_cnt = 0;
		memcpy(&filesize, point, sizeof(int));
		point += sizeof(int);
		std::string folder_name;
		size_t read_size = ReadCSVLine(point, _csvmap, folder_name);
		point += read_size;

		memcpy(&image_cnt, point, sizeof(int));
		point += sizeof(int);

		PackInfo* pack_info = new PackInfo();
		for (int i = 0; i < image_cnt; i++)
		{
			FileInfo* info = new FileInfo();
			memcpy(&filename_size, point, sizeof(int));
			point += sizeof(int);
			filename = new char[filename_size + 1];
			filename[filename_size] = '\0';
			memcpy(filename, point, filename_size);
			point += filename_size;

			int imagesize = 0;
			char* image_buf = nullptr;
			memcpy(&imagesize, point, sizeof(int));
			point += sizeof(int);
			image_buf = new char[imagesize];
			memcpy(image_buf, point, imagesize);
			point += imagesize;

			info->length = imagesize;
			info->buffer = image_buf;
			info->name = filename;

			delete []filename;

			pack_info->binbuf.push_back(info);
		}

		pack_info->name = folder_name;
		_packmap.insert(std::make_pair(folder_name, pack_info));
		return filesize;
	}

	void roka::file::NPKSystem::CreateNPK(std::string _image_path, std::string _txt_path, std::string _format, std::string _save_path)
	{
		size_t npk_size = 0;
		OpenCSV(_txt_path);
		for (auto& folder : std::filesystem::recursive_directory_iterator(_image_path))
		{
			std::string path = _image_path;
			std::string file_name = folder.path().filename().string();
			if (folder.is_directory() == false)
				continue;
			else
			{
				path += "\\";
				path += file_name;

				const FileInfo* Images = CreateImagePackage(path, _format);
				if (Images == nullptr)
					continue;
				npk_size += Images->length;

				mImagePackBuffers.push((FileInfo*)Images);
			}
		}
		size_t image_cnt = 0;
		image_cnt = mImagePackBuffers.size();

		npk_size += sizeof(int) * 2;
		FileInfo* npkfile = new FileInfo();
		npkfile->length = npk_size;
		npkfile->buffer = new char[npk_size];
		memset(npkfile->buffer, '\0', npk_size);
		char* npk_buff = npkfile->buffer;
		char* point = npk_buff;

		memcpy(point, &npk_size, sizeof(int));
		point += sizeof(int);
		memcpy(point, &image_cnt, sizeof(int));
		point += sizeof(int);

		while (mImagePackBuffers.empty() == false)
		{
			FileInfo* image_pack = mImagePackBuffers.front();
			mImagePackBuffers.pop();
			memcpy(point, image_pack->buffer, image_pack->length);
			point += image_pack->length;

			delete image_pack;
		}

		//save
		SaveFile(_save_path, npkfile);

		delete npkfile;
	}

	void roka::file::NPKSystem::ReadNPK(std::string _path, std::map<std::string, CSVInfo*>& _csvmap, std::map<std::string, PackInfo*>& _packmap)
	{
		//1. npk buf 읽어오기
		LoadFile(_path);
		FileInfo* file = GetLoadFile();
		char* npkbuf = file->buffer;

		char* point = npkbuf;

		//2. buf 총 size , image pack 갯수 추출
		int buf_size = 0;
		int image_packege_cnt = 0;

		memcpy(&buf_size, point, sizeof(int));
		point += sizeof(int);
		memcpy(&image_packege_cnt, point, sizeof(int));
		point += sizeof(int);

		//3. image pack cnt 만큼 for 순회하며 image package들 정보 뜯기
		for (int i = 0; i < image_packege_cnt; i++)
		{
			point += ReadImagePackage(point, _csvmap, _packmap);
		}
		delete file;
	}

	void roka::file::NPKSystem::OpenCSV(std::string _path)
	{
		if (mCSVBuffers != nullptr)
		{
			delete mCSVBuffers;
		}

		LoadFile(_path);
		mCSVBuffers = GetLoadFile();
	}

	roka::file::FileInfo* roka::file::NPKSystem::CreateCSVLineBuffer()
	{
		std::string temp = mCSVBuffers->buffer;
		int start_index = mCsvLine;
		int linesize = temp.find("\n", start_index);
		bool CanvasSameFlag = false;
		std::string base_buf = mCSVBuffers->buffer;
		std::string get_data;

		int get_index = 0;
		std::string file_name;
		int int_data_cnt = 0;

		get_index = base_buf.find("'", start_index);
		file_name = base_buf.substr(start_index, get_index - start_index);
		start_index = get_index + 1;

		get_index = base_buf.find("'", start_index);
		int_data_cnt = atoi(base_buf.substr(start_index, get_index - start_index).c_str());
		start_index = get_index + 1;

		get_index = base_buf.find("'", start_index);
		CanvasSameFlag = atoi(base_buf.substr(start_index, get_index - start_index).c_str());
		start_index = get_index + 1;


		int ElementCnt = CanvasSameFlag == 0 ? 24 : 16;
		int PlusCnt = CanvasSameFlag == 0 ? 0 : 2;
		int bufsize = (int_data_cnt * ElementCnt) 
			+ (sizeof(int)*(2/*filesize,datacnt*/ + PlusCnt/*same base*/)) + file_name.length()+1/*bool*/;
		char* buf = new char[bufsize];

		char* point = buf;
		FileInfo* result = new FileInfo();
		result->name = file_name;
		result->buffer = buf;
		result->length = bufsize;

		memset(buf, '\0', bufsize);

		int strsize = file_name.length();

		memcpy(point, &strsize, sizeof(int));
		point += sizeof(int);

		memcpy(point, file_name.c_str(), strsize);
		point += strsize;

		memcpy(point, &int_data_cnt, sizeof(int));
		point += sizeof(int);

		memcpy(point, &CanvasSameFlag, 1);
		point += 1;

		int base_size_x = 0;
		int base_size_y = 0;

		if (CanvasSameFlag == true)
		{
			get_index = base_buf.find("'", start_index);
			base_size_x = atoi(base_buf.substr(start_index, get_index - start_index).c_str());
			start_index = get_index + 1;
			memcpy(point, &base_size_x, sizeof(int));
			point += sizeof(int);

			get_index = base_buf.find("'", start_index);
			base_size_y = atoi(base_buf.substr(start_index, get_index - start_index).c_str());
			start_index = get_index + 1;
			memcpy(point, &base_size_y, sizeof(int));
			point += sizeof(int);
		}
		else
		{
			int a = 0;
		}


		for (int i = 0; i < ((int_data_cnt)*(ElementCnt/4)); i++)
		{
			int data = 0;
			get_index = base_buf.find("'", start_index);
			data = atoi(base_buf.substr(start_index, get_index - start_index).c_str());
			start_index = get_index + 1;

			memcpy(point, &data, sizeof(int));
			point += sizeof(int);
		}
		mCsvLine = (linesize + 1);
		return result;
	}



	size_t roka::file::NPKSystem::ReadCSVLine(const char* buf, std::map<std::string, CSVInfo*>& _csvmap, std::string& _out_str)
	{
		CSVInfo* csv = new CSVInfo();

		int size = 0;
		char* point = (char*)buf;

		int strsize = 0;
		char* filename;

		memcpy(&strsize, point, sizeof(int));
		point += sizeof(int);
		size += sizeof(int);

		filename = new char[strsize + 1];
		filename[strsize] = '\0';
		memcpy(filename, point, strsize);
		point += strsize;
		size += strsize;

		int cnt = 0;
		memcpy(&cnt, point, sizeof(int));
		point += sizeof(int);
		size += sizeof(int);

		int sameflag = false;
		memcpy(&sameflag, point, 1);
		point += 1;
		size += 1;


		int base_x = 0;
		int base_y = 0;

		if (sameflag == true)
		{
			memcpy(&base_x, point, sizeof(int));
			point += sizeof(int);
			size += sizeof(int);

			memcpy(&base_y, point, sizeof(int));
			point += sizeof(int);
			size += sizeof(int);
		}


		for (int i = 0; i < (cnt); i++)
		{
			//test 용 
			int x = 0;
			int y = 0;
			int sizex = 0;
			int sizey = 0;
			if (sameflag == false)
			{
				memcpy(&base_x, point, sizeof(int));
				point += sizeof(int);
				size += sizeof(int);

				memcpy(&base_y, point, sizeof(int));
				point += sizeof(int);
				size += sizeof(int);
			}
			
			csv->canvas.push_back(std::make_pair(base_x, base_y));
			memcpy(&x, point, sizeof(int));
			point += sizeof(int);
			size += sizeof(int);
			memcpy(&y, point, sizeof(int));
			point += sizeof(int);
			size += sizeof(int);
			memcpy(&sizex, point, sizeof(int));
			point += sizeof(int);
			size += sizeof(int);
			memcpy(&sizey, point, sizeof(int));
			point += sizeof(int);
			size += sizeof(int);
			csv->pos.push_back(std::make_pair(x, y));
			csv->size.push_back(std::make_pair(sizex, sizey));
		}

		csv->name = filename;
		delete []filename;
		_out_str = csv->name;
		_csvmap.insert(std::make_pair(csv->name, csv));
		return size;
	}

	void NPKSystem::Release()
	{
		if (mCSVBuffers != nullptr)
		{
			delete mCSVBuffers;
			mCSVBuffers = nullptr;
		}
		while (mImagePackBuffers.empty() == false)
		{
			FileInfo* file = mImagePackBuffers.front();
			mImagePackBuffers.pop();
			delete file;
		}
	}

	void NPKSystem::Clear()
	{
		Release();
		FileSystem::Release();
	}

}
