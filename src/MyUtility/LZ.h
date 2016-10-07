//-------------------------------------------------------------
//! @brief	LZ77�֘A
//! @author	��ĩ�=��ڽè�
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
// class (LZ�X���C�h��)
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

//! ����/���� �ɊY������f�[�^�p�^�[����Ԃ�
std::vector<unsigned char> GetPattern(const LZSlideWindow&, unsigned length, unsigned startDistance);

//! �f�[�^�p�^�[�����L�^����
void PushBackPattern(LZSlideWindow*, const std::vector<unsigned char> substr);


}// end namespace LZ
}// end namespace MyUtility