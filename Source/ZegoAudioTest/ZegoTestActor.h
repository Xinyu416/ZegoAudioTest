// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZegoExpressSDK.h"
#include "ZegoTestActor.generated.h"
using namespace ZEGO::EXPRESS;


class IMyEventHandler :public IZegoEventHandler
{
public:
	// ����״̬���»ص�
	void onRoomStateChanged(const std::string& roomID, ZegoRoomStateChangedReason reason, int errorCode, const std::string& extendedData)override;

	// �û�״̬���»ص�
	void onRoomUserUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoUser>& userList)override;

	// ��״̬���»ص�
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
		int32 appID = 383550049;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FString appSign = "3e6da86c4a2302165a3218c662a3c6a19c007464f82faf03531b5564d7321ab2";

	TArray<FString> Users;

	IZegoExpressEngine* engine;

	std::shared_ptr<IMyEventHandler> eventHandler = nullptr;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FString GetCurrentStreamID();

	UFUNCTION(BlueprintCallable)
		void SetCurrentStreamID(const FString& streamID);

	//��¼
	UFUNCTION(BlueprintCallable)
		void LoginRoom(const FString& roomID, const FString& userID);

	//��������ʵ��
	UFUNCTION(BlueprintCallable)
		void CreateEngine();

	//�˳���¼
	UFUNCTION(BlueprintCallable)
		void LoginOut(const FString& roomID);

	//��������ʵ��
	UFUNCTION(BlueprintCallable)
		void DestroyEngine();

	//��ʼ����
	UFUNCTION(BlueprintCallable)
		void StartPublishStream(const FString& streamID);

	//ֹͣ����
	UFUNCTION(BlueprintCallable)
		void StopPublishStream();

	//��ʼ����
	UFUNCTION(BlueprintCallable)
		void StartPlayStream(const FString& streamID);

	//ֹͣ����
	UFUNCTION(BlueprintCallable)
		void StopPlayStream(const FString& streamID);
};
