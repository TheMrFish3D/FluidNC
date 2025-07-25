#include "Axes.h"

#include "../Motors/MotorDriver.h"
#include "../Motors/NullMotor.h"
#include "../Config.h"
#include "../MotionControl.h"
#include "../Stepper.h"     // stepper_id_t
#include "MachineConfig.h"  // config->
#include "../Limits.h"

const EnumItem axisType[] = { { 0, "X" }, { 1, "Y" }, { 2, "Z" }, { 3, "A" }, { 4, "B" }, { 5, "C" }, EnumItem(0) };

namespace Machine {
    MotorMask Axes::posLimitMask = 0;
    MotorMask Axes::negLimitMask = 0;
    MotorMask Axes::limitMask    = 0;
    MotorMask Axes::motorMask    = 0;

    AxisMask Axes::homingMask = 0;

    bool Axes::disabled = false;

    Pin Axes::_sharedStepperDisable;
    Pin Axes::_sharedStepperReset;

    uint32_t Axes::_homing_runs = 2;  // Number of Approach/Pulloff cycles

    int Axes::_numberAxis = 0;

    Axis* Axes::_axis[MAX_N_AXIS] = { nullptr };

    Axes::Axes() {}

    void Axes::init() {
        log_info("Axis count " << Axes::_numberAxis);

        if (_sharedStepperDisable.defined()) {
            _sharedStepperDisable.setAttr(Pin::Attr::Output);
            _sharedStepperDisable.report("Shared stepper disable");
        }

        if (_sharedStepperReset.defined()) {
            _sharedStepperReset.setAttr(Pin::Attr::Output | Pin::Attr::InitialOn);
            _sharedStepperReset.on();
            _sharedStepperReset.report("Shared stepper reset");
        }

        // certain motors need features to be turned on. Check them here
        for (size_t axis = X_AXIS; axis < _numberAxis; axis++) {
            auto a = _axis[axis];
            if (a) {
                log_info("Axis " << axisName(axis) << " (" << limitsMinPosition(axis) << "," << limitsMaxPosition(axis) << ")");
                a->init();
            }
            auto homing = a->_homing;
            if (homing && !homing->_positiveDirection) {
                set_bitnum(Homing::direction_mask, axis);
            }
        }

        config_motors();
    }

    void IRAM_ATTR Axes::set_disable(int axis, bool disable) {
        for (int motor = 0; motor < Axis::MAX_MOTORS_PER_AXIS; motor++) {
            auto m = _axis[axis]->_motors[motor];
            if (m) {
                m->_driver->set_disable(disable);
            }
        }
        if (disable)  // any disable, !disable does not change anything here
            disabled = true;
    }

    void IRAM_ATTR Axes::set_disable(bool disable) {
        for (int axis = 0; axis < _numberAxis; axis++) {
            set_disable(axis, disable);
        }

        _sharedStepperDisable.synchronousWrite(disable);

        if (!disable && disabled) {
            disabled = false;
            if (Stepping::_disableDelayUsecs) {  // wait for the enable delay
                delay_us(Stepping::_disableDelayUsecs);
            }
        }
    }

    // Put the motors in the given axes into homing mode, returning a
    // mask of which motors can do homing.
    MotorMask Axes::set_homing_mode(AxisMask axisMask, bool isHoming) {
        MotorMask motorsCanHome = 0;

        for (size_t axis = X_AXIS; axis < _numberAxis; axis++) {
            if (bitnum_is_true(axisMask, axis)) {
                auto a = _axis[axis];
                if (a != nullptr) {
                    for (size_t motor = 0; motor < Axis::MAX_MOTORS_PER_AXIS; motor++) {
                        Stepping::unblock(axis, motor);
                        auto m = _axis[axis]->_motors[motor];
                        if (m) {
                            if (m->_driver->set_homing_mode(isHoming)) {
                                set_bitnum(motorsCanHome, motor_bit(axis, motor));
                            }
                        }
                    }
                }
            }
        }

        return motorsCanHome;
    }

    void Axes::config_motors() {
        for (int axis = 0; axis < _numberAxis; ++axis) {
            _axis[axis]->config_motors();
        }
    }

    // Some small helpers to find the axis index and axis motor index for a given motor. This
    // is helpful for some motors that need this info, as well as debug information.
    size_t Axes::findAxisIndex(const MotorDrivers::MotorDriver* const driver) {
        for (int i = 0; i < _numberAxis; ++i) {
            for (int j = 0; j < Axis::MAX_MOTORS_PER_AXIS; ++j) {
                if (_axis[i] != nullptr && _axis[i]->hasMotor(driver)) {
                    return i;
                }
            }
        }

        Assert(false, "Cannot find axis for motor driver.");
        return SIZE_MAX;
    }

    size_t Axes::findAxisMotor(const MotorDrivers::MotorDriver* const driver) {
        for (int i = 0; i < _numberAxis; ++i) {
            if (_axis[i] != nullptr && _axis[i]->hasMotor(driver)) {
                for (int j = 0; j < Axis::MAX_MOTORS_PER_AXIS; ++j) {
                    auto m = _axis[i]->_motors[j];
                    if (m && m->_driver == driver) {
                        return j;
                    }
                }
            }
        }

        Assert(false, "Cannot find axis for motor. Something wonky is going on here...");
        return SIZE_MAX;
    }

    // Configuration helpers:

    void Axes::group(Configuration::HandlerBase& handler) {
        handler.item("shared_stepper_disable_pin", _sharedStepperDisable);
        handler.item("shared_stepper_reset_pin", _sharedStepperReset);
        handler.item("homing_runs", _homing_runs, 1, 5);

        // Handle axis names xyzabc.  handler.section is inferred
        // from a template.
        char tmp[3];
        tmp[2] = '\0';

        // During the initial configuration parsing phase, _numberAxis is 0 so
        // we try for all the axes.  Subsequently we use the number of axes
        // that are actually present.
        size_t n_axis = _numberAxis ? _numberAxis : MAX_N_AXIS;
        for (size_t i = 0; i < n_axis; ++i) {
            tmp[0] = tolower(_names[i]);
            tmp[1] = '\0';

            handler.section(tmp, _axis[i], i);
        }
    }

    void Axes::afterParse() {
        // Find the last axis that was declared and set _numberAxis accordingly
        for (size_t i = MAX_N_AXIS; i > 0; --i) {
            if (_axis[i - 1] != nullptr) {
                _numberAxis = i;
                break;
            }
        }
        // Senders might assume 3 axes in reports
        if (_numberAxis < 3) {
            _numberAxis = 3;
        }

        for (size_t i = 0; i < _numberAxis; ++i) {
            if (_axis[i] == nullptr) {
                _axis[i] = new Axis(i);
            }
        }
    }

    std::string Axes::maskToNames(AxisMask mask) {
        std::string retval("");
        auto        n_axis = _numberAxis;
        for (int axis = 0; axis < n_axis; axis++) {
            if (bitnum_is_true(mask, axis)) {
                retval += _names[axis];
            }
        }
        return retval;
    }
    std::string Axes::motorMaskToNames(MotorMask mask) {
        std::string retval("");
        auto        n_axis = _numberAxis;
        for (int axis = 0; axis < n_axis; axis++) {
            if (bitnum_is_true(mask, axis)) {
                retval += " ";
                retval += _names[axis];
            }
        }
        mask >>= 16;
        for (int axis = 0; axis < n_axis; axis++) {
            if (bitnum_is_true(mask, axis)) {
                retval += " ";
                retval += _names[axis];
                retval += "2";
            }
        }
        return retval;
    }

    MotorMask Axes::hardLimitMask() {
        MotorMask mask = 0;
        for (int axis = 0; axis < _numberAxis; ++axis) {
            auto a = _axis[axis];

            for (int motor = 0; motor < Axis::MAX_MOTORS_PER_AXIS; ++motor) {
                auto m = a->_motors[motor];
                if (m && m->_hardLimits) {
                    set_bitnum(mask, axis);
                }
            }
        }
        return mask;
    }

    bool Axes::namesToMask(const char* names, AxisMask& mask) {
        bool       retval   = true;
        const auto lenNames = strlen(names);
        for (int i = 0; i < lenNames; i++) {
            char        axisName = toupper(names[i]);
            const char* pos      = index(_names, axisName);
            if (!pos) {
                log_error("Invalid axis name " << names[i]);
                retval = false;
            }
            set_bitnum(mask, pos - Machine::Axes::_names);
        }

        return retval;
    }

    Axes::~Axes() {
        for (int i = 0; i < MAX_N_AXIS; ++i) {
            if (_axis[i] != nullptr) {
                delete _axis[i];
            }
        }
    }
}
