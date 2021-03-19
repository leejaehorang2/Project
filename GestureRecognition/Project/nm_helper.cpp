#include "nm_helper.hpp"

#include <iostream>

#include <string.h>

NMHelper* NMHelper::helperInstance = nullptr;
mutex NMHelper::instanceMutex;

NMHelper::NMHelper()
{
    targetDevice = nullptr;
}

NMHelper::~NMHelper()
{
}

bool NMHelper::nmGetDevices()
{
    bool result = false;

    uint16_t return_code;
    uint8_t detected_count = 0;

    do{
        return_code = nm_get_devices(devices, &detected_count);

        if(detected_count <= 0){
            return_code = NM_ERROR_DEVICE_NOT_FOUND;
            break;
        }

        if(return_code == NM_SUCCESS){
            targetDevice = &devices[0];
            result = true;
            cout << "Get device success." << endl;
        }
        else {
             cout << "Failed to get device! Error code : " << return_code << endl;
        }

    }while(false);

    return result;
}

bool NMHelper::nmConnect()
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_connect(targetDevice);

    if(return_code == NM_SUCCESS){
        cout << "Device connection success." << endl;
        result = true;
    }
    else{
        cout << "Failed to get device! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmSetNetworkTypeToRBF()
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_set_network_type(targetDevice,RBF);

    if(return_code == NM_SUCCESS){
        cout << "Set network type success : RBF" << endl;
        result = true;
    }
    else{
        cout << "Failed to set network type! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmForget()
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_forget(targetDevice);

    if(return_code == NM_SUCCESS){
        cout << "Forget command success." << endl;
        result = true;
    }
    else{
        cout << "Failed to forget! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmPowerSave()
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_power_save(targetDevice);

    if(return_code == NM_SUCCESS){
        cout << "Power save success." << endl;
        result = true;
    }
    else{
        cout << "Failed to power save! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmClose()
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_close(targetDevice);

    if(return_code == NM_SUCCESS){
        cout << "Close success." << endl;
        result = true;
    }
    else{
        cout << "Failed to close! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmClassify(nm_classify_req* classifyReq)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_classify(targetDevice,classifyReq);

    if(return_code == NM_SUCCESS){
        if(classifyReq->matched_count > 0){
            cout << "Category : " << classifyReq->category[0] << " NID : " << classifyReq->nid[0] << endl;
        }
        else{
            cout << "Unknown Category !" << endl;
        }
        result = true;
    }
    else{
        cout << "Failed to classify! Error code : " << endl;
    }

    return result;
}

bool NMHelper::nmLearn(nm_learn_req* learnReq)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    //Neuron
    uint16_t return_code = nm_learn(targetDevice,learnReq);

    if(return_code == NM_SUCCESS){

        if(learnReq->query_affected == 1 && learnReq->affected_count > 0){

            uint16_t nCount = 0;
            nm_read(targetDevice,NM_NCOUNT,&nCount);

            for(int i = 0; i < learnReq->affected_count; ++i)
                cout << "[Affected Neuron] " << "NID : " << learnReq->affected_neurons[i].nid << " AIF : " << learnReq->affected_neurons[i].aif << " MINIF : " << learnReq->affected_neurons[i].minif << endl;
        }
        result = true;
    }
    else{
        cout << "Failed to learn! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmWriteMinIF(uint16_t minIF)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_write(targetDevice, NM_MINIF, minIF);

    if(return_code == NM_SUCCESS){
        cout << "Write MinIF success! MinIF value : " << minIF << endl;
        result = true;
    }
    else{
        cout << "Failed to write MaxIF! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmWriteMaxIF(uint16_t maxIF)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_write(targetDevice, NM_MAXIF, maxIF);

    if(return_code == NM_SUCCESS){
        cout << "Write MaxIF success! MaxIF value : " << maxIF << endl;
        result = true;
    }
    else{
        cout << "Failed to write MaxIF! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmWriteNeurons(nm_neuron* neurons, uint32_t& count)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_write_neurons(targetDevice, neurons, &count);

    if(return_code == NM_SUCCESS){
        cout << "Write neurons success! neuron count : " << count << endl;
        result = true;
    }
    else{
        cout << "Failed to write neurons! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmGetNeuronCount(uint32_t& count)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_get_neuron_count(targetDevice, &count);

    if(return_code == NM_SUCCESS){
        cout << "Get neurons count command success. Neuron count value : " << count << endl;
        result = true;
    }
    else{
        cout << "Failed to get neurons count command! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmReadNeurons(nm_neuron* neurons, uint32_t& count)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    uint16_t return_code = nm_read_neurons(targetDevice, neurons, &count);

    if(return_code == NM_SUCCESS){
        cout << "Read neurons success. Neuron count value : " << count << endl;
        result = true;
    }
    else{
        cout << "Failed to read neurons! Error code : " << return_code << endl;
    }

    return result;
}

bool NMHelper::nmSetContext(uint16_t inputContext, uint16_t minIF, uint16_t maxIF)
{
    bool result = false;

    if(!targetDevice){
        return false;
    }

    nm_context nmContext;
    memset(&nmContext, 0, sizeof(nm_context));

    nmContext.norm = L1;
    nmContext.minif = minIF;
    nmContext.maxif = maxIF;
    nmContext.context = inputContext;

    uint16_t return_code = nm_set_context(targetDevice,&nmContext);

    if(return_code == NM_SUCCESS){
        cout << "Set context Success!" << endl;
        result = true;
    }
    else{
        cout << "Failed to set context! Error code : " << return_code << endl;
    }
    return result;
}

bool NMHelper::nmSaveModel(const char* path)
{
    bool result = false;
    nm_neuron* neurons = nullptr;

    do{
        if(!targetDevice){
            break;
        }

        uint32_t neuron_count = 0;
        if(!nmGetNeuronCount(neuron_count)){
            break;
        }

        neurons = (nm_neuron*)malloc(sizeof(nm_neuron) * neuron_count);
        if(!neurons){
            break;
        }

        neurons[0].size = NEURON_MEMORY;
        if(!nmReadNeurons(neurons, neuron_count)){
            break;
        }

        uint16_t return_code = nm_save_model(targetDevice, neurons, neuron_count, path);

        if(return_code == NM_SUCCESS){
            cout << "Neuron data save to file success!" << endl;
            result = true;
        }
        else{
            cout << "Failed to save neuron data! Error code : " << return_code << endl;
        }

    }while(false);

    if(neurons){
        free(neurons);
    }

    return result;
}

bool NMHelper::nmLoadModel(const char* path)
{
    bool result = false;
    nm_neuron* neurons = nullptr;

    do{
        if(!targetDevice){
            break;
        }

        uint32_t neuron_count = 576;

        neurons = (nm_neuron*)malloc(sizeof(nm_neuron) * neuron_count);
        if(!neurons){
            break;
        }

        neurons[0].size = NEURON_MEMORY;

        uint16_t return_code = nm_load_model(targetDevice, neurons, &neuron_count, path);

        if(return_code == NM_SUCCESS){

            if(!nmWriteNeurons(neurons,neuron_count)){
                break;
            }
            cout << "Neuron data load success!" << endl;

            result = true;
        }
        else{
            cout << "Failed to load neuron data! Error code : " << return_code << endl;
        }

    }while(false);

    if(neurons){
        free(neurons);
    }

    return result;
}
