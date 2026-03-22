#include "PythonTTS.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <tlhelp32.h>

// 辅助函数：将UTF-8字符串转换为窄字符串（处理中文）
std::string to_narrow_string(const std::string& utf8_str) {
    return utf8_str; // 直接返回，不转换
}

// 构造函数
PythonTTS::PythonTTS(const std::string& api_key) : api_key_(api_key) {}

// 设置 API key
void PythonTTS::setApiKey(const std::string& api_key) {
    api_key_ = api_key;
}

std::string PythonTTS::synthesize(const std::string& text,
    const std::string& voice,
    const std::string& model) {

    // 检查 API key
    if (api_key_.empty()) {
        throw std::runtime_error("API key not set");
    }

    // 获取当前exe所在目录
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string exeDir = exePath;
    size_t pos = exeDir.find_last_of("\\/");
    if (pos != std::string::npos) {
        exeDir = exeDir.substr(0, pos);
    }

    std::cout << "Program directory: " << exeDir << std::endl;

    // 检查py_tts.exe是否存在
    std::string pyExe = exeDir + "\\py_tts.exe";
    DWORD attr = GetFileAttributesA(pyExe.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) {
        std::cout << "Error: Cannot find py_tts.exe, path: " << pyExe << std::endl;
        throw std::runtime_error("Cannot find py_tts.exe");
    }

    std::cout << "Found py_tts.exe" << std::endl;

    // 构建命令参数
    std::stringstream args;
    args << "py_tts.exe"
        << " --text \"" << text << "\""
        << " --voice \"" << voice << "\""
        << " --model \"" << model << "\""
        << " --apikey \"" << api_key_ << "\"";

    std::string commandLine = args.str();
    std::cout << "Command line: " << commandLine << std::endl;

    // 创建管道用于读取子进程输出
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hChildStdoutRd = NULL;
    HANDLE hChildStdoutWr = NULL;

    std::cout << "Creating pipe..." << std::endl;

    // 创建管道
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
        DWORD err = GetLastError();
        std::cout << "CreatePipe failed, error code: " << err << std::endl;
        throw std::runtime_error("CreatePipe failed: " + std::to_string(err));
    }

    std::cout << "Pipe created successfully" << std::endl;

    // 确保读取句柄不被子进程继承
    if (!SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0)) {
        DWORD err = GetLastError();
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        std::cout << "SetHandleInformation failed, error code: " << err << std::endl;
        throw std::runtime_error("SetHandleInformation failed: " + std::to_string(err));
    }

    std::cout << "Handle information set successfully" << std::endl;

    // 准备进程启动信息
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdOutput = hChildStdoutWr;
    si.hStdError = hChildStdoutWr;
    si.hStdInput = NULL;  // 设置为NULL，因为不需要输入
    si.wShowWindow = SW_HIDE;  // 隐藏窗口

    ZeroMemory(&pi, sizeof(pi));

    // 注意：CreateProcess需要可修改的字符串
    std::vector<char> cmdLineVec(commandLine.begin(), commandLine.end());
    cmdLineVec.push_back('\0');  // 确保以null结尾

    std::cout << "Preparing to create process..." << std::endl;

    // 创建进程
    BOOL bSuccess = CreateProcessA(
        NULL,                     // 使用命令行参数中的程序名
        cmdLineVec.data(),        // 命令行（需要可修改）
        NULL,                     // 进程安全属性
        NULL,                     // 线程安全属性
        TRUE,                     // 继承句柄
        CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP, // 创建标志
        NULL,                     // 使用父进程环境
        exeDir.c_str(),           // 启动目录
        &si,                      // 启动信息
        &pi                       // 进程信息
    );

    // 关闭不再需要的写句柄
    CloseHandle(hChildStdoutWr);
    hChildStdoutWr = NULL;

    std::cout << "CreateProcess called, result: " << (bSuccess ? "success" : "failed") << std::endl;

    if (!bSuccess) {
        DWORD err = GetLastError();
        CloseHandle(hChildStdoutRd);
        std::string errorMsg = "Cannot create process, error code: " + std::to_string(err);

        // 获取更详细的错误信息
        switch (err) {
        case ERROR_FILE_NOT_FOUND:
            errorMsg += " (File or program not found)";
            break;
        case ERROR_PATH_NOT_FOUND:
            errorMsg += " (Path does not exist)";
            break;
        case ERROR_ACCESS_DENIED:
            errorMsg += " (Access denied)";
            break;
        case ERROR_ELEVATION_REQUIRED:
            errorMsg += " (Administrator privileges required)";
            break;
        case ERROR_BAD_EXE_FORMAT:
            errorMsg += " (Invalid executable format)";
            break;
        default:
            break;
        }

        std::cout << "Error details: " << errorMsg << std::endl;
        throw std::runtime_error(errorMsg);
    }

    std::cout << "Process created successfully, Process ID: " << pi.dwProcessId << std::endl;

    // 关闭线程句柄（我们不需要）
    CloseHandle(pi.hThread);
    pi.hThread = NULL;

    // 读取子进程输出
    std::string result;
    const int BUFSIZE = 4096;
    char chBuf[BUFSIZE];
    DWORD dwRead;
    BOOL bReadSuccess = FALSE;

    std::cout << "Reading process output..." << std::endl;

    // 首先等待进程结束，然后读取输出
    DWORD waitResult = WaitForSingleObject(pi.hProcess, 60000); // 60秒超时

    if (waitResult == WAIT_TIMEOUT) {
        std::cout << "Process timeout, trying to terminate..." << std::endl;
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(hChildStdoutRd);
        CloseHandle(pi.hProcess);
        throw std::runtime_error("Process execution timeout");
    }
    else if (waitResult == WAIT_FAILED) {
        DWORD err = GetLastError();
        CloseHandle(hChildStdoutRd);
        CloseHandle(pi.hProcess);
        throw std::runtime_error("Wait for process failed: " + std::to_string(err));
    }

    std::cout << "Process execution completed, starting to read output..." << std::endl;

    // 读取所有可用的输出
    while (true) {
        bReadSuccess = ReadFile(hChildStdoutRd, chBuf, BUFSIZE - 1, &dwRead, NULL);

        if (!bReadSuccess || dwRead == 0) {
            break;
        }

        chBuf[dwRead] = '\0';
        result += chBuf;
        std::cout << "Read output: " << chBuf;
    }

    std::cout << "Output reading completed, total length: " << result.length() << std::endl;

    // 获取退出代码
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    std::cout << "Process exit code: " << exitCode << std::endl;

    // 清理资源
    CloseHandle(hChildStdoutRd);
    CloseHandle(pi.hProcess);

    // 解析输出
    if (exitCode == 0) {
        // 查找音频文件路径
        size_t start = result.find("AUDIO_FILE:");
        if (start != std::string::npos) {
            std::string audioFile = result.substr(start + 11);

            // 清理字符串
            size_t end = audioFile.find_first_of("\n\r");
            if (end != std::string::npos) {
                audioFile = audioFile.substr(0, end);
            }

            // 移除末尾空白
            end = audioFile.find_last_not_of(" \t\n\r");
            if (end != std::string::npos) {
                audioFile = audioFile.substr(0, end + 1);
            }

            std::cout << "Parsed audio file path: " << audioFile << std::endl;

            // 如果是相对路径，转换为绝对路径
            if (!audioFile.empty()) {
                if (audioFile.find(":\\") == std::string::npos &&
                    audioFile.find("\\\\") == std::string::npos) {
                    audioFile = exeDir + "\\" + audioFile;
                }

                // 标准化路径
                char fullPath[MAX_PATH];
                if (GetFullPathNameA(audioFile.c_str(), MAX_PATH, fullPath, NULL) > 0) {
                    audioFile = fullPath;
                }
            }

            std::cout << "Final audio file path: " << audioFile << std::endl;

            // 验证文件是否存在
            DWORD fileAttr = GetFileAttributesA(audioFile.c_str());
            if (fileAttr == INVALID_FILE_ATTRIBUTES || (fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                std::cout << "Warning: Audio file does not exist: " << audioFile << std::endl;
            }

            return audioFile;
        }
        else {
            std::cout << "Full process output: " << std::endl << result << std::endl;
            throw std::runtime_error("Audio file path not found");
        }
    }
    else {
        std::cout << "Process output content: " << std::endl << result << std::endl;
        throw std::runtime_error("TTS synthesis failed, return code: " + std::to_string(exitCode));
    }
}