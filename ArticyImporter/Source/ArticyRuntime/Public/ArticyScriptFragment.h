//  
// Copyright (c) articy Software GmbH & Co. KG. All rights reserved.  
// Licensed under the MIT license. See LICENSE file in the project root for full license information.  
//
#pragma once

#include "ArticyBaseObject.h"
#include "ArticyNode.h"
#include "ArticyConditionProvider.h"
#include "ArticyInstructionProvider.h"
#include "ArticyScriptFragment.generated.h"

/**
 * Base class for the script fragments (ArticyScriptCondition or ArticyScriptInstruction)
 */
UCLASS(BlueprintType, abstract)
class ARTICYRUNTIME_API UArticyScriptFragment : public UArticyPrimitive
{
	GENERATED_BODY()

protected:

	//for some reason, the editor crashes on save if this property is an FName
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Expression = "";

	//returns a cached FName version of the expression
	FName GetExpression() const;

	template<typename Type, typename PropType>
		friend struct ArticyObjectTypeInfo;

	void InitFromJson(TSharedPtr<FJsonValue> Json) override;

private:

	mutable FName CachedExpression = "";
};

/** -------------------------------------------------------------------------------- */

/**
 * A script condition.
 */
UCLASS(BlueprintType)
class ARTICYRUNTIME_API UArticyScriptCondition : public UArticyScriptFragment, public IArticyConditionProvider
{
	GENERATED_BODY()

public:

	bool Evaluate(class UArticyGlobalVariables* GV = nullptr, class UObject* MethodProvider = nullptr) override;
};

/**
 * A condition node, which has a script condition.
 */
UCLASS(BlueprintType)
class ARTICYRUNTIME_API UArticyCondition : public UArticyNode, public IArticyConditionProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Condition")
	UArticyScriptCondition* GetCondition() const;

	EArticyPausableType GetType() override { return EArticyPausableType::Condition; }

	bool Evaluate(class UArticyGlobalVariables* GV = nullptr, class UObject* MethodProvider = nullptr) override;

	void Explore(UArticyFlowPlayer* Player, TArray<FArticyBranch>& OutBranches, const uint32& Depth) override;

private:

	UPROPERTY(transient)
	mutable UArticyScriptCondition* Condition = nullptr;
};

/** -------------------------------------------------------------------------------- */

/**
 * A script instruction.
 */
UCLASS(BlueprintType)
class ARTICYRUNTIME_API UArticyScriptInstruction : public UArticyScriptFragment, public IArticyInstructionProvider
{
	GENERATED_BODY()

public:

	void Execute(class UArticyGlobalVariables* GV = nullptr, class UObject* MethodProvider = nullptr) override;
};

/**
 * A instruction node, which has a script instruction.
 */
UCLASS(BlueprintType)
class ARTICYRUNTIME_API UArticyInstruction : public UArticyNode, public IArticyInstructionProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Instruction")
	UArticyScriptInstruction* GetInstruction() const;

	EArticyPausableType GetType() override { return EArticyPausableType::Instruction; }

	void Execute(class UArticyGlobalVariables* GV = nullptr, class UObject* MethodProvider = nullptr) override;

	void Explore(UArticyFlowPlayer* Player, TArray<FArticyBranch>& OutBranches, const uint32& Depth) override;
	
private:

	UPROPERTY(transient)
	mutable UArticyScriptInstruction* Instruction = nullptr;
};
