#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <regex>

struct Params
{
    std::string input_file;
    std::string output_file;
    std::string const_name;
    std::string header_file;
    std::string source_file;

    void parse(int argc, char *argv[])
    {
        using namespace std;

        // Pluck all the parameters
        // TODO: handle quotation marks

        for (auto i = 1; i < argc; i++)
        {			
            string arg(argv[i]);

            if (arg == "-i")                            // short option to specify input filename
            {
                if (++i == argc) throw runtime_error("-i must be followed by filename");
                input_file = argv[i];
            }
            else if (arg.substr(0, 8) == "--input=")    // ditto long form
            {
                input_file = arg.substr(8);
            }
            else if (arg == "-o")                       // short option to specify output filename
            {
                if (++i == argc) throw runtime_error("-o must be followed by filename");
                output_file = argv[i];
            }
            else if (arg.substr(0, 9) == "--output=")
            {
                output_file = arg.substr(9);
            }
            else if (arg == "-c")                       // short option to specify const variable name
            {
                if (++i == argc) throw runtime_error("-c must be followed by name of const variable");
                const_name = argv[i];
            }
            else if (arg.substr(0, 8) == "--const=")    // ditto long form
            {
                const_name = arg.substr(8);
            }
            else if (arg == "-h")                       // short option to specify header file name
            {
                if (++i == argc) throw runtime_error("-h must be followed by header file name/path");
                header_file = argv[i];
            }
            else if (arg.substr(0, 9) == "--header=")    // ditto long form
            {
                header_file = arg.substr(9);
            }
            else if (arg == "-s")                       // short option to specify source file name
            {
                if (++i == argc) throw runtime_error("-s must be followed by source file name/path");
                source_file = argv[i];
            }
            else if (arg.substr(0, 9) == "--source=")    // ditto long form
            {
                source_file = arg.substr(9);
            }
        }

        // TODO: check for leftovers...
    }

    void check() const
    {
        using namespace std;
        using namespace std::string_literals;

        // Plausibility checks

        if (!output_file.empty() && !const_name.empty())
            throw runtime_error("Cannot specify both an output file (-o, --output) and a const data name (-c, --const)");

        if (!const_name.empty())
        {
            if (!regex_match(const_name, regex{ "[_a-zA-Z][_a-zA-Z0-9]{0,30}" }))
                throw runtime_error("\""s + const_name + "\" is not a valid C/C++ identifier");
        }
    }

    void set_defaults()
    {
       if (!const_name.empty())
       {
           if (header_file.empty()) header_file = const_name + ".h";
           if (source_file.empty()) source_file = const_name + ".c";
       }
    }
};

static auto convert(std::istream &istream, std::ostream &ostream) -> std::size_t
{
    using namespace std;

    size_t n = 0;
    while (true) {
        unsigned char byte = static_cast<unsigned int>(istream.get());
        if (istream.eof()) break;
        if (n > 0) {
            ostream << ", ";
            if (n % 16 == 0) ostream << endl;
        }
        ostream << "0x" << hex << setw(2) << setfill('0') << static_cast<unsigned int>(byte);
        n++;
    }

    return n;
}

static void convert_data(std::istream &istream, const std::string output_file)
{
    using namespace std;

    ofstream ofs;
    ostream *os = nullptr;

    if (!output_file.empty()) 
    {
        ofs.open(output_file);
        os = &ofs;
    }
    else {
        os = &cout;
    }

    convert(istream, *os);
}

static void generate_files(std::istream &istream, const Params &params)
{
    using namespace std;

    ofstream hdr { params.header_file };
    ofstream src { params.source_file };
    
    src << "const unsigned char " << params.const_name << "[] = {\n";
    auto size = convert(istream, src);
    src << "};\n";

    hdr << "extern const unsigned char " << params.const_name << "[" << size << "];\n";
}

int main(int argc, char *argv[])
{
	using namespace std;

	try 
    {
        Params params;
        params.parse(argc, argv);
        params.check();
        params.set_defaults();

        ios_base::sync_with_stdio(false); // speed-up

        // Open input file / stream

        ifstream ifs;
        istream *is = nullptr;
        if (!params.input_file.empty()) 
        {
            ifs.open(params.input_file, ios::binary);
            is = &ifs;
        }
        else {
            is = &cin;
        }

        // Either convert to output file / stream or generate a pair of files

        if (params.const_name.empty())
        {
            convert_data(*is, params.output_file);
        }
        else
        {
            generate_files(*is, params);
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