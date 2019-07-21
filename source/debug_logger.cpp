#include "debug_logger.h"

#include <fstream>
#include <cstring>

using namespace std;
using namespace stxDebug;

namespace stxDebug {
	struct Data {
		Data(int num, std::wstring text) {
			this->num = num;
			this->text = text;
		}

		Data(int num, const char* text) {
			this->num = num;

			wchar_t* wctext = new wchar_t[strlen(text) + 1];
			std::mbstowcs(wctext, text, strlen(text) + 1);
		
			this->text = wctext;

			delete[] wctext;
		}
	
		int num;
		std::wstring text;
	};

	template<>
	struct GetDbgStringT<Data> {
		static std::wstring GetDbgString(const Data& data) {
			return L"{class: 'Data, object: { num: '" +	to_wstring(data.num) 
				+ L"\', text: \'" + data.text + L"\' } }";
		}
	};

	struct StdFileWrite {
		void operator()(const std::wstring& msg) {
			std::wstring wfname = msg.substr(0, msg.find_first_of(L' '));
			const_cast<std::wstring&>(msg) = msg.substr(msg.find_first_of(L' ') + 1);
			
			char* fname = new char[wfname.length() + 1];
			wcstombs(fname, wfname.c_str(), wfname.length());

			fname[wfname.length()] = '\0';

			std::wofstream fout(fname);
			if (fout) {
				fout << msg << std::endl; 
				fout.close();
			}

			delete[] fname;
		}
	};

	stxDebug::DebugLogger<StdFileWrite> File(const std::wstring& fname) {
		return DebugLogger<StdFileWrite>() << fname + L" ";
	}
} // namespace stxDebug


int main(int argc, const char* argv[]) {
	cout << "stx C++ test task" << endl;

	stxDebug::Out() << L"wide text" << std::wstring(L" - std::wstring") << " - ascii text" << std::string(" - std::string");
	stxDebug::Err() << L"wide text" << std::wstring(L" - std::wstring") << " - ascii text" << std::string(" - std::string");

  	// stx: task is to uncomment and revive this line
	stxDebug::File(L"dump.txt") << L"Dump: " << Data(55, "stx data");

	getchar();

  return 0;
}
