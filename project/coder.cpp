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
 * To make all unit tests pass you may try to puzzle out the ::encode() algorithm
 * and it should help you to write the decoding one. But there are other ways to
 * make them pass.
 *
 * Good luck!
 *
 */
void Coder::encode()
{
	::encode( m_buf, m_size );
}

