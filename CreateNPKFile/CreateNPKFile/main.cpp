#include "FileSystem.h"
#include "NPKSystem.h"
#include <cstdlib>
#include <crtdbg.h>
int main()
{
	//_CrtSetBreakAlloc(14665);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//roka::file::FileSystem file;
	//file.LoadFile("..\\NPKTestFile\\image\\1\\0005.bmp");
	//file.SaveFile("..\\NPKTestFile\\Result\\testImageBin.txt");
	/*roka::file::FileSystem::LoadFile("..\\NPKTestFile\\Result\\testImageBin.txt");
	roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\testImageBin.bmp");*/

	//폴더 단위 파일 읽어와서 복사 저장.

	//roka::file::FileSystem::LoadFiles("..\\NPKTestFile\\image\\test1",".bmp",roka::file::DataType::FILE);
	//roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result\\test1");


	//bin 로 읽어와서 bmp로 저장.
	/*roka::file::FileSystem::LoadFiles("..\\NPKTestFile\\Result\\test1", ".txt");
	roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result\\test1", ".bmp");*/

	//create npk - image 들만 
	//const roka::file::FileInfo* npk_info = roka::file::FileSystem::CreateNPK("..\\NPKTestFile\\image\\test1", ".bmp", "..\\NPKTestFile\\Result\\test1");
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result", npk_info, "test.npk");

	//npk read
	//roka::file::FileSystem::ReadNPK("..\\NPKTestFile\\Result\\test.npk");
	//
	//roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result\\test1", ".bmp");

	//create npk -image + txt
	//roka::file::FileSystem::LoadFile("..\\NPKTestFile\\text\\arsmagna.txt", roka::file::DataType::CSV);
	//const roka::file::FileInfo* npk_info = roka::file::FileSystem::CreateNPK("..\\NPKTestFile\\image\\test1", ".bmp", "..\\NPKTestFile\\Result\\test1");
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result", npk_info, "test.npk");

	//npk와 image package 분리 작업 - 상위 폴더 입력 받으면 하위 폴더 1개 당 1개의 image package 정보 생성.
	//                               이 image package 들을 모아서 하나의 npk로 만들기.
	//roka::file::FileSystem::CreateNPK("..\\NPKTestFile\\image",".bmp","..\\NPKTestFile\\Result\\test2.npk");


	//npk load
	//roka::file::FileSystem::ReadNPK("..\\NPKTestFile\\Result\\test2.npk");
	//roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result");


	//csv test binary 줄단위로 쓰기 test
	//roka::file::FileSystem::OpenCSV("..\\NPKTestFile\\text\\arsmagna.txt");
	//roka::file::FileInfo* file = roka::file::FileSystem::CreateCSVLineBuffer();
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\csvtest1.txt", file);

	//file = roka::file::FileSystem::CreateCSVLineBuffer();
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\csvtest2.txt", file);




	//csv binary 읽기
	//roka::file::FileSystem::OpenCSV("..\\NPKTestFile\\Result\\csvtest1.txt");
	//roka::file::FileSystem::ReadCSVLine();
	//roka::file::FileSystem::OpenCSV("..\\NPKTestFile\\Result\\csvtest2.txt");
	//roka::file::FileSystem::ReadCSVLine();


	//npk - csv,image 쓰기
	//std::map<std::string, roka::file::CSVInfo*> csvmap;
	//std::map<std::string, roka::file::PackInfo*> packmap;
	/*roka::file::FileSystem::CreateNPK("..\\NPKTestFile\\image","..\\NPKTestFile\\text\\arsmagna.txt",".bmp","..\\NPKTestFile\\Result\\csvimage.npk");

	roka::file::FileSystem::ReadNPK("..\\NPKTestFile\\Result\\csvimage.npk",csvmap,packmap);

	roka::file::FileSystem::SavePacks("..\\NPKTestFile\\Result\\myTest",packmap);
	*/
	//roka::file::FileSystem::Release();

	//filesystem과 npksystem 분리
	roka::file::NPKSystem npk;

	npk.CreateNPK("..\\NPKTestFile\\Image Group\\baseskin", "..\\NPKTestFile\\text\\baseskin.txt", ".png", "..\\NPKTestFile\\Result\\baseskin.npk");
	npk.Clear();
	std::map<std::string, roka::file::CSVInfo*> csvmap;
	std::map<std::string, roka::file::PackInfo*> packmap;
	/*npk.ReadNPK("..\\NPKTestFile\\Result\\arsmagna.npk", csvmap, packmap);
	for (auto item : csvmap)
	{
		delete (item.second);
	}
	for (auto item : packmap)
	{
		delete (item.second);
	}
	csvmap.clear();
	packmap.clear();*/
	npk.ReadNPK("..\\NPKTestFile\\Result\\baseskin.npk", csvmap, packmap);
	npk.SavePacks("..\\NPKTestFile\\Result\\myTest", packmap);
	for (auto& item : csvmap)
	{
		delete (item.second);
	}
	for (auto& item : packmap)
	{
		delete (item.second);
	}

	//npk.SavePacks("..\\NPKTestFile\\Result\\myTest", packmap);
}
