#include "TobiiMex/types.h"

#include "TobiiMex/utils.h"

namespace TobiiTypes
{
    eyeTracker::eyeTracker(TobiiResearchEyeTracker* et_) :
        et(et_)
    {
        if (et)
        {
            refreshInfo();
        }
    };

    eyeTracker::eyeTracker(std::string deviceName_, std::string serialNumber_, std::string model_, std::string firmwareVersion_, std::string runtimeVersion_, std::string address_,
        TobiiResearchCapabilities capabilities_, std::vector<float> supportedFrequencies_, std::vector<std::string> supportedModes_) :
        deviceName(deviceName_),
        serialNumber(serialNumber_),
        model(model_),
        firmwareVersion(firmwareVersion_),
        runtimeVersion(runtimeVersion_),
        address(address_),
        capabilities(capabilities_),
        supportedFrequencies(supportedFrequencies_),
        supportedModes(supportedModes_)
    {

    }

    void eyeTracker::refreshInfo(std::optional<std::string> paramToRefresh_ /*= std::nullopt*/)
    {
        bool singleOpt = paramToRefresh_.has_value();
        // get all info about the eye tracker
        TobiiResearchStatus status;
        // first bunch of strings
        if (!singleOpt || paramToRefresh_=="deviceName")
        {
            char* device_name;
            status = tobii_research_get_device_name(et, &device_name);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker device name", status);
            deviceName = device_name;
            tobii_research_free_string(device_name);
        }
        if (!singleOpt || paramToRefresh_ == "serialNumber")
        {
            char* serial_number;
            status = tobii_research_get_serial_number(et, &serial_number);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker serial number", status);
            serialNumber = serial_number;
            tobii_research_free_string(serial_number);
        }
        if (!singleOpt || paramToRefresh_ == "model")
        {
            char* modelT;
            status = tobii_research_get_model(et, &modelT);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker model", status);
            model = modelT;
            tobii_research_free_string(modelT);
        }
        if (!singleOpt || paramToRefresh_ == "firmwareVersion")
        {
            char* firmware_version;
            status = tobii_research_get_firmware_version(et, &firmware_version);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker firmware version", status);
            firmwareVersion = firmware_version;
            tobii_research_free_string(firmware_version);
        }
        if (!singleOpt || paramToRefresh_ == "runtimeVersion")
        {
            char* runtime_version;
            status = tobii_research_get_runtime_version(et, &runtime_version);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker runtime version", status);
            runtimeVersion = runtime_version;
            tobii_research_free_string(runtime_version);
        }
        if (!singleOpt || paramToRefresh_ == "address")
        {
            char* addressT;
            status = tobii_research_get_address(et, &addressT);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker address", status);
            address = addressT;
            tobii_research_free_string(addressT);
        }

        // its capabilities
        if (!singleOpt || paramToRefresh_ == "capabilities")
        {
            status = tobii_research_get_capabilities(et, &capabilities);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker capabilities", status);
        }

        // get supported sampling frequencies
        if (!singleOpt || paramToRefresh_ == "supportedFrequencies")
        {
            TobiiResearchGazeOutputFrequencies* tobiiFreqs = nullptr;
            supportedFrequencies.clear();
            status = tobii_research_get_all_gaze_output_frequencies(et, &tobiiFreqs);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker output frequencies", status);
            supportedFrequencies.insert(supportedFrequencies.end(), &tobiiFreqs->frequencies[0], &tobiiFreqs->frequencies[tobiiFreqs->frequency_count]);   // yes, pointer to one past last element
            tobii_research_free_gaze_output_frequencies(tobiiFreqs);
        }

        // get supported eye tracking modes
        if (!singleOpt || paramToRefresh_ == "supportedModes")
        {
            TobiiResearchEyeTrackingModes* tobiiModes = nullptr;
            supportedModes.clear();
            status = tobii_research_get_all_eye_tracking_modes(et, &tobiiModes);
            if (status != TOBII_RESEARCH_STATUS_OK)
                ErrorExit("Cannot get eye tracker's tracking modes", status);
            supportedModes.insert(supportedModes.end(), &tobiiModes->modes[0], &tobiiModes->modes[tobiiModes->mode_count]);   // yes, pointer to one past last element
            tobii_research_free_eye_tracking_modes(tobiiModes);
        }
    }
}