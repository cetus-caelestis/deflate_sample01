//-------------------------------------------------------------
//! @brief	�Ȉ�Deflate����
//! @author	��ĩ�=��ڽè�
//-------------------------------------------------------------
#pragma once

//-------------------------------------------------------------
// include
//-------------------------------------------------------------
#include <vector>

namespace MyUtility
{
namespace Deflate
{

//! �f�R�[�h����
std::vector<char> Decode(const char* binary, size_t numByte);


}// end namespace Deflate
}// end namespace MyUtility