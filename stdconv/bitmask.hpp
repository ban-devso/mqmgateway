#pragma once

#include <cmath>
#include "libmodmqttconv/converter.hpp"

class BitmaskConverter : public IStateConverter {
    public:
        virtual MqttValue toMqtt(const ModbusRegisters& data) const {
            int val = data.getValue(0) & mask;
            return MqttValue::fromInt(val);
        }

        virtual void setArgs(const std::vector<std::string>& args) {
            mask = ConverterTools::getHex16Arg(0, args);
        }

        virtual ~BitmaskConverter() {}
    private:
        u_int16_t mask = 0xffff;
};
