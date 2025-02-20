#include <SFML/Network/IpAddress.hpp>

#include <doctest.h>

#include <sstream>

using namespace std::string_literals;

TEST_CASE("sf::IpAddress class - [network]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::IpAddress ipAddress;
            CHECK(ipAddress.toString() == "0.0.0.0"s);
            CHECK(ipAddress.toInteger() == 0);
            CHECK(ipAddress == sf::IpAddress::None);
        }

        SUBCASE("std::string constructor")
        {
            const sf::IpAddress ipAddress = "192.168.0.1"s;
            CHECK(ipAddress.toString() == "192.168.0.1"s);
            CHECK(ipAddress.toInteger() == 0xC0A80001);
            CHECK(ipAddress != sf::IpAddress::None);
            CHECK(ipAddress != sf::IpAddress::Any);
            CHECK(ipAddress != sf::IpAddress::Broadcast);
            CHECK(ipAddress != sf::IpAddress::LocalHost);

            const sf::IpAddress broadcast("255.255.255.255"s);
            CHECK(broadcast.toString() == "255.255.255.255"s);
            CHECK(broadcast.toInteger() == 0xFFFFFFFF);
            CHECK(broadcast == sf::IpAddress::Broadcast);

            const sf::IpAddress any("0.0.0.0"s);
            CHECK(any.toString() == "0.0.0.0"s);
            CHECK(any.toInteger() == 0x00000000);
            CHECK(any == sf::IpAddress::Any);

            const sf::IpAddress localHost("localhost"s);
            CHECK(localHost.toString() == "127.0.0.1"s);
            CHECK(localHost.toInteger() == 0x7F000001);
            CHECK(localHost == sf::IpAddress::LocalHost);

            const sf::IpAddress invalidIpAddress("255.255.255.256"s);
            CHECK(invalidIpAddress.toString() == "0.0.0.0"s);
            CHECK(invalidIpAddress.toInteger() == 0);
            CHECK(invalidIpAddress == sf::IpAddress::None);
        }

        SUBCASE("const char* constructor")
        {
            const sf::IpAddress ipAddress = "192.168.0.1";
            CHECK(ipAddress.toString() == "192.168.0.1"s);
            CHECK(ipAddress.toInteger() == 0xC0A80001);
            CHECK(ipAddress != sf::IpAddress::None);

            const sf::IpAddress broadcast("255.255.255.255");
            CHECK(broadcast.toString() == "255.255.255.255"s);
            CHECK(broadcast.toInteger() == 0xFFFFFFFF);
            CHECK(broadcast == sf::IpAddress::Broadcast);

            const sf::IpAddress any("0.0.0.0");
            CHECK(any.toString() == "0.0.0.0"s);
            CHECK(any.toInteger() == 0x00000000);
            CHECK(any == sf::IpAddress::Any);

            const sf::IpAddress localHost("localhost");
            CHECK(localHost.toString() == "127.0.0.1"s);
            CHECK(localHost.toInteger() == 0x7F000001);
            CHECK(localHost == sf::IpAddress::LocalHost);

            const sf::IpAddress invalidIpAddress("255.255.255.256");
            CHECK(invalidIpAddress.toString() == "0.0.0.0"s);
            CHECK(invalidIpAddress.toInteger() == 0);
            CHECK(invalidIpAddress == sf::IpAddress::None);
        }

        SUBCASE("Byte constructor")
        {
            const sf::IpAddress ipAddress(142, 250, 69, 238);
            CHECK(ipAddress.toString() == "142.250.69.238"s);
            CHECK(ipAddress.toInteger() == 0x8EFA45EE);
            CHECK(ipAddress != sf::IpAddress::None);
        }

        SUBCASE("Uint32 constructor")
        {
            const sf::IpAddress ipAddress(0xDEADBEEF);
            CHECK(ipAddress.toString() == "222.173.190.239"s);
            CHECK(ipAddress.toInteger() == 0xDEADBEEF);
            CHECK(ipAddress != sf::IpAddress::None);
        }
    }

    SUBCASE("Static functions")
    {
        SUBCASE("getLocalAddress")
        {
            const sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
            CHECK(ipAddress.toString() != "0.0.0.0");
            CHECK(ipAddress.toInteger() != 0);
            CHECK(ipAddress != sf::IpAddress::None);
        }

        SUBCASE("getPublicAddress")
        {
            const sf::IpAddress ipAddress = sf::IpAddress::getPublicAddress();
            CHECK(ipAddress.toString() != "0.0.0.0");
            CHECK(ipAddress.toInteger() != 0);
            CHECK(ipAddress != sf::IpAddress::None);
        }
    }

    SUBCASE("Static constants")
    {
        CHECK(sf::IpAddress::None.toString() == "0.0.0.0"s);
        CHECK(sf::IpAddress::None.toInteger() == 0);

        CHECK(sf::IpAddress::Any.toString() == "0.0.0.0"s);
        CHECK(sf::IpAddress::Any.toInteger() == 0);

        CHECK(sf::IpAddress::LocalHost.toString() == "127.0.0.1"s);
        CHECK(sf::IpAddress::LocalHost.toInteger() == 0x7F000001);

        CHECK(sf::IpAddress::Broadcast.toString() == "255.255.255.255"s);
        CHECK(sf::IpAddress::Broadcast.toInteger() == 0xFFFFFFFF);
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::IpAddress() == sf::IpAddress());
            CHECK(sf::IpAddress("") == sf::IpAddress(""s));
            CHECK(sf::IpAddress("8.8.8.8") == sf::IpAddress(8, 8, 8, 8));
            CHECK(sf::IpAddress(0x42, 0x69, 0x96, 0x24) == sf::IpAddress(0x42699624));
            CHECK(sf::IpAddress(0xABCDEF01) == sf::IpAddress("171.205.239.1"));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::IpAddress() != sf::IpAddress("1.1.1.1"));
            CHECK(sf::IpAddress(0x12344321) != sf::IpAddress(""));
            CHECK(sf::IpAddress(192, 168, 1, 10) != sf::IpAddress(192, 168, 1, 11));
        }

        SUBCASE("operator<")
        {
            CHECK(sf::IpAddress(1) < sf::IpAddress(2));
            CHECK(sf::IpAddress() < sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 0) < sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(1, 0, 0, 0) < sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) < sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) < sf::IpAddress(0, 0, 0, 1));
            CHECK(sf::IpAddress(0, 0, 0, 1) < sf::IpAddress(1, 0, 0, 1));
        }

        SUBCASE("operator>")
        {
            CHECK(sf::IpAddress(2) > sf::IpAddress(1));
            CHECK(sf::IpAddress(0, 0, 0, 0) > sf::IpAddress());
            CHECK(sf::IpAddress(1, 0, 0, 0) > sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) > sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) > sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) > sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(1, 0, 0, 1) > sf::IpAddress(0, 0, 0, 1));
        }

        SUBCASE("operator<=")
        {
            CHECK(sf::IpAddress(1) <= sf::IpAddress(2));
            CHECK(sf::IpAddress() <= sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 0) <= sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(1, 0, 0, 0) <= sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) <= sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) <= sf::IpAddress(0, 0, 0, 1));
            CHECK(sf::IpAddress(0, 0, 0, 1) <= sf::IpAddress(1, 0, 0, 1));

            CHECK(sf::IpAddress() <= sf::IpAddress());
            CHECK(sf::IpAddress("") <= sf::IpAddress(""s));
            CHECK(sf::IpAddress("8.8.8.8") <= sf::IpAddress(8, 8, 8, 8));
            CHECK(sf::IpAddress(0x42, 0x69, 0x96, 0x24) <= sf::IpAddress(0x42699624));
            CHECK(sf::IpAddress(0xABCDEF01) <= sf::IpAddress("171.205.239.1"));
        }

        SUBCASE("operator>=")
        {
            CHECK(sf::IpAddress(2) >= sf::IpAddress(1));
            CHECK(sf::IpAddress(0, 0, 0, 0) >= sf::IpAddress());
            CHECK(sf::IpAddress(1, 0, 0, 0) >= sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) >= sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) >= sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) >= sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(1, 0, 0, 1) >= sf::IpAddress(0, 0, 0, 1));

            CHECK(sf::IpAddress() >= sf::IpAddress());
            CHECK(sf::IpAddress("") >= sf::IpAddress(""s));
            CHECK(sf::IpAddress("8.8.8.8") >= sf::IpAddress(8, 8, 8, 8));
            CHECK(sf::IpAddress(0x42, 0x69, 0x96, 0x24) >= sf::IpAddress(0x42699624));
            CHECK(sf::IpAddress(0xABCDEF01) >= sf::IpAddress("171.205.239.1"));
        }

        SUBCASE("operator>>")
        {
            sf::IpAddress ipAddress;
            std::istringstream("4.4.4.4") >> ipAddress;
            CHECK(ipAddress.toString() == "4.4.4.4"s);
            CHECK(ipAddress.toInteger() == 0x04040404);
            CHECK(ipAddress != sf::IpAddress::None);

            std::istringstream("92.100.0.72") >> ipAddress;
            CHECK(ipAddress.toString() == "92.100.0.72"s);
            CHECK(ipAddress.toInteger() == 0x5C640048);
            CHECK(ipAddress != sf::IpAddress::None);
        }

        SUBCASE("operator<<")
        {
            std::ostringstream out;
            out << sf::IpAddress(10, 9, 8, 7);
            CHECK(out.str() == "10.9.8.7"s);
        }
    }
}
