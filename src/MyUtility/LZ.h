//-------------------------------------------------------------
//! @brief	LZ77関連
//! @author	ｹｰﾄｩｽ=ｶｴﾚｽﾃｨｽ
//-------------------------------------------------------------
#pragma once

//-------------------------------------------------------------
// include
//-------------------------------------------------------------
#include <vector>
#include <assert.h>

namespace MyUtility
{
namespace LZ
{
//-------------------------------------------------------------
// class (LZスライド窓)
//-------------------------------------------------------------	
class LZSlideWindow
{
public:

	explicit LZSlideWindow(size_t buffersize);

	void push_back(unsigned char value);
	unsigned char& at(unsigned index);
	const unsigned char& at(size_t index) const;
	size_t size() const { return m_buffer.size();}

private:

	unsigned					m_top    = 0;
	const size_t				m_maxbufferSize;
	std::vector<unsigned char>	m_buffer;
};

//-------------------------------------------------------------
// helper function
//-------------------------------------------------------------

//! 長さ/距離 に該当するデータパターンを返す
std::vector<unsigned char> GetPattern(const LZSlideWindow&, unsigned length, unsigned startDistance);

//! データパターンを記録する
void PushBackPattern(LZSlideWindow*, const std::vector<unsigned char> substr);


}// end namespace LZ
}// end namespace MyUtility