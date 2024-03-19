#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class dataFile {
protected:
    string* lines;
public:
    void readFile(string path) {
        ifstream readerFile(path);
        string newline;
        int i = 0;
        while (getline(readerFile, newline)) {
            lines[i++] = newline;
        }
        readerFile.close();
        name = lines[0];
        author = lines[1];
        dateModified = lines[2];
        timeModified = lines[3];
    }
    string* zeroDTo1d(string zeroD) {
        int numOfArrayEntries = 0;
        for (int i = 0; i < sizeof(zeroD) / sizeof(char); i++) {
            if (zeroD[i] == ',') {
                numOfArrayEntries++;
            }
        }
        string* outarr = new string[numOfArrayEntries];
        int x = 0;
        for (int i = 0; i < sizeof(zeroD) / sizeof(char); i++) {
            if (zeroD[i] == ',') {
                x++;
            }
            else {
                outarr[x] += zeroD[i];
            }
        }
        return outarr;
    }
    string* zeroDTo1d(string zeroD, int *length) {
        int numOfArrayEntries = 0;
        for (int i = 0; i < sizeof(zeroD) / sizeof(char); i++) {
            if (zeroD[i] == ',') {
                numOfArrayEntries++;
            }
        }
        string* outarr = new string[numOfArrayEntries];
        int x = 0;
        for (int i = 0; i < sizeof(zeroD) / sizeof(char); i++) {
            if (zeroD[i] == ',') {
                x++;
            }
            else {
                outarr[x] += zeroD[i];
            }
        }
        *length = x;
        return outarr;
    }
    string name;
    string author;
    string dateModified;
    string timeModified;
};
class codeFunction {
public:
    string codeName;
    string* requiredFunctions;
    string* requiredAssets;
    bool overridable;
    string* incompatableFunctions;
    string* masterCode;
    string* functionCode;
    void initCodeFunction(string codeName, string* requiredFunctions, string* requiredAssets, bool overridable, string* incompatableFunctions, string* masterCode, string* functionCode) {
        this->codeName = codeName;
        this->requiredFunctions = requiredFunctions;
        this->requiredAssets = requiredAssets;
        this->overridable = overridable;
        this->incompatableFunctions = incompatableFunctions;
        this->masterCode = masterCode;
        this->functionCode = functionCode;
    }

};
class codeFile : public dataFile {
private:
    string codenames0d;
    string overridablecontents0d;
    string requiredcontents0d;//more specific name than contents?
public:
    string* codenames;
    string* overridablecontents;
    string* requiredcontents;
    codeFunction* functions;
    void init(string packageName, string moduleName, string codetype) {
        readFile("./" + packageName + "./" + moduleName + ".module" + codetype);
        codenames0d = lines[4];
        overridablecontents0d = lines[5];
        requiredcontents0d = lines[6];
        codenames = zeroDTo1d(codenames0d);
        overridablecontents = zeroDTo1d(overridablecontents0d);
        requiredcontents = zeroDTo1d(requiredcontents0d);
        int startpos = 7;
        int curpos = 7;
        int numOfFunctions = 0;
        int i = 0;
        while (!(lines[i++].empty())) {
            if (lines[i] == "{=={") {
                numOfFunctions++;
            }
        }
        functions = new codeFunction[numOfFunctions];
        int functionPos = 0;
        while (!(lines[startpos].empty() || lines[curpos].empty()) && lines[startpos] == "{=={" && functionPos <= numOfFunctions) {
            string functionCodeName = lines[curpos++];
            string functionRequiredFunctions = lines[curpos++];
            string functionRequiredAssets = lines[curpos++];
            bool functionOverridable = (lines[curpos] == "true");
            if (!functionOverridable) {
                if (lines[curpos] != "false") {
                    cout << "WARNING::codeFile(" << packageName << "/" << moduleName << "/" << codetype << "_" << functionCodeName << ")::FUNCTION(" << functionCodeName << ")::NOT::VALID::OVERRIDE::BOOLEAN::VALUE ==> Assuming overridable=true";
                    functionOverridable = true;
                }
            }
            curpos++;
            string functionIncompatableFunctions = lines[curpos++];
            int codeLength = 0;
            if (lines[curpos] == "{-{") {
                startpos = curpos;
                while (lines[startpos + codeLength + 1] != "}-}") {
                    codeLength++;
                }
            }
            string* functionMasterCode = new string[codeLength];
            if (lines[curpos] == "{-{") {
                for (int i = 0; i < codeLength; i++) {
                    functionMasterCode[i] = lines[startpos + i + 1];
                }
            }
            startpos += codeLength + 2;
            curpos = startpos;
            int codeLength = 0;
            if (lines[curpos] == "{--{") {
                startpos = curpos;
                while (lines[startpos + codeLength + 1] != "}--}") {
                    codeLength++;
                }
            }
            string* functionFunctionCode = new string[codeLength];
            if (lines[curpos] == "{--{") {
                for (int i = 0; i < codeLength; i++) {
                    functionFunctionCode[i] = lines[startpos + i + 1];
                }
            }
            startpos += codeLength + 3;
            curpos = startpos;
            functions[functionPos].initCodeFunction(functionCodeName, zeroDTo1d(functionRequiredFunctions), zeroDTo1d(functionRequiredAssets), functionOverridable, zeroDTo1d(functionIncompatableFunctions), functionMasterCode, functionFunctionCode);
        }
    }
};
class module : public dataFile {
private:
    string requiredfunctions0d;
    string overridablefunctions0d;
    string requiredassets0d;
    string overridableassets0d;
    string incompatablepackagefunctions0d;
public:
    string* requiredfunctions;
    string* overridablefunctions;
    string* requiredassets;
    string* overridableassets;
    string* incompatablepackagefunctions;
    codeFile moduleclass;
    codeFile moduleutil;
    codeFile modulemenu;
    codeFile modulegame;
    void initModule(string packageName, string moduleName) {
        moduleclass.init(packageName, moduleName, "class");
        moduleutil.init(packageName, moduleName, "util");
        modulemenu.init(packageName, moduleName, "menu");
        modulegame.init(packageName, moduleName, "game");
        readFile("./" + packageName + "./" + moduleName + ".moduleinfo");
        requiredfunctions0d = lines[4];
        overridablefunctions0d = lines[5];
        requiredassets0d = lines[6];
        overridableassets0d = lines[7];
        incompatablepackagefunctions0d = lines[8];
        requiredfunctions = zeroDTo1d(requiredfunctions0d);
        overridablefunctions = zeroDTo1d(overridablefunctions0d);
        requiredassets = zeroDTo1d(requiredassets0d);
        overridableassets = zeroDTo1d(overridableassets0d);
        incompatablepackagefunctions = zeroDTo1d(incompatablepackagefunctions0d);
    }
    //function to create a masterlist of required/overridable functions?
    //or should it be programmed in later when doing selection
    //maybe just block access to a pcakage without its requirements and remove the oveeridables and define that lower down?
    //as required functions is just for things needed throughout, overridable should realistically be defined at the function level

    //have an array of packages and a equal string array of their names, then when you want to access something, enter through the corrensponding string to the package array, then check requirements there in order to solve discrepancies/requirements, then you can go recursively thorugh modules or something and on the module level it is pretty consistent??
    //they can mix and match individual functions (for that is the whole point of overrides) so therefore we need to also when we enter into a module array we need to do the same checks for requirements and then we need to enter each sub-module and check, once again, recursively (or sequentially idk the word) for the requirements then by doing this, we should create a string array of normalised ids (function/module/functioncodename), to get all requirements, then we can present this to the user or try to solve automatically, then we can recalculate every time changes are made, dont try to optimise something like that to only remove parts of it as that would require keeping track of where they are coming from and  will be much more complex than just running the code again
};
class package : public dataFile {
private:
    string moduleNames0d;
public:
    module* modules;
    string* moduleNames;
    void initPackage(string packageName) {
        readFile("./" + packageName + "./" + packageName + ".packageinfo");
        moduleNames0d = lines[4];
        int numOfModules;
        moduleNames = zeroDTo1d(moduleNames0d, &numOfModules);
        modules = new module[numOfModules];
        for (int i = 0; i < numOfModules; i++) {
            modules[i].initModule(packageName, moduleNames[i]);
        }
    }
};
class packageArray {
public:
    package* packages;
    void createPackages(string* packageNames) {
        int i = 0;
        while (!packageNames[i].empty()) {
            i++;
        }
        packages = new package[i];
        int i = 0;
        while (!packageNames[i].empty()) {
            packages[i].initPackage(packageNames[i]);
            i++;
        }
    }
    void createPackages(string packageNamesString) {
        int numOfPackages;
        string* packageNames = zeroDTo1d(packageNamesString, &numOfPackages);
        packages = new package[numOfPackages];
        int i = 0;
        while (!packageNames[i].empty()) {
            packages[i].initPackage(packageNames[i]);
            i++;
        }
    }
    string* zeroDTo1d(string zeroD, int* length) {
        int numOfArrayEntries = 0;
        for (int i = 0; i < sizeof(zeroD) / sizeof(char); i++) {
            if (zeroD[i] == ',') {
                numOfArrayEntries++;
            }
        }
        string* outarr = new string[numOfArrayEntries];
        int x = 0;
        for (int i = 0; i < sizeof(zeroD) / sizeof(char); i++) {
            if (zeroD[i] == ',') {
                x++;
            }
            else {
                outarr[x] += zeroD[i];
            }
        }
        *length = x;
        return outarr;
    }
};
int main()
{
    std::cout << "Hello World!\n";
}
