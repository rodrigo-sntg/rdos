#ifndef __RDOS__DRIVERS__DRIVER_H
#define __RDOS__DRIVERS__DRIVER_H

namespace rdos
{
    namespace drivers
    {
        class Driver
        {
        private:
            /* data */
        public:
            Driver(/* args */);
            ~Driver();

            virtual void Activate();
            virtual int Reset();
            virtual void DeActivate();
        };
        

        class DriverManager
        {
        public:
            Driver* drivers[256];
            int numDrivers;

            DriverManager();
            void AddDriver(Driver* drv);

            void ActivateAll();
        };
    }
}
    

#endif

