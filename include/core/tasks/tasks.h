#ifndef TASKS_H
#define TASKS_H

#include "physical_network.h"
#include "http_backend_network.h"
#include "eeprom.h"
#include "room_config.h"
#include "random_source.h"
#include "clock_source.h"
#include "pin_io.h"
#include "uuid.h"

namespace Tasks {
    namespace Device 
    {
        //! @brief Retrieve the device ID from the EEPROM
        //! @param uuid[out] The UUID to store the device ID in
        //! @param eeprom[in] The EEPROM interface
        void retrieve_device_id(Uuid&, Eeprom&);
    };

    namespace Config {

        void retrieve_config(Uuid&, PhysicalNetwork&, HttpBackendNetwork&);
        void update_config(Uuid&, PhysicalNetwork&, HttpBackendNetwork&);
        void save_config(Uuid&, Eeprom&);
    };

    namespace Time {
        void set_time(Clock& clock, 
                      PinIO& pin_io);
    };
};

#endif