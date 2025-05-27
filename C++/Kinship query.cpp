#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <stack>
using namespace std;

struct FamilyMember {
    string name;
    char gender;
    int birthYear;
    int deathYear;
    string spouse;
};
struct stackelement {
    string name;
    int dir;
};

class FamilyTree {
private:
    vector<FamilyMember> heap;
    unordered_map<string, int> nameToIndex;

    int getParentIndex(int index) { 
        if (index < 0)
            return -1;
        return (index - 1) / 2; 
    }
    int getLeftChildIndex(int index) { 
        int leftChildIndex = 2 * index + 1;
        if (leftChildIndex >= heap.size())
            leftChildIndex = -1;
        return leftChildIndex;
    }
    int getRightChildIndex(int index) { 
        int rightChildIndex = 2 * index + 2;
        if (rightChildIndex >= heap.size())
            return -1;
        return rightChildIndex;
    }

    string determineRelationship(const vector<string>& path) {

        int length = path.size();
        FamilyMember endmember = heap[nameToIndex[path.back()]];
        FamilyMember endmemberparent = heap[getParentIndex(nameToIndex[path.back()])];

        FamilyMember startmember = heap[nameToIndex[path.front()]];
        FamilyMember startmemberparent = heap[getParentIndex(nameToIndex[path.front()])];

        if (length == 2) {
            if (endmember.birthYear < startmember.birthYear) {
                if (endmember.gender == 'F')
                    return path.back() + "是" + path.front() + "的媽媽";
                if (endmember.gender == 'M')
                    return path.back() + "是" + path.front() + "的爸爸";
            }
            else {
                if (endmember.gender == 'F')
                    return path.back() + "是" + path.front() + "的女兒";
                if (endmember.gender == 'M')
                    return path.back() + "是" + path.front() + "的兒子";
            }
        }
        else if (length == 3) {
            if (endmemberparent.name == startmemberparent.name) {
                if (endmember.birthYear < startmember.birthYear) {
                    if(endmember.gender == 'M')
                        return path.back() + "是" + path.front() + "的哥哥";
                    else
                        return path.back() + "是" + path.front() + "的姊姊";
                }
                else {
                    if (endmember.gender == 'F')
                        return path.back() + "是" + path.front() + "的弟弟";
                    else
                        return path.back() + "是" + path.front() + "的妹妹";
                }

            }
            else if (endmember.birthYear < startmember.birthYear) {
                if (endmember.gender == 'M')
                    return path.back() + "是" + path.front() + "的祖父";
                else
                    return path.back() + "是" + path.front() + "的祖母";
            }
            else if (endmember.birthYear > startmember.birthYear) {
                if (endmember.gender == 'M') {
                    if(endmemberparent.gender == 'M')
                        return path.back() + "是" + path.front() + "的孫子";
                    else if (endmemberparent.gender == 'F')
                        return path.back() + "是" + path.front() + "的外孫";
                }
                else {
                    if (endmemberparent.gender == 'M')
                        return path.back() + "是" + path.front() + "的孫女";
                    else if (endmemberparent.gender == 'F')
                        return path.back() + "是" + path.front() + "的外孫";
                }
            }
        }
        else if (length == 4) {
            if (isallparent(path)) {
                return path.back() + "是" + path.front() + "的阿祖";
            }
            else if (endmember.birthYear < startmember.birthYear) {
                if (startmemberparent.gender == 'M') {
                    if (endmember.gender == 'M')
                        return path.back() + "是" + path.front() + "的伯父";
                    else if (endmember.gender == 'F')
                        return path.back() + "是" + path.front() + "的姑媽";
                }
                else if (startmemberparent.gender == 'F') {
                    if (endmember.gender == 'M')
                        return path.back() + "是" + path.front() + "的舅舅";
                    else if (endmember.gender == 'F')
                        return path.back() + "是" + path.front() + "的姨媽";
                }
            }
            else {
                if (endmemberparent.gender == 'M') {
                    if (endmember.gender == 'M')
                        return path.back() + "是" + path.front() + "的姪子";
                    else if (endmember.gender == 'F')
                        return path.back() + "是" + path.front() + "的姪女";
                }
                else if (endmemberparent.gender == 'F') {
                    if (endmember.gender == 'M')
                        return path.back() + "是" + path.front() + "的外甥";
                    else if (endmember.gender == 'F')
                        return path.back() + "是" + path.front() + "的外甥女";
                }
            }
        }
        else if (length == 5) {
            if (startmemberparent.gender == 'M') {
                if (endmemberparent.gender == 'M') {
                    if (endmember.gender == 'M') {
                        if(endmember.birthYear < startmember.birthYear)
                            return path.back() + "是" + path.front() + "的堂哥";
                        else
                            return path.back() + "是" + path.front() + "的堂弟";
                    }
                    else if (endmember.gender == 'F') {
                        if (endmember.birthYear < startmember.birthYear)
                            return path.back() + "是" + path.front() + "的堂姊";
                        else
                            return path.back() + "是" + path.front() + "的堂妹";
                    }
                }
                else {
                    if (endmember.gender == 'M') {
                        if (endmember.birthYear < startmember.birthYear)
                            return path.back() + "是" + path.front() + "的表兄";
                        else
                            return path.back() + "是" + path.front() + "的表弟";
                    }
                    else if (endmember.gender == 'F') {
                        if (endmember.birthYear < startmember.birthYear)
                            return path.back() + "是" + path.front() + "的表姊";
                        else
                            return path.back() + "是" + path.front() + "的表妹";
                    }
                }
            }
            else if (startmemberparent.gender == 'F') {
                if (endmember.gender == 'M') {
                    if (endmember.birthYear < startmember.birthYear)
                        return path.back() + "是" + path.front() + "的表兄";
                    else
                        return path.back() + "是" + path.front() + "的表弟";
                }
                else if (endmember.gender == 'F') {
                    if (endmember.birthYear < startmember.birthYear)
                        return path.back() + "是" + path.front() + "的表姊";
                    else
                        return path.back() + "是" + path.front() + "的表妹";
                }
            }
        }
    }

    int judgeIfgo(int currentindex, int dir) {
        int nextindex;
        switch (dir)
        {
        case 1:
            nextindex = getParentIndex(currentindex);
            break;
        case 2:
            nextindex = getLeftChildIndex(currentindex);
            break;
        case 3:
            nextindex = getRightChildIndex(currentindex);
            break;

        }
        return nextindex;

    }
    bool isspouse(string start) {
        bool isspouse = false;

        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].name == start)
                isspouse = false;
            else
                isspouse = true;
        }
        return isspouse;
    }
    bool isallparent(vector<string> path) {

        if (heap[nameToIndex[path.front()]].birthYear < heap[nameToIndex[path.back()]].birthYear) {
            for (int i = path.size() - 1; i >= 1; i--) {
                if (path[i - 1] != heap[getParentIndex(nameToIndex[path[i]])].name) {
                    return false;
                }
            }
            return true;
        }
        else {
            for (int i = 0; i < path.size() - 1; i++) {
                if (path[i + 1] != heap[getParentIndex(nameToIndex[path[i]])].name) {
                    return false;
                }
            }
            return true;
        }

    }

    vector<string> findShortestPath(const string& start, const string& end) {
        stack<stackelement> s;
        unordered_map<string, bool> visited;
        bool found = false;
        stackelement first;
        first.name = start;
        first.dir = 0;

        if (isspouse(start)) {
            for (int i = 0; i < heap.size(); i++) {
                if (start == heap[i].spouse)
                    heap[i].name = start;
            }
        }

        s.push(first);
        visited[start] = true;

        while (!s.empty()) {
            stackelement current = s.top();
            s.pop();
            int dir = ++current.dir;

            while (dir <= 3 && !found) {
                int currentIndex = nameToIndex[current.name];
                //cout <<"currentIndex: " << currentIndex << "dir: " << dir << endl;
                int nextindex;
                nextindex = judgeIfgo(currentIndex, dir);
                //cout << "nextindex" << nextindex << endl;
                stackelement next;
                if (nextindex != -1) {
                    next.name = heap[nextindex].name;
                    next.dir = dir;
                }


                if (next.name == end) {
                    s.push(current);
                    found = true;
                }
                else if (!visited[next.name] && nextindex != -1) {
                    //cout << next.name << " " << next.dir << endl;
                    current.dir = next.dir;
                    s.push(current);
                    //cout << "stack: " << current.name << " " << current.dir << endl;
                    visited[next.name] = true;
                    current = next;
                    dir = 1;
                }
                else
                    dir++;
            }

            if (found) {
                vector<string> path;
                path.push_back(end);
                while (!s.empty()) {
                    string stackname = s.top().name;
                    s.pop();
                    path.push_back(stackname);
                }
                reverse(path.begin(), path.end());
                return path;
            }
        }
        return {};
    }

public:
    void addMember(const FamilyMember& member) {
        nameToIndex[member.name] = heap.size();
        heap.push_back(member);
    }

    void processData(const string& start, const string& end) {
        vector<string> path = findShortestPath(start, end);

        if (path.empty()) {
            cout << "無法找到有效路徑" << endl;
            return;
        }

        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) 
                cout << " > ";
        }
        cout << endl;

        string relationship = determineRelationship(path);
        cout << relationship << endl;
    }
};

void readdata(const string& filename, FamilyTree& tree, string& data) {
    ifstream fin(filename);
    string line;


    while (getline(fin, line)) {
        istringstream iss(line);
        string name, gender, birthStr, deathStr, spouse;

        if (iss >> name >> gender >> birthStr >> deathStr >> spouse) {
            FamilyMember member;
            member.name = name;
            member.gender = gender[0];
            member.birthYear = stoi(birthStr);
            member.deathYear = (deathStr == "NULL") ? -1 : stoi(deathStr);
            member.spouse = spouse;

            tree.addMember(member);
        }
        else {
            data = line;
        }
    }

}

int main(int argc, char* argv[]) {

    FamilyTree tree;
    string data;
    readdata("in4.txt", tree, data);
    string start, end;

    istringstream iss(data);
    iss >> start >> end;
    tree.processData(start, end);

    return 0;
}