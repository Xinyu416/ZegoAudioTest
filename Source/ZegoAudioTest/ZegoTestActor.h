// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZegoExpressSDK.h"
#include "ZegoTestActor.generated.h"
using namespace ZEGO::EXPRESS;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginSucess, const TArray<FString>&, OtherUsers);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchRoomEvent, const FString&, ToRoomID);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDelegateWithTwoParam, bool, bResult, FString, str);

class IMyEventHandler :public IZegoEventHandler
{
public:
	// 推流状态更新回调
	void onRoomStateChanged(const std::string& roomID, ZegoRoomStateChangedReason reason, int errorCode, const std::string& extendedData)override;

	// 用户状态更新回调
	void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList)override;

	// 流状态更新回调
	void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList, const std::string& extendedData)override;

	AZegoTestActor* AZegoTestActorRef;
};



UCLASS()
class ZEGOAUDIOTEST_API AZegoTestActor : public AActor
{
	GENERATED_BODY()

private:

	FString currentStreamID;

public:
	// Sets default values for this actor's properties
	AZegoTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		int32 appID = 2029645280;//383550049
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FString appSign = "0d6be013972c4549d027b59ca4935e5f809f023ccd5947438c5ef471c90d44a6";//3e6da86c4a2302165a3218c662a3c6a19c007464f82faf03531b5564d7321ab2


	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FLoginSucess NotifyLogined;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FSwitchRoomEvent NotifySwitchRoom;

	UFUNCTION(BlueprintCallable)
		bool SDTwoParamsCallback(const FString& str, FDelegateWithTwoParam SDTwoParam);


	TArray<FString> Users;
	IZegoExpressEngine* engine;
	std::shared_ptr<IMyEventHandler> eventHandler = nullptr;

	//获取当前StreamID
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FString GetCurrentStreamID();

	//设置当前StreamID
	UFUNCTION(BlueprintCallable)
		void SetCurrentStreamID(const FString& streamID);

	//登录
	UFUNCTION(BlueprintCallable)
		void LoginRoom(const FString& roomID, const FString& userID);

	//创建引擎实例
	UFUNCTION(BlueprintCallable)
		void CreateEngine();

	//退出登录
	UFUNCTION(BlueprintCallable)
		void LoginOut(const FString& roomID);

	//销毁引擎实例
	UFUNCTION(BlueprintCallable)
		void DestroyEngine();

	//开始推流
	UFUNCTION(BlueprintCallable)
		void StartPublishStream(const FString& streamID);

	//停止推流
	UFUNCTION(BlueprintCallable)
		void StopPublishStream();

	//开始拉流
	UFUNCTION(BlueprintCallable)
		void StartPlayStream(const FString& streamID);

	//停止拉流
	UFUNCTION(BlueprintCallable)
		void StopPlayStream(const FString& streamID);

	//切换房间
	UFUNCTION(BlueprintCallable)
		void SwitchRoom(const FString& fromRoomID, const FString& toRoomID);
};
