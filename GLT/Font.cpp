#include "Font.hpp"
#include <sstream>
#include <fstream>

using namespace std;
using namespace glt;

Font::Font() {

}
Font::Font(const char* fileName) {
	loadFile(fileName);
}

void Font::loadFile(const char* file) {
	std::string fileStr(file);
	std::string folder("");

	for (int i = fileStr.size() - 1; i >= 0; i--) {
		if (fileStr[i] == '\\' || fileStr[i] == '/') {
			folder = fileStr.substr(0, i + 1);
			break;
		}
	}

	//Clear the previous charmap
	charMap.clear();

	string line;
	ifstream myfile(file);

	int textureWidth = 0, textureHeight = 0;

	//Open the file
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			//Split the line in spaces
			//First param is the data type
			vector<string> params = splitString(line, ' ');

			//Common, contains texture size
			if (params[0] == "common") {
				textureWidth = stoi(findValue(params, "scaleW"));
				textureHeight = stoi(findValue(params, "scaleH"));
			}

			//Page, contains file name
			if (params[0] == "page" && stoi(findValue(params, "id")) == 0)
				texture.loadFile((folder + findValue(params, "file")).c_str());

			//Char, character data
			if (params[0] == "char") {
				unsigned int id = stoi(findValue(params, "id"));
				int x = stoi(findValue(params, "x"));
				int y = stoi(findValue(params, "y"));
				int width = stoi(findValue(params, "width"));
				int height = stoi(findValue(params, "height"));

				//Add character to the character bank
				//Divide all values by texture size, converting from pixels to UV
				charMap[id] = CharInfo(
					id,
					(float)x / textureWidth, (float)y / textureHeight,
					(float)width / textureWidth, (float)height / textureHeight
					);

				//If id == 32 (space), set the ascpect to 0.25
				//BitmapFont doesn't set space width properly :(
				if (id == 32)
					charMap[id].aspect = 0.25f;
			}
		}

		//Cleanup
		myfile.close();
	}
}

//Simple split string function
vector<string> Font::splitString(std::string input, char denominator) {
	vector<string> output;
	stringstream ss(input);
	string str;

	while (getline(ss, str, denominator)) {
		//Don't add empty strings
		if (!str.empty())
			output.push_back(str);
	}

	return output;
}

//Search params for specific value
string Font::findValue(vector<string> params, string option) {
	for (int i = 0; i < params.size();i++) {
		//Current parameter
		vector<string> val = splitString(params[i], '=');

		if (val.size() >= 2 && val[0] == option) {
			//Remove quotes, if present
			if (val[1][0] == '"') {
				val[1].erase(val[1].begin());
				val[1].erase(val[1].end() - 1);
			}

			//Found it :)
			return val[1];
		}
	}

	return "";
}

//Get a vector of charinfos for every character in a string
vector<Font::CharInfo> Font::getString(string str) {
	vector<CharInfo> output;

	for (int i = 0; i < str.size(); i++) {
		//If this char doesn't exist in this font, ignore it
		if (charMap.find(str[i]) == charMap.end()) continue;

		output.push_back(charMap[str[i]]);
	}

	return output;
}