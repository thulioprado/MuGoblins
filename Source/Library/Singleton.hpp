#pragma once

#include <memory>

template<typename T>
class Singleton
{
public:
    Singleton()
    {
    }

    ~Singleton()
    {
    }

public:
    static T* GetInstance() 
    {
        static std::unique_ptr<T> Instance = std::make_unique<T>();
        return Instance.get();
    }
};