// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
#pragma once

#include <cstdint>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

uint32_t Utf8Decode(uint32_t* state, uint32_t* codep, uint8_t ch);
