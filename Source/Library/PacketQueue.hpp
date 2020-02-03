#pragma once

#include "Types.hpp"
#include <queue>
#include <memory>
#include <mutex>

class PacketQueue
{
public:
    struct Packet
    {
        Packet(ui16 Index, ui8 Head, ui8* Data, ui32 Size) :
            Index(Index), Head(Head), Data(std::make_unique<ui8[]>(Size)), Size(Size)
        {
            memcpy(this->Data.get(), Data, Size);
        }
        
        ui16                    Index;
        ui8                     Head;
        std::unique_ptr<ui8[]>  Data;
        ui32                    Size;
    };

public:
    PacketQueue() :
        Mutex(), Packets()
    {
    }
    
    ~PacketQueue()
    {
        this->Clear();
    }

public:
    void Add(ui16 Index, ui8 Head, ui8* Data, ui32 Size)
    {
        std::lock_guard<std::mutex> Guard(this->Mutex);

        this->Packets.push({
            Index, Head, Data, Size
        });
    }

    bool Get(Packet* Packet)
    {
        std::lock_guard<std::mutex> Guard(this->Mutex);

        if (!this->Packets.empty())
        {
            const auto& Front = this->Packets.front();

            Packet->Index = Front.Index;
            Packet->Head  = Front.Head;
            Packet->Data  = std::make_unique<ui8[]>(Front.Size);
            Packet->Size  = Front.Size;

            memcpy(Packet->Data.get(), Front.Data.get(), Front.Size);

            this->Packets.pop();

            return true;
        }

        return false;
    }

    void Clear()
    {
        std::lock_guard<std::mutex> Guard(this->Mutex);

        this->Packets.swap(std::queue<Packet>());
    }

private:
    std::mutex          Mutex;
    std::queue<Packet>  Packets;
};