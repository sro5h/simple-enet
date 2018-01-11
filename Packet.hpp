#ifndef _PACKET_HPP_
#define _PACKET_HPP_

#include <cstdint>
#include <string>
#include <vector>

typedef struct _ENetPacket ENetPacket;

// Fixed size integers
typedef std::int8_t Int8;
typedef std::uint8_t Uint8;
typedef std::int16_t Int16;
typedef std::uint16_t Uint16;
typedef std::int32_t Int32;
typedef std::uint32_t Uint32;

class Packet
{
public:
        enum class Flag : Uint32;

public:
        Packet();
        Packet(Flag flag);
        ~Packet();

        void append(const void* data, std::size_t size);
        void clear();
        const void* getData() const;
        std::size_t getDataSize() const;
        bool dataLeft() const;

public:
        explicit operator bool() const;

        Packet& operator>>(bool& data);
        Packet& operator>>(Int8& data);
        Packet& operator>>(Uint8& data);
        Packet& operator>>(Int16& data);
        Packet& operator>>(Uint16& data);
        Packet& operator>>(Int32& data);
        Packet& operator>>(Uint32& data);
        Packet& operator>>(float& data);
        Packet& operator>>(double& data);
        Packet& operator>>(char* data);
        Packet& operator>>(std::string& data);

        Packet& operator<<(bool data);
        Packet& operator<<(Int8 data);
        Packet& operator<<(Uint8 data);
        Packet& operator<<(Int16 data);
        Packet& operator<<(Uint16 data);
        Packet& operator<<(Int32 data);
        Packet& operator<<(Uint32 data);
        Packet& operator<<(float data);
        Packet& operator<<(double data);
        Packet& operator<<(const char* data);
        Packet& operator<<(const std::string& data);

public:
        enum class Flag : Uint32
        {
                Reliable,
                Unreliable,
                Unsequenced,
        };

public:
        Flag flag;

private:
        bool operator==(const Packet& p) const;
        bool operator!=(const Packet& p) const;

        bool checkSize(std::size_t size);

        std::vector<char> mData;
        std::size_t mReadPos;
        bool mIsValid;
};

ENetPacket* toENetPacket(const Packet& packet);

#endif // _PACKET_HPP_
