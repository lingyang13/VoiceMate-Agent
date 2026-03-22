// AudioRecorder.cpp - 音频录制类实现
#include "AudioRecorder.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include <cmath>      // 添加cmath头文件用于sqrt
#include <algorithm>  // 添加algorithm头文件用于std::min

// WAV文件头结构
struct WavHeader {
    char riff[4] = { 'R', 'I', 'F', 'F' };
    uint32_t chunkSize;
    char wave[4] = { 'W', 'A', 'V', 'E' };
    char fmt[4] = { 'f', 'm', 't', ' ' };
    uint32_t fmtSize = 16;
    uint16_t audioFormat = 1;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4] = { 'd', 'a', 't', 'a' };
    uint32_t dataSize;
};

// 构造函数
AudioRecorder::AudioRecorder()
    : state_(RecorderState::STATE_IDLE)  // 修改为STATE_IDLE
    , shouldStop_(false)
    , hWaveIn_(nullptr)
    , currentPosition_(0)
    , recordingStartTime_(0) {

    // 初始化默认配置
    config_ = AudioConfig();
    initializeAudioFormat();  // 现在这个函数已经声明了
}

AudioRecorder::AudioRecorder(const AudioConfig& config)
    : config_(config)
    , state_(RecorderState::STATE_IDLE)  // 修改为STATE_IDLE
    , shouldStop_(false)
    , hWaveIn_(nullptr)
    , currentPosition_(0)
    , recordingStartTime_(0) {

    initializeAudioFormat();  // 现在这个函数已经声明了
}

// 初始化音频格式（修复：添加实现）
void AudioRecorder::initializeAudioFormat() {
    waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat_.nChannels = config_.channels;  // 现在可以访问config_
    waveFormat_.nSamplesPerSec = config_.sampleRate;
    waveFormat_.wBitsPerSample = config_.bitsPerSample;
    waveFormat_.nBlockAlign = waveFormat_.nChannels * waveFormat_.wBitsPerSample / 8;
    waveFormat_.nAvgBytesPerSec = waveFormat_.nSamplesPerSec * waveFormat_.nBlockAlign;
    waveFormat_.cbSize = 0;
}

// 析构函数
AudioRecorder::~AudioRecorder() {
    stopRecording();
    cleanup();
}

// 开始录音
bool AudioRecorder::startRecording() {
    if (state_ != RecorderState::STATE_IDLE) {  // 修改为STATE_IDLE
        std::cerr << "error: The recorder is not in an idle state." << std::endl;
        return false;
    }

    // 清理之前的录音数据
    audioBuffer_.clear();
    currentPosition_ = 0;

    // 打开录音设备
    MMRESULT result = waveInOpen(&hWaveIn_, WAVE_MAPPER, &waveFormat_,
        0, 0, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "error: Cannot open the recording device" << std::endl;
        state_ = RecorderState::STATE_ERROR;  // 修改为STATE_ERROR
        return false;
    }

    // 准备缓冲区
    int bufferSize = config_.calculateBufferSize();  // 现在可以访问config_
    audioBuffer_.resize(bufferSize, 0);

    // 准备WAVE头
    WAVEHDR waveHeader = { 0 };
    waveHeader.lpData = audioBuffer_.data();
    waveHeader.dwBufferLength = bufferSize;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;

    waveHeaders_.clear();
    waveHeaders_.push_back(waveHeader);

    result = waveInPrepareHeader(hWaveIn_, &waveHeaders_[0], sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "error: Failed to prepare the recording head" << std::endl;
        cleanup();
        return false;
    }

    result = waveInAddBuffer(hWaveIn_, &waveHeaders_[0], sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "error: Failed to add audio buffer" << std::endl;
        cleanup();
        return false;
    }

    // 启动录音线程
    shouldStop_ = false;
    recordingStartTime_ = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
        );

    recordingThread_ = std::thread(&AudioRecorder::recordingThreadFunc, this);

    // 开始录音
    result = waveInStart(hWaveIn_);
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "error: Failed to start recording" << std::endl;
        shouldStop_ = true;
        if (recordingThread_.joinable()) {
            recordingThread_.join();
        }
        cleanup();
        return false;
    }

    state_ = RecorderState::STATE_RECORDING;  // 修改为STATE_RECORDING
    std::cout << "Start recording..." << std::endl;

    return true;
}

// 录音线程函数
void AudioRecorder::recordingThreadFunc() {
    auto startTime = std::chrono::steady_clock::now();

    while (!shouldStop_) {
        // 计算已录音时间
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime).count();

        currentPosition_ = static_cast<int>(elapsedMs);

        // 调用回调函数（如果设置）
        if (recordingCallback_) {
            // 计算当前已录制的数据量
            int bytesPerSecond = config_.getByteRate();  // 现在可以访问config_
            int currentBytes = (elapsedMs * bytesPerSecond) / 1000;

            if (currentBytes > 0 && currentBytes <= static_cast<int>(audioBuffer_.size())) {
                std::vector<char> currentData(audioBuffer_.data(),
                    audioBuffer_.data() + currentBytes);
                recordingCallback_(currentData, static_cast<int>(elapsedMs));
            }
        }

        // 检查是否达到最大录音时间
        if (elapsedMs >= config_.recordSeconds * 1000) {  // 现在可以访问config_
            shouldStop_ = true;
            break;
        }

        // 休眠一段时间以减少CPU占用
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // 停止录音
    if (hWaveIn_ && state_ == RecorderState::STATE_RECORDING) {  // 修改为STATE_RECORDING
        waveInStop(hWaveIn_);
        waveInUnprepareHeader(hWaveIn_, &waveHeaders_[0], sizeof(WAVEHDR));
    }

    state_ = RecorderState::STATE_IDLE;  // 修改为STATE_IDLE
}

// 停止录音
bool AudioRecorder::stopRecording() {
    if (state_ != RecorderState::STATE_RECORDING && state_ != RecorderState::STATE_PAUSED) {  // 修改状态名
        return false;
    }

    shouldStop_ = true;

    if (recordingThread_.joinable()) {
        recordingThread_.join();
    }

    if (hWaveIn_) {
        waveInStop(hWaveIn_);
        if (!waveHeaders_.empty()) {
            waveInUnprepareHeader(hWaveIn_, &waveHeaders_[0], sizeof(WAVEHDR));
        }
        waveInClose(hWaveIn_);
        hWaveIn_ = nullptr;
    }

    state_ = RecorderState::STATE_IDLE;  // 修改为STATE_IDLE
    std::cout << "...Recording has stopped." << std::endl;

    return true;
}

// 获取录音数据
std::vector<char> AudioRecorder::getRecordedData() const {
    // 根据实际录音时长截取数据
    if (currentPosition_ <= 0) {
        return std::vector<char>();
    }

    int bytesPerSecond = config_.getByteRate();  // 现在可以访问config_
    int actualBytes = (currentPosition_ * bytesPerSecond) / 1000;
    actualBytes = std::min<int>(actualBytes, audioBuffer_.size());

    return std::vector<char>(audioBuffer_.begin(), audioBuffer_.begin() + actualBytes);
}

// 获取录音时长（毫秒）
int AudioRecorder::getRecordedDuration() const {
    return currentPosition_;
}

// 获取当前录音时间（毫秒）
int AudioRecorder::getCurrentTime() const {
    if (state_ != RecorderState::STATE_RECORDING) {  // 修改为STATE_RECORDING
        return currentPosition_;
    }

    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    return static_cast<int>(currentTime - recordingStartTime_);
}

// 播放录音
bool AudioRecorder::playRecordedAudio() const {
    if (audioBuffer_.empty() || currentPosition_ <= 0) {
        std::cerr << "error: There are no audio recordings available for playback." << std::endl;
        return false;
    }

    // 先保存到临时文件，然后播放
    std::string tempFile = "temp_recording.wav";
    if (!saveToFile(tempFile)) {
        return false;
    }

    // 播放WAV文件
    if (!PlaySoundA(tempFile.c_str(), NULL, SND_FILENAME | SND_SYNC)) {
        std::cerr << "error: Error: Failed to play the audio recording" << std::endl;
        return false;
    }

    // 删除临时文件（可选）
    // std::remove(tempFile.c_str());

    return true;
}

// 保存到文件
bool AudioRecorder::saveToFile(const std::string& filename) const {
    if (audioBuffer_.empty() || currentPosition_ <= 0) {
        std::cerr << "error: There are no audio recording data available for saving." << std::endl;
        return false;
    }

    std::vector<char> audioData = getRecordedData();
    if (audioData.empty()) {
        return false;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "error: Unable to create file" << filename << std::endl;
        return false;
    }

    // 创建WAV文件头
    WavHeader header;
    header.numChannels = config_.channels;  // 现在可以访问config_
    header.sampleRate = config_.sampleRate;
    header.bitsPerSample = config_.bitsPerSample;
    header.byteRate = config_.getByteRate();
    header.blockAlign = config_.getBlockAlign();
    header.dataSize = static_cast<uint32_t>(audioData.size());
    header.chunkSize = 36 + header.dataSize;

    // 写入文件头
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // 写入音频数据
    file.write(audioData.data(), audioData.size());
    file.close();

    std::cout << "The recording has been saved as: " << filename
        << " (" << audioData.size() << " 字节)" << std::endl;

    return true;
}

// 设置录音回调
void AudioRecorder::setRecordingCallback(RecordingCallback callback) {
    recordingCallback_ = callback;
}

// 设置配置
void AudioRecorder::setConfig(const AudioConfig& config) {
    if (state_ != RecorderState::STATE_IDLE) {  // 修改为STATE_IDLE
        std::cerr << "Warning: Configurations cannot be modified during the recording process." << std::endl;
        return;
    }

    config_ = config;
    initializeAudioFormat();  // 重新初始化音频格式
}

// 清理资源
void AudioRecorder::cleanup() {
    if (hWaveIn_) {
        waveInClose(hWaveIn_);
        hWaveIn_ = nullptr;
    }

    waveHeaders_.clear();
    audioBuffer_.clear();
    currentPosition_ = 0;
}