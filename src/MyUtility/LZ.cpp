//-------------------------------------------------------------
//! @brief	LZ77関連
//! @author	ｹｰﾄｩｽ=ｶｴﾚｽﾃｨｽ
//-------------------------------------------------------------

//-------------------------------------------------------------
// include
//-------------------------------------------------------------
#include <assert.h>
#include "LZ.h"

//-------------------------------------------------------------
// using
//-------------------------------------------------------------
using namespace MyUtility;

// @brief	コンストラクタ
//-------------------------------------------------------------
LZ::LZSlideWindow::LZSlideWindow(size_t buffersize)
	:m_maxbufferSize(buffersize)
{
	m_buffer.reserve(buffersize);
	assert(m_buffer.size() == 0);
}

// @brief	要素のプッシュ
//-------------------------------------------------------------
void LZ::LZSlideWindow::push_back(unsigned char value)
{
	// 1周目
	if (m_buffer.size() < m_maxbufferSize)
	{
		m_buffer.push_back(value);
	}
	// 2周目以降
	else
	{
		m_buffer.at(m_top) = value;
		m_top = (m_top + 1) % m_maxbufferSize;
	}
}

// @brief	ランダムアクセス
// @note	構造はだいたい環状バッファと同じ
//-------------------------------------------------------------
unsigned char& LZ::LZSlideWindow::at(unsigned index)
{
	// note; 
	// 本当はバッファサイズを２の乗数に固定することで
	// 「%」を「&」の計算に置き換えられる
	return m_buffer.at((index + m_top)% m_maxbufferSize);
}
//-------------------------------------------------------------
const unsigned char& LZ::LZSlideWindow::at(size_t index) const
{
	return m_buffer.at((index + m_top) % m_maxbufferSize);
}

// @brief	長さ/距離 に該当するデータパターンを返す
//-------------------------------------------------------------
std::vector<unsigned char> LZ::GetPattern(const LZSlideWindow& slideWnd, unsigned length, unsigned startDistance)
{
	std::vector<unsigned char> vec;
	for (unsigned i = 0; i < length; ++i)
	{
		// note:
		// 距離より長さのほうが大きい場合は
		// 足りない分は繰り返しの文字列で埋める
		unsigned distance = startDistance - (i % startDistance);
		size_t   index = slideWnd.size() - distance;

		vec.push_back(slideWnd.at(index));
	}
	return vec;
}

// @brief データパターンを記録する
//-------------------------------------------------------------
void LZ::PushBackPattern(LZSlideWindow* slideWnd, const std::vector<unsigned char> substr)
{
	for (const auto& value : substr)
	{
		slideWnd->push_back(value);
	}
}