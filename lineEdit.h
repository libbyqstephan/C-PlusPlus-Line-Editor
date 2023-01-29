#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>

using namespace std;

class lineEdit
{
private:
    vector<string> fileList;
    string line;
    fstream File;

public:
    lineEdit(string inputFile);
    lineEdit();
    ~lineEdit();
    void beginEdit();

protected:
    string userFile;
    string lastFileSaved;
    int currentLine = 1;
    vector<string> tokenizeLine(string line);
    void openFile();
    void beginEditHelper();
    void printFile();
    bool QCommand();
    bool validCommand(vector<string> tokenizedInput);
    void WCommand(vector<string> lineVector);
    void JCommand(vector<string> lineVector);
    void ICommand(vector<string> lineVector);
    void ACommand(vector<string> lineVector);
    void LCommand(vector<string> lineVector);
    void DCommand(vector<string> lineVector);
    bool isNumber(vector<string> lineVector);
};
lineEdit::lineEdit(string inputFile)
{
    userFile = inputFile;
}
lineEdit::lineEdit()
{
}
lineEdit::~lineEdit()
{
}

void lineEdit::beginEdit()
{
    openFile();
    beginEditHelper();    
}
void lineEdit::openFile()
{
    if(userFile.empty() == 0) //userFile is not empty
    {
        string inLine;
        File.open(userFile);

        if(File.is_open())
        {
            while(getline(File, inLine))
                fileList.push_back(inLine);
        }
        File.close();
        printFile();
    }
}
void lineEdit::beginEditHelper()
{
    bool continueOn = true;
    while(continueOn == true)
    {
        try{
            cout << "Awaiting command:" << endl;
            getline(cin, line);
            vector<string> tokenizedLine = tokenizeLine(line);
            continueOn = validCommand(tokenizedLine);
        }catch(const char* msg){
            cerr << msg << endl;
        }
    }
}
bool lineEdit::QCommand()
{
    //turn last saved file into vector
    vector<string> originalFile;
    string FileLine;
    File.open(lastFileSaved);
    if(File.is_open())
    {
        while(getline(File, FileLine))
            originalFile.push_back(FileLine);
    }
    File.close();

    //compare to buffer vector
    vector<string>::iterator it1 = originalFile.begin();
    vector<string>::iterator it2 = fileList.begin();

    if(originalFile.size() == fileList.size())
    {
        int counter = 0;
        for(it1 = originalFile.begin(); it1 != originalFile.end(); it1++)
        {
            for(it2 = fileList.begin(); it2 != fileList.end(); it2++)
            {
                if(*it1 == *it2)
                    counter++;
            }
        }

        if(counter == originalFile.size())
            return false; //save was made, continueOn will now be false
    }
    //save has not been made  
        cout << "Warning: changes have not been saved." << endl;
        cout << "Press 'Q' again if you would like to quit." << endl;
        cout << "Press any other key to resume session." << endl;
        string input;
        getline(cin, input);
        if(input == "Q")
            return false; //user does not care
        else
        {
            line = "";
            return true; //user will go back
        }
}

void lineEdit::printFile()
{ 
    int lineNum = 1;
    vector<string>::iterator it;

    cout << "------Current File Editing------" << endl;
    for(it = fileList.begin(); it != fileList.end(); it++)
    {
        cout << lineNum << ":";
        if(lineNum == currentLine)
        {
            cout << "> " << *it << endl;
            lineNum++;
            continue;
        }
        cout << "  " << *it << endl;
        lineNum++;
    }
}

vector<string> lineEdit::tokenizeLine(string line)
{
    // Vector of string to save tokens
    vector <string> tokens;
     
    // stringstream class lineStream
    stringstream lineStream(line);
    string word;
     
    // Tokenizing w.r.t. space ' '
    while(lineStream>>word)
        tokens.push_back(word);
     
    return tokens;
}

bool lineEdit::validCommand(vector<string> tokenizedInput)
{
    if(tokenizedInput[0] == "W")
    {
        //throw if..
        //size is greater than 2
        if(tokenizedInput.size() > 2)
            throw "Only 2 max arguments allowed.";
        //size is equal to 1 but there is no file opened
        if((tokenizedInput.size() == 1) && (userFile.empty() == 1))
            throw "A file must be included if no file has been opened.";
        WCommand(tokenizedInput);
    }
    else if(tokenizedInput[0] == "J")
    {
        //throw if...
        //size is not equal to 2
        if(tokenizedInput.size() != 2)
            throw "Must include 2 arguments.";
        //content is not a number (excluding -1)
        if(tokenizedInput[1] == "-1")
        {
            JCommand(tokenizedInput);
            return true;
        }
        if(isNumber(tokenizedInput) == false)
            throw "Argument must be an integer.";
        //content is not in range
        if(stoi(tokenizedInput[1]) <  0 || stoi(tokenizedInput[1]) > fileList.size())
            throw "Number line does not exist.";
        JCommand(tokenizedInput);
    }
    else if(tokenizedInput[0] == "I")
        ICommand(tokenizedInput);
    else if(tokenizedInput[0] == "A")
    {   
        //throw if...
        //file size is less than 1 (a line must exist already)
        if(fileList.size() < 1)
            throw "Cannot append to an empty file.";
        ACommand(tokenizedInput);
    }
    else if(tokenizedInput[0] == "L" || tokenizedInput[0] == "D" )
    {
        //throw if...
        //size is greater than 3
        if(tokenizedInput.size() > 3)
            throw "Only 2 max arguments allowed.";
        //contents are numbers
        if(tokenizedInput.size() > 1)
        {
            if(isNumber(tokenizedInput) == false)
                throw "Arguments must be an integer.";
            //second num is < first num, and nums are in range
            if(tokenizedInput.size() == 3)
            {
                if(stoi(tokenizedInput[2]) <= stoi(tokenizedInput[1]))
                    throw "Second argument must be greater than first argument";
                if(stoi(tokenizedInput[1]) <  1 || stoi(tokenizedInput[1]) > fileList.size())
                    throw "Line does not exist.";
                if(stoi(tokenizedInput[2]) <  1 || stoi(tokenizedInput[2]) > fileList.size())
                    throw "Line does not exist.";
            }
            //num is in range
            if(stoi(tokenizedInput[1]) <  1 || stoi(tokenizedInput[1]) > fileList.size())
                throw "Line does not exist.";
        }
        if(tokenizedInput[0] == "L")
            LCommand(tokenizedInput);
        if(tokenizedInput[0] == "D")
            DCommand(tokenizedInput);
    }
    else if(tokenizedInput[0] == "Q")
    {
        //throw if...
        //size is more than 1
        if(tokenizedInput.size() > 1)
            throw "Does not need arguments.";
        return QCommand();
    }
    else //anything else that's not a command
        throw "Invalid command. Try again.";
    return true;
}
void lineEdit::WCommand(vector<string> lineVector)
{
    if(lineVector.size() == 2) //file name given
    {
        vector<string>::iterator it;
        vector<string>::iterator it2;
        it2 = lineVector.begin() + 1;
        File.open(*it2, ios::out|ios::trunc);
        for(it = fileList.begin(); it != fileList.end(); it++)
            File << *it << endl;
        lastFileSaved = *it2;
        File.close();
    }
    else if (lineVector.size() == 1) //no file name given
    {
        vector<string>::iterator it;
        File.open(userFile, ios::out|ios::trunc);
        for(it = fileList.begin(); it != fileList.end(); it++)
            File << *it << endl;
        lastFileSaved = userFile;
        File.close(); 
    }
    printFile();
}
void lineEdit::JCommand(vector<string> lineVector)
{
    //covert string to int
    int num = stoi(lineVector[1]);
    if(num == -1)
        currentLine = fileList.size();
    else if(num == 0)
        currentLine = 1;
    else
        currentLine = num;
    printFile();
}
void lineEdit::ICommand(vector<string> lineVector)
{   
    string message;
    vector<string>::iterator it;

    //remove command
    lineVector.erase(lineVector.begin());

    //translate vector into single string
    for(it = lineVector.begin(); it != lineVector.end(); it++)
    {
        message += *it;
        message += " ";
    }
    //cout << "currentLine in ICommand: " << currentLine << endl;

    fileList.insert(fileList.begin()+currentLine-1, message);
    if(fileList.size() > 1)
        currentLine++;
    printFile();
}
void lineEdit::ACommand(vector<string> lineVector)
{
    string message;
    vector<string>::iterator it;

    //remove command
    lineVector.erase(lineVector.begin());

    //translate vector into single string
    for(it = lineVector.begin(); it != lineVector.end(); it++)
    {
        message += *it;
        message += " ";
    }

    fileList.insert(fileList.begin()+currentLine, message);
    currentLine++;
    printFile();
}
void lineEdit::LCommand(vector<string> lineVector)
{
    vector<string>::iterator it;

    cout << "---Contents---" << endl;
    if (lineVector.size() == 1) //only L
    {
        for(it = fileList.begin(); it != fileList.end(); it++)
            cout << *it << endl;
    }
    else if (lineVector.size() == 2) //L with a num
        cout << fileList[stoi(lineVector[1])-1] << endl;
    else //L with 2 nums
    {
        int firstNum = stoi(lineVector[1]);
        int lastNum = stoi(lineVector[2]);
        for(it = fileList.begin()+firstNum-1; it != fileList.begin()+lastNum; it++)
            cout << *it << endl;
    }
    printFile();
}
void lineEdit::DCommand(vector<string> lineVector)
{
    if (lineVector.size() == 1)
        fileList.erase(fileList.begin() + currentLine - 1);
    else if (lineVector.size() == 2)
    {
        int firstNum = stoi(lineVector[1]);
        fileList.erase(fileList.begin()+firstNum-1);
    }
    else
    {
        int firstNum = stoi(lineVector[1]);
        int lastNum = stoi(lineVector[2]);
        fileList.erase(fileList.begin()+firstNum-1, fileList.begin()+lastNum);
    }
    //moving currentLine if it was deleted
    if(currentLine > fileList.size())
    {
        if(fileList.size() == 0)
            currentLine = fileList.size() + 1;
        else
            currentLine = fileList.size();
    }
    printFile();
}
bool lineEdit::isNumber(vector<string> lineVector)
{
    //remove command
    lineVector.erase(lineVector.begin());

    vector<string>::iterator it;
    for(it = lineVector.begin(); it != lineVector.end(); it++)
    {
        char charArray[it->size() + 1];
        strcpy(charArray, it->c_str());
        for(int i = 0; i < it->size(); i++)
        {
            if(!isdigit(charArray[i]))
                return false;
        }
    }
    return true;
}