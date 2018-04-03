#include "addon.h"

static CHelper_libXBMC_pvr *PVR = NULL;

extern "C"
{

// ADDON

ADDON_STATUS ADDON_Create(void *callbacks, void *props)
{
    PVR = new CHelper_libXBMC_pvr;
    if (!PVR->RegisterMe(callbacks))
    {
        ADDON_Destroy();
        return ADDON_STATUS_PERMANENT_FAILURE;
    }

    return ADDON_STATUS_OK;
}

void ADDON_Destroy()
{
    delete PVR;
    PVR = NULL;
}

PVR_ERROR GetAddonCapabilities(PVR_ADDON_CAPABILITIES *pCapabilities)
{
    pCapabilities->bHandlesDemuxing = false;
    pCapabilities->bHandlesInputStream = false;
    pCapabilities->bSupportsChannelGroups = false;
    pCapabilities->bSupportsChannelScan = false;
    pCapabilities->bSupportsChannelSettings = false;
    pCapabilities->bSupportsEPG = true;
    pCapabilities->bSupportsLastPlayedPosition = false;
    pCapabilities->bSupportsRadio = false;
    pCapabilities->bSupportsRecordingEdl = false;
    pCapabilities->bSupportsRecordingPlayCount = false;
    pCapabilities->bSupportsRecordings = false;
    pCapabilities->bSupportsRecordingsUndelete = false;
    pCapabilities->bSupportsTimers = false;
    pCapabilities->bSupportsTV = true;
    return PVR_ERROR_NO_ERROR;
}

int GetChannelsAmount(void)
{
    return 1;
}

PVR_ERROR GetChannels(ADDON_HANDLE handle, bool bRadio)
{
    PVR_CHANNEL entry;
    entry.bIsHidden = false;
    entry.bIsRadio = false;
    entry.iChannelNumber = 0;
    entry.iEncryptionSystem = 0;
    entry.iSubChannelNumber = 0;
    entry.iUniqueId = 131;
    strcpy(entry.strChannelName, "Первый канал");
//    strcpy(entry.strIconPath, "");
//    strcpy(entry.strInputFormat, "HTTP");
    strcpy(entry.strStreamURL, "http://spacetv.in/stream/BES5W7VUMB/131.m3u8");

    PVR->TransferChannelEntry(handle, &entry);

    return PVR_ERROR_NO_ERROR;
}

// ------------------------------------------- UNUSED ----------------------------------------------

// ADDON

void ADDON_Stop()
{
}

ADDON_STATUS ADDON_GetStatus()
{
    return ADDON_STATUS_OK;
}

bool ADDON_HasSettings()
{
    return false;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
    return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue)
{
    return ADDON_STATUS_OK;
}

void ADDON_FreeSettings()
{
}

// PVR

const char* GetPVRAPIVersion(void)
{
    return XBMC_PVR_API_VERSION;
}

const char* GetMininumPVRAPIVersion(void)
{
    return XBMC_PVR_MIN_API_VERSION;
}

const char* GetGUIAPIVersion(void)
{
    return KODI_GUILIB_API_VERSION;
}

const char* GetMininumGUIAPIVersion(void)
{
    return KODI_GUILIB_MIN_API_VERSION;
}

const char* GetBackendName(void)
{
    return "";
}

const char* GetBackendVersion(void)
{
    return "";
}

const char* GetConnectionString(void)
{
    return "";
}

PVR_ERROR GetDriveSpace(long long* iTotal, long long* iUsed)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR CallMenuHook(const PVR_MENUHOOK& menuhook, const PVR_MENUHOOK_DATA &item)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR GetEPGForChannel(ADDON_HANDLE handle, const PVR_CHANNEL& channel, time_t iStart, time_t iEnd)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

int GetChannelGroupsAmount(void)
{
    return 0;
}

PVR_ERROR GetChannelGroups(ADDON_HANDLE handle, bool bRadio)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR GetChannelGroupMembers(ADDON_HANDLE handle, const PVR_CHANNEL_GROUP& group)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR OpenDialogChannelScan(void)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR DeleteChannel(const PVR_CHANNEL& channel)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR RenameChannel(const PVR_CHANNEL& channel)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR MoveChannel(const PVR_CHANNEL& channel)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR OpenDialogChannelSettings(const PVR_CHANNEL& channel)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR OpenDialogChannelAdd(const PVR_CHANNEL& channel)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

int GetRecordingsAmount(bool deleted)
{
    return 0;
}

PVR_ERROR GetRecordings(ADDON_HANDLE handle, bool deleted)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR DeleteRecording(const PVR_RECORDING& recording)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR UndeleteRecording(const PVR_RECORDING& recording)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR DeleteAllRecordingsFromTrash()
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR RenameRecording(const PVR_RECORDING& recording)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR SetRecordingPlayCount(const PVR_RECORDING& recording, int count)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR SetRecordingLastPlayedPosition(const PVR_RECORDING& recording, int lastplayedposition)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

int GetRecordingLastPlayedPosition(const PVR_RECORDING& recording)
{
    return 0;
}

PVR_ERROR GetRecordingEdl(const PVR_RECORDING&, PVR_EDL_ENTRY edl[], int *size)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR GetTimerTypes(PVR_TIMER_TYPE types[], int *typesCount)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

int GetTimersAmount(void)
{
    return 0;
}

PVR_ERROR GetTimers(ADDON_HANDLE handle)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR AddTimer(const PVR_TIMER& timer)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR DeleteTimer(const PVR_TIMER& timer, bool bForceDelete)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR UpdateTimer(const PVR_TIMER& timer)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

bool OpenLiveStream(const PVR_CHANNEL& channel)
{
    return false;
}

void CloseLiveStream(void)
{
}

int ReadLiveStream(unsigned char* pBuffer, unsigned int iBufferSize)
{
    return 0;
}

long long SeekLiveStream(long long iPosition, int iWhence)
{
    return 0L;
}

long long PositionLiveStream(void)
{
    return 0L;
}

long long LengthLiveStream(void)
{
    return 0L;
}

bool SwitchChannel(const PVR_CHANNEL& channel)
{
    return false;
}

PVR_ERROR SignalStatus(PVR_SIGNAL_STATUS& signalStatus)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

const char* GetLiveStreamURL(const PVR_CHANNEL& channel)
{
    return "";
}

PVR_ERROR GetStreamProperties(PVR_STREAM_PROPERTIES* pProperties)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

bool OpenRecordedStream(const PVR_RECORDING& recording)
{
    return false;
}

void CloseRecordedStream(void)
{
}

int ReadRecordedStream(unsigned char* pBuffer, unsigned int iBufferSize)
{
    return 0;
}

long long SeekRecordedStream(long long iPosition, int iWhence)
{
    return 0L;
}

long long PositionRecordedStream(void)
{
    return 0;
}

long long LengthRecordedStream(void)
{
    return 0;
}

void DemuxReset(void)
{
}

void DemuxAbort(void)
{
}

void DemuxFlush(void)
{
}

DemuxPacket* DemuxRead(void)
{
    return NULL;
}

unsigned int GetChannelSwitchDelay(void)
{
    return 0;
}

bool CanPauseStream()
{
    return false;
}

bool CanSeekStream()
{
    return false;
}

void PauseStream(bool bPaused)
{
}

bool SeekTime(double time, bool backwards, double *startpts)
{
    return false;
}

void SetSpeed(int speed)
{
}

time_t GetPlayingTime()
{
    return 0L;
}

time_t GetBufferTimeStart()
{
    return 0L;
}

time_t GetBufferTimeEnd()
{
    return 0L;
}

const char* GetBackendHostname()
{
    return "";
}

bool IsTimeshifting()
{
    return false;
}

bool IsRealTimeStream()
{
    return false;
}

PVR_ERROR SetEPGTimeFrame(int iDays)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
}

void OnSystemSleep()
{

}

void OnSystemWake()
{

}

void OnPowerSavingActivated()
{

}

void OnPowerSavingDeactivated()
{

}

}
