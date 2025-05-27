#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include<stack>
using namespace std;

struct Position {
    int row;
    int col;
    char pixel;  // '@' for black, '#' for white
};

int mark[25][25]{};
vector<string> grid;

// 摩斯密碼字典
unordered_map<string, int> morseToNumber = {
    {".-", 1}, {"-...", 2}, {"-.-.", 3}, {"-..", 4}, {".", 5},
    {"..-.", 6}, {"--.", 7}, {"....", 8}, {"..", 9}, {".---", 10},
    {"-.-", 11}, {".-..", 12}, {"--", 13}, {"-.", 14}, {"---", 15},
    {".--.", 16}, {"--.-", 17}, {".-.", 18}, {"...", 19}, {"-", 20},
    {"..-", 21}, {"...-", 22}, {".--", 23}, {"-..-", 24}, {"-.--", 25}, {"--..", 26}
};

// 將摩斯密碼轉換為數字
vector<int> morseToNum(const vector<string>& morse) {
    vector<int> result;
    for (int i = 0; i < morse.size(); i++) {
        result.push_back(morseToNumber[morse[i]]);
    }
    return result;
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void readData(const string& filename, int& imagesize, vector<vector<int>>& columndata, vector<vector<int>>& rowdata) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open the file: " << filename << endl;
        return;
    }
    string line;
    vector<string> firstline;
    int num = 1;
    while (getline(file, line)) {
        if (!line.empty()) {
            if (num == 1) {
                firstline = split(line, ' ');
                imagesize = stoi(firstline[0]);
                num++;
                continue;
            }
            else if (num <= imagesize + 1) {
                num++;
                vector<int> input;
                input = morseToNum(split(line, ' '));
                columndata.push_back(input);
            }
            else {
                vector<int> input;
                input = morseToNum(split(line, ' '));
                rowdata.push_back(input);
            }
        }
    }

    file.close();
}

struct blockselements {
    int count = 0;
    bool ifcontinuous = true;;
};

bool equals(vector<blockselements> a, vector<int> b) {
    if (a.size() != b.size()) 
        return false;
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i].count != b[i]) 
            return false;
    }
    return true;
}

// 檢查一行的連續黑色像素數是否符合要求
bool checkLine(const string& line, const vector<int>& requirement, int row) {
    vector<blockselements> blocks;
    blockselements block;
    bool rowiscomplete;

    for (int i = 0; i < line.size(); i++) {

        if (line[i] == '@') {
            block.count++;

        }
        else if (block.count > 0) {
            if (i > 0) {
                if (line[i - 1] == '@' && line[i] == '#')
                    block.ifcontinuous = false;
            }
            blocks.push_back(block);
            block.count = 0;
            block.ifcontinuous = true;
        }

    }
    if (block.count > 0) {
        blocks.push_back(block);
    }

    if (line[line.size() - 1] != '?' && mark[row][line.size() - 1] != 1) {
        if (!equals(blocks, requirement)) {
            return false;
        }
    }

    if (blocks.size() > requirement.size()) {
        return false;
    }

    if (blocks.size() <= requirement.size()) {
        for (size_t i = 0; i < blocks.size(); i++) {
            if (blocks[i].count > requirement[i]) {
                return false;
            }
            else if (blocks[i].count < requirement[i] && !blocks[i].ifcontinuous) {
                return false;
            }
        }
    }


    return true;
}

// 檢查當前填充是否可能有效
bool isValid(int row, int col, const vector<vector<int>>& rowReqs, const vector<vector<int>>& colReqs) {
    // 檢查當前列
    string currentRow = grid[row];
    if (!checkLine(currentRow, rowReqs[row], row)) {
        return false;
    }

    // 檢查當前行
    string currentCol;
    //bool colComplete = (col == grid.size() - 1);
    for (int i = 0; i < grid.size(); i++) {
        currentCol += grid[i][col];
    }
  
    if (!checkLine(currentCol, colReqs[col], col)) {
        return false;
    }
    return true;
}

void Ifcontinouswhite(stack<Position>& s) {
    s.pop();
    Position p = s.top();

    if (grid[p.row][p.col] == '#') {
        grid[p.row][p.col] = '?';
        Ifcontinouswhite(s);
    }
    else if (p.pixel == '@') {
        s.pop();
        s.push({ p.row, p.col, '#' });
        return;
    }
}

void generateImage(int imagesize, const vector<vector<int>>& columndata, const vector<vector<int>>& rowdata) {

    grid.assign(imagesize, string(imagesize, '?'));

    stack<Position> s;

    int count = 0;

    
    //把確定的行先做
    for (int i = 0; i < imagesize; i++) {
        int col = 0;
        int totalcolumnblacks = 0;

        for (int j = 0; j < columndata[i].size(); j++) {
            totalcolumnblacks += columndata[i][j];
        }

        if (totalcolumnblacks + columndata[i].size() - 1 == imagesize || totalcolumnblacks == imagesize) {
            for (int c = 0; c < imagesize; c++) {
                count++;
                mark[c][i] = 1;
            }
            for (int length : columndata[i]) {
                for (int j = i, k = 0; k < length; col++, k++) {
                    grid[col][j] = '@';
                }
                if (col < imagesize) {
                    grid[col][i] = '#';
                    col++;
                }
            }

        }
    }
    //把確定的列先做
    for (int i = 0; i < imagesize; i++) {
        int row = 0;
        int totalrowblacks = 0;

        for (int j = 0; j < rowdata[i].size(); j++) {
            totalrowblacks += rowdata[i][j];
        }

        if (totalrowblacks + rowdata[i].size() - 1 == imagesize || totalrowblacks == imagesize) {
            for (int c = 0; c < imagesize; c++) {
                count++;
                mark[i][c] = 1;
            }
            for (int length : rowdata[i]) {
                for (int j = i, k = 0; k < length; row++, k++) {
                    grid[j][row] = '@';
                }
                if (row < imagesize) {
                    grid[i][row] = '#';
                    row++;
                }
            }

        }
    }

    if (count < (imagesize * imagesize)/2) {
        grid.assign(imagesize, string(imagesize, '?'));
        for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 25; j++)
                mark[i][j] = 0;
        }
    }


   // 從左上角開始
    s.push({ 0, 0, '@' }); // 先嘗試黑色像素
    int row, col;
    char pixel;
    bool found = false;

    while (!s.empty()) {
        Position current = s.top();
        //s.pop();
        row = current.row;
        col = current.col;
        pixel = current.pixel;

        grid[row][col] = current.pixel;

        if (row < imagesize && !found && mark[row][col] == 0) {

            if (isValid(row, col, rowdata, columndata)) {
                if (row == imagesize - 1 && col == imagesize - 1) {
                    found = true;
                    break;
                }

                // 計算下一個位置
                row = row;
                col = col + 1;
                if (col == imagesize) {
                    row++;
                    col = 0;
                }
                s.push({ row, col, '@' });

            }
            else {// 如果無效，嘗試其他顏色

                grid[row][col] = '?';
                int currentrow = row;
                int currentcolumn = col;
                s.pop();
                if (pixel == '@') {
                    s.push({ row, col, '#' }); // 嘗試白色像素
                    //grid[row][col] = '#';
                }
                else {
                    if ((grid[currentrow][imagesize - 1] == '@' && !rowdata[currentrow].empty() && imagesize - currentcolumn <= rowdata[currentrow].back()) || (grid[imagesize - 1][currentcolumn] == '@' && !columndata[currentcolumn].empty() && imagesize - currentrow <= columndata[currentcolumn].back())) {
                        grid[currentrow][currentcolumn] = '@';
                        s.push({ currentrow, currentcolumn, '@' });
                        mark[currentrow][currentcolumn] = 1;
                        continue;
                    }

                    Position pre = s.top();
                    Position pre1(pre);
                    //s.pop();
                    if (grid[pre.row][pre.col] == '@') {
                        s.pop();
                        s.push({ pre.row, pre.col, '#' });
                    }

                    if (grid[pre1.row][pre1.col] == '#') {
                        grid[pre1.row][pre1.col] = '?';
                        Ifcontinouswhite(s);
                    }

                    continue;
                }
            }

        }
        if (mark[row][col] == 1) {
            if (row == imagesize - 1 && col == imagesize - 1) {
                found = true;
                break;
            }
            row = row;
            col = col + 1;
            if (col == imagesize) {
                row++;
                col = 0;
            }
            if (mark[row][col] != 1)
                s.push({ row, col, '@' });
            else
                s.push({ row, col, grid[row][col] });
        }
    }

    if (found) {
        for (int i = 0; i < imagesize; i++) {
            for (int j = 0; j < imagesize; j++) {
                cout << grid[i][j];
                if (j < imagesize - 1)
                    cout << " ";
            }
            if (i == imagesize - 1)
                break;
            cout << endl;
        }
    }
}





int main(int argc, char* argv[]) {

    int imagesize;
    vector<vector<int>> columndata;
    vector<vector<int>> rowdata;
    readData("input4.txt", imagesize, columndata, rowdata);
    generateImage(imagesize, columndata, rowdata);

    return 0;
}