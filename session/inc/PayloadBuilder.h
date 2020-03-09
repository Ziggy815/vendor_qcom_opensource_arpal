/*
 * Copyright (c) 2019-2020, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PAYLOAD_BUILDER_H_
#define PAYLOAD_BUILDER_H_

#include "QalDefs.h"
#include "gsl_intf.h"
#include "kvh2xml.h"
#include "QalCommon.h"
#include <vector>
#include <algorithm>
#include <expat.h>
#include <map>
#include "Stream.h"
#include "Device.h"
#include "ResourceManager.h"

#define MSM_MI2S_SD0 (1 << 0)
#define MSM_MI2S_SD1 (1 << 1)
#define MSM_MI2S_SD2 (1 << 2)
#define MSM_MI2S_SD3 (1 << 3)
#define MSM_MI2S_SD4 (1 << 4)
#define MSM_MI2S_SD5 (1 << 5)
#define MSM_MI2S_SD6 (1 << 6)
#define MSM_MI2S_SD7 (1 << 7)

#define PCM_ENCODER STREAM_PCM_ENCODER
#define PCM_DECODER STREAM_PCM_DECODER
#define PCM_CONVERTOR STREAM_PCM_CONVERTER
#define IN_MEDIA STREAM_INPUT_MEDIA_FORMAT
#define CODEC_DMA 0
#define I2S 1
#define HW_EP_TX DEVICE_HW_ENDPOINT_TX
#define HW_EP_RX DEVICE_HW_ENDPOINT_RX
#define TAG_STREAM_MFC_SR TAG_STREAM_MFC
#define TAG_DEVICE_MFC_SR PER_STREAM_PER_DEVICE_MFC

struct sessionToPayloadParam {
    uint32_t sampleRate;                /**< sample rate */
    uint32_t bitWidth;                  /**< bit width */
    uint32_t numChannel;
    struct qal_channel_info *ch_info;    /**< channel info */
    int direction;
    int native;
    int rotation_type;
    void *metadata;
};

struct codecDmaConfig {
    uint32_t intfLinkIdx;
    uint32_t lpaifType;
    uint32_t intfIdx;
};

struct i2sConfig {
    uint32_t intfLinkIdx;
    uint32_t lpaifType;
    uint32_t intfIdx;
    uint16_t sdLineIdx;
    uint16_t wsSrc;
};

struct tdmConfig{
    uint32_t intfLinkIdx;
    uint32_t lpaifType;
    uint32_t intfIdx;
    uint32_t syncSrc;
    uint32_t ctrlDataOutEnable;
    uint32_t syncMode;
    uint32_t ctrlInvertSyncPulse;
    uint32_t ctrlSyncDataDelay;
};

struct auxpcmConfig{
    uint32_t intfLinkIdx;
    uint32_t lpaifType;
    uint32_t intfIdx;
    uint32_t syncSrc;
    uint32_t ctrlDataOutEnable;
    uint32_t slotMask;
    uint32_t frameSetting;
    uint32_t auxMode;
};

struct slimConfig {
    uint32_t intfLinkIdx;
    uint32_t dev_id;
    uint32_t sh_mapping_idx_0;
    uint32_t sh_mapping_idx_1;
};

struct usbAudioConfig {
    uint32_t usb_token;
    uint32_t svc_interval;
};

struct dpAudioConfig{
  uint32_t channel_allocation;
  uint32_t mst_idx;
  uint32_t dptx_idx;
};

class SessionGsl;

class PayloadBuilder
{
private:
    static std::vector<codecDmaConfig> codecConf;
    static std::vector<i2sConfig> i2sConf;
    static std::vector<tdmConfig> tdmConf;
    static std::vector<auxpcmConfig> auxpcmConf;
    static std::vector<slimConfig> slimConf;
    void payloadInMediaConfig(uint8_t** payload, size_t* size,
                              struct gsl_module_id_info* moduleInfo,
                              struct sessionToPayloadParam* data);
    void payloadOutMediaConfig(uint8_t** payload, size_t* size,
                               struct gsl_module_id_info* moduleInfo,
                               struct sessionToPayloadParam* data);
    void payloadCodecDmaConfig(uint8_t** payload, size_t* size,
                               struct gsl_module_id_info* moduleInfo,
                               struct sessionToPayloadParam* data,
                               std::string epName);
    void payloadI2sConfig(uint8_t** payload, size_t* size,
                          struct gsl_module_id_info* moduleInfo,
                          struct sessionToPayloadParam* data,
                          std::string epName);
    void payloadTdmConfig(uint8_t** payload, size_t* size,
                          struct gsl_module_id_info* moduleInfo,
                          struct sessionToPayloadParam* data, std::string epName);
    void payloadAuxpcmConfig(uint8_t** payload, size_t* size,
                             struct gsl_module_id_info* moduleInfo,
                             struct sessionToPayloadParam* data, std::string epName);
    void payloadHwEpConfig(uint8_t** payload, size_t* size,
                           struct gsl_module_id_info* moduleInfo,
                           struct sessionToPayloadParam* data);
    void payloadSlimConfig(uint8_t** payload, size_t* size,
                           struct gsl_module_id_info* moduleInfo,
                           struct sessionToPayloadParam* data, std::string epName);
public:
    void payloadUsbAudioConfig(uint8_t** payload, size_t* size,
                           uint32_t miid,
                           struct usbAudioConfig *data);
    void payloadDpAudioConfig(uint8_t** payload, size_t* size,
                           uint32_t miid,
                           struct dpAudioConfig *data);
    void payloadStreamConfig(uint8_t** payload, size_t* size,
                             struct gsl_module_id_info* moduleInfo,
                             int payloadTag, struct sessionToPayloadParam* data);
    void payloadDeviceEpConfig(uint8_t** payload, size_t* size,
                               struct gsl_module_id_info* moduleInfo,
                               int payloadTag, struct sessionToPayloadParam* data,
                               std::string epName);
    void payloadDeviceConfig(uint8_t** payload, size_t* size,
                             struct gsl_module_id_info* moduleInfo,
                             int payloadTag, struct sessionToPayloadParam* data);
    void payloadMFCConfig(uint8_t** payload, size_t* size,
                           uint32_t miid,
                           struct sessionToPayloadParam* data);
    void payloadVolume(uint8_t **payload, size_t *size, uint32_t moduleId,
                       struct qal_volume_data *volumedata, int tag);
    void payloadPause(uint8_t **payload, size_t *size, uint32_t moduleId);
    void payloadResume(uint8_t **payload, size_t *size, uint32_t moduleId);
    int payloadCustomParam(uint8_t **alsaPayload, size_t *size,
                            uint32_t *customayload, uint32_t customPayloadSize,
                            uint32_t moduleInstanceId, uint32_t dspParamId);
    void payloadSVASoundModel(uint8_t **payload, size_t *size, uint32_t moduleId,
                              struct qal_st_sound_model *soundModel);
    void payloadSVAWakeUpConfig(uint8_t **payload, size_t *size, uint32_t moduleId,
                                struct detection_engine_config_voice_wakeup *pWakeUp);
    void payloadSVAWakeUpBufferConfig(uint8_t **payload, size_t *size, uint32_t moduleId,
                    struct detection_engine_voice_wakeup_buffer_config *pBufferConfig);
    void payloadSVAStreamSetupDuration(uint8_t **payload, size_t *size, uint32_t moduleId,
                    struct audio_dam_downstream_setup_duration *pSetupDuration);
    void payloadSVAEventConfig(uint8_t **payload, size_t *size, uint32_t moduleId,
                       struct detection_engine_generic_event_cfg *pEventConfig);
    void payloadSVAEngineReset(uint8_t **payload, size_t *size, uint32_t moduleId);
    void payloadDOAInfo(uint8_t **payload, size_t *size, uint32_t moduleId);
    void payloadQuery(uint8_t **payload, size_t *size, uint32_t moduleId,
                            uint32_t paramId, uint32_t querySize);
    void populateChannelMap(uint16_t* pcmChannel, uint8_t num_ch);
    void payloadRATConfig(uint8_t** payload, size_t* size, uint32_t miid,
                          struct qal_media_config *data);
    void payloadCopPackConfig(uint8_t** payload, size_t* size, uint32_t miid,
                          struct qal_media_config *data);
    void payloadTWSConfig(uint8_t** payload, size_t* size, uint32_t miid,
                          bool isTwsMonoModeOn, uint32_t codecFormat);
    int populateStreamKV(Stream* s, std::vector <std::pair<int,int>> &keyVector);
    int populateStreamKV(Stream* s, std::vector <std::pair<int,int>> &keyVectorRx,
        std::vector <std::pair<int,int>> &keyVectorTx ,struct vsid_info vsidinfo);
    int populateStreamPPKV(Stream* s, std::vector <std::pair<int,int>> &keyVectorRx,
        std::vector <std::pair<int,int>> &keyVectorTx);
    int populateStreamDeviceKV(Stream* s, int32_t beDevId, std::vector <std::pair<int,int>> &keyVector);
    int populateStreamDeviceKV(Stream* s, int32_t rxBeDevId, std::vector <std::pair<int,int>> &keyVectorRx,
        int32_t txBeDevId, std::vector <std::pair<int,int>> &keyVectorTx, struct vsid_info vsidinfo);
    int populateDeviceKV(Stream* s, int32_t beDevId, std::vector <std::pair<int,int>> &keyVector);
    int populateDeviceKV(Stream* s, int32_t rxBeDevId, std::vector <std::pair<int,int>> &keyVectorRx,
        int32_t txBeDevId, std::vector <std::pair<int,int>> &keyVectorTx);
    int populateDevicePPKV(Stream* s, int32_t rxBeDevId, std::vector <std::pair<int,int>> &keyVectorRx,
        int32_t txBeDevId, std::vector <std::pair<int,int>> &keyVectorTx,
        std::vector<kvpair_info> kvpair, bool is_lpi);
    int populateGkv(Stream *s, struct gsl_key_vector *gkv);
    int populateCkv(Stream *s, struct gsl_key_vector *ckv, int tag, struct qal_volume_data **);
    int populateStreamCkv(Stream *s, std::vector <std::pair<int,int>> &keyVector, int tag, struct qal_volume_data **);
    int populateTkv(Stream *s, struct gsl_key_vector *tkv, int tag, uint32_t* gsltag);
    int populateCalKeyVector(Stream *s, std::vector <std::pair<int,int>> &ckv, int tag);
    int populateTagKeyVector(Stream *s, std::vector <std::pair<int,int>> &tkv, int tag, uint32_t* gsltag);
    void payloadTimestamp(uint8_t **payload, size_t *size, uint32_t moduleId);
    static int init();
    static void endTag(void *userdata __unused, const XML_Char *tag_name);
    static void processCodecInfo(const XML_Char **attr);
    static void processI2sInfo(const XML_Char **attr);
    static void processTdmInfo(const XML_Char **attr);
    static void processAuxpcmInfo(const XML_Char **attr);
    static void processSlimInfo(const XML_Char **attr);
    static void startTag(void *userdata __unused, const XML_Char *tag_name, const XML_Char **attr);
    PayloadBuilder();
    ~PayloadBuilder();
};
#endif //SESSION_H
