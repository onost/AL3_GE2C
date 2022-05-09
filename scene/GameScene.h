#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
  public:
	  enum PartId {
		  Root,		// 大元　０
		  Spine,	// 脊椎　１
		  Chest,	// 胸　　２
		  Head,		// 頭　　３
		  ArmL,		// 左腕　４
		  ArmR,		// 右腕　５
		  Hip,		// 尻　　６
		  LegL,		// 左足　７
		  LegR		// 右足　８
	  };
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルの生成
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_[100];
	// ビュープロジェクション
	ViewProjection viewProjection_;
};
