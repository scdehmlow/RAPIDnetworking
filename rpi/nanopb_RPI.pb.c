/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9-dev at Thu Mar 30 17:45:47 2017. */

#include "nanopb_RPI.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t SensorData_fields[4] = {
    PB_FIELD(  1, FLOAT   , REQUIRED, STATIC  , FIRST, SensorData, value, value, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, SensorData, sensorId, value, 0),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, SensorData, timestamp, sensorId, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
