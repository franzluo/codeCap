/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 17337 2009-01-01 14:28:29Z magfr $
 */
#ifndef TUNER_H
#define TUNER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
} TunerConfig;

void init_tuner(void);

#ifdef __cplusplus
}
#endif
Netsnmp_Node_Handler handle_playTuner;
Netsnmp_Node_Handler handle_resetDuration;
Netsnmp_Node_Handler handle_channelUp;
Netsnmp_Node_Handler handle_channelDown;
Netsnmp_Node_Handler handle_tunerMode;
Netsnmp_Node_Handler handle_tunerScan;
Netsnmp_Node_Handler handle_tunerChannelStrength;
Netsnmp_Node_Handler handle_forceTxMode;
Netsnmp_Node_Handler handle_tunerChannelMode;

#endif                          /* TUNER_H */