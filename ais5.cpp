// Class A shipdata

#include "ais.h"

Ais5::Ais5(const char *nmea_payload, const size_t pad) {
    assert(nmea_payload);
    assert(pad < 6);
    init();

    if (2 != pad || strlen(nmea_payload) != 71) { status = AIS_ERR_BAD_BIT_COUNT; return; }

    bitset<426> bs;
    status = aivdm_to_bits(bs, nmea_payload);
    if (had_error()) return;

    message_id = ubits(bs, 0, 6);
    if (5 != message_id) { status = AIS_ERR_WRONG_MSG_TYPE; return; }
    repeat_indicator = ubits(bs, 6, 2);
    mmsi = ubits(bs, 8, 30);

    ais_version = ubits(bs, 38, 2);
    imo_num = ubits(bs, 40, 30);
    callsign = ais_str(bs, 70, 42);

    name = ais_str(bs, 112, 120);

    type_and_cargo = ubits(bs, 232, 8);
    dim_a = ubits(bs, 240, 9);
    dim_b = ubits(bs, 249, 9);
    dim_c = ubits(bs, 258, 6);
    dim_d = ubits(bs, 264, 6);
    fix_type = ubits(bs, 270, 4);
    eta_month = ubits(bs, 274, 4);
    eta_day = ubits(bs, 278, 5);
    eta_hour = ubits(bs, 283, 5);
    eta_minute = ubits(bs, 288, 6);
    draught = ubits(bs, 294, 8) / 10.;
    destination = ais_str(bs, 302, 120);
    dte = bs[422];
    spare = bs[423];
}


ostream& operator<< (ostream& o, const Ais5 &msg) {
    return o << 5 << ": " << msg.mmsi << " \"" << msg.name << "\" "
             << msg.type_and_cargo << " " << msg.dim_a + msg.dim_b
             << "x" << msg.dim_c + msg.dim_d << "x" << msg.draught << "m";
}
