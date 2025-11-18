#include "Audio.h"

#include "fstream"
#include "cassert"

///=====================================================/// 
/// Audioのシングルトンインスタンスを取得
///=====================================================///
Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}

///=====================================================/// 
/// XAudio2の初期化
///=====================================================///
void Audio::Initialize() {

	HRESULT result;

	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

///=====================================================/// 
/// 登録されている全オーディオの状態を更新
///=====================================================///
void Audio::Update() {

	//登録されている全要素を参照
	for (auto& [name, object] : sounds_) {

		//生成されている全インスタンスを参照
		for (int i = 0; i < object.instance.size(); i++) {

			//音声が流れていなければ
			if (!isPlayed(object.instance[i])) {

				//音声インスタンスを削除
				object.instance.erase(object.instance.begin() + i);
			}
		}
	}
}

///=====================================================/// 
/// オーディオシステムの終了処理
///=====================================================///
void Audio::Finalize() {

	//登録されている全サウンドを参照
	for (auto& [name, object] : sounds_) {

		//生成されている全インスタンスを参照
		for (int i = 0; i < static_cast<int>(object.instance.size()); i++) {

			//再生中の音声を停止
			xAudio2_->CommitChanges(object.instance[i].pSourceVoice->Stop());

			//音声インスタンスを削除
			object.instance.erase(object.instance.begin() + i);

			//eraseで要素が詰められるのでインデックスを戻す
			i--;
		}

		//サウンドリソースを解放
		SoundUnLoad(name);
	}

	//サウンド管理データをクリア
	sounds_.clear();

	//XAudio2インスタンスをリセット
	xAudio2_.Reset();
}

///=====================================================/// 
/// 指定した名前でサウンドを読み込み、管理リストに登録
///=====================================================///
void Audio::SoundLoad(std::string soundName, std::string fileName) {

	//既に登録されているかを確認
	if (sounds_.contains(soundName)) {

		//登録されていたら早期リターン
		return;
	}

	std::string filePath = "Resource/Sound/SE/" + fileName;

	//新しく登録する音源
	SoundObject object;

	//音源データを読み込む
	SoundData data = LoadWavFile(filePath);

	//音源データを設定する
	object.data = data;

	//音源を登録する
	sounds_.insert(std::make_pair(soundName, object));
}

///=====================================================/// 
/// 指定したサウンドを再生
///=====================================================///
void Audio::StartSound(std::string soundName, bool isLoop) {

	//登録されていないサウンドの場合は早期リターン
	if (!sounds_.contains(soundName)) {
		return;
	}

	HRESULT result;

	//新しいサウンドインスタンスを作成
	SoundInstance newInstance;
	newInstance = CreateSoundInstance(sounds_[soundName].data, isLoop);

	//サウンドバッファをソースボイスに送信
	result = newInstance.pSourceVoice->SubmitSourceBuffer(&newInstance.buf);

	//サウンド再生開始
	result = newInstance.pSourceVoice->Start();

	//管理用リストに追加
	sounds_[soundName].instance.push_back(newInstance);
}

///=====================================================/// 
/// 指定したサウンドを停止
///=====================================================///
void Audio::StopSound(std::string soundName) {

	//登録されていないサウンドの場合は早期リターン
	if (!sounds_.contains(soundName)) {
		return;
	}

	HRESULT result;

	//登録されているすべてのインスタンスを停止
	for (int i = 0; i < sounds_[soundName].instance.size(); i++) {

		//サウンドを停止
		xAudio2_->CommitChanges(sounds_[soundName].instance[i].pSourceVoice->Stop());

		//ソースバッファをフラッシュ
		result = sounds_[soundName].instance[i].pSourceVoice->FlushSourceBuffers();
	}
}

///=====================================================/// 
/// 指定したサウンドをアンロード（メモリ解放）
///=====================================================///
void Audio::SoundUnLoad(std::string soundName) {

	// 登録されていない場合は早期リターン
	if (!sounds_.contains(soundName)) {
		return;
	}

	// 音源データのメモリを解放
	delete[] sounds_[soundName].data.pBuffer;

	// 解放後、構造体の内容を初期化
	sounds_[soundName].data.pBuffer = nullptr;
	sounds_[soundName].data.bufferSize = 0;
	sounds_[soundName].data.wfex = {};
}

///=====================================================/// 
/// 指定したサウンドインスタンスが再生中かどうかを確認
///=====================================================///
bool Audio::isPlayed(SoundInstance soundInstance) {

	XAUDIO2_VOICE_STATE state;

	soundInstance.pSourceVoice->GetState(&state);

	if (state.BuffersQueued == 0) {
		return false;
	}

	return true;
}

///=====================================================/// 
/// 指定したWAVファイルの読み込み
///=====================================================///
Audio::SoundData Audio::LoadWavFile(std::string fileName) {

	/// === ローカル変数 === ///

	//ファイル情報
	std::ifstream file;

	//Riffチャンクの先頭情報
	RiffHeader riff;

	//音声のフォーマット
	FormatChunk format = {};

	//Dataチャンクの先頭情報
	ChunkHeader data;

	//音声データ
	SoundData soundData = {};

	/// === ファイルを開く === ///

	//.wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);

	//ファイルが開けなかったらエラー
	assert(file.is_open());

	/// === ファイル識別子の確認 === ///

	//Riffチャンクの読み込み
	file.read((char*)&riff, sizeof(riff));

	//IDが"RIFF"となっているかをチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	//Waveファイルであるかのチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	/// === ファイル構造の読み込み === ///

	//Formatチャンクの先頭部分のみを読み込む
	file.read((char*)&format, sizeof(ChunkHeader));

	//IDが"fmt "となっているかをチェック
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//Formatチャンクのサイズをチェック
	assert(format.chunk.size <= sizeof(format.fmt));

	//Formatチャンクの内容を読み込む
	file.read((char*)&format.fmt, format.chunk.size);

	/// === 音声情報の読み込み === ///

	//Dataチャンクの読み込み
	file.read((char*)&data, sizeof(data));

	//IDが"bext"となっていたら
	if (strncmp(data.id, "bext", 4) == 0) {

		//読み取り位置をBextチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//IDが"junk"となっていたら
	if (strncmp(data.id, "junk", 4) == 0) {

		//読み取り位置をJunkチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//IDが"JUNK"となっていたら
	if (strncmp(data.id, "JUNK", 4) == 0) {

		//読み取り位置をJunkチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//IDが"LIST"となっていたら
	if (strncmp(data.id, "LIST", 4) == 0) {

		//読み取り位置をListチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//Dataチャンクがなかった場合
	if (strncmp(data.id, "data", 4) != 0) {

		//エラーを出す
		assert(0);
	}

	//音声データのサイズ分のバッファ
	char* pBuffer = new char[data.size];

	//音声情報の読み込み
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	/// === 音声データの設定 === ///

	//波形フォーマットの設定
	soundData.wfex = format.fmt;

	//バッファアドレスの設定
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);

	//バッファサイズの設定
	soundData.bufferSize = data.size;

	return soundData;
}

///=====================================================/// 
/// 指定された音声データから再生用のSoundInstanceを生成
///=====================================================///
Audio::SoundInstance Audio::CreateSoundInstance(SoundData soundData, bool isLoop) {

	HRESULT result;

	//SourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//音声バッファの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//SoundInstance構造体に情報をセット
	SoundInstance resultInstance;
	resultInstance.pSourceVoice = pSourceVoice;
	resultInstance.buf = buf;
	resultInstance.soundData = soundData;

	return resultInstance;
}
