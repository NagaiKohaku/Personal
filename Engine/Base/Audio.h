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

namespace MyEngine {

	/// <summary>
	/// XAudio2 を使用したオーディオ管理クラスです。
	/// </summary>
	class Audio {

	public:

		Audio();

		~Audio();

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
			WAVEFORMATEX wfex;         //波形フォーマット
			std::vector<BYTE> buffer; //バッファ
			unsigned int bufferSize;   //バッファサイズ
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
		/// XAudio2の初期化を行います。
		/// </summary>
		void Initialize();

		/// <summary>
		/// 登録されている全オーディオの状態を更新します。
		/// </summary>
		void Update();

		/// <summary>
		/// オーディオシステムの終了処理します。
		/// </summary>
		void Finalize();

		/// <summary>
		/// 指定した名前でサウンドを読み込み、管理リストに登録します。
		/// </summary>
		/// <param name="soundName">管理用のサウンド名</param>
		/// <param name="fileName">読み込むWAVファイル名</param>
		void SoundLoad(std::string soundName, std::string fileName);

		/// <summary>
		/// 指定したサウンドを再生します。
		/// </summary>
		/// <param name="soundName">再生するサウンドの管理名</param>
		/// <param name="isLoop">ループ再生するかどうか</param>
		void StartSound(std::string soundName, bool isLoop);

		/// <summary>
		/// 指定したサウンドを停止します。
		/// </summary>
		/// <param name="soundName">停止するサウンドの管理名</param>
		void StopSound(std::string soundName);

		/// <summary>
		/// 指定したサウンドをアンロード（メモリ解放）します。
		/// </summary>
		/// <param name="soundName">アンロードするサウンドの管理名</param>
		void SoundUnLoad(std::string soundName);

		/// <summary>
		/// 指定したサウンドインスタンスが再生中かどうかを確認します。
		/// </summary>
		/// <param name="soundInstance">確認する音声インスタンス</param>
		bool isPlayed(SoundInstance soundInstance);

	private:

		/// <summary>
		/// 指定したWAVファイルを読み込み、Audio用のサウンドデータとして返します。
		/// </summary>
		/// <param name="fileName">読み込むWAVファイルのパス</param>
		/// <returns>読み込んだ音声データを格納した SoundData 構造体</returns>
		SoundData LoadWavFile(std::string fileName);

		/// <summary>
		/// 指定された音声データから再生用のSoundInstanceを生成します。
		/// </summary>
		/// <param name="soundData">再生する音声データ</param>
		/// <param name="isLoop">ループ再生するかどうか</param>
		/// <returns>生成された SoundInstance</returns>
		SoundInstance CreateSoundInstance(SoundData soundData, bool isLoop);

		///-------------------------------------------///
		/// メンバ変数
		///-------------------------------------------///
	private:

		Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

		IXAudio2MasteringVoice* masterVoice_;

		std::map<std::string, SoundObject> sounds_;
	};
}