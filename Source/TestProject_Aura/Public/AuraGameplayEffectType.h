#pragma once

#include "GameplayEffectTypes.h"
#include "AuraGameplayEffectType.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext: public FGameplayEffectContext
{
	GENERATED_BODY()
public:

	bool IsCriticalHit() const { 
		return bIsCriticalHit; 
	}
	bool IsBlockedHit()  const;

	void SetIsCriticalHit(const bool bInCriticalHit) { bIsCriticalHit = bInCriticalHit; }
	void SetIsBlockedHit(const bool bInBlockedHit);
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	/** 为了将新增的两个变量加入序列化，重写了这个函数 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	
protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithCopy = true,		// Necessary so that TSharedPtr<FGameplayEffectContext> Data is copied around
		WithNetSerializer = true
	};
};