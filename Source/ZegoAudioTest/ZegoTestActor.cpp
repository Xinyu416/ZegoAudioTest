// Fill out your copyright notice in the Description page of Project Settings.


#include "ZegoTestActor.h"

// Sets default values
AZegoTestActor::AZegoTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZegoTestActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZegoTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//获取当前StreamID
FString AZegoTestActor::GetCurrentStreamID()
{
	return this->currentStreamID;
}

//设置当前StreamID
void AZegoTestActor::SetCurrentStreamID(const FString& streamID)
{
	this->currentStreamID = *streamID;
}

/// <summary>
/// 创建引擎实例
/// </summary>
void AZegoTestActor::CreateEngine()
{
	ZEGO::EXPRESS::ZegoEngineProfile profile;
	profile.appID = appID;
	profile.appSign = TCHAR_TO_UTF8(*appSign);
	profile.scenario = ZegoScenario::ZEGO_SCENARIO_BROADCAST;
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, FString::Printf(TEXT("BeginPlay() --> profile.appID: %d"), profile.appID));

	eventHandler = std::make_shared<IMyEventHandler>();
	eventHandler->AZegoTestActorRef = this;

	// 创建实例
	engine = ZegoExpressSDK::createEngine(profile, eventHandler);

	if (engine == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "create engine failed");
	else
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "create engine success");
}

/// <summary>
/// 登录
/// </summary>
/// <param name="roomID"></param>
/// <param name="userID"></param>
void AZegoTestActor::LoginRoom(const FString& roomID, const FString& userID)
{
	ZegoUser user;
	user.userID = TCHAR_TO_UTF8(*userID);
	user.userName = user.userID;
	ZegoRoomConfig config;
	config.isUserStatusNotify = true;

	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}
	engine->loginRoom(TCHAR_TO_UTF8(*roomID), user, config,
		[=](int errorCode, std::string extendedData) {

			AsyncTask(ENamedThreads::GameThread, [=]() {
				UE_LOG(LogTemp, Warning, TEXT("loginRoom result, errorCode:%d,extendedData:%s"), errorCode, *FString(extendedData.c_str()))
				});
		});

	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "loginRoom End");
}

/// <summary>
/// 退出登录
/// </summary>
/// <param name="roomID"></param>
void AZegoTestActor::LoginOut(const FString& roomID)
{
	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}
	engine->logoutRoom(TCHAR_TO_UTF8(*roomID));
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "LoginOut");
}

/// <summary>
/// 销毁引擎实例
/// </summary>
void AZegoTestActor::DestroyEngine()
{
	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}

	ZegoExpressSDK::destroyEngine(engine, nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "DestroyEngine Sucess");
	eventHandler->AZegoTestActorRef = nullptr;
}

/// <summary>
/// 开始推流
/// </summary>
/// <param name="streamID"></param>
void AZegoTestActor::StartPublishStream(const FString& streamID)
{
	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}
	engine->startPublishingStream(TCHAR_TO_UTF8(*streamID));
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "StartPublishStream");
}


/// <summary>
/// 停止推流
/// </summary>
void AZegoTestActor::StopPublishStream()
{
	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}
	engine->stopPublishingStream();
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "StopPublishStream");
}


/// <summary>
/// 开始拉流
/// </summary>
/// <param name="streamID"></param>
void AZegoTestActor::StartPlayStream(const FString& streamID)
{
	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}
	engine->startPlayingStream(TCHAR_TO_UTF8(*streamID));
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "StartPlayStream");
}

/// <summary>
/// 停止拉流
/// </summary>
/// <param name="streamID"></param>
void AZegoTestActor::StopPlayStream(const FString& streamID)
{
	if (engine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "please create engine first");
		return;
	}
	engine->stopPlayingStream(TCHAR_TO_UTF8(*streamID));
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "StopPlayStream");
}


bool AZegoTestActor::SDTwoParamsCallback(const FString& str, FDelegateWithTwoParam SDTwoParam)
{
	SDTwoParam.ExecuteIfBound(true,"Test SDOneParamCallback");
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, *FString(str));
	return true;
}


/// <summary>
/// 房间状态更新回调
/// </summary>
/// <param name="roomID"></param>
/// <param name="reason"></param>
/// <param name="errorCode"></param>
/// <param name="extendedData"></param>
void IMyEventHandler::onRoomStateChanged(const std::string& roomID, ZegoRoomStateChangedReason reason, int errorCode, const std::string& extendedData) {

	//只有当房间状态是登录成功或重连成功时，推流（startPublishingStream）、拉流（startPlayingStream）才能正常收发音视频
	//将自己的音视频流推送到 ZEGO 音视频云

	switch (reason)
	{
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_LOGINING:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "LOGINING");
		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_LOGINED:
		//登录成功
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "LOGINED");
		this->AZegoTestActorRef->StartPublishStream(this->AZegoTestActorRef->GetCurrentStreamID());
		//推流
		this->AZegoTestActorRef->StartPublishStream(this->AZegoTestActorRef->GetCurrentStreamID());
		//拉流
		for (size_t i = 0; i < this->AZegoTestActorRef->Users.Num(); ++i)
		{
			this->AZegoTestActorRef->StartPlayStream(this->AZegoTestActorRef->Users[i]);
		}
		//广播
		this->AZegoTestActorRef->NotifyLogined.Broadcast(this->AZegoTestActorRef->Users);

		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_LOGIN_FAILED:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "FAILED");
		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_RECONNECTING:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "RECONNECTING");
		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_RECONNECTED:
		//重连成功
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "RECONNECTED");
		//推流
		this->AZegoTestActorRef->StartPublishStream(this->AZegoTestActorRef->GetCurrentStreamID());
		//拉流
		for (size_t i = 0; i < this->AZegoTestActorRef->Users.Num(); ++i)
		{
			this->AZegoTestActorRef->StartPlayStream(this->AZegoTestActorRef->Users[i]);
		}
		//广播
		this->AZegoTestActorRef->NotifyLogined.Broadcast(this->AZegoTestActorRef->Users);

		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_RECONNECT_FAILED:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "RECONNECT_FAILED");
		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_KICK_OUT:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "KICK_OUT");
		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_LOGOUT:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "LOGOUT");
		break;
	case ZEGO::EXPRESS::ZEGO_ROOM_STATE_CHANGED_REASON_LOGOUT_FAILED:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, "LOGOUT_FAILED");
		break;
	default:
		break;
	}
}

/// <summary>
/// 用户状态更新回调
/// </summary>
/// <param name="roomID"></param>
/// <param name="updateType"></param>
/// <param name="userList"></param>
void IMyEventHandler::onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList)
{
	switch (updateType)
	{
	case ZEGO::EXPRESS::ZEGO_UPDATE_TYPE_ADD:
		//用户新增 拉流
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "User_ADD");
		for (size_t i = 0; i < userList.size(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, *FString((userList[i].userID).c_str()));
			this->AZegoTestActorRef->StartPlayStream(*FString((userList[i].userID).c_str()));

			//保存用户ID信息
			this->AZegoTestActorRef->Users.Add(*FString((userList[i].userID).c_str()));
		}

		break;
		//用户减少 停止拉流
	case ZEGO::EXPRESS::ZEGO_UPDATE_TYPE_DELETE:
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, "User_DELETE");

		for (size_t i = 0; i < userList.size(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, *FString((userList[i].userID).c_str()));
			this->AZegoTestActorRef->StopPlayStream(*FString((userList[i].userID).c_str()));

			//删除用户ID信息
			this->AZegoTestActorRef->Users.Remove(*FString((userList[i].userID).c_str()));
		}
		break;

	default:
		break;
	}
}

//
void IMyEventHandler::onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList, const std::string& extendedData)
{
	switch (updateType)
	{
	case ZEGO::EXPRESS::ZEGO_UPDATE_TYPE_ADD:
		//用户新增 拉流
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, *FString(roomID.c_str()));
		for (size_t i = 0; i < streamList.size(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, *FString((streamList[i].streamID).c_str()));
			this->AZegoTestActorRef->StartPlayStream(*FString((streamList[i].streamID).c_str()));
		}
		break;

	case ZEGO::EXPRESS::ZEGO_UPDATE_TYPE_DELETE:
		for (size_t i = 0; i < streamList.size(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, *FString((streamList[i].streamID).c_str()));
			this->AZegoTestActorRef->StopPlayStream(*FString((streamList[i].streamID).c_str()));
		}
		break;

	default:
		break;
	}
}