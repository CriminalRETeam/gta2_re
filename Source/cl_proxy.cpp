#if defined(__clang__) || (_MSC_VER <= 1200)
    #define _Lockit MyStubLockit
#endif

#include <algorithm>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>

#if defined(__clang__)
namespace std
{
MyStubLockit::MyStubLockit()
{
}

MyStubLockit::~MyStubLockit()
{
}
} // namespace std
#endif

//#define DEBUG_ENABLED

#ifndef INVALID_FILE_ATTRIBUTES
    #define INVALID_FILE_ATTRIBUTES ((DWORD) - 1)
#endif

class Logger
{
  public:
    Logger()
    {
        DWORD pid = GetCurrentProcessId();
        wsprintf(logPath, "proxy_log_%lu.txt", pid);
        logBuffer = "";
    }

    ~Logger()
    {
        flush(); // Show output in debugger
#if defined(DEBUG_ENABLED)
        writeToFile(); // Persist output to disk
#endif
    }

    void debug(const char* msg)
    {
        append(msg);
        flushLine();
    }

    void debugCode(const char* label, DWORD code)
    {
        char temp[64];
        wsprintf(temp, "%s: %lu", label, code);
        append(temp);
        flushLine();
    }

    void debugStr(const std::string& msg)
    {
        append(msg.c_str());
        flushLine();
    }

  private:
    char logPath[MAX_PATH];
    std::string logBuffer;

    void append(const char* msg)
    {
        logBuffer += msg;
        logBuffer += "\n";
    }

    void flushLine()
    {
        // Emit in chunks if needed
        const size_t chunkSize = 512;
        size_t offset = 0;
        while (offset < logBuffer.size())
        {
            std::string chunk = logBuffer.substr(offset, chunkSize);
            char header[64];
            wsprintf(header, "[proxy-%lu] ", GetCurrentProcessId());
            std::string output = std::string(header) + chunk;
            OutputDebugStringA(output.c_str());
            offset += chunkSize;
        }
    }

    void flush()
    {
        // Optional: do nothing, or flush additional details
    }

    void writeToFile()
    {
        FILE* fp = fopen(logPath, "a");
        if (!fp)
            return;
        fwrite(logBuffer.c_str(), 1, logBuffer.size(), fp);
        fclose(fp);
    }
};

std::string stripExtension(const std::string& path)
{
    size_t dot = path.find_last_of('.');
    return (dot != std::string::npos) ? path.substr(0, dot) : path;
}

bool fileExists(const std::string& path)
{
    DWORD attr = GetFileAttributes(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

std::string extractBaseName(const std::string& fullPath)
{
    size_t sep = fullPath.find_last_of("\\/");
    std::string justName = (sep != std::string::npos) ? fullPath.substr(sep + 1) : fullPath;

    size_t dot = justName.find_last_of('.');
    return (dot != std::string::npos) ? justName.substr(0, dot) : justName;
}

std::vector<std::string> tokenize(const std::string& line)
{
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    bool inQuotes = false;
    std::string current;

    while (stream >> std::ws)
    {
        char ch = stream.peek();
        if (ch == '"')
        {
            stream.get(); // consume quote
            std::getline(stream, current, '"');
            tokens.push_back(current);
        }
        else
        {
            stream >> token;
            tokens.push_back(token);
        }
    }

    return tokens;
}

enum CrtMode
{
    CRT_ML, // Single-threaded static
    CRT_MT, // Multi-threaded static
    CRT_MD // Multi-threaded DLL
};

CrtMode detectCrtMode(const std::string& compilerFlags)
{
    if (compilerFlags.find("/MD") != std::string::npos)
        return CRT_MD;
    if (compilerFlags.find("/MT") != std::string::npos)
        return CRT_MT;
    if (compilerFlags.find("/ML") != std::string::npos)
        return CRT_ML;

    // Default to ML if nothing matches
    return CRT_ML;
}

std::string getCrtDefines(CrtMode mode)
{
    std::string flags = "-D_X86_ -DWIN32 -D_WINDOWS -fms-compatibility -fms-extensions -m32 ";
    switch (mode)
    {
        case CRT_MT:
            flags += "-D_MT ";
            break;
        case CRT_MD:
            flags += "-D_MT -D_DLL ";
            break;
        case CRT_ML:
        default:
            // No extra defines
            break;
    }
    return flags;
}

bool sanitizeFlags(const std::string& rawLine, std::string& cleanFlags, std::string& sourcePath)
{
    cleanFlags = "";
    sourcePath = "";

    std::vector<std::string> tokens = tokenize(rawLine);
    bool expectD = false, expectI = false;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const std::string& token = tokens[i];

        if (token == "-c" || token == "/c")
        {
            if (i + 1 < tokens.size())
                sourcePath = tokens[i + 1];
            ++i;
            continue;
        }

        if (token.find("/Fo") == 0 || token.find("/Fd") == 0 || token == "/TP" || token == "/EHsc" || token == "/GX" || token == "/O2" ||
            token == "/ML" || token == "/Zm1000")
            continue;

        if (expectD)
        {
            cleanFlags += "/D" + token + " ";
            expectD = false;
        }
        else if (expectI)
        {
            cleanFlags += "/I" + token + " ";
            expectI = false;
        }
        else if (token == "/D" || token == "-D")
        {
            expectD = true;
        }
        else if (token == "/I" || token == "-I")
        {
            expectI = true;
        }
        else if ((token.find("/D") == 0 || token.find("-D") == 0) && token.length() > 2)
        {
            cleanFlags += token + " ";
        }
        else if ((token.find("/I") == 0 || token.find("-I") == 0) && token.length() > 2)
        {
            cleanFlags += token + " ";
        }
        else if (token == "/W3" || token == "/nologo" || token == "/DWIN32" || token == "/D_WINDOWS")
        {
            cleanFlags += token + " ";
        }
    }

    // Add INCLUDE env paths
    const char* includeEnv = getenv("INCLUDE");
    if (includeEnv)
    {
        std::istringstream incs(includeEnv);
        std::string path;
        while (std::getline(incs, path, ';'))
        {
            if (!path.empty())
                cleanFlags += "-I\"" + path + "\" ";
        }
    }

    // Compatibility defines
    cleanFlags += "-Wno-nonportable-include-path ";

    CrtMode mode = detectCrtMode(rawLine);
    std::string defines = getCrtDefines(mode);
    cleanFlags += defines;

    return !sourcePath.empty();
}

std::string quoteIfNeeded(const std::string& path)
{
    if (path.find(' ') != std::string::npos || path.find('\t') != std::string::npos)
        return "\"" + path + "\"";
    return path;
}

// Jenkins One-at-a-Time hash (C++98 safe)
unsigned int jenkins_hash_string(const std::string& input)
{
    unsigned int hash = 0;
    for (std::string::const_iterator it = input.begin(); it != input.end(); ++it)
    {
        hash += static_cast<unsigned char>(*it);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

std::string jenkins_hash_hex(const std::string& input)
{
    unsigned int hash = jenkins_hash_string(input);

    char buffer[16];
    sprintf(buffer, "%08X", hash);

    return std::string(buffer);
}


class NamedMutex
{
public:
    NamedMutex(const std::string& name)
        : hMutex_(NULL), acquired_(false)
    {
        hMutex_ = CreateMutexA(NULL, FALSE, name.c_str());
        if (hMutex_ != NULL)
        {
            DWORD waitResult = WaitForSingleObject(hMutex_, INFINITE);
            acquired_ = (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_ABANDONED);
        }
    }

    ~NamedMutex()
    {
        if (acquired_ && hMutex_ != NULL)
            ReleaseMutex(hMutex_);

        if (hMutex_ != NULL)
            CloseHandle(hMutex_);
    }

    // Optional: check if we successfully acquired the mutex
    bool is_acquired() const
    {
        return acquired_;
    }

private:
    HANDLE hMutex_;
    bool acquired_;

    // Disable copy semantics to avoid multiple releases
    NamedMutex(const NamedMutex&);
    NamedMutex& operator=(const NamedMutex&);
};

std::string to_lowercase(const std::string& input)
{
    std::string result = input;
    for (std::string::size_type i = 0; i < result.length(); ++i)
    {
        result[i] = static_cast<char>(tolower(result[i]));
    }
    return result;
}


class Preprocessor
{
  public:
    Preprocessor(Logger& logger) : mLogger(logger)
    {
    }

    bool preprocess(const std::string& sourcePath, const std::string& cleanFlags, std::string& fullPathToUniqueFile)
    {
        char cwd[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, cwd);
        const std::string strCwd(cwd);

        const std::string sourceFileNameNoPathOrExt = extractBaseName(sourcePath);
        const std::string mutexName = "Global\\" + to_lowercase(sourceFileNameNoPathOrExt);
        NamedMutex mutex(sourceFileNameNoPathOrExt);
        if (!mutex.is_acquired())
        {
            // Well now the build is fucked, good luck
            mLogger.debug("Failed to get mutex");
        }

        // My god why? clang-cl will just use input.cpp and produce output.i, so when input.cpp is compiled twice
        // with diff arguments it is totally fucked, take the base name and use it as mutex to prevent this.
        // Is it clang? Are we passing bad arguments? Is it wine? Who knows, mutex works, sigh.
        const std::string outputClangCLWillUse = sourceFileNameNoPathOrExt + ".i";
        const std::string fullPathToOutputClangCLWillUse = strCwd + "\\" + outputClangCLWillUse;

        const std::string tmpUniqueFile = sourceFileNameNoPathOrExt + "_" + jenkins_hash_hex(sourceFileNameNoPathOrExt + cleanFlags);
        fullPathToUniqueFile = strCwd + "\\" + tmpUniqueFile + ".pre.c";

        std::string cmd = "clang-cl -E -P " + cleanFlags + " " + quoteIfNeeded(sourcePath) + " -o " + quoteIfNeeded(outputClangCLWillUse);

        mLogger.debug("Running:");
        mLogger.debug(cmd.c_str());
/*
        // Warning: output names may clash, careful when using for debugging
        std::string cmdLog = sourceFileNameNoPathOrExt + ".cmd.txt";
        std::ofstream log(cmdLog.c_str());
        log << cmd << "\n";
        log.close();
*/
        STARTUPINFOA si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            mLogger.debug("clang-cl failed to launch.");
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode = 1;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        mLogger.debugCode("clang exit", exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (fileExists(fullPathToOutputClangCLWillUse))
        {
            MoveFileExA(fullPathToOutputClangCLWillUse.c_str(), fullPathToUniqueFile.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
            std::string s = "Moved " + fullPathToOutputClangCLWillUse + " to " + fullPathToUniqueFile;
            mLogger.debugStr(s);
        }
        else
        {
            mLogger.debug("No .i found in CWD — preprocessing failed.");
            return false;
        }

        return true;
    }

  private:
    Logger& mLogger;
};

// Is this a huge hack? Ofc, do I care? No
bool shouldSkipClangForArg(const std::string& arg)
{
    // 3rdParty dir structure doesn't get re-created inside the build dir resulting in the input not being found
    // so bin the whole thing, we don't use new macros or matching code in there anyway
    return arg.find("detours.cpp") != std::string::npos || arg.find("disasm.cpp") != std::string::npos ||
        arg.find("3rdParty") != std::string::npos;
}

class ClInvoker
{
  public:
    ClInvoker(Logger& logger) : mLogger(logger)
    {
    }

    bool invoke(const std::string& cmdLine)
    {
        mLogger.debug("Launching cl_real.exe:");
        mLogger.debug(cmdLine.c_str());

        STARTUPINFOA si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            printf("Failed to launch cl_real.exe\n");
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }

  private:
    Logger& mLogger;
};

class ClangArtifactFixer
{
  public:
    bool clean(const std::string& path)
    {
        std::vector<std::string> lines;

        FILE* fp = fopen(path.c_str(), "r");
        if (!fp)
            return false;

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp))
        {
            std::string line(buffer);
            stripNewline(line);
            lines.push_back(line);
        }
        fclose(fp);

        std::vector<std::string> cleaned;
        cleaned.push_back("typedef unsigned short wchar_t;");

        int i;
        for (i = 0; i < lines.size(); ++i)
        {
            if (!isClangNoise(lines[i]))
                cleaned.push_back(lines[i]);
        }

        std::vector<std::string> fixed = fixBrokenStrings(cleaned);
        std::vector<std::string> merged = mergeAdjacentStrings(fixed);

        fp = fopen(path.c_str(), "w");
        if (!fp)
            return false;

        for (i = 0; i < merged.size(); ++i)
            fprintf(fp, "%s\n", merged[i].c_str());

        fclose(fp);
        return true;
    }

  private:
    void stripNewline(std::string& line)
    {
        size_t pos = line.find_last_not_of("\r\n");
        if (pos != std::string::npos)
            line.erase(pos + 1);
        else
            line = "";
    }

    bool isClangNoise(const std::string& line)
    {
        if (line.empty() || line[0] != '#')
            return false;
        if (line.length() > 2 && isdigit(line[2]) && line.find('"') != std::string::npos)
            return true;
        return false;
    }

    int countQuotes(const std::string& s)
    {
        int count = 0;
        for (size_t i = 0; i < s.length(); ++i)
            if (s[i] == '"')
                ++count;
        return count;
    }

    std::vector<std::string> fixBrokenStrings(const std::vector<std::string>& lines)
    {
        std::vector<std::string> result;
        std::string buffer;
        bool inString = false;

        for (size_t i = 0; i < lines.size(); ++i)
        {
            const std::string& line = lines[i];
            if (inString)
            {
                buffer += line;
                if (countQuotes(buffer) % 2 == 0)
                {
                    result.push_back(buffer);
                    buffer = "";
                    inString = false;
                }
            }
            else
            {
                if (countQuotes(line) % 2 != 0)
                {
                    buffer = line;
                    inString = true;
                }
                else
                {
                    result.push_back(line);
                }
            }
        }

        if (!buffer.empty())
            result.push_back(buffer);

        return result;
    }

    std::vector<std::string> mergeAdjacentStrings(const std::vector<std::string>& lines)
    {
        std::vector<std::string> result;
        std::string buffer;
        bool merging = false;

        for (size_t i = 0; i < lines.size(); ++i)
        {
            const std::string& line = lines[i];
            std::string trimmed = line;
            size_t first = trimmed.find_first_not_of(" \t");
            if (first != std::string::npos)
                trimmed = trimmed.substr(first);

            // Check if line is a pure string literal
            if (isPureStringLiteral(trimmed))
            {
                if (!merging)
                {
                    buffer = trimmed;
                    merging = true;
                }
                else
                {
                    // Merge with previous
                    buffer = buffer.substr(0, buffer.length() - 1) + trimmed.substr(1); // remove trailing " and leading "
                }
            }
            else
            {
                if (merging)
                {
                    result.push_back(buffer);
                    buffer = "";
                    merging = false;
                }
                result.push_back(line);
            }
        }

        if (merging && !buffer.empty())
            result.push_back(buffer);

        return result;
    }

    bool isPureStringLiteral(const std::string& line)
    {
        if (line.length() < 2)
            return false;
        if (line[0] != '"' || line[line.length() - 1] != '"')
            return false;
        for (size_t i = 1; i < line.length() - 1; ++i)
        {
            if (line[i] == '"' && line[i - 1] != '\\')
                return false; // embedded quote
        }
        return true;
    }
};

class ToolchainEngine
{
  public:
    ToolchainEngine() : mPreprocessor(mLogger), mInvoker(mLogger)
    {
    }

    bool isResponseFile(const std::string& arg)
    {
        return !arg.empty() && arg[0] == '@';
    }

    bool processResponseFile(const std::string& rspPath, std::vector<std::string>& outLines, std::string& outPreCFile, bool& runClang)
    {
        FILE* fp = fopen(rspPath.c_str(), "r");
        if (!fp)
        {
            mLogger.debug("Failed to open response file:");
            mLogger.debug(rspPath.c_str());
            return false;
        }

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp))
        {
            std::string line(buffer);
            line.erase(line.find_last_not_of("\r\n") + 1);
            //mLogger.debug(line.c_str());

            // Extract flags + source path for clang
            std::string clangFlags, sourcePath;
            if (!sanitizeFlags(line, clangFlags, sourcePath))
            {
                mLogger.debug("Could not extract source path:");
                mLogger.debug(line.c_str());
                outLines.push_back(line); // leave line untouched
                continue;
            }

            std::string debugStr = sourcePath;
            if (shouldSkipClangForArg(sourcePath))
            {
                runClang = false;
                debugStr = "SKIP " + debugStr;
                mLogger.debug(debugStr.c_str());
                return true;
            }
            else
            {
                debugStr = "NO DO SKIP " + debugStr;
                mLogger.debug(debugStr.c_str());
            }

            // Preprocess the source with sanitized flags
            if (!mPreprocessor.preprocess(sourcePath, clangFlags, outPreCFile))
            {
                mLogger.debug("Preprocessing failed:");
                mLogger.debug(sourcePath.c_str());
                fclose(fp);
                return false;
            }

            // Replace /c <original> with /c <outPreCFile>
            size_t cPos = line.find("/c ");
            if (cPos == std::string::npos)
                cPos = line.find("-c ");
            if (cPos != std::string::npos)
            {
                std::string rewritten = line.substr(0, cPos + 3) + outPreCFile;
                outLines.push_back(rewritten);
            }
            else
            {
                mLogger.debug("Could not rewrite line — missing /c flag:");
                mLogger.debug(line.c_str());
                outLines.push_back(line);
            }
        }

        fclose(fp);
        return true;
    }

    bool writeResponseFile(const std::string& path, const std::vector<std::string>& lines)
    {
        FILE* out = fopen(path.c_str(), "w");
        if (!out)
            return false;

        for (size_t i = 0; i < lines.size(); ++i)
            fprintf(out, "%s\n", lines[i].c_str());

        fclose(out);
        return true;
    }

    int run(int argc, char* argv[])
    {
        mLogger.debug("Received arguments:");
        int i;
        for (i = 0; i < argc; ++i)
            mLogger.debug(argv[i]);

        //const char* includeEnv = getenv("INCLUDE");
        //if (includeEnv)
        //    mLogger.debug("INCLUDE env var:");
        //else
        //mLogger.debug("INCLUDE env var is NOT set.");

        //if (includeEnv)
        //    mLogger.debug(includeEnv);

        if (argc < 2)
            return 1;

        bool runClang = true;
        bool hadRspFile = false;
        std::vector<std::string> rewrittenLines;
        std::string outPreCFile;
        std::string rspName;

        mLogger.debug("Checking for RSP response file...");
        for (i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (isResponseFile(arg))
            {
                hadRspFile = true;

                std::string path = arg.substr(1);
                rspName = path;
                if (!processResponseFile(path, rewrittenLines, outPreCFile, runClang))
                    return 1;
            }
            else if (shouldSkipClangForArg(arg))
            {
                runClang = false;
            }
        }

        if (hadRspFile && runClang)
        {
            mLogger.debug("Apply fixes to preprocessed file...");
            ClangArtifactFixer fixer;
            fixer.clean(outPreCFile);

            // Write new response file
            mLogger.debug("Rewrite response file to new preprocessed file location...");
            std::string rewrittenRsp = rspName + "rewritten.rsp";
            if (!writeResponseFile(rewrittenRsp, rewrittenLines))
                return 1;

            // Build cl_real.exe call
            char fullPath[MAX_PATH];
            GetFullPathNameA(rewrittenRsp.c_str(), MAX_PATH, fullPath, NULL);
            std::string cmd = "cl_real.exe @" + std::string(fullPath);

            bool ok = mInvoker.invoke(cmd) ? 0 : 1;
            return ok;
        }
        else
        {
            mLogger.debug("No response file, call real CL as pass through");

            std::string cmd = "cl_real.exe";
            for (int i = 1; i < argc; ++i)
            {
                cmd += " ";
                cmd += argv[i];
            }
            return mInvoker.invoke(cmd) ? 0 : 1;
        }
    }

  private:
    Logger mLogger;
    Preprocessor mPreprocessor;
    ClInvoker mInvoker;
};

int main(int argc, char* argv[])
{
    ToolchainEngine engine;
    return engine.run(argc, argv);
}
