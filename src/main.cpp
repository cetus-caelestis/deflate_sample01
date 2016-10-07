#include <iostream>
#include <fstream>
#include <string>
#include "MyUtility/Deflate.h"

int main()
{
	using namespace MyUtility;
	try
	{
		// 固定ハフマン "aaaaa"
		const char codedData[] = "\x4B\x4C\x04\x02\x00";

		// ここからDeflateデータのパース処理
		auto data = Deflate::Decode(codedData, std::size(codedData));

		// 結果の出力
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

