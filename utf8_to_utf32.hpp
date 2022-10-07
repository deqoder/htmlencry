#ifndef UTF8_TO_UTF32_HPP
#define UTF8_TO_UTF32_HPP

#include <string>
static bool utf8_to_utf32(std::string const &u8, std::u32string &out) {
	int elem_len = 1;
	out.clear();
	for (size_t i = 0; i < u8.size(); i += elem_len) {
		uint32_t tmp = (uint32_t)u8[i] & 0xff;
		if (tmp < 0x80UL) {
			elem_len = 1;
			out.push_back(u8[i]);
		} else if (tmp < 0xe0UL) {
			elem_len = 2;
			out.push_back(
				((u8[i] & 0x1f) << 6) 
				| (u8[i+1] & 0x3f)
			);
		} else if (tmp < 0xf0UL) {
			elem_len = 3;
			out.push_back(
			       	((u8[i] & 0xf) << 12) 
				| ((u8[i+1] & 0x3f) << 6)
				| (u8[i+2] & 0x3f)
			);
		} else if (tmp < 0xf8UL) {
			elem_len = 4;
			out.push_back(
				((u8[i] & 0x7) << 18)
				| ((u8[i+1] & 0x3f) << 12)
				| ((u8[i+2] & 0x3f) << 6)
				| (u8[i+3] & 0x3f)
			);
		} else {
			return false;
		}
	}
	return true;
}
static bool utf32_to_utf8(std::u32string const &u32, std::string &out) {
	out.clear();
	for (size_t i = 0; i < u32.size(); i++) {
		if (u32[i] < 0x80) {
			out.push_back(u32[i]);
		} else if (u32[i] < 0x800) {
			out.push_back( (u32[i] >> 6) | 0xc0 );
			out.push_back( (u32[i] & 0x3f) | 0x80 );
		} else if (u32[i] < 0x10000) {
			out.push_back( (u32[i] >> 12) | 0xe0 );
			out.push_back( ((u32[i] >> 6) & 0x3f) | 0x80 );
			out.push_back( (u32[i] & 0x3f) | 0x80 );
		} else if (u32[i] < 0x110000) {
			out.push_back( (u32[i] >> 18) | 0xf0 );
			out.push_back( ((u32[i] >> 12) & 0x3f) | 0x80 );
			out.push_back( ((u32[i] >> 6) & 0x3f) | 0x80 );
			out.push_back( (u32[i] & 0x3f) | 0x80 );
		} else {
			return false;
		}

	}
	return true;
}
#endif
