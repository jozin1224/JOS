#include "../Include/cstdint.h"
#include "../Drivers/io.h"

#define CMOS_ADDRESS_PORT 0x70
#define CMOS_DATA_PORT    0x71

struct BiosTime {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
};

uint8_t read_rtc_register(int reg) {
    outb(CMOS_ADDRESS_PORT, reg);
    return inb(CMOS_DATA_PORT);
}

bool is_rtc_updating() {
    outb(CMOS_ADDRESS_PORT, 0x0A);
    return (inb(CMOS_DATA_PORT) & 0x80);
}

BiosTime get_bios_time() {
    while (is_rtc_updating());
    uint8_t second = read_rtc_register(0x00);
    uint8_t minute = read_rtc_register(0x02);
    uint8_t hour   = read_rtc_register(0x04);
    uint8_t day    = read_rtc_register(0x07);
    uint8_t month  = read_rtc_register(0x08);
    uint32_t year  = read_rtc_register(0x09);
    uint8_t registerB = read_rtc_register(0x0B);
    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour   = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
        day    = (day & 0x0F) + ((day / 16) * 10);
        month  = (month & 0x0F) + ((month / 16) * 10);
        year   = (year & 0x0F) + ((year / 16) * 10);
    }
    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }
    year += 2000; 
    return {second, minute, hour, day, month, year};
}