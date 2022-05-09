#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 親(0番)
	worldTransform_[0].Initialize();
	// 子(1番)
	worldTransform_[1].translation_ = {0, 4.5f, 0};
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].Initialize();

	/*
	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	// 乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		// X, Y, Z方向のスケーリングを設定
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		// X, Y, Z軸回りの回転角を設定
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		// X, Y, Z軸回りの平行移動を設定
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		// ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}
	*/

	// カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	// アスペクト比を設定
	//viewProjection_.aspectRatio = 16.0f / 9.0f;
	// ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	// ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;
}

void GameScene::Update()
{
	/*
	// FoV変更処理
	{
		// 上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
		}
		// 下キーで視野角が広がる
		if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		// デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	}

	// クリップ距離変更処理
	{
		// 上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_W)) {
			viewProjection_.nearZ += 0.1f;
		} else if (input_->PushKey(DIK_S)) {
			viewProjection_.nearZ -= 0.1f;
		}

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		// デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	}*/

	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		XMFLOAT3 move = {0, 0, 0};

		// キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		// 注視点移動(ベクトルの加算)
		worldTransform_[PartId::Root].translation_.x += move.x;
		worldTransform_[PartId::Root].translation_.y += move.y;
		worldTransform_[PartId::Root].translation_.z += move.z;

		// デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
		  "Root:(%f,%f,%f)", worldTransform_[PartId::Root].translation_.x,
		  worldTransform_[PartId::Root].translation_.y,
		  worldTransform_[PartId::Root].translation_.z);
	}

	worldTransform_[0].UpdateMatrix();
	worldTransform_[1].UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
	/*
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}
	*/
	model_->Draw(worldTransform_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[1], viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}