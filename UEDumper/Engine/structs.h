#pragma once

/// structs.h, this file contains all the structs used by the dumper. You might have to adjust them depending on the game

#include "Engine/Userdefined/UEdefinitions.h"
#include <cstdint>
#include <Windows.h>

#include "enums.h"


// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/Core/Public/UObject/NameTypes.h#L1096
// https://github.com/EpicGames/UnrealEngine/blob/5.1/Engine/Source/Runtime/Core/Public/UObject/NameTypes.h#L1098
struct FName
{
    /** Index into the Names array (used to find String portion of the string/number pair used for comparison) */
	int32_t ComparisonIndex = 0;

	#if UE_VERSION >= UE_5_01
	#if !UE_FNAME_OUTLINE_NUMBER
    /** Number portion of the string/number pair (stored internally as 1 more than actual, so zero'd memory will be the default, no-instance case) */
	int32_t Number = 0;
	#endif
	#endif

	#if WITH_CASE_PRESERVING_NAME
    /** Index into the Names array (used to find String portion of the string/number pair used for display) */
	int32_t DisplayIndex = 0;
	#endif


	#if UE_VERSION < UE_5_01
    /** Number portion of the string/number pair (stored internally as 1 more than actual, so zero'd memory will be the default, no-instance case) */
	int32_t Number = 0;
	#endif
};

struct FUObjectItem
{
    // Pointer to the allocated object
    uintptr_t Object;
    // Internal flags
    int32_t Flags;
    // UObject Owner Cluster Index
    int32_t ClusterRootIndex;
    // Weak Object Pointer Serial number associated with the object
    int32_t SerialNumber;

};



struct FQWord
{
	int32_t A;
	int32_t B;
};

template <class T>
struct TArray
{
    friend struct FString;

public:
    inline TArray()
    {
        Data = nullptr;
        Count = Max = 0;
    };

    inline int Num() const
    {
        return Count;
    };

    inline T& operator[](int i)
    {
        return Data[i];
    };

    inline const T& operator[](int i) const
    {
        return Data[i];
    };

    inline bool IsValidIndex(int i) const
    {
        return i < Num();
    }

    inline int Slack() const
    {
        return Max - Count;
    }

    FORCEINLINE bool RemoveSingle(const int Index)
    {
        if (Index < Count)
        {
            if (Index != Count - 1)
                Data[Index] = Data[Count - 1];

            --Count;

            return true;
        }
        return false;
    }

    FORCEINLINE void RemoveAt(int Index, int Length = 1)
    {
        for (; Length != 0; --Length)
        {
            if (!RemoveSingle(Index++))
                break;
        }
    }

public:
    T* Data;
    int32_t Count;
    int32_t Max;
};

struct FString : public TArray<wchar_t>
{
    inline FString() {};

    FString(const wchar_t* other)
    {
        Max = Count = *other ? static_cast<int32_t>(std::wcslen(other)) + 1 : 0;

        if (Count)
        {
            Data = const_cast<wchar_t*>(other);
        }
    };

    inline bool IsValid() const
    {
        return Data != nullptr;
    }

    inline const wchar_t* c_str() const
    {
        return Data;
    }

    std::string ToString() const
    {
        const auto length = std::wcslen(Data);

        std::string str(length, '\0');

        std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

        return str;
    }
};

template<typename Key, typename Value>
class TMap
{
public:
    char UnknownData[0x50];
};

class FScriptInterface
{
private:
    uint64_t* ObjectPointer;
    void* InterfacePointer;

public:
    inline uint64_t* GetObject() const
    {
        return ObjectPointer;
    }

    inline uint64_t*& GetObjectRef()
    {
        return ObjectPointer;
    }

    inline void* GetInterface() const
    {
        return ObjectPointer != nullptr ? InterfacePointer : nullptr;
    }
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
    inline InterfaceType* operator->() const
    {
        return (InterfaceType*)GetInterface();
    }

    inline InterfaceType& operator*() const
    {
        return *((InterfaceType*)GetInterface());
    }

    inline operator bool() const
    {
        return GetInterface() != nullptr;
    }
};

template<class TEnum>
class TEnumAsByte
{
public:
    inline TEnumAsByte()
    {
    }

    inline TEnumAsByte(TEnum _value)
        : value(static_cast<uint8_t>(_value))
    {
    }

    explicit inline TEnumAsByte(int32_t _value)
        : value(static_cast<uint8_t>(_value))
    {
    }

    explicit inline TEnumAsByte(uint8_t _value)
        : value(_value)
    {
    }

    inline operator TEnum() const
    {
        return (TEnum)value;
    }

    inline TEnum GetValue() const
    {
        return (TEnum)value;
    }

private:
    uint8_t value;
};

template <typename KeyType, typename ValueType>
class TPair
{
private:
    KeyType First;
    ValueType Second;

public:
    TPair(KeyType Key, ValueType Value)
        : First(Key)
        , Second(Value)
    {
    }
    TPair(){};

public:
    FORCEINLINE KeyType& Key()
    {
        return First;
    }
    FORCEINLINE const KeyType& Key() const
    {
        return First;
    }
    FORCEINLINE ValueType& Value()
    {
        return Second;
    }
    FORCEINLINE const ValueType& Value() const
    {
        return Second;
    }
};

template <typename PtrType>
class TUniquePtr
{
private:
    PtrType* Ptr = nullptr;

public:
    bool IsValid() const
    {
        return Ptr != nullptr;
    }

    FORCEINLINE explicit operator bool() const
    {
        return IsValid();
    }

    FORCEINLINE bool operator!() const
    {
        return !IsValid();
    }

    FORCEINLINE PtrType* operator->() const
    {
        return Ptr;
    }

    FORCEINLINE PtrType& operator*() const
    {
        return *Ptr;
    }

    FORCEINLINE const PtrType*& Get() const
    {
        return Ptr;
    }

    FORCEINLINE PtrType*& Get()
    {
        return Ptr;
    }
};

struct FScriptMulticastDelegate
{
    char UnknownData[16];
    char b : 1;
};

class FTextData {
public:
    char pad_0x0000[0x28];  //0x0000
    wchar_t* Name;          //0x0028 
    __int32 Length;         //0x0030 

};

struct FText
{
    FTextData* Data;
    char UnknownData[0x10];
};