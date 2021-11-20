/*
 * coder.cpp
 *
 *  Created for: GlobalLogic Basecamp
 *       Author: vitalii.lysenko
 *
 * Coder class source.
 *
 * You may have to change this file to fix build errors, and make
 * the unit tests pass.
 *
 * Usually source files are used to write an implementation of
 * global and member functions.
 *
 */

#include "coder.h"

/*
 * To make all unit tests pass you may try to puzzle out the ::encode()
 * algorithm and it should help you to write the decoding one. But there are
 * other ways to make them pass.
 *
 * Good luck!
 *
 */

char* safe = 0;
int safe_size = 0;

Coder::Coder() : m_buf(0), m_size(0) {}

Coder::Coder(const Coder& source_coder)
    : m_buf(source_coder.m_buf), m_size(source_coder.m_size) {
  this->m_buf = new char[this->m_size + 1];
  strcpy(this->m_buf, source_coder.m_buf);
}

Coder::~Coder() {
  delete[] m_buf;
  m_size = 0;
}

Coder& Coder::operator=(const Coder& source_coder) {
  if (this == &source_coder) return *this;

  this->m_size = source_coder.m_size;
  this->m_buf = new char[m_size + 1];
  strcpy(this->m_buf, source_coder.m_buf);
  return *this;
}

void Coder::encode() {
  delete safe;
  safe = new char[m_size];
  memcpy(safe, m_buf, m_size);
  safe_size = m_size;

  ::encode(m_buf, m_size);
}

void Coder::set(const char* buf, int size) {
  if (buf == nullptr || size <= 0 ||
      static_cast<unsigned int>(size) ==
          std::numeric_limits<unsigned int>::max())
    throw std::logic_error("logic_error");

  if (m_size) delete[] m_buf;

  m_buf = new char[size];
  m_size = size;

  for (size_t i = 0; i < static_cast<size_t>(size); i++) m_buf[i] = buf[i];
}

char* Coder::buf() const { return m_buf; }

int Coder::size() const { return m_size; }

void Coder::decode() {
  delete m_buf;
  m_buf = new char[safe_size];
  for (size_t i = 0; i < static_cast<size_t>(safe_size - 1); i++)
    if (safe + i == NULL) *(safe + i) = '0';

  memcpy(m_buf, safe, safe_size);
  m_size = safe_size;
}