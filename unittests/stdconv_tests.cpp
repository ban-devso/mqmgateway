#include <libmodmqttsrv/config.hpp>
#include "catch2/catch.hpp"
#include <boost/dll/import.hpp>

#include "libmodmqttconv/converterplugin.hpp"

TEST_CASE ("Scale value with integer result") {
    std::string stdconv_path = "../stdconv/stdconv.so";

    boost::shared_ptr<ConverterPlugin> plugin = boost_dll_import<ConverterPlugin>(
        stdconv_path,
        "converter_plugin",
        boost::dll::load_mode::append_decorations
    );

    std::shared_ptr<DataConverter> conv(plugin->getConverter("scale"));
    std::vector<std::string> args = {
        "4","20",
        "0","400"
    };
    conv->setArgs(args);

    ModbusRegisters data;
    data.appendValue(8);
    MqttValue ret = conv->toMqtt(data);

    REQUIRE(ret.getString() == "100");
}

TEST_CASE ("divide int32 into two modbus registers") {
    std::string stdconv_path = "../stdconv/stdconv.so";

    boost::shared_ptr<ConverterPlugin> plugin = boost_dll_import<ConverterPlugin>(
        stdconv_path,
        "converter_plugin",
        boost::dll::load_mode::append_decorations
    );

    std::shared_ptr<DataConverter> conv(plugin->getConverter("divide"));
    std::vector<std::string> args = {
        "2"
    };
    conv->setArgs(args);

    MqttValue input(0x20004);

    ModbusRegisters output = conv->toModbus(input, 2);

    REQUIRE(output.getValue(0) == 0x1);
    REQUIRE(output.getValue(1) == 0x2);
}

TEST_CASE("int32 tests") {
    std::string stdconv_path = "../stdconv/stdconv.so";

    boost::shared_ptr<ConverterPlugin> plugin = boost_dll_import<ConverterPlugin>(
        stdconv_path,
        "converter_plugin",
        boost::dll::load_mode::append_decorations
    );

    std::shared_ptr<DataConverter> conv(plugin->getConverter("int32"));

    SECTION("read int32 from two modbus registers (high, low)") {

        ModbusRegisters input({32768,1});
        MqttValue output = conv->toMqtt(input);

        REQUIRE(output.getInt() == -2147483647);
    }

    SECTION("read int32 from two modbus registers (low, high)") {
        std::vector<std::string> args = {
            "low_first"
        };
        conv->setArgs(args);

        ModbusRegisters input({1, 32768});
        MqttValue output = conv->toMqtt(input);

        REQUIRE(output.getInt() == -2147483647);
    }

    SECTION("write int32 to two modbus registers (low, high)") {
        std::vector<std::string> args = {
            "low_first"
        };
        conv->setArgs(args);

        MqttValue input(0x20004);
        ModbusRegisters output = conv->toModbus(input, 2);

        REQUIRE(output.getValue(0) == 0x4);
        REQUIRE(output.getValue(1) == 0x2);
    }

    SECTION("write int32 to two modbus registers (high, low)") {
        MqttValue input(0x20004);
        ModbusRegisters output = conv->toModbus(input, 2);

        REQUIRE(output.getValue(0) == 0x2);
        REQUIRE(output.getValue(1) == 0x4);
    }

}

TEST_CASE ("read int16 value") {
    std::string stdconv_path = "../stdconv/stdconv.so";

    boost::shared_ptr<ConverterPlugin> plugin = boost_dll_import<ConverterPlugin>(
        stdconv_path,
        "converter_plugin",
        boost::dll::load_mode::append_decorations
    );

    std::shared_ptr<DataConverter> conv(plugin->getConverter("int16"));

    ModbusRegisters data;
    data.appendValue(0xFFFF);
    MqttValue ret = conv->toMqtt(data);

    REQUIRE(ret.getString() == "-1");
}


