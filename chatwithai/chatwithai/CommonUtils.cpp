#include "CommonUtils.h"
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <io.h>
#include <cstdlib>
#include <vector>
#include <mmsystem.h>
#include <cstdio>
#include <array>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>


// UTF-8字符串转换为系统默认编码字符串
std::string CommonUtils::UTF8ToString(const std::string& utf8_str)
{
    if (utf8_str.empty()) return "";

    // 将UTF-8转换为宽字符
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    if (wlen == 0) return "";

    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wstr[0], wlen);

    // 将宽字符转换为ANSI（系统默认编码）
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len == 0) return "";

    std::string ansi_str(len, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &ansi_str[0], len, nullptr, nullptr);

    // 去掉末尾的 null 字符
    ansi_str.pop_back();

    return ansi_str;
}

// 系统默认编码字符串转换为UTF-8字符串
std::string CommonUtils::StringToUTF8(const std::string& str)
{
    if (str.empty()) return "";
    // 将ANSI（系统默认编码）转换为宽字符
    int wlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (wlen == 0) return "";
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], wlen);
    // 将宽字符转换为UTF-8
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len == 0) return "";
    std::string utf8_str(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], len, nullptr, nullptr);
    // 去掉末尾的 null 字符
    utf8_str.pop_back();

	return utf8_str;
}


// 清理临时文件，保留指定文件（如果提供）
void CommonUtils::CleanTemporaryFiles(const std::string& keepFile)
{
    std::vector<std::string> patterns = {
        "audio_dict_debug.json",
        "audio_downloaded_*.wav",
        "audio_from_*.wav",
        "audio_task_*.wav",
        "audio_request_*.wav",
        "audio_decoded.wav",
        "audio_bytes.wav"
    };

    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    for (const auto& pattern : patterns) {
        hFind = FindFirstFileA(pattern.c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    std::string filename = findData.cFileName;

                    // 不删除要保留的文件
                    if (filename != keepFile) {
                        // 检查文件是否正在被使用
                        if (_access(filename.c_str(), 0) == 0) {  // 文件存在
                            if (DeleteFileA(filename.c_str())) {
                                std::cout << "清理临时文件: " << filename << std::endl;
                            }
                        }
                    }
                }
            } while (FindNextFileA(hFind, &findData) != 0);
            FindClose(hFind);
        }
    }
}


// 播放音频文件 (有问题，已弃用)
bool CommonUtils::playAudioSilent(const std::string& audioFile)
{
    // 检查文件是否存在
    if (GetFileAttributesA(audioFile.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "Error: The audio file does not exist." << audioFile << std::endl;

        // 尝试在当前目录查找
        char currentDir[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, currentDir);
        std::string fullPath = std::string(currentDir) + "\\" + audioFile;

        if (GetFileAttributesA(fullPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            std::cerr << "Error: Unable to locate the audio file" << std::endl;
            return false;
        }

        // 使用找到的完整路径
        return playAudioSilent(fullPath);
    }

    std::cout << "be playing: " << audioFile << std::endl;

    // 使用PlaySound播放，SND_SYNC会阻塞直到播放完成
    
    if (PlaySoundA(audioFile.c_str(), NULL, SND_FILENAME | SND_SYNC)) {
        std::cout << "Playback completed" << std::endl;
        return true;
    }

    // 如果失败，尝试其他标志
    std::cout << "Play using the backup method..." << std::endl;
    if (PlaySoundA(audioFile.c_str(), NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT)) {
        std::cout << "Playback completed" << std::endl;
        return true;
    }

    std::cerr << "Error: Playback failed. Error code: " << GetLastError() << std::endl;
    return false;
}

// 播放音频文件 ( 新 )
bool CommonUtils::playAudioSilent2(const std::string& audioFile)
{
    // 1. 检查文件是否存在
    DWORD fileAttr = GetFileAttributesA(audioFile.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        DWORD error = GetLastError();
        std::cerr << u8"错误：音频文件不存在 - " << audioFile
            << u8" 错误码: " << error << std::endl;
        return false;
    }

    // 2. 检查是否是目录
    if (fileAttr & FILE_ATTRIBUTE_DIRECTORY) {
        std::cerr << u8"错误：路径是目录而不是文件" << std::endl;
        return false;
    }

    std::cout << u8"正在播放: " << audioFile << std::endl;

    // 3. 方法1: 使用MCI（更可靠）
    std::cout << u8"尝试方法1: MCI播放..." << std::endl;

    // 构建MCI命令
    std::string openCmd = "open \"" + audioFile + "\" type waveaudio alias myaudio";
    std::string playCmd = "play myaudio wait";  // wait参数会阻塞直到播放完成
    std::string closeCmd = "close myaudio";

    // 发送MCI命令
    MCIERROR mciError;
    mciError = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
    if (mciError == 0) {
        // 打开成功，开始播放
        mciError = mciSendStringA(playCmd.c_str(), NULL, 0, NULL);
        if (mciError == 0) {
            // 播放成功
            mciSendStringA(closeCmd.c_str(), NULL, 0, NULL);
            std::cout << u8"MCI播放完成" << std::endl;
            return true;
        }
        else {
            // 播放失败，关闭并继续尝试其他方法
            char errorStr[256];
            mciGetErrorStringA(mciError, errorStr, sizeof(errorStr));
            std::cerr << u8"MCI播放失败: " << errorStr << std::endl;
            mciSendStringA(closeCmd.c_str(), NULL, 0, NULL);
        }
    }
    else {
        char errorStr[256];
        mciGetErrorStringA(mciError, errorStr, sizeof(errorStr));
        std::cerr << u8"MCI打开失败: " << errorStr << std::endl;
    }

    // 4. 方法2: 使用PlaySound（原始方法）
    std::cout << u8"尝试方法2: PlaySound播放..." << std::endl;

    // 尝试多种标志组合
    DWORD flags[] = {
        SND_FILENAME | SND_SYNC,
        SND_FILENAME | SND_SYNC | SND_NODEFAULT,
        SND_FILENAME | SND_ASYNC,  // 异步试试
        SND_FILENAME | SND_SYNC | SND_LOOP | SND_NODEFAULT  // 循环播放1次
    };

    for (int i = 0; i < 4; i++) {
        if (PlaySoundA(audioFile.c_str(), NULL, flags[i])) {
            if (flags[i] & SND_ASYNC) {
                // 异步播放需要等待一会儿然后停止
                Sleep(100);
                PlaySound(NULL, NULL, 0);
            }
            std::cout << u8"PlaySound播放成功 (标志组合 " << i + 1 << u8")" << std::endl;
            return true;
        }
    }

    DWORD lastError = GetLastError();
    std::cerr << u8"PlaySound播放失败，错误码: " << lastError << std::endl;

    // 5. 方法3: 使用ShellExecute（用默认播放器）
    std::cout << u8"尝试方法3: 系统默认播放器..." << std::endl;
    HINSTANCE shellResult = ShellExecuteA(NULL, "open", audioFile.c_str(), NULL, NULL, SW_SHOW);
    if ((INT_PTR)shellResult > 32) {
        std::cout << u8"已启动系统默认播放器" << std::endl;
        return true;
    }

    std::cerr << u8"错误：所有播放方法都失败" << std::endl;
    return false;
}


std::string CommonUtils::RunPS(const std::string& cmd) {
    std::string result;
    std::array<char, 128> buffer;

    // 将多行命令中的换行符替换为分号（单行执行）
    std::string singleLineCmd = UTF8ToString(cmd);
    size_t pos = 0;
    while ((pos = singleLineCmd.find("\\n", pos)) != std::string::npos) {
        singleLineCmd.replace(pos, 2, "; ");
        pos += 2;
    }

    // 构建完整的PowerShell命令
    std::string fullCommand = "powershell.exe -Command \"" + singleLineCmd + "\"";

    // 打开管道执行命令
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(
        _popen(fullCommand.c_str(), "r"),
        _pclose
    );

    if (!pipe) {
        return "执行失败！无法打开管道";
    }

    // 读取输出
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += StringToUTF8(buffer.data());
    }

    return result.empty() ? "命令执行完成，无输出" : result;
}

bool CommonUtils::WriteFile(const std::string& filePath, const std::string& content)
{
    std::ofstream file(filePath);
    if (!file) return false;
    file << content;
    return true;
}

std::string CommonUtils::ReadFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "";  // 文件打开失败，返回空字符串
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // 一次性读取整个文件
    return buffer.str();
}

void CommonUtils::WriteAIDebugLog(const std::string& type, const std::string& content)
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string exePath = buffer;
    std::string exeDir = exePath.substr(0, exePath.find_last_of("\\"));

    std::string logDir = exeDir + "\\DebugComputerManagerAI";
    CreateDirectoryA(logDir.c_str(), NULL);

    // 使用localtime_s
    time_t now = time(0);
    struct tm ltm;
    localtime_s(&ltm, &now);

    std::ostringstream filename;
    filename << logDir << "\\AI_Debug_"
        << std::put_time(&ltm, "%Y%m%d") << ".log";

    std::ofstream log(filename.str(), std::ios::app);
    if (log.is_open()) {
        log << "[" << std::put_time(&ltm, "%H:%M:%S") << "] "
            << "[" << type << "] "
            << content << std::endl;
        log.close();
    }
}

void CommonUtils::OpenConsole() {
    AllocConsole();

    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    // 关键：关闭缓冲区同步，立即输出
    std::ios::sync_with_stdio(true);

    // 清空缓冲区
    std::cout.clear();

    SetConsoleTitle(L"Debug Console");
}

// 关闭控制台
void CommonUtils::CloseConsole() {
    // 释放控制台
    FreeConsole();

}