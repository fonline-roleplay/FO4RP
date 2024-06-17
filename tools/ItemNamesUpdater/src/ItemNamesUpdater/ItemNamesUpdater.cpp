// FOnline engine's "ItemNames.lst" file generator.
// Author: APAMk2, special for FO4RP
// July 17, 2024.

#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef unsigned int uint;
using namespace std;

map<uint, string> foobj_eng;	// Key - PID * 100
map<uint, string> foobj_rus;	// Key - PID * 100
map<uint, string> defines;		// Key - PID
filesystem::path serverRoot;

class StringReader // Tool class for fast std::string parse
{
public:
	uint cursor;
	string* stringToParse = NULL;
	string buffer;

	StringReader(string* toParse, uint initCursor)
	{
		this->cursor = initCursor;
		this->stringToParse = toParse;
	}
	
	void ParseUntil(vector<string> terminator)
	{
		buffer = "";
		size_t len = stringToParse->size();
		size_t term_len = terminator.size();
		bool parse = true;
		while (cursor < len && parse)
		{
			for (uint i = 0; i < term_len; i++)
			{
				if (stringToParse->substr(cursor, terminator[i].size()) == terminator[i]) parse = false;
			}
			if (!parse) break;

			buffer.push_back(stringToParse->at(cursor));
			cursor++;
		}
		cursor++; // To count terminator in cursor
	}
};

string FixedSizeStr(string in, uint tabs_count)
{
	string out = in;
	int total_tabs = tabs_count - uint(out.size() / 4);
	for (int i = 0; i < total_tabs; i++)
	{
		out += "\t";
	}
	return out;
}

void LoadFOOBJ()
{
	for(uint i = 0; i < 2; i++)
	{
		string currString;
		ifstream input(serverRoot.string() + "/text/" + ((i < 1) ? "engl" : "russ") + "/FOOBJ.MSG");
		if (input.is_open())
		{
			while (getline(input, currString))
			{
				if (currString.length() < 1 || currString.substr(0, 1) != "{") continue;
				StringReader currParser(&currString, 1);
				uint num;
				string name;
				currParser.ParseUntil({"{"});
				num = stoi(currParser.buffer);
				if (num % 2 > 0) continue; // We're skipping all %PID%01 sections because that's descriptions & we don't need to hold this in memory
				currParser.cursor += 2;
				currParser.ParseUntil({"}"});
				name = currParser.buffer;
				if(i < 1) foobj_eng[num] = name;
				else foobj_rus[num] = name;
			}
		}
		input.close();
	}
}

void LoadDefines()
{
	string currString;
	ifstream input(serverRoot.string() + "/scripts/_itempid.fos");
	if (input.is_open())
	{
		while (getline(input, currString))
		{
			if (currString.length() < 1) continue;
			bool defineTest = currString.substr(0, 12) == "#define PID_";
			if (!defineTest) continue;

			string define;
			uint pid;
			StringReader currParser(&currString, 8);
			currParser.ParseUntil({" ", "\t"});
			define = currParser.buffer;
			currParser.ParseUntil({"("});
			currParser.ParseUntil({")"});
			pid = stoi(currParser.buffer);
			defines[pid] = define;
		}
	}
	input.close();
}

void WriteFile(string filename, string& output)
{
	ofstream out;
	out.open(filename);
	if (out.is_open())
	{
		out << output << endl;
	}
	out.close();
}

void GenerateItemNames(string& out)
{
	for (const auto& [key, val] : defines)
	{
		out += FixedSizeStr(to_string(key), 2) + FixedSizeStr(val, 8);
		if (foobj_eng.contains(key * 100)) out += "// " + FixedSizeStr(foobj_eng[key * 100], 10) + " ";
		if (foobj_rus.contains(key * 100)) out += "// " + foobj_rus[key * 100];
		out += "\n";
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 2)
	{
		cout << "Wrong args, the only arg is server root path" << endl;
		return 1;
	}

	serverRoot = argv[1];
	LoadFOOBJ();
	LoadDefines();
	string itemNames;
	GenerateItemNames(itemNames);
	WriteFile(serverRoot.string() + "/data/ItemNames.lst", itemNames);
	return 0;
}
