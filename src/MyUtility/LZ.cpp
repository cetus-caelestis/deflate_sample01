//-------------------------------------------------------------
//! @brief	LZ77�֘A
//! @author	��ĩ�=��ڽè�
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

// @brief	�R���X�g���N�^
//-------------------------------------------------------------
LZ::LZSlideWindow::LZSlideWindow(size_t buffersize)
	:m_maxbufferSize(buffersize)
{
	m_buffer.reserve(buffersize);
	assert(m_buffer.size() == 0);
}

// @brief	�v�f�̃v�b�V��
//-------------------------------------------------------------
void LZ::LZSlideWindow::push_back(unsigned char value)
{
	// 1����
	if (m_buffer.size() < m_maxbufferSize)
	{
		m_buffer.push_back(value);
	}
	// 2���ڈȍ~
	else
	{
		m_buffer.at(m_top) = value;
		m_top = (m_top + 1) % m_maxbufferSize;
	}
}

// @brief	�����_���A�N�Z�X
// @note	�\���͂���������o�b�t�@�Ɠ���
//-------------------------------------------------------------
unsigned char& LZ::LZSlideWindow::at(unsigned index)
{
	// note; 
	// �{���̓o�b�t�@�T�C�Y���Q�̏搔�ɌŒ肷�邱�Ƃ�
	// �u%�v���u&�v�̌v�Z�ɒu����������
	return m_buffer.at((index + m_top)% m_maxbufferSize);
}
//-------------------------------------------------------------
const unsigned char& LZ::LZSlideWindow::at(size_t index) const
{
	return m_buffer.at((index + m_top) % m_maxbufferSize);
}

// @brief	����/���� �ɊY������f�[�^�p�^�[����Ԃ�
//-------------------------------------------------------------
std::vector<unsigned char> LZ::GetPattern(const LZSlideWindow& slideWnd, unsigned length, unsigned startDistance)
{
	std::vector<unsigned char> vec;
	for (unsigned i = 0; i < length; ++i)
	{
		// note:
		// ������蒷���̂ق����傫���ꍇ��
		// ����Ȃ����͌J��Ԃ��̕�����Ŗ��߂�
		unsigned distance = startDistance - (i % startDistance);
		size_t   index = slideWnd.size() - distance;

		vec.push_back(slideWnd.at(index));
	}
	return vec;
}

// @brief �f�[�^�p�^�[�����L�^����
//-------------------------------------------------------------
void LZ::PushBackPattern(LZSlideWindow* slideWnd, const std::vector<unsigned char> substr)
{
	for (const auto& value : substr)
	{
		slideWnd->push_back(value);
	}
}