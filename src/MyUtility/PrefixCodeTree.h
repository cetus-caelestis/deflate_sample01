//-------------------------------------------------------------
//! @brief	�ړ������c���[
//! @author	��ĩ�=��ڽè�
//-------------------------------------------------------------
#pragma once

//-------------------------------------------------------------
// include
//-------------------------------------------------------------
#include <vector>
#include <bitset>
#include <array>

namespace MyUtility
{
namespace PrefixC
{
//-------------------------------------------------------------
// constant
//-------------------------------------------------------------
using index_t		  = int;
using ChildIndexArray = std::array<index_t, 2>;

static constexpr index_t invalidIndex = -1;

//-------------------------------------------------------------
// class (�ړ������c���[)
//-------------------------------------------------------------
template<typename T>
class BasicPrefixCTree
{
public:

	//! �m�[�h�N���X
	class Node;

	//! �c���[�����ǂ�N���X
	class TreeWalker;	

	//! �����_���A�N�Z�X
	template<unsigned numBit>
	Node& operator[] (std::bitset<numBit> key);

	//! �����Ƃ���ɑΉ�����l��o�^����
	template<unsigned numBit>
	bool Entry(std::bitset<numBit> bitset, T value);

	//! �R���X�g���N�^
	BasicPrefixCTree()
	{
		//! �擪�m�[�h�̍쐬
		m_nodeList.push_back(Node{});
		m_nodeTop = 0;
	}

private:

	std::vector<Node>		m_nodeList;
	index_t					m_nodeTop = invalidIndex;

	template<unsigned numBit>
	std::pair<Node&, bool> CreateNodeIfNotFound(std::bitset<numBit> bitset);
};

//-------------------------------------------------------------
// helpler class (�ړ������c���[�̃m�[�h)
//-------------------------------------------------------------
template<typename T>
class BasicPrefixCTree<T>::Node
{
	friend class BasicPrefixCTree<T>;

public:
	//! �����o�A�N�Z�X
	T    GetValue() const noexcept { return m_value; }
	void SetValue(T val) noexcept;

	//! ���̃m�[�h���l�������Ă��邩
	bool HasValue() const noexcept { return m_hasvalue; }

	//! �q�ւ�Index��Ԃ�
	index_t ChildIndex(int bit) const;

private:

	T				m_value = T{};
	bool			m_hasvalue = false;
	ChildIndexArray	m_childIndex{ invalidIndex ,invalidIndex };
};

//-------------------------------------------------------------
// helpler class (�c���[�����ǂ�N���X)
//-------------------------------------------------------------
template<typename T>
class BasicPrefixCTree<T>::TreeWalker
{
	friend class BasicPrefixCTree<T>;

public:

	//! ���ݎw���Ă���m�[�h��Ԃ�
	const typename BasicPrefixCTree<T>::Node& Get() const;

	//! ���ݎw���Ă���m�[�h�փA�N�Z�X
	const typename BasicPrefixCTree<T>::Node* operator->() const { return &Get(); }

	//! �w�肵�������Ɏq�����݂��邩
	bool HasChild(int bit) const;

	//! �ړ�
	bool Next(int nextBit);

	//! �R���X�g���N�^
	TreeWalker(const BasicPrefixCTree& tree)
		:m_tree(tree)
		, m_current(tree.m_nodeTop)
	{}

private:

	const BasicPrefixCTree&	m_tree;
	index_t					m_current;


	index_t GetNextIndex(int bit) const;
};

//-------------------------------------------------------------
// interface (�r�b�g�X�g���[���C���^�[�t�F�[�X)
//-------------------------------------------------------------
class IbitStream
{
public:
	//! 1�r�b�g���[�h����
	virtual IbitStream& operator >> (int& out) = 0;

	//! �I�[��
	virtual bool Eof() const = 0;

	virtual ~IbitStream() {};
};

//-------------------------------------------------------------
// helpler function
//-------------------------------------------------------------

// @brief �r�b�g����P�P�ʃf�R�[�h����
//-------------------------------------------------------------
template<typename T>
inline bool Decode(IbitStream& stream, const BasicPrefixCTree<T>& tree, T* out)
{
	auto walker = BasicPrefixCTree<T>::TreeWalker(tree);
	while(!stream.Eof())
	{
		int bit;
		stream >> bit;

		// �Ή����镄����������Ȃ�
		if (walker.HasChild(bit) == false) break;

		walker.Next(bit);

		// ��������
		if (walker->HasValue())
		{
			*out = walker->GetValue();
			return true;		
		}
	}
	return false;
}

//-------------------------------------------------------------
// alias
//-------------------------------------------------------------
using PrefixCodeTree = BasicPrefixCTree<unsigned>;


//-------------------------------------------------------------
// implement
//-------------------------------------------------------------

// @brief �l��ݒ�
//-------------------------------------------------------------
template<typename T>
inline void BasicPrefixCTree<T>::Node::SetValue(T val) noexcept
{
	m_value = val;
	m_hasvalue = true;
}

// @brief �q�ւ�Index��Ԃ�
// @note  �l��BasicPrefixCTree�̎���vector�ւ�Index���w��
//-------------------------------------------------------------
template<typename T>
inline index_t BasicPrefixCTree<T>::Node::ChildIndex(int bit) const
{
	return m_childIndex.at(bit);
}

// @brief �����_���A�N�Z�X��
// @note  �����_���A�N�Z�X�ł͂Ȃ�
//-------------------------------------------------------------
template<typename T>
template<unsigned numBit>
typename BasicPrefixCTree<T>::Node& BasicPrefixCTree<T>::operator[] (std::bitset<numBit> key)
{
	return CreateNodeIfNotFound(key).first;
}

// @brief	�����Ƃ���ɑΉ�����l��o�^����
// @note	�ꉞ 255bit���Ƃ����ł��Ă��Ή��ł���悤�ɂ��Ă���..
//-------------------------------------------------------------
template<typename T>
template<unsigned numBit>
bool BasicPrefixCTree<T>::Entry(std::bitset<numBit> bitset, T value)
{
	auto result = CreateNodeIfNotFound(bitset);
	if (result.second == false) 
		return false;

	result.first.SetValue(value);
	return true;
}

// @brief �w�肳�ꂽ�ړ������ɑΉ�����m�[�h��Ԃ�
//-------------------------------------------------------------
template<typename T>
template<unsigned numBit>
std::pair<typename BasicPrefixCTree<T>::Node&, bool> BasicPrefixCTree<T>::CreateNodeIfNotFound(std::bitset<numBit> bits)
{
	TreeWalker walker(*this);

	// �擪�r�b�g���珇�ɓǂ�
	int i = static_cast<int>(bits.size()) - 1;

	// �c���[�����ǂ��āA�w�肳�ꂽkey�ɂ��łɃm�[�h�����݂��邩���m�F
	for (; i >= 0; --i)
	{
		if (walker.HasChild(bits[i]) == false)
		{
			break;
		}
		walker.Next(bits[i]);
	}
	if(i < 0)
		return std::pair<Node&, bool>(m_nodeList.at(walker.m_current), false);

	// (�m�[�h������Ă��Ȃ�������������) �V�����m�[�h���쐬
	for (; i >= 0; --i)
	{
		index_t newIndex = static_cast<index_t>(m_nodeList.size());
		m_nodeList.push_back(Node{});

		auto& node = m_nodeList.at(walker.m_current);
		node.m_childIndex[(bits[i])] = newIndex;

		walker.Next(bits[i]);
	}
	return std::pair<Node&, bool>(m_nodeList.at(walker.m_current), true);
}

// @brief ���ݎw���Ă���m�[�h��Ԃ�
//-------------------------------------------------------------
template<typename T>
inline const typename typename BasicPrefixCTree<T>::Node& BasicPrefixCTree<T>::TreeWalker::Get() const
{
	return m_tree.m_nodeList.at(m_current);
}


// @brief �w�肵�������Ɏq�����݂��邩
//-------------------------------------------------------------
template<typename T>
inline bool BasicPrefixCTree<T>::TreeWalker::HasChild(int bit) const
{
	return GetNextIndex(bit) != invalidIndex;
}

// @brief �ړ�
//-------------------------------------------------------------
template<typename T>
inline bool BasicPrefixCTree<T>::TreeWalker::Next(int nextBit)
{
	m_current = GetNextIndex(nextBit);
	return m_current != invalidIndex;
}

// @brief �w�肵�������̎q���w��Index��Ԃ�
//-------------------------------------------------------------
template<typename T>
inline index_t BasicPrefixCTree<T>::TreeWalker::GetNextIndex(int bit) const
{
	return Get().ChildIndex(bit);
}

}// end namespace

 //-------------------------------------------------------------
 // using
 //-------------------------------------------------------------
using PrefixC::PrefixCodeTree;

}// end namespace