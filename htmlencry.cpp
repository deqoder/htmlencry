#include <iostream>
#include <string>
#include <sstream>
#include "utf8_to_utf32.hpp"
std::string stream2str(std::istream &istr) {
	std::string out;
	for (std::string tmp; std::getline(istr, tmp); out += tmp);
	return out;
}
bool seg(std::string const &in, std::string &head, std::string &body, std::string &tail) {
	auto begin = in.find("<body>");
	auto end = in.find("</body>");
	if (begin < 0 || end < 0) {
		return false;
	}
	head = in.substr(0, begin + 6);
	body = in.substr(begin + 6, end - (begin + 6));
	tail = in.substr(end);
	return true;
}
void toasciientity(std::string &in) {
	std::u32string out;
	utf8_to_utf32(in, out);
	in.clear();
	for (size_t i = 0; i < out.size(); i++) {
		if (out[i] < 0x80) {
			in.push_back(out[i]);
		} else {
			std::ostringstream oss;
			oss << "&#" << out[i] << ";";
			in += oss.str();
		}
	}
}
void encrypt(std::string &in, std::string const &key) {
	for (size_t i = 0; i < in.size(); i++) {
		uint32_t tmp = in[i] & 0xff;
		if (tmp >= 0x20 && tmp <= 0x7e) {
			tmp -= 0x20;
			tmp += key[i % key.size()] - 0x20;
			tmp %= (0x7f - 0x20);
			in[i] = (char)(tmp + 0x20);
		}
	}	
}
void tohex(std::string &in) {
	std::string incpy(in);
	in.clear();
	for (size_t i = 0; i < incpy.size(); i++) {
		char tmp[3];
		sprintf(tmp, "%02x", incpy[i]);
		in += tmp;
	}
}
int main(int n, char **a) {
	auto str = stream2str(std::cin);
	std::string head, body, tail;
	if (!seg(str, head, body, tail)) {
		std::cerr << "input error" << std::endl;
	}
	toasciientity(body);
	encrypt(body, a[1]);
	tohex(body);
	std::string HEAD = R"(<p style='display:none'>)";
	std::string TAIL = R"(</p>
<div style='position:fixed; left:50%; top:50%; transform: translate(-50%, -50%)'>
key: <input type='text' />
<button style='color:#fff; background:#000; border:0px; cursor: pointer;' onclick='decode()'>decode</button>
</div>
<script>
	document.querySelector('body').style.background = "#000";
	document.querySelector('body').style.color = "#fff";

function decode() {
	var hex = document.querySelector('p').innerHTML;
	var ascii = '';
	for (var i = 0; i < hex.length; i += 2) { 
		ascii += String.fromCharCode("0x" + hex.substr(i, 2)); 
	}
	key = document.querySelector('input').value; 
	out = "" ; 
	for (var i = 0; i < ascii.length; i++) {
		tmp = ascii.charCodeAt(i);
		if(tmp >= 0x20) { 
			tmp -= key.charCodeAt(i % key.length); 
			if (tmp < 0) { tmp += (0x7f - 0x20)} 
			out += String.fromCharCode(0x20 + tmp)
		} else {
			out += ascii[i]
		}
	}
	document.write(out);
}
</script>
)";
	std::cout << head << HEAD << body << TAIL << tail;
	return 0;
}

