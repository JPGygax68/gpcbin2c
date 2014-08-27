#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main(int argc, char *argv[])
{
	using namespace std;

	try {

		string input_file, output_file;

		// TODO: handle quotation marks

		for (int i = 1; i < argc; i++) {
			
			string arg(argv[i]);

			if (arg == "-i") {
				if (++i == argc) throw runtime_error("-i must be followed by filename");
				input_file = argv[i];
			}
			else if (arg.substr(0, 8) == "--input=") {
				input_file = arg.substr(8);
			}
			else if (arg == "-o") {
				if (++i == argc) throw runtime_error("-o must be followed by filename");
				output_file = argv[i];
			}
			else if (arg.substr(0, 9) == "--output=") {
				output_file = arg.substr(9);
			}
		}

		ifstream ifs;
		ofstream ofs;
		istream *is = nullptr;
		ostream *os = nullptr;

		if (!input_file.empty()) {
			ios_base::sync_with_stdio(false);
			ifs.open(input_file, ios::binary);
			is = &ifs;
		}
		else {
			is = &cin;
		}

		if (!output_file.empty()) {
			ios_base::sync_with_stdio(false);
			ofs.open(output_file);
			os = &ofs;
		}
		else {
			os = &cout;
		}

		// Convert

		size_t n = 0;
		while (true) {
			unsigned char byte = static_cast<unsigned int>(is->get());
			if (is->eof()) break;
			if (n > 0) {
				*os << ", ";
				if (n % 16 == 0) *os << endl;
			}
			*os << "0x" << hex << setw(2) << setfill('0') << (unsigned int) byte;
			n++;
		}

		return 0;
	}
	catch (const exception &e) {
		cerr << "Error: " << e.what() << endl;
	}

	/*
	cout << endl;
	cout << "Press RETURN to terminate" << endl;
	cin.ignore();
	*/

	return -1;
}