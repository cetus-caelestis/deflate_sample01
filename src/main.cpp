#include <iostream>
#include <fstream>
#include <string>
#include "MyUtility/Deflate.h"

int main()
{
	using namespace MyUtility;
	try
	{
		// �Œ�n�t�}�� "aaaaa"
		const char codedData[] = "\x4B\x4C\x04\x02\x00";

		// ��������Deflate�f�[�^�̃p�[�X����
		auto data = Deflate::Decode(codedData, std::size(codedData));

		// ���ʂ̏o��
		std::string resultStr(data.data(), data.size());
		std::cout << resultStr << std::endl;
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what();
	}
	catch (...)
	{

	}
	getchar();
	return 0;
}

