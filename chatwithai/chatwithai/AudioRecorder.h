// AudioRecorder.h - 音频录制类头文件（修复版）
#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>

#pragma comment(lib, "winmm.lib")

class AudioRecorder {
public:
    // 音频格式配置结构
    struct AudioConfig {
        int sampleRate = 16000;      // 采样率，默认16kHz
        int bitsPerSample = 16;      // 位深度，默认16位
        int channels = 1;            // 声道数，默认单声道
        int recordSeconds = 60;       // 默认录音60秒

        // 计算缓冲区大小
        int calculateBufferSize() const {
            int bytesPerSample = bitsPerSample / 8;
            int bytesPerSecond = sampleRate * channels * bytesPerSample;
            return bytesPerSecond * recordSeconds;
        }

        // 获取WAV文件字节率
        int getByteRate() const {
            return sampleRate * channels * (bitsPerSample / 8);
        }

        // 获取块对齐大小
        int getBlockAlign() const {
            return channels * (bitsPerSample / 8);
        }
    };

    // 状态枚举 - 使用不同的名称避免冲突
    enum class RecorderState {
        STATE_IDLE,           // 空闲
        STATE_RECORDING,      // 录音中
        STATE_PAUSED,         // 已暂停
        STATE_ERROR           // 错误
    };

    // 回调函数类型
    using RecordingCallback = std::function<void(const std::vector<char>& data, int currentTimeMs)>;

public:
    // 构造函数和析构函数
    AudioRecorder();
    explicit AudioRecorder(const AudioConfig& config);
    ~AudioRecorder();

    // 禁止拷贝
    AudioRecorder(const AudioRecorder&) = delete;
    AudioRecorder& operator=(const AudioRecorder&) = delete;

    // 主要接口方法
    bool startRecording();                          // 开始录音
    bool stopRecording();                           // 停止录音

    // 数据获取方法
    std::vector<char> getRecordedData() const;      // 获取录音数据
    int getRecordedDuration() const;                // 获取录音时长（毫秒）
    int getCurrentTime() const;                     // 获取当前录音时间（毫秒）

    // 播放方法
    bool playRecordedAudio() const;                 // 播放录音(用于测试)
    bool saveToFile(const std::string& filename) const; // 保存到文件

    // 回调设置
    void setRecordingCallback(RecordingCallback callback);

    // 状态查询
    RecorderState getState() const { return state_; }
    bool isRecording() const { return state_ == RecorderState::STATE_RECORDING; }
    bool isPaused() const { return state_ == RecorderState::STATE_PAUSED; }
    bool isIdle() const { return state_ == RecorderState::STATE_IDLE; }

    // 配置方法
    void setConfig(const AudioConfig& config);
    AudioConfig getConfig() const { return config_; }

private:
    // 初始化音频格式
    void initializeAudioFormat();

    // 清理资源
    void cleanup();

    // 录音线程函数
    void recordingThreadFunc();

    // 删除 createWavHeader 函数声明，我们不需要它

    // 成员变量
    AudioConfig config_;                    // 音频配置
    RecorderState state_;                   // 当前状态
    std::atomic<bool> shouldStop_;          // 停止标志

    // Windows音频相关
    HWAVEIN hWaveIn_;                       // 录音设备句柄
    WAVEFORMATEX waveFormat_;               // 音频格式

    // 音频数据
    std::vector<char> audioBuffer_;         // 音频数据缓冲区
    std::vector<WAVEHDR> waveHeaders_;      // 多个音频头（用于流式录音）
    int currentPosition_;                   // 当前录音位置

    // 线程和回调
    std::thread recordingThread_;           // 录音线程
    RecordingCallback recordingCallback_;   // 录音回调函数
    std::atomic<int> recordingStartTime_;   // 录音开始时间
};