# ========== 添加的代码开始 ==========
import sys
import os

# 获取当前脚本所在目录
current_dir = os.path.dirname(os.path.abspath(__file__))

# 本地依赖包路径（与py_tts.py同级的python_packages文件夹）
local_packages = os.path.join(current_dir, "python_packages")

# 如果存在本地包目录，添加到Python路径的最前面
if os.path.exists(local_packages):
    sys.path.insert(0, local_packages)
    print(f"[INFO] 使用本地依赖包: {local_packages}")

# 现在可以正常导入包了
try:
    import dashscope
    import requests
    print("[INFO] 成功导入dashscope和requests")
except ImportError as e:
    print(f"[ERROR] 导入失败: {e}")
    print("[INFO] 请确保python_packages文件夹中包含必要的包")
    sys.exit(1)
# ========== 添加的代码结束 ==========

import base64
import json
import time
import argparse

def synthesize_speech(text, voice="Cherry", model="qwen3-tts-flash", api_key=""):
    try:
        # 设置 API key
        if api_key:
            dashscope.api_key = api_key
        
        response = dashscope.audio.qwen_tts.SpeechSynthesizer.call(
            model=model,
            text=text,
            voice=voice,
        )
        
        if response.status_code != 200:
            return None
        
        output = response.output
        audio_dict = output.audio
        
        # 调试：保存 audio_dict 到文件
        with open("audio_dict_debug.json", "w", encoding="utf-8") as f:
            json.dump(audio_dict, f, ensure_ascii=False, indent=2)
        
        audio_file = None
        
        # 处理不同的返回格式
        if 'url' in audio_dict:
            audio_file = download_from_url(audio_dict['url'], api_key)
        elif 'id' in audio_dict:
            audio_file = download_from_file_id(audio_dict['id'], api_key)
        elif 'data' in audio_dict and audio_dict['data']:
            audio_data = audio_dict['data']
            if isinstance(audio_data, str) and audio_data.strip():
                audio_file = decode_base64_audio(audio_data, "audio_from_data.wav")
            elif isinstance(audio_data, bytes):
                audio_file = save_bytes_to_file(audio_data, "audio_from_bytes.wav")
        else:
            for key, value in audio_dict.items():
                if isinstance(value, str) and len(value) > 100:
                    audio_file = decode_base64_audio(value, f"audio_from_{key}.wav")
                    if audio_file:
                        break
        
        if not audio_file:
            if 'task_id' in audio_dict:
                audio_file = query_tts_task(audio_dict['task_id'], api_key)
            else:
                audio_file = query_by_request_id(response.request_id, api_key)
        
        return audio_file
        
    except Exception as e:
        print(f"[ERROR] 合成过程中出错: {e}")
        return None

def download_from_url(url, api_key):
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Accept": "audio/*"
    }
    
    try:
        response = requests.get(url, headers=headers, timeout=30)
        
        if response.status_code == 200:
            content_type = response.headers.get('Content-Type', '')
            
            if 'wav' in content_type.lower():
                ext = 'wav'
            elif 'mp3' in content_type.lower():
                ext = 'mp3'
            elif 'ogg' in content_type.lower():
                ext = 'ogg'
            else:
                ext = 'wav'
            
            filename = f"audio_downloaded_{int(time.time())}.{ext}"
            
            with open(filename, "wb") as f:
                f.write(response.content)
            
            return filename
        else:
            print(f"[ERROR] 下载失败，状态码: {response.status_code}")
            return None
            
    except Exception as e:
        print(f"[ERROR] 下载过程中出错: {e}")
        return None

def download_from_file_id(file_id, api_key):
    url = f"https://dashscope.aliyuncs.com/api/v1/files/{file_id}/content"
    return download_from_url(url, api_key)

def decode_base64_audio(base64_str, filename="audio_decoded.wav"):
    try:
        if base64_str.startswith('data:audio/'):
            base64_str = base64_str.split(',', 1)[1]
        
        audio_bytes = base64.b64decode(base64_str)
        
        with open(filename, "wb") as f:
            f.write(audio_bytes)
        
        return filename
        
    except Exception as e:
        print(f"[ERROR] 解码base64时出错: {e}")
        return None

def save_bytes_to_file(audio_bytes, filename="audio_bytes.wav"):
    try:
        with open(filename, "wb") as f:
            f.write(audio_bytes)
        return filename
    except Exception as e:
        print(f"[ERROR] 保存字节数据时出错: {e}")
        return None

def query_tts_task(task_id, api_key):
    query_url = f"https://dashscope.aliyuncs.com/api/v1/tasks/{task_id}"
    
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    
    for i in range(10):
        try:
            response = requests.get(query_url, headers=headers, timeout=10)
            
            if response.status_code == 200:
                result = response.json()
                task_status = result.get('output', {}).get('task_status')
                
                if task_status == 'SUCCESSED':
                    if 'audio_url' in result.get('output', {}):
                        return download_from_url(result['output']['audio_url'], api_key)
                    elif 'audio' in result.get('output', {}):
                        return decode_base64_audio(result['output']['audio'], f"audio_task_{task_id[:8]}.wav")
                
                elif task_status in ['FAILED', 'CANCELLED']:
                    return None
                
                time.sleep(2)
                
        except Exception as e:
            print(f"[ERROR] 查询任务状态时出错: {e}")
            time.sleep(2)
    
    return None

def query_by_request_id(request_id, api_key):
    query_url = "https://dashscope.aliyuncs.com/api/v1/services/audio/tts/results"
    
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    
    params = {"request_id": request_id}
    
    for i in range(5):
        try:
            response = requests.get(query_url, headers=headers, params=params, timeout=10)
            
            if response.status_code == 200:
                result = response.json()
                
                if 'output' in result and 'audio' in result['output']:
                    return decode_base64_audio(result['output']['audio'], f"audio_request_{request_id[:8]}.wav")
                
            time.sleep(3)
            
        except Exception as e:
            print(f"[ERROR] 查询请求ID时出错: {e}")
            time.sleep(3)
    
    return None

def main():
    parser = argparse.ArgumentParser(description='TTS转换')
    parser.add_argument('--text', type=str, required=True, help='要转换的文本')
    parser.add_argument('--voice', type=str, default='Cherry', help='声音类型')
    parser.add_argument('--model', type=str, default='qwen3-tts-flash', help='模型名称')
    parser.add_argument('--apikey', type=str, required=True, help='API密钥')
    parser.add_argument('--output', type=str, help='输出文件路径（可选）')
    
    args = parser.parse_args()
    
    print(f"[INFO] 开始合成语音，文本长度: {len(args.text)}")
    print(f"[INFO] 使用声音: {args.voice}, 模型: {args.model}")
    
    audio_file = synthesize_speech(
        text=args.text,
        voice=args.voice,
        model=args.model,
        api_key=args.apikey
    )
    
    if audio_file:
        print(f"[INFO] 合成成功，音频文件: {audio_file}")
        print(f"AUDIO_FILE:{audio_file}")
        sys.exit(0)  # 成功退出
    else:
        print("ERROR:语音合成失败")
        sys.exit(1)  # 失败退出

if __name__ == "__main__":
    main()
