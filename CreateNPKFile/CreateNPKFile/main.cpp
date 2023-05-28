#include "FileSystem.h"
#include "NPKSystem.h"
#include <cstdlib>
#include <crtdbg.h>
int main()
{
	//_CrtSetBreakAlloc(2214);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//roka::file::FileSystem::LoadFile("..\\NPKTestFile\\image\\test1\\0005.bmp");
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\testImageBin.txt");
	/*roka::file::FileSystem::LoadFile("..\\NPKTestFile\\Result\\testImageBin.txt");
	roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\testImageBin.bmp");*/

	//���� ���� ���� �о�ͼ� ���� ����.

	//roka::file::FileSystem::LoadFiles("..\\NPKTestFile\\image\\test1",".bmp",roka::file::DataType::FILE);
	//roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result\\test1");


	//bin �� �о�ͼ� bmp�� ����.
	/*roka::file::FileSystem::LoadFiles("..\\NPKTestFile\\Result\\test1", ".txt");
	roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result\\test1", ".bmp");*/

	//create npk - image �鸸 
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

	//npk�� image package �и� �۾� - ���� ���� �Է� ������ ���� ���� 1�� �� 1���� image package ���� ����.
	//                               �� image package ���� ��Ƽ� �ϳ��� npk�� �����.
	//roka::file::FileSystem::CreateNPK("..\\NPKTestFile\\image",".bmp","..\\NPKTestFile\\Result\\test2.npk");


	//npk load
	//roka::file::FileSystem::ReadNPK("..\\NPKTestFile\\Result\\test2.npk");
	//roka::file::FileSystem::SaveFiles("..\\NPKTestFile\\Result");
	

	//csv test binary �ٴ����� ���� test
	//roka::file::FileSystem::OpenCSV("..\\NPKTestFile\\text\\arsmagna.txt");
	//roka::file::FileInfo* file = roka::file::FileSystem::CreateCSVLineBuffer();
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\csvtest1.txt", file);
	
	//file = roka::file::FileSystem::CreateCSVLineBuffer();
	//roka::file::FileSystem::SaveFile("..\\NPKTestFile\\Result\\csvtest2.txt", file);
	

	

	//csv binary �б�
	//roka::file::FileSystem::OpenCSV("..\\NPKTestFile\\Result\\csvtest1.txt");
    //roka::file::FileSystem::ReadCSVLine();
	//roka::file::FileSystem::OpenCSV("..\\NPKTestFile\\Result\\csvtest2.txt");
	//roka::file::FileSystem::ReadCSVLine();


	//npk - csv,image ����
	//std::map<std::string, roka::file::CSVInfo*> csvmap;
	//std::map<std::string, roka::file::PackInfo*> packmap;
	/*roka::file::FileSystem::CreateNPK("..\\NPKTestFile\\image","..\\NPKTestFile\\text\\arsmagna.txt",".bmp","..\\NPKTestFile\\Result\\csvimage.npk");

	roka::file::FileSystem::ReadNPK("..\\NPKTestFile\\Result\\csvimage.npk",csvmap,packmap);

	roka::file::FileSystem::SavePacks("..\\NPKTestFile\\Result\\myTest",packmap);
	*/
	//roka::file::FileSystem::Release();

	//filesystem�� npksystem �и�
	roka::file::NPKSystem npk;
	
	npk.CreateNPK("..\\NPKTestFile\\image", "..\\NPKTestFile\\text\\arsmagna.txt", ".bmp", "..\\NPKTestFile\\Result\\csvimage.npk");
	std::map<std::string, roka::file::CSVInfo*> csvmap;
	std::map<std::string, roka::file::PackInfo*> packmap;
	npk.ReadNPK("..\\NPKTestFile\\Result\\csvimage.npk", csvmap, packmap);

    
	//npk.SavePacks("..\\NPKTestFile\\Result\\myTest", packmap);
}