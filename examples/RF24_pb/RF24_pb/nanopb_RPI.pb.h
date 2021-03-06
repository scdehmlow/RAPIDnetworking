/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9-dev at Fri Apr  7 20:55:22 2017. */

#ifndef PB_NANOPB_RPI_PB_H_INCLUDED
#define PB_NANOPB_RPI_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _SensorData {
    int32_t sensorId;
    bool has_value;
    float value;
    bool has_timestamp;
    int32_t timestamp;
    bool has_temperature;
    float temperature;
    bool has_humidity;
    float humidity;
/* @@protoc_insertion_point(struct:SensorData) */
} SensorData;

/* Default values for struct fields */

/* Initializer values for message structs */
#define SensorData_init_default                  {0, false, 0, false, 0, false, 0, false, 0}
#define SensorData_init_zero                     {0, false, 0, false, 0, false, 0, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define SensorData_sensorId_tag                  1
#define SensorData_value_tag                     2
#define SensorData_timestamp_tag                 3
#define SensorData_temperature_tag               4
#define SensorData_humidity_tag                  5

/* Struct field encoding specification for nanopb */
extern const pb_field_t SensorData_fields[6];

/* Maximum encoded size of messages (where known) */
#define SensorData_size                          37

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define NANOPB_RPI_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
