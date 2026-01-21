#pragma once

//レイヤータイプ
enum class LayerType {
	FIRST,       //最後列
	BACKGROUND,  //背景
	OBJECT,      //オブジェクト
	PARTICLE,    //パーティクル
	DEBUG,       //デバッグ
	UI,          //UI
	LAST         //最前列
};