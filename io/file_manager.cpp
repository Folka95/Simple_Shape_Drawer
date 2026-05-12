#include "file_manager.h"
using namespace std;
#define WORD_SIZE 24

FileManager::FileManager() {}

FileManager::~FileManager() {}

string FileManager::decimalToBinary(int decimal) {
    string binary(WORD_SIZE, '0');
    for(int i = 0; i < WORD_SIZE; i++) {
        bool bit = (decimal >> i) & 1;
        if(bit) {
            binary[WORD_SIZE - 1 - i] = '1';
        }
    }
    return binary;
}

int FileManager::binaryToDecimal(const string &binary) {
    int result = 0;
    for (char c : binary) {
        result = result * 2 + (c - '0');
    }
    return result;
}

string FileManager::encode(string binary) {
    string encoded;
    for(int i = 0; i < binary.size(); i += 8) {
        encoded += char(binaryToDecimal(binary.substr(i, 8)));
    }
    return encoded;
}

string FileManager::decode(string content) {
    string decoded;
    for(int i = 0; i < content.size(); i++) {
        decoded += decimalToBinary(content[i]).substr(WORD_SIZE - 8);
    }
    return decoded;
}

void FileManager::saveToFile(const string &content, const string &filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Cannot open file for writing\n";
        return;
    }
    file << encode(content);
    file.close();
}

string FileManager::loadFromFile(const string & filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Cannot open file for reading\n";
        return "";
    }
    string content(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    file.close();
    return decode(content);
}

string FileManager::actionToString(const vector<Action*> &actions) {
    string result;
    result += decimalToBinary(actions.size());
    for (Action* action : actions) {
        result += decimalToBinary(action->getActionType());
        result += decimalToBinary(action->getRank());
        vector< short > data = action->getData();
        result += decimalToBinary((int)data.size());
        for (short value : data) {
            result += decimalToBinary(value);
        }
    }
    return result;
}

string FileManager::screenToString(const vector<vector<COLORREF>> &screen) {
    string result;
    int width = screen.size();
    int length = (width ? screen[0].size() : 0);
    result += decimalToBinary(width);
    result += decimalToBinary(length);
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < length; j++) {
            int r = GetRValue(screen[i][j]);
            int g = GetGValue(screen[i][j]);
            int b = GetBValue(screen[i][j]);
            result += decimalToBinary( (r << 16) + (g << 8) + b);
            // result += decimalToBinary(r);
            // result += decimalToBinary(g);
            // result += decimalToBinary(b);
        }
    }
    return result;
}

vector<Action*> FileManager::stringToAction(const string & binary) {
    int it = 0;
    int n = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
    vector<Action*> actions(n);
    for (Action* &action : actions) {
        int type = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
        int typeRank = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
        int dataSize = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
        vector< short > data(dataSize);
        for(int i = 0; i < dataSize; i++) {
            data[i] = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
        }
        if(type == ACTION_MENU_SELECT) {
            action = new MenuSelectAction(typeRank, data);
        }
        else if(type == ACTION_LEFT_CLICK) {
            action = new LeftClickAction(typeRank, data[0], data[1]);
        }
        else if(type == ACTION_RIGHT_CLICK) {
            action = new RightClickAction(typeRank, data[0], data[1]);
        }
        else {
            cerr << "Unknown action type\n";
        }
    }
    return actions;
}

vector<vector<COLORREF>> FileManager::stringToScreen(const string & binary) {
    int it = 0;
    int width = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
    int length = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
    vector<vector<COLORREF>> screen(width, vector<COLORREF> (length));
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < length; j++) {
            int rgb = binaryToDecimal(binary.substr(WORD_SIZE * (it++), WORD_SIZE));
            int r = rgb >> 16;
            int g = (rgb >> 8) & ((1 << 8) - 1);
            int b = rgb & ((1 << 8) - 1);
            screen[i][j] = RGB(r, g, b);
        }
    }
    return screen;
}

void FileManager::saveActions(const vector<Action*> &actions, const string & filepath) {
    string content = actionToString(actions);
    saveToFile(content, filepath);
}

vector<Action*> FileManager::loadActions(const string & filepath) {
    string content = loadFromFile(filepath);
    vector<Action*> actions = stringToAction(content);
    return actions;
}

void FileManager::saveScreen(const vector<vector<COLORREF>> &screen, const string & filepath) {
    string content = screenToString(screen);
    saveToFile(content, filepath);
}

vector<vector<COLORREF>> FileManager::loadScreen(const string & filepath) {
    string content = loadFromFile(filepath);
    vector<vector<COLORREF>> screen = stringToScreen(content);
    return screen;
}