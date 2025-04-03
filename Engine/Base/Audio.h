#pragma once
#include "xaudio2.h"

#include "Windows.h"
#include "wrl.h"
#include "stdint.h"
#include "map"
#include "string"
#include "memory"
#include "vector"

#pragma comment(lib,"xaudio2.lib")

///=====================================================/// 
/// 音声
///=====================================================///
class Audio {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//チャンクの先頭
	struct ChunkHeader {
		char id[4];   //ID
		int32_t size; //サイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk; //チャンクの先頭
		char type[4];      //音声ファイルの識別子
	};

	//FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; //チャンクの先頭
		WAVEFORMATEX fmt;  //音声のフォーマット
	};

	//音声データ
	struct SoundData {
		WAVEFORMATEX wfex;       //波形フォーマット
		BYTE* pBuffer;           //バッファの先頭アドレス
		unsigned int bufferSize; //バッファサイズ
	};

	//音声インスタンス
	struct SoundInstance {
		IXAudio2SourceVoice* pSourceVoice; //ソースボイス
		XAUDIO2_BUFFER buf{};              //波形データの設定
		SoundData soundData;               //音声データ
	};

	//音声オブジェクト
	struct SoundObject {
		SoundData data;
		std::vector<SoundInstance> instance;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 静的インスタンスの生成
	/// </summary>
	/// <returns>静的インスタンス</returns>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 音声データの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void SoundLoad(const char* fileName);

	/// <summary>
	/// 音声データの読み込み
	/// </summary>
	/// <param name="soundName">音声データ名</param>
	/// <param name="fileName">ファイル名</param>
	void SoundLoad(std::string soundName, std::string fileName);

	/// <summary>
	/// 音声の再生
	/// </summary>
	/// <param name="soundName">音声名</param>
	/// <param name="isLoop">ループ</param>
	void StartSound(std::string soundName, bool isLoop);

	/// <summary>
	/// 音声の停止
	/// </summary>
	/// <param name="soundName">音声名</param>
	void StopSound(std::string soundName);

	/// <summary>
	/// 音声データの解放
	/// </summary>
	/// <param name="soundData">音声データ</param>
	void SoundUnLoad(std::string soundName);

	/// <summary>
	/// 音声が流れているか
	/// </summary>
	/// <param name="soundInstance">音声インスタンス</param>
	/// <returns>フラグ</returns>
	bool isPlayed(SoundInstance soundInstance);

private:

	/// <summary>
	/// Wavファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>音源データ</returns>
	SoundData LoadWavFile(std::string fileName);

	/// <summary>
	/// 音声インスタンスの生成
	/// </summary>
	/// <param name="soundData">音源データ</param>
	/// <param name="isLoop">ループ</param>
	/// <returns>音声インスタンス</returns>
	SoundInstance CreateSoundInstance(SoundData soundData, bool isLoop);

	///-------------------------------------------///
	/// メンバ変数
	///-------------------------------------------///
private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

	IXAudio2MasteringVoice* masterVoice_;

	std::map<std::string, SoundObject> sounds_;
};