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

/// <summary>
/// XAudio2 を使用したオーディオ管理クラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で1つのインスタンスを共有します。  
/// - WAVファイルを読み込み、SoundObject構造体として管理します。  
/// - サウンドインスタンスを生成し、再生・停止・ループ制御を行います。  
/// </remarks>
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
	/// Audioのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static Audio* GetInstance();

	/// <summary>
	/// XAudio2の初期化を行います。
	/// </summary>
	/// <remarks>
	/// - XAudio2インスタンスを生成し、マスターボイスを作成します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 登録されている全オーディオの状態を更新します。
	/// </summary>
	/// <remarks>
	/// - リストに登録されている全てのサウンドを確認します。
	/// - 各サウンドのインスタンスをチェックし、再生が終了している場合は削除します。
	/// </remarks>
	void Update();

	/// <summary>
	/// オーディオシステムの終了処理します。
	/// </summary>
	/// <remarks>
	/// - 登録されている全サウンドの再生を停止し、インスタンスを削除します。
	/// - SoundUnLoad関数を使用して各サウンドのリソースを解放します。
	/// - リストをクリアして管理データをリセットします。
	/// - XAudio2インスタンスをリセットして終了します。
	/// </remarks>
	void Finalize();

	/// <summary>
	/// 指定した名前でサウンドを読み込み、管理リストに登録します。
	/// </summary>
	/// <param name="soundName">管理用のサウンド名</param>
	/// <param name="fileName">読み込むWAVファイル名</param>
	/// <remarks>
	/// - すでに同名のサウンドが登録されている場合は何もせずに終了します。
	/// - WAVファイルを読み込み、SoundObject構造体に格納してリストに登録します。
	/// </remarks>
	void SoundLoad(std::string soundName, std::string fileName);

	/// <summary>
	/// 指定したサウンドを再生します。
	/// </summary>
	/// <param name="soundName">再生するサウンドの管理名</param>
	/// <param name="isLoop">ループ再生するかどうか</param>
	/// <remarks>
	/// - 指定したサウンド名が登録されていない場合は何もせずに終了します。
	/// - サウンドデータから新しいSoundInstanceを作成し、XAudio2で再生開始します。
	/// - 作成したSoundInstanceはインスタンスリストに追加されます。
	/// </remarks>
	void StartSound(std::string soundName, bool isLoop);

	/// <summary>
	/// 指定したサウンドを停止します。
	/// </summary>
	/// <param name="soundName">停止するサウンドの管理名</param>
	/// <remarks>
	/// - 指定したサウンド名が登録されていない場合は何もせずに終了します。
	/// - 登録されているすべてのサウンドインスタンスを停止し、バッファをフラッシュします。
	/// - サウンドインスタンス自体は削除せず、再利用可能な状態にします。
	/// </remarks>
	void StopSound(std::string soundName);

	/// <summary>
	/// 指定したサウンドをアンロード（メモリ解放）します。
	/// </summary>
	/// <param name="soundName">アンロードするサウンドの管理名</param>
	/// <remarks>
	/// - 登録されていないサウンド名の場合は何も行わずに終了します。
	/// - サウンドデータのバッファを解放し、構造体の内容を初期化します。
	/// - サウンドインスタンスは停止済みであることを前提としています。
	/// </remarks>
	void SoundUnLoad(std::string soundName);

	/// <summary>
	/// 指定したサウンドインスタンスが再生中かどうかを確認します。
	/// </summary>
	/// <param name="soundInstance">確認する音声インスタンス</param>
	/// <remarks>
	/// - XAUDIO2_VOICE_STATE の BuffersQueued を確認することで、
	///   音声の再生状態を判定しています。
	/// - 再生中にバッファがキューに残っていない場合は false を返します。
	/// </remarks>
	bool isPlayed(SoundInstance soundInstance);

private:

	/// <summary>
	/// 指定したWAVファイルを読み込み、Audio用のサウンドデータとして返します。
	/// </summary>
	/// <param name="fileName">読み込むWAVファイルのパス</param>
	/// <returns>読み込んだ音声データを格納した SoundData 構造体</returns>
	/// <remarks>
	/// - ファイルはバイナリモードで開かれ、RIFF/WAVE形式であることを確認します。
	/// - "fmt "チャンクを読み込み、フォーマット情報を設定します。
	/// - "data"チャンクの音声データをバッファに読み込みます。
	/// - "bext"、"junk"、"LIST"チャンクはスキップして読み込みを続行します。
	/// - 音声バッファは動的確保されるため、不要になったら delete[] で解放する必要があります。
	/// </remarks>
	SoundData LoadWavFile(std::string fileName);

	/// <summary>
	/// 指定された音声データから再生用のSoundInstanceを生成します。
	/// </summary>
	/// <param name="soundData">再生する音声データ</param>
	/// <param name="isLoop">ループ再生するかどうか</param>
	/// <returns>生成された SoundInstance</returns>
	/// <remarks>
	/// - IXAudio2SourceVoice を生成し、指定された波形フォーマットを設定します。
	/// - XAUDIO2_BUFFER に音声データを設定し、必要に応じて無限ループを指定します。
	/// - 戻り値として SoundInstance 構造体を返します。
	/// </remarks>
	SoundInstance CreateSoundInstance(SoundData soundData, bool isLoop);

	///-------------------------------------------///
	/// メンバ変数
	///-------------------------------------------///
private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

	IXAudio2MasteringVoice* masterVoice_;

	std::map<std::string, SoundObject> sounds_;
};