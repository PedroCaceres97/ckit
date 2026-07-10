#include <stdbool.h>

const char* u32str();
const char* u64str();
const char* i32str();
const char* i64str();
const char* f32str();
const char* f64str();
const char* x32str();
const char* x64str();
const char* ptrstr();
const char* boolstr();
const char* sizestr();
const char* diffstr();

/*
    const char* 32STR();
    const char* u64STR();
    const char* i32STR();
    const char* i64STR();
    const char* f32STR();
    const char* f64STR();
    const char* x32STR();
    const char* x64STR();
    const char* ptrSTR();
    const char* boolSTR();
    const char* sizeSTR();
    const char* diffSTR();
*/

const char* ansi256();
const char* ansirgb();
const char* ansipos();
const char* ansimove();
const char* ansiformat();

/*
const char* ansi256();
const char* ansiRGB();
const char* ansiPOS();
const char* ansiMOVE();
const char* ansiFORMAT();

const char* ANSI256();
const char* ANSIrgb();
const char* ANSIpos();
const char* ANSImove();
const char* ANSIformat();
*/

typedef enum {
    FOPEN_NONE = 0,
    FOPEN_READ = 1,
    FOPEN_WRITE = 2,
} FileOpen;

typedef enum {
    FSEEK_SET,
    FSEEK_CUR,
    FSEEK_END
} FileSeek;

typedef struct File File;

bool copydir();
bool checkdir();
bool createdir();
bool removedir();
bool renamedir();

bool copyfile();
bool checkfile();
bool createfile();
bool removefile();
bool renamefile();

int openfile();
void closefile();
size_t dumpfile();
size_t readfile();
size_t writefile();
size_t printfile();
size_t tellfile();
size_t seekfile();

size_t format();
size_t fformat();
size_t sformat();
size_t snformat();
size_t vsnformat();

void print();
void printlog();
void printinform();

void getinput();

/*
bool copyDIR();
bool checkDIR();
bool createDIR();
bool removeDIR();
bool renameDIR();

bool copyFILE();
bool checkFILE();
bool createFILE();
bool removeFILE();
bool renameFILE();

bool DIRcopy();
bool DIRcheck();
bool DIRcreate();
bool DIRremove();
bool DIRrename();

bool FILEcopy();
bool FILEcheck();
bool FILEcreate();
bool FILEremove();
bool FILErename();
*/

/*
#include <mystd\mystd.h>

#ifdef MY_OS_WINDOWS
    #include <direct.h>
    #define chdir _chdir
    #define getcwd _getcwd
#else
    #include <unistd.h>
#endif

#include <MySetupProject/flags.h>
#include <MySetupProject/templates.h>

char current[512] = {0};
char* parent = current;

void UnkownFlagCallback(const char* arg) {
    printlog(PRINT_FATAL, "Unkown flag (%s)", arg);
}

void WriteSensible(const char* filename, const char* template) {
    printlog(PRINT_WARNING, "Writting latest %s template will erase any change.", filename);
    print("(Insert 'y' to confirm) > ");
    char* confirm = getinput();
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printlog(PRINT_WARNING, "Latest %s aborted", filename);
        return;
    }

    print("\n");
    File* file = openfile(filename, FOPEN_WRITE | FOPEN_TRUNCATE);
    printfile(file, template);
    closefile(file);
    printlog(PRINT_SUCCESS, "Latest %s template written", filename);
}

void MakeDirectories() {
    createdir("src");
    createdir("bin");
    createdir("obj");
    createdir("lib");
    createdir("debug/bin");
    createdir("debug/obj");
    createdir("debug/lib");
    createdir(".vscode");
    char temp[512] = {0};
    for (size_t i = 0; i < strlen(parent); i++) {
        temp[i] = tolower(parent[i]);
    }
    createdir(format("include/%s", temp));
}
void WriteMakefile() {
    MyFile* makefile = MyFileOpen("Makefile", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(makefile, makefileTemplate);
    MyFileClose(makefile);
}
void WriteMkconfig() {
    MyFile* mkconfig = MyFileOpen("config.mk", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(mkconfig, makefileConfigTemplate);
    MyFileClose(mkconfig);
}
void WriteMktargets() {
    MyFile* mktargets = MyFileOpen("targets.mk", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(mktargets, makefileTargetsTemplate);
    MyFileClose(mktargets);
}
void WriteMIT() {
    const char* date = MY_TERNARY(year.trigged, year.value, "<YEAR>");
    const char* auth = MY_TERNARY(author.trigged, author.value, "<AUTHOR>");
    if (!year.trigged) { MyShortLog(MY_WARNING, "Missing year writting '<YEAR>' as a default, to provide a year use --year=[value] or -y[value]"); }
    if (!author.trigged) { MyShortLog(MY_WARNING, "Missing author writting '<AUTHOR>' as a default, to provide an author use --author=[value] or -a[value]"); }
    
    MyFile* MITfile = MyFileOpen("LICENSE", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(MITfile, "MIT License\n\n");
    MyFprintf(MITfile, "Copyright (c) %s %s\n\n", date, auth);
    MyFilePrint(MITfile, MITLicense);
    MyFileClose(MITfile);
    MyShortLog(MY_SUCCESS, "Latest MIT LICENSE template written");
}
void WriteGenerics() {
    MyFile* main = MyFileOpen("src/main.c", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(main, mainTemplate);
    MyFileClose(main);

    MyFile* gitignore = MyFileOpen(".gitignore", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(gitignore, gitignoreTemplate);
    MyFileClose(gitignore);

    MyFile* clangd = MyFileOpen(".clangd", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(clangd, clangdTemplate);
    MyFileClose(clangd);

    MyFile* settings = MyFileOpen(".vscode/settings.json", MY_FILE_WRITE | MY_FILE_CREATE);
    MyFilePrint(settings, settingsTemplate);
    MyFileClose(settings);

    MyShortLog(MY_SUCCESS, "Generic files were succesfully created");
}
void PublicRepositorie() {
    system("git init --initial-branch=main");
    system(MySprintf("gh repo create %s --public --source=. --remote=origin", parent));
    system(MySprintf("git remote add origin git@github.com:%s/%s.git", githubPublic.value, parent));
    system("git add .");
    system("git commit -m \"Initial Commit\"");
    system("git push -u origin main");
    MyShortLog(MY_SUCCESS, "Public github repositorie created");
}
void PrivateRepositorie() {
    system("git init --initial-branch=main");
    system(MySprintf("gh repo create %s --private --source=. --remote=origin", parent));
    system(MySprintf("git remote add origin git@github.com:%s/%s.git", githubPrivate.value, parent));
    system("git add .");
    system("git commit -m \"Initial Commit\"");
    system("git push -u origin main");
    MyShortLog(MY_SUCCESS, "Private github repositorie created");
}

int main(int argc, const char** argv) {
    atexit(MyTerminate);
    MyInit();

    MY_ASSERT(argc > 1,"No arguments were provided try: MySetupProject --project=[value] or MySetupProject --dirs");
    MyPrintf("\n");

    getcwd(current, sizeof(current));
    MY_ASSERT(strlen(current) > 0, "Failed getcwd");
    parent = MyLastPathDivisor(current);
    MY_ASSERT(parent++, "Invalid value returned by getcwd() -> %s", current);

    bool help = MyFlags_Parse(flags, sizeof(flags) / sizeof(MyFlag*), &argv[1], argc - 1, UnkownFlagCallback);
    if (help) { return 0; }

    if (project.trigged) {
        MyMakeDir(project.value);
        chdir(project.value);
        parent = project.value;

        MakeDirectories();
        WriteMIT();
        WriteMktargets();
        WriteMkconfig();
        WriteMakefile();
        WriteGenerics();
        MyShortLog(MY_SUCCESS, "Project created");
    }

    if (version.trigged) {
        MyPrintf("MySetupProject v0.5 (Compiled under %s mode)\n\n",
            #ifdef NDEBUG
                "release"
            #else
                "debug"
            #endif
        );
    }

    if (dirs.trigged) {
        MakeDirectories();
        MyShortLog(MY_SUCCESS, "Directories were succesfully created");
    }

    if (generics.trigged) {
        WriteGenerics();
    }

    if (MIT.trigged) {
        WriteMIT();
    }

    if (makefile.trigged) {
        WriteSensible("Makefile", makefileTemplate);
    }

    if (mkconfig.trigged) {
        WriteSensible("config.mk", makefileConfigTemplate);
    }

    if (mktargets.trigged) {
        WriteSensible("targets.mk", makefileTargetsTemplate);
    }

    if (clangd.trigged) {
        WriteSensible(".clangd", clangdTemplate);
    }

    if (settings.trigged) {
        MyMakeDir(".vscode");
        WriteSensible(".vscode/settings.json", settingsTemplate);
    }

    if (gitignore.trigged) {
        WriteSensible(".gitignore", gitignoreTemplate);
    }

    if (githubPublic.trigged) {
        PublicRepositorie();
    } else if (githubPrivate.trigged) {
        PrivateRepositorie();
    }
}
*/