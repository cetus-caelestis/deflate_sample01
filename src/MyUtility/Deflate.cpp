//-------------------------------------------------------------
//! @brief	�Ȉ�Deflate����
//! @author	��ĩ�=��ڽè�
//-------------------------------------------------------------

//-------------------------------------------------------------
// include
//-------------------------------------------------------------
#include <exception>
#include <iostream>

#include "PrefixCodeTree.h"
#include "LZ.h"
#include "Deflate.h"

//-------------------------------------------------------------
// using
//-------------------------------------------------------------
using namespace MyUtility;

namespace
{
//-------------------------------------------------------------
// inner class
//-------------------------------------------------------------
class DeflateBitStream : public PrefixC::IbitStream
{
public:
	explicit DeflateBitStream(const char* binary, size_t numByte)
		:m_binary(binary)
		,m_numByte(numByte)
	{}

	//! 1�r�b�g���[�h����
	IbitStream& operator >> (int& out) override
	{
		out = Get();
		return *this;
	}
	//! �I�[��
	bool Eof() const noexcept override
	{
		return m_nextByte >= m_numByte;
	}
	//! �P�r�b�g���[�h (�߂�l�ɒl��Ԃ�)
	unsigned Get()
	{
		unsigned bit = GetBitImpl();
		Next();
		return bit;
	}

	//! �r�b�g������[�h
	//! �����ł͂Ȃ��f�[�^�� �v�f�̍ŉ��ʃr�b�g���珇�Ƀp�b�N����Ă���
	unsigned GetRange(unsigned numbit)
	{
		unsigned bit = 0;
		for(unsigned i = 0; i < numbit; ++i)
		{
			bit |= (Get() << i);
		}
		return bit;
	}
	//! �r�b�g������[�h
	//! ���������ꂽ�f�[�^�͍ŏ�ʃr�b�g���珇�Ƀp�b�N����Ă���
	unsigned GetCodedRange(unsigned numbit)
	{
		unsigned bit = 0;
		for (unsigned i = 0; i < numbit; ++i)
		{
			bit <<= 1;
			bit |= Get();
		}
		return bit;
	}

private:

	//! ���݌��Ă���bit�𔲂��o��
	unsigned GetBitImpl() const
	{
		const unsigned byte		  = m_binary[m_nextByte];
		const unsigned maskedByte = byte & (0x01 << m_nextBit);

		return (maskedByte == 0) ? 0 : 1;
	}

	//! ���̃r�b�g�ɃV�t�g
	void Next() noexcept
	{
		++m_nextBit;
		if (m_nextBit >= 8)
		{
			m_nextBit = 0;
			++m_nextByte;
		}
	}

	// note �ǂނ����ŁA�������m�ۂ͂��Ȃ����Ƃɂ���
	// note �o�C�i���f�[�^�̐������Ԃɒ���
	const char* m_binary;
	size_t		m_numByte;

	unsigned	m_nextBit  = 0;	// ���ɓǂ�bit
	size_t		m_nextByte = 0;	// ���ɓǂ�Byte
};

//-------------------------------------------------------------
// inner function
//-------------------------------------------------------------

// @brief �g���r�b�g�f�[�^�̓ǂݏo��
//-------------------------------------------------------------
unsigned ReadExValue(DeflateBitStream& bitstream, unsigned baseVal, unsigned exBit)
{
	if (exBit == 0) return baseVal;
	return baseVal + bitstream.GetRange(exBit);
}

// @brief �X���C�h������q�؂���p�^�[���̒�������ǂݏo��
//-------------------------------------------------------------
unsigned ReadLengthCode(unsigned code, DeflateBitStream& bitstream)
{
	const unsigned CODE_BEGIN = 257;
	const unsigned CODE_END = 286;
	assert(code >= CODE_BEGIN && code < CODE_END);

	// �e�[�u���̐錾
	const std::pair<unsigned, unsigned> CODE_TABLE[] =
	{
		// �x�[�X�l / �g���r�b�g��
		std::make_pair(3,	0),
		std::make_pair(4,	0),
		std::make_pair(5,	0),
		std::make_pair(6,	0),
		std::make_pair(7,	0),
		std::make_pair(8,	0),
		std::make_pair(9,	0),
		std::make_pair(10,	0),
		std::make_pair(11,	1),
		std::make_pair(13,	1),
		std::make_pair(15,	1),
		std::make_pair(17,	1),
		std::make_pair(19,	2),
		std::make_pair(23,	2),
		std::make_pair(27,	2),
		std::make_pair(31,	2),
		std::make_pair(35,	3),
		std::make_pair(43,	3),
		std::make_pair(51,	3),
		std::make_pair(59,	3),
		std::make_pair(67,	4),
		std::make_pair(83,	4),
		std::make_pair(99,	4),
		std::make_pair(115,	4),
		std::make_pair(131,	5),
		std::make_pair(163,	5),
		std::make_pair(195,	5),
		std::make_pair(227,	5),
		std::make_pair(258,	0),
	};
	auto info = CODE_TABLE[code - CODE_BEGIN];
	return ReadExValue(bitstream, info.first, info.second);
}

// @brief �X���C�h���̎Q�ƊJ�n�n�_(����)�̏���ǂݏo��
//-------------------------------------------------------------
unsigned ReadDistanceCode(unsigned code, DeflateBitStream& bitstream)
{
	const unsigned CODE_END = 30;
	assert(code < CODE_END);

	// �e�[�u���̐錾
	const std::pair<unsigned, unsigned> CODE_TABLE[] =
	{
		// �x�[�X�l / �g���r�b�g��
		std::make_pair(1,		0),
		std::make_pair(2,		0),
		std::make_pair(3,		0),
		std::make_pair(4,		0),
		std::make_pair(5,		1),
		std::make_pair(7,		1),
		std::make_pair(9,		2),
		std::make_pair(13,		2),
		std::make_pair(17,		3),
		std::make_pair(25,		3),
		std::make_pair(33,		4),
		std::make_pair(49,		4),
		std::make_pair(65,		5),
		std::make_pair(97,		5),
		std::make_pair(129,		6),
		std::make_pair(193,		6),
		std::make_pair(257,		7),
		std::make_pair(385,		7),
		std::make_pair(513,		8),
		std::make_pair(769,		8),
		std::make_pair(1025,	9),
		std::make_pair(1537,	9),
		std::make_pair(2049,	10),
		std::make_pair(3073,	10),
		std::make_pair(4097,	11),
		std::make_pair(6145,	11),
		std::make_pair(8193,	12),
		std::make_pair(12289,	12),
		std::make_pair(16385,	13),
		std::make_pair(24577,	13),
	};
	auto info = CODE_TABLE[code];
	return ReadExValue(bitstream, info.first, info.second);
}

//@brief �Œ胊�e�����n�t�}���c���[���쐬
//-------------------------------------------------------------
PrefixCodeTree MakeFixedHuffmanTree()
{
	PrefixCodeTree fixedTree;

	// 0 - 143 -> 8bit
	// [0011 0000] �` [10111111]
	for (int i = 0; i <= 143; ++i)
	{
		int key = 0x30 + i;
		fixedTree.Entry<8>(key, i);
	}
	// 144 - 255 -> 9bit
	// [110010000] �` [111111111]
	for (int i = 0; i <= (255 - 144); ++i)
	{
		int key = 0x190 + i;
		fixedTree.Entry<9>(key, i + 144);
	}
	// 256 - 279 -> 7bit
	// [0000000] �` [0010111]
	for (int i = 0; i <= (279 - 256); ++i)
	{
		int key = 0x00 + i;
		fixedTree.Entry<7>(key, i + 256);
	}
	// 280 - 287 -> 8bit
	// [11000000] �` [11000111]
	for (int i = 0; i <= (287 - 280); ++i)
	{
		int key = 0xC0 + i;
		fixedTree.Entry<8>(key, i + 280);
	}
	return fixedTree;
}

//@brief �Œ�n�t�}�������ɂ��p�[�X����
//-------------------------------------------------------------
void DecodeWithFixedHuffman(DeflateBitStream& bitstream, LZ::LZSlideWindow& slideWnd, std::vector<char>* resultbuffer)
{
	// �Œ�n�t�}���c���[�쐬
	auto tree = MakeFixedHuffmanTree();

	// �n�t�}������ -> (0 �` 286)
	unsigned val;
	while (PrefixC::Decode(bitstream, tree, &val))
	{
		// �I�[
		if (val == 256)
		{
			break;
		}
		// �l���̂܂�
		if (val <= 255)
		{
			resultbuffer->push_back(static_cast<char>(val));
			slideWnd.push_back(static_cast<char>(val));
			continue;
		}
		// if (val > 256)

		// �������
		unsigned length = ReadLengthCode(val, bitstream);

		// ������� (5bit�Œ�)
		auto exVal = bitstream.GetCodedRange(5);
		unsigned distance = ReadDistanceCode(exVal, bitstream);

		// ��v�����l�p�^�[���𒊏o
		auto valPattern = LZ::GetPattern(slideWnd, length, distance);

		resultbuffer->insert(resultbuffer->end(), valPattern.begin(), valPattern.end());
		LZ::PushBackPattern(&slideWnd, valPattern);
	}
}

} // end namespace


// @brief �f�R�[�h����
//-------------------------------------------------------------	
std::vector<char> MyUtility::Deflate::Decode(const char* binary, size_t numByte)
{
	DeflateBitStream	bitstream(binary, numByte);
	LZ::LZSlideWindow	slideWnd(32768);

	std::vector<char> result;

	while (!bitstream.Eof())
	{
		bool isLast = (bitstream.Get() == 1);
		int  type   = bitstream.GetRange(2);

		switch (type)
		{
		case 0:
			throw std::runtime_error("�񈳏k�^�C�v�͖��Ή�");
		case 1:
			DecodeWithFixedHuffman(bitstream, slideWnd, &result); break;
		case 2:
			throw std::runtime_error("�J�X�^���n�t�}���^�C�v�͖��Ή�");
		case 3:
			throw std::runtime_error("�悭�킩��Ȃ��f�[�^������");
		}
		if (isLast) 
			break;
	}
	return result;
}