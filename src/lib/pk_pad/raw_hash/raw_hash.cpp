/*
* (C) 2023 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#include <botan/internal/raw_hash.h>
#include <botan/exceptn.h>

namespace Botan {

void RawHashFunction::add_data(const uint8_t input[], size_t length)
   {
   m_bits += std::make_pair(input, length);
   }

void RawHashFunction::final_result(uint8_t out[])
   {
   if(m_output_length > 0 && m_bits.size() != m_output_length)
      {
      m_bits.resize(0);
      throw Invalid_Argument("EMSA_Raw was configured to use a " +
                             std::to_string(m_output_length) +
                             " byte hash but instead was used for a " +
                             std::to_string(m_bits.size()) + " hash");
      }

   copy_mem(out, m_bits.data(), m_bits.size());
   m_bits.resize(0);
   }

void RawHashFunction::clear()
   {
   m_bits.resize(0);
   }

std::unique_ptr<HashFunction> RawHashFunction::copy_state() const
   {
   return std::make_unique<RawHashFunction>(*this);
   }

std::unique_ptr<HashFunction> RawHashFunction::new_object() const
   {
   return std::make_unique<RawHashFunction>(m_name, m_output_length);
   }

size_t RawHashFunction::output_length() const
   {
   if(m_output_length > 0)
      return m_output_length;
   return m_bits.size();
   }

}