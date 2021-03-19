#ifndef NM_HELPER_HPP
#define NM_HELPER_HPP

#include "../include/nmengine.h"
#include <mutex>
#include <vector>

using namespace std;

#define MAX_DEVICE_COUNT 10

class NMHelper
{
public:
    NMHelper();
    ~NMHelper();

    static NMHelper* getInstance(){

        lock_guard<mutex> lock(instanceMutex);

        if(!helperInstance){
            helperInstance = new NMHelper;
        }

        return helperInstance;
    }

    static void releaseInstance(){

        if(helperInstance){
            delete helperInstance;
            helperInstance = nullptr;
        }
    }

    bool nmGetDevices();
    bool nmConnect();
    bool nmForget();
    bool nmPowerSave();
    bool nmClose();

    bool nmSetNetworkTypeToRBF();
    bool nmWriteMinIF(uint16_t minIF);
    bool nmWriteMaxIF(uint16_t maxIF);
    bool nmWriteNeurons(nm_neuron* neurons, uint32_t& count);
    bool nmGetNeuronCount(uint32_t& count);
    bool nmReadNeurons(nm_neuron* neurons, uint32_t& count);

    bool nmSetContext(uint16_t inputContext, uint16_t minIF = 0, uint16_t maxIF = 7500);

    bool nmClassify(nm_classify_req* classifyReq);
    bool nmLearn(nm_learn_req* learnReq);

    bool nmSaveModel(const char* path);
    bool nmLoadModel(const char* path);

private:
    static NMHelper* helperInstance;
    static mutex instanceMutex;

    nm_device devices[MAX_DEVICE_COUNT];
    nm_device* targetDevice;
};

#endif // NM_HELPER_HPP
